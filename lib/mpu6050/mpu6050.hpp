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

/********************************* mpu6050.hpp *********************************
 * 
 * Library for mpu6050 gyroscope and accelerometer. Uses I2C.
 * 
 * Only supports acceleration readings for now. 
 * Readings are stored in fourEyed::Accel structs
 * 
 * use setup () to initialize
 * use getAccel (res) to store readings
 * 
 * see main.cpp for details on configuration
 * 
*******************************************************************************/

#include <Arduino.h>

#ifndef FOUREYED_6050
#define FOUREYED_6050

#include <Wire.h>
#include "foureyedTypes.hpp"

namespace fourEyed 
{
	class MPU6050
	{
		private:
			float m_accelPerBits;
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