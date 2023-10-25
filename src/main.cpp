
/************************************
  4eyedMan, 2023
  
  MIT license.

  if your stuff goes boom boom because 
  of this, don't blame me!

  you can use this in your products idc

*************************************/
#include <Arduino.h>

#include <Wire.h>
#include <SPI.h>

// Please refer to the README to study how things should be wired

/* HX711 LOAD CELL AMP */
#define LCLK 5 // Clock
#define LDAT 4 // Data


/* MPU6050 */
#define MPU_ADDR 0x68 
// I2C interface
// Address of MPU6050, changes to 0x69 if ADO is pulled down

#define MPU_ACCEL_RANGE 8
// Range of accelerometer
// Available options (units are in gs or 9.8m/s^2): 2 4 8 16

#define MPU_SAMPLE_RATE 160


/* MAX7219 (7 SEGMENT DISPLAY)*/
#define SVN_CS 10
// Chip Select, SPI interface


/* EEPROM */
#define EEPROM_ADDR B01010000
#define EEPROM_SIZE 32000 //32768
// Address, I2C

/* MISC */
#define CAPTURE_BUTTON 3
#define TARE_BUTTON	   2
#define REC_LED		   6
#define SER_LED 	   7


#include "forceMeter.hpp"
//#include "foureyed_types.h"
//#include "hx711.h"
//#include "mpu6050.h"
//#include "max7219.hpp"
//#include "button.hpp"
//#include "eepromStorage.hpp"

//fourEyed::Storage eeprom (EEPROM_ADDR, 32768, 4);
/*

fourEyed::MPU6050 accelerometer (
	MPU_ADDR, 
	MPU_SAMPLE_RATE, 
	MPU_ACCEL_RANGE
	);
fourEyed::LoadCell loadCell (LCLK, LDAT);

accel accelMeasurement { 0, 0, 0 };

*/

// fourEyed::MAX7219 sevenSeg (SVN_CS);

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
	//put your main code here, to run repeatedly:
}