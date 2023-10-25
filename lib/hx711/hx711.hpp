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