#ifndef PINS_N_DEFINES

// Please refer to the README to study how things should be wired

/* HX711 LOAD CELL AMP */

#define LCLK 3 // Clock
#define LDAT 2 // Data


/* MPU6050 */

#define MPU_ADDR 0x68 // I2C interface
// Address of MPU6050, changes to 0x69 if ADO is pulled down
#define MPU_ACCEL_RANGE 8
// Range of accelerometer
// Available options (units are in gs or 9.8m/s^2): 2 4 8 16
#define MPU_SAMPLE_DIV 49 // sampling div = 8000 / sample rate - 1; sample rate = 160Hz

/* MAX7219 (7 SEGMENT DISPLAY)*/

#define SVN_CS 8 // Chip Select, SPI interface


#endif