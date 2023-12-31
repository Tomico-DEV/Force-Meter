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

/***************************** eepromStorage.hpp ********************************
 * 
 * EEPROM storage interface. Uses I2C
 * 
 * No setup is required. Just make sure I2C is running and keep in mind that
 * the eeprom takes time to write data (max 5 ms) and won't accept commands
 * during the time frame
 * 
 * How storage is used: (note - units are bytes, not bits!)
 * 0x0000..............m_size - 1 - m_confidSize.........m_size - 1
 * |  whatever data             |      configuration data      |
 * 
 * I think the functions are pretty self-explanatory
 * 
*******************************************************************************/

#include <Arduino.h>

#ifndef FOUREYED_EEPROMSTORE
#define FOUREYED_EEPROMSTORE

#include <Wire.h>

namespace fourEyed
{
	class Storage
	{
		private:
			uint8_t 	m_address;

			void writeNoWarn (uint16_t address, uint8_t size, byte * data);
			void readNoWarn	 (uint16_t address, uint8_t size, byte * res);

		public:
			Storage (uint8_t address, uint16_t sizeBytes, uint8_t configSizeBytes);
			
			uint16_t 	m_size; // in bytes, up to 512K bits (64kbytes)
			uint8_t 	m_configsize; // size of space allocated to config, in bytes

			void write 		 (uint16_t address, uint8_t size, byte * data);
			void read  		 (uint16_t address, uint8_t size, byte * buffer);
			void getConfig 	 (uint8_t  config,  uint8_t size, byte * buffer);
			void saveConfig  (uint8_t  config,  uint8_t size, byte * data);
	};
} // namespace fourEyed

#endif