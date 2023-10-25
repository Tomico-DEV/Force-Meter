/******************************** MIT LICENSE **********************************
 * 
 * Copyright 2023 4eyedMan
 * 
 * Permission is hereby granted, free of charge, to any person obtaining 
 * a copy of this software and associated documentation files (the “Software”),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 * 
*******************************************************************************/

/******************************* forceMeter.hpp ********************************
 * 
 * The forceMeter class
 * 
 * don't forget to call setup and loop in main!
 * 
 * see readme for more details
 * 
*******************************************************************************/

#include <Arduino.h>

#ifndef FOUREYED_FORCE_METER
#define FOUREYED_FORCE_METER

#include "foureyedTypes.hpp"
#include "hx711.hpp"
#include "mpu6050.hpp"
#include "max7219.hpp"
#include "eepromStorage.hpp"
#include "button.hpp"


namespace fourEyed
{
	class ForceMeter;

	typedef void (*ForceMeterFn) (ForceMeter * classptr);

	class ForceMeter
	{

		private:
			static const uint8_t configSize = 4; 
			// Size of bytes dedicated to device config on the EEPROM; ie load cell calibration

			enum State
			{
				IDLE,
				RECORDING,
				CONFIG,
				PLAYBACK,
			};

			// pins for the LEDs
			uint8_t m_serLight;
			uint8_t m_recLight;

			// references to object
			LoadCell * m_loadCell;
			MPU6050 *  m_accelerometer;
			Button<ForceMeterFn, ForceMeter> * m_captureButton;
			Button<ForceMeterFn, ForceMeter> * m_tareButton;
			Storage *  m_eeprom;
			MAX7219 *  m_sevenSeg;

			bool m_SerialPrev { false }; 
			// stores previous state of Serial. 
			// False if closed, True if opened

			State m_state 	  { IDLE }; // state of force meter

			void checkSerial  (void);	// serial functionality
			void checkState   (void);
			void tare 		  (void);
			void capture 	  (void);	// records data
			void captureHold  (void);	// calls capture() when button is held
			void captureDblT  (void);	// calls capture() when button is double pressed

		public:
			ForceMeter (
				uint8_t tareButton, 	uint8_t captureButton, 	// buttons
				uint8_t recordingLight, uint8_t serialLight, 	// lights
				uint8_t hx711DIN, 		uint8_t hx711CLK,		// hx711 data and clock pin
				uint8_t mpu6050ADDR, 	uint8_t accel_range,	// mpu6050 address and range
				uint8_t eepromADDR, 	uint16_t eepromSize,	// EEPROM address and size
				uint8_t max7219CS							 	// max7219 chip select
			);

			bool  m_capturing  	 { false }; // is capturing

			float m_loadReading  { 0.0f }; 				// load cell reading (newtons)
			accel m_accelReading { 0.0f, 0.0f, 0.0f };  // accelerometer reading (m/s^2)

			float m_loadTare 	 { 0.0f };
			accel m_accelTare	 { 0.0f, 0.0f, 0.0f };

			void setup 	(void);
			void loop 	(void);
	};
}

#endif