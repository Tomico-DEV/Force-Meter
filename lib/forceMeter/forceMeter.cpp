#include "forceMeter.hpp"


using namespace fourEyed;

typedef void (*ForceMeterFn) (ForceMeter * classptr);


union floatByte
{
	float f;
	byte b[4];
};

enum
{
	FLOAT_LOAD_CELL_CAL
};

bool isAllOnes (floatByte x)
{
	for (int i = 0; i < 4; i++)
	{
		if ( x.b[i] != 255 )
		{
			return false;
		}
	}
	return true;
}

ForceMeter::ForceMeter (
	uint8_t tareButton, uint8_t captureButton, // buttons
	uint8_t recordingLight, uint8_t serialLight, // lights
	uint8_t hx711DIN, uint8_t hx711CLK, // hx711
	uint8_t mpu6050ADDR, uint8_t accel_range, // mpu6050
	uint8_t eepromADDR, uint16_t eepromSize, // AT24C256
	uint8_t max7219CS // max7219
)
{
	m_recLight	    = recordingLight;
	m_serLight		= serialLight;
	m_loadCell 		= new LoadCell (hx711CLK, hx711DIN);
	m_accelerometer = new MPU6050 (mpu6050ADDR, 160, accel_range);
	m_captureButton = new Button<ForceMeterFn, ForceMeter>  (captureButton);
	m_tareButton	= new Button<ForceMeterFn, ForceMeter>  (tareButton);
	m_eeprom		= new Storage (eepromADDR, eepromSize, configSize);
	m_sevenSeg		= new MAX7219 (max7219CS);
}

void ForceMeter::tare (void)
{
	m_loadTare = m_loadReading;
	m_accelTare = m_accelReading;
}

void ForceMeter::setup (void)
{
	// make sure Wire, Serial, and SPI are already set up!
	/*
	Wire.begin ();

	SPI.beginTransaction (SPISettings (10000000, MSBFIRST, SPI_MODE0));
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.begin ();
	
	Serial.begin (115200);
	*/
	pinMode (m_serLight, OUTPUT);
	pinMode (m_recLight, OUTPUT);

	m_tareButton-> setup ();
	m_tareButton->classPointer = this;
	m_tareButton->m_onDown = [] (ForceMeter * fmptr) { fmptr->tare (); };

	m_captureButton->setup 	  ();
	m_captureButton->classPointer = this;
	m_captureButton->m_onShortPress	= [] (ForceMeter * fmptr) { fmptr->m_capturing = false; fmptr->capture (); };
	m_captureButton->m_onHold 	= [] (ForceMeter * fmptr) { fmptr->captureHold (); };
	m_captureButton->m_onDouble = [] (ForceMeter * fmptr) { fmptr->captureDblT (); };

	m_accelerometer -> setup ();

	m_sevenSeg->setup ();
	m_sevenSeg->clear ();

	/* Startup animation
	 ### ###  ### ### ### ###
	#       #    #       #   # see max7219.h for more details
	 ### ###      ### ###    #

	 also gives time for the EEPROM to write in case 
	 it had been written to previously for any reason
	*/
	const uint16_t frameDelay = 50;
	for (uint8_t i = 0; i < 2; i++)
	{
		uint8_t digit = 7 - i * 3;
		m_sevenSeg->transfer (digit - 1, B00010000);
		delay (frameDelay);
		m_sevenSeg->transfer (digit - 1, B00011000);
		delay (frameDelay);
		m_sevenSeg->transfer (digit, B00001000);
		delay (frameDelay);
		m_sevenSeg->transfer (digit, B00001100);
		delay (frameDelay);
		m_sevenSeg->transfer (digit, B00001101);
		delay (frameDelay);
		m_sevenSeg->transfer (digit - 1, B00011001);
		delay (frameDelay);
		m_sevenSeg->transfer (digit - 2, B00000001);
		delay (frameDelay);
	}
	m_sevenSeg->transfer (1, B00010000);
	delay (frameDelay);
	m_sevenSeg->transfer (1, B00010001);

	floatByte conversionFactor;
	//m_eeprom->saveConfig (FLOAT_LOAD_CELL_CAL, 4, (byte *)&conversionFactor);
	//delay(5);
	m_eeprom->getConfig (FLOAT_LOAD_CELL_CAL, 4, (byte *)&conversionFactor);
	m_loadCell->setup   (conversionFactor.f);
	
}

void ForceMeter::checkSerial (void)
{
	if ( Serial )
	{
		pinMode (m_serLight, HIGH);

		if ( m_state == IDLE )
		{
			if ( Serial != m_SerialPrev ) // on serial connect
			{
				Serial.println ("Hello! Press P for playback or C to enter config");
			}

			// check for input
			if ( Serial.available() )
			{
				String commandString = Serial.readString();
				commandString.trim(); // remove leading/trailing whitespace

				char commandChar = ( commandString.length() > 0 ) ? commandString[0] : ' ';

				Serial.print ("> ");
				Serial.println (commandChar);

				switch (commandChar)
				{
					case 'c':
					case 'C':
						m_state = CONFIG;
						Serial.println ("Welcome to config");
						Serial.println ("Attach calibration load then press to get reading");
						Serial.println ("Enter new conversion value when ready");
						break;
					case 'p':
					case 'P':
						m_state = PLAYBACK;
						break;
					
					default:
						Serial.println ("Unrecognized command.");
				}
			}
		}
	}
	else
	{
		pinMode (m_serLight, LOW);
	}

	m_SerialPrev = Serial;
}

void ForceMeter::checkState  (void)
{
	switch ( m_state )
	{
		case CONFIG:
			while ( !Serial.available () ) ;
			
			{
				String readString = Serial.readString ();
				readString.trim ();

				Serial.print ("> ");
				Serial.println (readString);

				floatByte newConversionFactor = { .f=readString.toFloat () };

				if ( newConversionFactor.f == 0.0f )
				{
					Serial.println ("ERROR:   Converted value is 0; this is not allowed. The conversion may have failed");
				}
				else
				{
					m_eeprom->saveConfig(FLOAT_LOAD_CELL_CAL, 4, (byte *)&newConversionFactor);
					Serial.println ("NOTICE:  New value saved.");
				}
			}

			break;
		case PLAYBACK:
			floatByte loadReading;
			floatByte accelReading;

			digitalWrite (m_serLight, HIGH);

			for (uint16_t readAddr = 0; readAddr < m_eeprom->m_size - configSize - 9; readAddr += 8)
			{
				m_eeprom->read(readAddr, 4, (byte *)&loadReading);
				m_eeprom->read(readAddr + 4, 4, (byte *)&accelReading);
				
				if ( isAllOnes (accelReading) and isAllOnes (loadReading))
				{
					// end of recording
					break;
				}
				while ( !Serial ) ;

				Serial.print (loadReading.f);
				Serial.print ("\t");
				Serial.print (accelReading.f);
				Serial.println ();
			}

			digitalWrite (m_serLight, LOW);

			break;

		default:
			break;
	}
	m_state = IDLE;
}

void ForceMeter::capture 	 (void)
{
	m_state = RECORDING;

	uint16_t saveAddr { 0 };
	uint16_t maxSave = m_eeprom->m_size - configSize - 9;

	floatByte * loadReadingptr = (floatByte *)&m_loadReading;
	floatByte * accelYReadingptr = (floatByte *)&(m_accelReading.y);
	do  
	{
		m_captureButton->loop ();
		m_tareButton->loop ();

		m_loadReading = m_loadCell->getReadingNewtons (true) - m_loadTare;
		m_accelerometer->getAccel (m_accelReading);

		m_accelReading.x -= m_accelTare.x;
		m_accelReading.y -= m_accelTare.y;
		m_accelReading.z -= m_accelTare.z;

		m_sevenSeg->showFloat (m_loadReading, 1, 4);
		m_sevenSeg->showFloat (m_accelReading.y, 5, 8);

		m_eeprom->write (saveAddr, 4, (byte *)loadReadingptr);
		delay (5);
		m_eeprom->write (saveAddr + 4, 4, (byte *)accelYReadingptr);
		
		saveAddr += 8; // size of two floats, one for each reading

		if (saveAddr >= maxSave )
		{
			break;
		}

		
		uint8_t ledVal = map(saveAddr, 0, maxSave, 255, 0);

		analogWrite (m_recLight, ledVal);
	} while ( m_capturing );

	if (saveAddr >= maxSave)
	{
		m_sevenSeg->clear ();
		m_sevenSeg->writeChar (4, 'F');
		m_sevenSeg->writeChar (3, 'U');
		m_sevenSeg->writeChar (2, 'L');
		m_sevenSeg->writeChar (1, 'L');
		
		delay (1000);
		
		m_sevenSeg->showFloat (m_loadReading, 1, 4);
		m_sevenSeg->showFloat (m_accelReading.y, 5, 8);
	}
	else 
	{
		floatByte allOnes = { .b = { 255, 255, 255, 255 } };
		
		delay (5);
		m_eeprom->write (saveAddr, 4, (byte *)&allOnes);
		delay (5);
		m_eeprom->write (saveAddr + 4, 4, (byte *)&allOnes);
		delay (5);
	}

	digitalWrite (m_recLight, LOW);
}

void ForceMeter::captureHold (void)
{
	m_capturing = true;

	m_captureButton->m_onUp = [] (ForceMeter * fmptr) { fmptr->m_capturing = false; };

	capture ();

	m_captureButton->m_onUp = NULL;
}

void ForceMeter::captureDblT (void)
{
	m_capturing = true;

	m_captureButton->m_onShortPress = [] (ForceMeter * fmptr) { fmptr->m_capturing = false; };

	capture ();

	m_captureButton->m_onShortPress = [] (ForceMeter * fmptr) { fmptr->m_capturing = false; fmptr->capture (); };
}

void ForceMeter::loop (void)
{
	m_tareButton->loop ();
	m_captureButton->loop ();

	checkSerial ();
	checkState ();
}