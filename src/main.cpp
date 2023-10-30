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

/********************************** main.cpp ***********************************
 * 
 * Pins are defined here. You may reassign them and add extra functionality
 * Please refer to the README to see how things should be wired
 * 
*******************************************************************************/


#include <Arduino.h>

#include <Wire.h>
#include <SPI.h>

/* HX711 (LOAD CELL) */
#define LCLK 5 
// Clock pin

#define LDAT 4 
// Data pin


/* MPU6050 (ACCELLEROMETER) */
#define MPU_ADDR 0x68 
// (I2C) Address of MPU6050, changes to 0x69 if ADO is pulled down

#define MPU_ACCEL_RANGE 8
// Range of accelerometer. Available options (units are in gs or 9.8m/s^2): 2 4 8 16

#define MPU_SAMPLE_RATE 160
// Samples per second


/* MAX7219 (7 SEGMENT DISPLAY) */
#define SVN_CS 10
// Chip Select, SPI interface


/* EEPROM */
#define EEPROM_ADDR B01010000
// (I2C) Address of EEPROM

#define EEPROM_SIZE 32000 
// Size of eeprom, in bytes. not sure if 256Kbits means 32000 bytes or 32768 bytes


/* MISC */
#define CAPTURE_BUTTON 3 // record button
#define TARE_BUTTON	   2 // tare button
#define REC_LED		   7 // LED to indicate recording
#define SER_LED 	   6 // LED to indicate serial activity


#include "forceMeter.hpp"

fourEyed::ForceMeter forceMeter (
	TARE_BUTTON, CAPTURE_BUTTON,
	REC_LED, SER_LED,
	LDAT, LCLK,
	MPU_ADDR, MPU_ACCEL_RANGE,
	EEPROM_ADDR, EEPROM_SIZE,
	SVN_CS
);

void setup () {

	Wire.begin ();

	SPI.beginTransaction (SPISettings (10000000, MSBFIRST, SPI_MODE0));
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.begin ();

	Serial.begin (115200);

	Serial.println ("Hello world!");

	forceMeter.setup ();
}

void loop () {
	forceMeter.loop ();
}