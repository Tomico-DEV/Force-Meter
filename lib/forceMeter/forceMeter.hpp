#include <Arduino.h>

#ifndef FOUREYED_FORCE_METER
#define FOUREYED_FORCE_METER

#include "foureyed_types.hpp"
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


			enum State
			{
				IDLE,
				RECORDING,
				CONFIG,
				PLAYBACK,
			};


			uint8_t m_serLight;
			uint8_t m_recLight;

			LoadCell * m_loadCell;
			MPU6050 *  m_accelerometer;
			Button<ForceMeterFn, ForceMeter> * m_captureButton;
			Button<ForceMeterFn, ForceMeter> * m_tareButton;
			Storage *  m_eeprom;
			MAX7219 *  m_sevenSeg;

			bool m_SerialPrev { false };
			State m_state 	  { IDLE };

			void checkSerial (void);
			void checkState  (void);
			void tare 		 (void);
			void capture 	 (void);
			void captureHold (void);
			void captureDblT (void);

		public:
			ForceMeter (
				uint8_t tareButton, uint8_t captureButton, // button
				uint8_t recordingLight, uint8_t serialLight, // lights
				uint8_t hx711DIN, uint8_t hx711CLK, // hx711
				uint8_t mpu6050ADDR, uint8_t accel_range, // mpu6050
				uint8_t eepromADDR, uint16_t eepromSize, // AT24C256
				uint8_t max7219CS // max7219
			);

			bool  m_capturing  	 { false };
			float m_loadReading  { 0.0f };
			accel m_accelReading { 0.0f, 0.0f, 0.0f };

			float m_loadTare { 0.0f };
			accel m_accelTare { 0.0f, 0.0f, 0.0f };

			void setup (void);
			void loop (void);
	};
}

#endif