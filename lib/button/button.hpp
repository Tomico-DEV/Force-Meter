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

/********************************* button.hpp **********************************
 * 
 * Adds extra functionality to button inputs, such as multiple taps and
 * hold detection
 * 
 * call Button.setup(pullup) for pinMode
 * call Button.loop() to poll for inputs
 * 
 * attach functions by setting Button.onSomeTrigger to a function pointer to
 * a void function accepting one pointer to class ot type C
 * (tip: use lambdas!)
 *  
 * This is done so the button can call member functions by
 * foo (classPointer)
 * {
 * 	classPointer->member_function ();
 * }
 * if the classPointer is not null. If it is, the button assumes
 * the attached function is non-member and will try to call it normally,
 * resulting in an error
 * 
*******************************************************************************/

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