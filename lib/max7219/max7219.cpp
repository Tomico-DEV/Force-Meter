// For more info on the registers and their values
// Go here: https://www.analog.com/media/en/technical-documentation/data-sheets/max7219-max7221.pdf

#include "max7219.hpp"

using namespace fourEyed;

MAX7219::MAX7219 (uint8_t chipSelectPin)
{
	m_chipSel = chipSelectPin;
}

void MAX7219::transfer (uint8_t address, uint8_t data)
{
	digitalWrite (m_chipSel, LOW);

	SPI.transfer (address);
	SPI.transfer (data);

	digitalWrite (m_chipSel, HIGH);
}

void MAX7219::setup (void)
{
	pinMode (m_chipSel, OUTPUT);
	// enter shutdown mode so we can program it
	transfer (0x0C, 0x00); 
	
	// set decode mode to no decode; each bit will
	// individually represent the state of a segment
	// refer to diagram in max7219.h!
	transfer (0x09, 0x00); 

	// full scan limit
	transfer (0x0B, 0x07);

	// low brightness
	transfer (0x0A, 0x00);

	// start normal operation!
	transfer (0x0C, 0x01);
}

void MAX7219::clear (void)
{
	for (uint8_t i = 1; i <= 8; i++)
	{
		transfer (i, 0);
	}
}

void MAX7219::showFloat (float num, uint8_t from, uint8_t to)
{
	// note: displays absolute value!
	if (num < 0)
	{
		num = -num;
	}

	// invariance
	from = constrain (from, 1, 8);
	to   = constrain (to, 1, 8);

	if ( from > to )
	{
		uint8_t temp = from;
		from = to;
		to = temp;
	}

	int inum = round (num * 100); // limit to first two decimal places

	for ( int i = from; i <= to; i++ )
	{
		uint8_t digit = ksvnNumbers[inum % 10];
		if ( i > from + 2 and inum == 0 ) // remove leading zeros
		{
			transfer (i, 0);
		}
		else if ( i == from + 2 ) // decimal place
		{
			transfer (i, digit | B10000000);
		}
		else
		{
			transfer (i, digit);
		}

		inum /= 10;
	}
}

void MAX7219::writeNum (uint8_t digit, uint8_t num)
{
	// invariance again!
	num = constrain (num, 0, 9);

	transfer (digit, ksvnNumbers[num]);
}

void MAX7219::writeChar (uint8_t digit, char c)
{
	// invariance!
	digit = constrain (digit, 1, 8);

	if ( !( ( c <= 'z' && c >= 'a' ) || ( c <= 'Z' && c >= 'A' ) ) )
	{
		transfer (digit, ksvnNumbers[SVN_DASH]);
	}

	// convert to uppercase
	if ( c >= 'a' )
	{
		c -= 20;
	}

	uint8_t charByte;

	switch (c)
	{
		// DEFINES ARE NOT IN THIS ORDER!
		case 'A':
			charByte = ksvnAlphabets[SVN_A];
			break;
		case 'B':
			charByte = ksvnAlphabets[SVN_B];
			break;
		case 'C':
			charByte = ksvnAlphabets[SVN_C];
			break;
		case 'D':
			charByte = ksvnAlphabets[SVN_D];
			break;
		case 'E':
			charByte = ksvnAlphabets[SVN_E];
			break;
		case 'F':
			charByte = ksvnAlphabets[SVN_F];
			break;
		case 'H':
			charByte = ksvnAlphabets[SVN_H];
			break;
		case 'L':
			charByte = ksvnAlphabets[SVN_L];
			break;
		case 'S':
			charByte = ksvnAlphabets[SVN_S];
			break;
		case 'U':
			charByte = ksvnAlphabets[SVN_U];
			break;
		case 'Y':
			charByte = ksvnAlphabets[SVN_Y];
			break;
		case '-':
			charByte = ksvnAlphabets[SVN_DASH];
			break;
		default:
			charByte = ksvnAlphabets[SVN_BLANK];
	}

	transfer (digit, charByte);
}
