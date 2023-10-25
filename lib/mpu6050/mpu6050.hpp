#include <Arduino.h>

#ifndef FOUREYED_6050
#define FOUREYED_6050

#include <Wire.h> // I2C
#include "foureyed_types.hpp"

namespace fourEyed 
{
	class MPU6050
	{
		private:
			float m_newtonPerBits;
			uint8_t m_address;
			uint8_t m_sampleRangeVal;
			uint8_t m_sampleDiv;

			void communicate (uint8_t address, uint8_t data);

		public:
			MPU6050 (
				uint8_t address, 
				uint8_t sampleRate, 
				uint8_t sampleRange
			);

			void 	setup (void);
			void 	getAccel (accel& result);
	};

}
#endif