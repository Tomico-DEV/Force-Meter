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

/********************************* max7219.hpp *********************************
 * 
 * 7 segment array driver. Uses SPI
 * 
 * use setup () to initialize
 * 
 * transfer (a, data) writes data to register with address a
 * with the current configuration, when a is the number of a digit,
 * each bit in data will determine the state of a segment in that
 * digit. Refer to the diagram below
 * 
 * NOTE: digits ARE 1-INDEXED! (1-8) THEY DO NOT START AT 0
 * 
 * Diagram:
 * 
 *  AAAA
 * F    B
 * F    B
 * F    B
 *  GGGG
 * E    C
 * E    C
 * E    C
 *  DDDD  DP
 *
 * The bits in data (written as a binary numeber - begins with B) 
 * represents: DP A B C D E F G
 * 
 * so transfer (1, B01111011) will display 9 at digit 1
 * 
 * I'd advise you to use writeNum or writeChar, they are much more intuitive
 * 
 * showFloat (num, a, b) displays a float num of two decimal places from digits
 * a to b. Everything is truncated
 * 
 * clear () writes all digits with B00000000
 * 
 * see the SVN defines, kSvnAlphabets, and writeChar if you want to add
 * extra characters
 * 
*******************************************************************************/

#include <Arduino.h>

#ifndef FOUREYED_MAX7219
#define FOUREYED_MAX7219

#include <SPI.h>

#define SVN_BLANK 0
#define SVN_A     1
#define SVN_B     2
#define SVN_C     3
#define SVN_D     4
#define SVN_E     5
#define SVN_F     6
#define SVN_L     7
#define SVN_S     8
#define SVN_Y     9
#define SVN_DASH  10
#define SVN_H	  11
#define SVN_U     12
// These values represent corresponding indexes for alphabetic
// characets in the ksvnAlphabets[] array

namespace fourEyed 
{
	// font for characters
  	const uint8_t ksvnAlphabets[] = {
		B00000000,
		B01110111,
		B00011111,
		B01001110,
		B00111101,
		B01001111,
		B01000111,
		B00001110,
		B01011011,
		B00100111,
		B00000001,
		B00110111,
		B00111110
  	};

	// font for numbers
	const uint8_t ksvnNumbers[] = {
		B01111110, // 0
		B00110000, // 1
		B01101101, // 2
		B01111001, // 3
		B00110011, // 4
		B01011011, // 5
		B01011111, // 6
		B01110000, // 7
		B01111111, // 8
		B01111011, // 9
	};

  class MAX7219
  {
	private:
		uint8_t m_chipSel;
	public:
		MAX7219 (uint8_t chipSelectPin);

		void transfer  (uint8_t address, uint8_t data);
		void setup     (void);
		void clear     (void);
		void showFloat (float num, uint8_t from, uint8_t to);
		void writeNum  (uint8_t digit, uint8_t num);
		void writeChar (uint8_t digit, char c);
  };
}

#endif