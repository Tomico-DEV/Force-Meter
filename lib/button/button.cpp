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

/********************************* button.cpp **********************************
 * 
 * idk what to put here
 * 
*******************************************************************************/

#include "button.hpp"

using namespace fourEyed;


template <typename L, typename C>
Button<L, C>::Button (uint8_t buttonPin, bool prevState)
{
	m_pin = buttonPin;
	m_prevState = prevState;
}

template <typename L, typename C>
void Button<L, C>::call (L lambda)
{
	if ( lambda == NULL )
	{
		return;
	}

	if ( classPointer == NULL ) // no class
	{
		lambda (0);
		return;
	}

	lambda (classPointer);
}

template <typename L, typename C>
void Button<L, C>::setup (bool pullup)
{
	if (pullup)
	{
		pinMode (m_pin, INPUT_PULLUP);
	}
	else
	{
		pinMode (m_pin, INPUT);
	}
}

template <typename L, typename C>
void Button<L, C>::loop (void)
{
	m_prevState = m_state;

	m_state = digitalRead (m_pin);

	if ( m_state == HIGH ) // on
	{
		call (m_whenHigh);



		if ( m_prevState != m_state ) // on down
		{
			call (m_onDown);

			m_pressRegisterTime = millis () + pressWindow;

		}

		if ( millis () > m_pressRegisterTime ) // hold!
		{

			if ( !m_holding )
			{
				m_holding = true;
				call (m_onHold);
			}

			m_presses = 0;
		}

	}
	else // off
	{
		call (m_whenLow);

		if ( millis () > m_pressRegisterTime ) // let go or inactivity
		{
			uint8_t m_pressCopy = m_presses;
			m_presses = 0;

			switch (m_pressCopy)
			{
				case 1:
					call (m_onShortPress);
					break;
				/* not as snappy implementation of double press but
				allows for explandability to triple or quadruple press if you so desire!
				case 2:
					call (m_onDouble);
					break;
				*/
			}
		}
		
		if ( m_prevState != m_state ) // on up
		{
			call (m_onUp);

			m_holding = false;

			if ( millis () < m_pressRegisterTime )
			{
				m_presses++;

				// comment the part below out if you want to have triple
				// or quadruple taps or etc. See above
				if ( m_presses == 2 )
				{
					call (m_onDouble);
				}
			}
		}

	}

}