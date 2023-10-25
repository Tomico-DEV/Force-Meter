#include <Arduino.h>

#ifndef FOUREYED_BUTTON
#define FOUREYED_BUTTON

namespace fourEyed
{
	// if you're going to use this without a class
	// set L to void (*) (uint_8 _);
	template <typename L, typename C = void>
	class Button
	{
		private:
			static const uint16_t pressWindow { 300 };

			unsigned long m_pressRegisterTime;

			uint8_t m_pin;
			bool m_prevState;

			void call (L lambda);

		public:
			Button (uint8_t buttonPin, bool prevState = LOW);

			uint8_t m_presses;
			bool m_holding = false;
			bool m_state;

			void setup (bool pullup = false);
			void loop  (void);

			C * classPointer 	= NULL;

			L (m_whenHigh) 	 	= NULL;
			L (m_whenLow) 		= NULL;
			L (m_onDown) 		= NULL;
			L (m_onUp)	 		= NULL;
			L (m_onShortPress)  = NULL;
			L (m_onHold) 		= NULL;
			L (m_onDouble) 	 	= NULL;
	};
}

#endif