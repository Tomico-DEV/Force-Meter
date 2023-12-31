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

/********************************** hx711.hpp **********************************
 * 
 * Library for the HX711 load cell ADC
 * 
 * use setup (conversion factor) to set pinmode and conversion factor for bits
 * to newtons
 * 
 * use getReadingNewtons (bool waitForConversion) to get readings (Newtons)
 * 
 * if waitForConversion is true, the Arduino will wait for the HX711 to signal
 * the completion of its conversion. This depends on the HX711's sample rate 
 * and could take a while, so I suggest you set it to true and call it after 
 * everything else has been processed!
 * 
*******************************************************************************/

#include <Arduino.h>

#ifndef LOADCELL
#define LOADCELL

namespace fourEyed
{

	class LoadCell
	{
		private:
			int m_clockPin;
			int m_dataPin;
			float m_conversionFactor;
		
		public:
			LoadCell (int clockPin, int dataPin);
			
			void setup (float conversionFactor);
			long getRawReading (bool waitForConversion);
			float getReadingNewtons (bool waitForConversion);
	};

	}

#endif