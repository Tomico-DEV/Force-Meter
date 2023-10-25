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

/***************************** eepromStorage.cpp ********************************
 * 
 * EEPROM storage interface functionality
 * 
 * write and read are all done in page/sequential mode
 * 
 * implementation of this 
 * http://ww1.microchip.com/downloads/en/devicedoc/atmel-8568-seeprom-at24c256c-datasheet.pdf
 * 
*******************************************************************************/



#include "eepromStorage.hpp"


using namespace fourEyed;

const char kTwrError[]		= "ERROR:  Can't write to EEPROM! EEPROM might still be writing";
const char kRwBreachWarn[]  = "WARNING: read/write size breaches config space!";

Storage::Storage (uint8_t address, uint16_t sizeBytes, uint8_t configSizeBytes)
{
	m_address = address;
	m_size = sizeBytes;
	m_configsize = configSizeBytes;
}

void Storage::writeNoWarn (uint16_t address, uint8_t size, byte * data)
{
	Wire.beginTransmission (m_address);
	
	// write address of data
	if ( !Wire.write ((uint8_t)(address >> 8)) ) // first 8 bits
	{
		Serial.println (kTwrError);
	}
	Wire.write ((uint8_t)(address & 0xFF)); // last 8 bits

	// write data one byte at a time

	for ( int i = 0; i < size; i++ )
	{
		Wire.write (data[i]);
	}

	// stop condition
	Wire.endTransmission (true);
}

void Storage::readNoWarn (uint16_t address, uint8_t size, byte * buffer)
{
	Wire.beginTransmission (m_address);

	// write address of data
	Wire.write ((uint8_t)(address >> 8)); // first 8 bits
	Wire.write ((uint8_t)(address & 0xFF)); // last 8 bits

	// false start to initiate data transfer
	Wire.endTransmission (false);
	if ( !Wire.requestFrom (m_address, size, (uint8_t)true) )
	{
		Serial.println (kTwrError);
	}

	for ( uint8_t i = 0; i < size; i++ )
	{
		if (Wire.available()) buffer[i] = Wire.read();
	}
}

void Storage::write (uint16_t address, uint8_t size, byte * data)
{
	if ( address + (uint16_t)size > m_size - 1 - m_configsize )
	{
		Serial.println (kRwBreachWarn);
	}
	writeNoWarn (address, size, data);
}

void Storage::read (uint16_t address, uint8_t size, byte * buffer)
{
	if ( address + (uint16_t)size > m_size - 1 - m_configsize )
	{
		Serial.println (kRwBreachWarn);
	}
	readNoWarn (address, size, buffer);
}

void Storage::getConfig (uint8_t config, uint8_t size, byte * buffer)
{
	uint16_t configAddr = m_size - 1 - m_configsize + config;

	readNoWarn (configAddr, size, buffer);
}

void Storage::saveConfig (uint8_t config, uint8_t size, byte * data)
{
	uint16_t configAddr = m_size - 1 - m_configsize + config;

	writeNoWarn (configAddr, size, data);
}

