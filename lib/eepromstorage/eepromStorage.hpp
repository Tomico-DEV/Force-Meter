#include <Arduino.h>

/*

How storage is used:

 0x0000.........................m_size - 1
   |  whatever data   | m_configsize |

*/

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