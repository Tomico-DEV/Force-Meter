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

// NOTE: digits ARE 1-INDEXED! THEY DO NOT START AT 0

/*
Diagram:

   AAAA
  F    B
  F    B
  F    B
   GGGG
  E    C
  E    C
  E    C
   DDDD  DP

The binary number (begins with B) represents: DP A B C D E F G
*/


namespace fourEyed 
{

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