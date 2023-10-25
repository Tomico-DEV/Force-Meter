// For more info on the registers and their values
// Go here: https://cdn.sparkfun.com/datasheets/Sensors/Accelerometers/RM-MPU-6000A.pdf

#include "mpu6050.hpp"

using namespace fourEyed;

MPU6050::MPU6050 (uint8_t address, uint8_t sampleRate, uint8_t sampleRange)
{
	m_address = address;

	// find sample rate divider

	if ( sampleRate == 0 )
	{
		Serial.println ("WARNING: sampleRate is 0, defaulting to 160!");
		sampleRate = 160u;
	}

	m_sampleDiv = round (8000.0f / sampleRate) - 1;

	// get appropriate hex value for accel range
	switch (sampleRange)
	{
	case 2:
		m_sampleRangeVal = 000;
		m_newtonPerBits  = 1671.8367f;
		break;
	case 4:
		m_sampleRangeVal = 010;
		m_newtonPerBits  = 835.9183f;
		break;
	case 8:
		m_sampleRangeVal = 020;
		m_newtonPerBits  = 417.9591f;
		break;
	default:
		Serial.println (
			"WARNING: specified accel sample range does not match "
			"(+-) 2, 4, 8, or 16. Defaulting to 16!"
			);
	case 16:
		m_sampleRangeVal = 030;
		m_newtonPerBits  = 208.9795f;
		break;
	}
}

void MPU6050::communicate (uint8_t address, uint8_t data)
{
	Wire.beginTransmission 	(m_address);
	Wire.write 				(address);
	Wire.write 				(data);
	Wire.endTransmission 	(true);
}

void MPU6050::setup (void)
{
	// reset the MPU6050 by filling the 0x6B reg with 0s
	MPU6050::communicate (0x6B, 0x00); 

	// set sample rate divisor
	MPU6050::communicate (0x19, m_sampleDiv);

	// set acceleration sampling range
	MPU6050::communicate (0x1C, m_sampleRangeVal);
}

void MPU6050::getAccel (accel& result)
{
	Wire.beginTransmission (m_address);
	Wire.write (0x3B); // access accel readings register
	Wire.endTransmission (true);

	// get values of 6 following registers
	Wire.requestFrom (m_address, (uint8_t) 6, (uint8_t) true);

	result = 
	{
		.x = ((Wire.read() << 8 | Wire.read()) / m_newtonPerBits),
		.y = ((Wire.read() << 8 | Wire.read()) / m_newtonPerBits),
		.z = ((Wire.read() << 8 | Wire.read()) / m_newtonPerBits)	
	};
}