#ifndef FOUREYEDTYPES
#define FOUREYEDTYPES

#include <Arduino.h>

struct accel
{
	float x;
	float y;
	float z;

	void print (void) // only for debug
	{
		Serial.print (x);
		Serial.print ("\t");
		Serial.print (y);
		Serial.print ("\t");
		Serial.print (z);
		Serial.print ("\n");
	}
};

#endif