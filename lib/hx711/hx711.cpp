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

/********************************** hx711.cpp **********************************
 * 
 * implementation of 
 * https://cdn.sparkfun.com/datasheets/Sensors/ForceFlex/hx711_english.pdf
 * 
*******************************************************************************/

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
