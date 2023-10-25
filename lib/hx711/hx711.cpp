#include "hx711.hpp"

using namespace fourEyed;

LoadCell::LoadCell (int clockPin, int dataPin)
{
	m_clockPin = clockPin;
	m_dataPin  = dataPin;
}

void LoadCell::setup (float conversionFactor)
{
	pinMode (m_clockPin, OUTPUT);
	pinMode (m_dataPin,  INPUT);
	m_conversionFactor = conversionFactor;
}

long LoadCell::getRawReading (bool waitForConversion)
{
	if ( waitForConversion )
	{
		while ( digitalRead (m_dataPin) ) ; // wait for conversion period to complete
	}

	digitalWrite (m_clockPin, LOW); // clock pin should already be low but just in case!

	/* READING DIN */
	long out { 0L };

	// two's complement check

	digitalWrite (m_clockPin, HIGH); // shift first bit in

	if ( digitalRead (m_dataPin) )	 // if high, the value is negative!
	{
		out = ~(0L); // set all bits to 1
	}

	digitalWrite (m_clockPin, LOW);

	// shift the rest in

	for ( uint8_t i = 0; i < 23; i++ )
	{
		digitalWrite (m_clockPin, HIGH);

		out = (out << 1) | digitalRead (m_dataPin);

		digitalWrite (m_clockPin, LOW);
	}

	// send a clock pulse to let the HX711 begin its next conversion
	digitalWrite (m_clockPin, HIGH);
	digitalWrite (m_clockPin, LOW);

	return out;
}

float LoadCell::getReadingNewtons (bool waitForConversion)
{
	return (float)(getRawReading (waitForConversion) / m_conversionFactor );
}
