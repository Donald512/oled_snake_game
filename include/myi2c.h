#ifndef MYI2C_H
#define MYI2C_H

// 31 is 62.5/2, to round up the number
#define NS_TO_CYCLES(ns) (((ns) + 31)/62.5) 
// convert cycles to the delay_loop_1 
// total cycles = 1 (to load the variable) + (3 * loop count)
// loop count = (total cycles - 1)/3
// 1 is 3/2, to round up the number 
#define CYCLES_TO_LOOP1(cycles) (((cycles) + 1)/3)
#define NS_TO_LOOP1(ns) CYCLES_TO_LOOP1(NS_TO_CYCLES(ns))

#define I2C_FAST_TLOW 1250 // ns
#define I2C_FAST_THIGH  1250  // ns
#define I2C_FAST_TSD    100   // ns Data setup time
#define I2C_FAST_THSTART 600  // ns Hold time for start condition
#define I2C_FAST_TSSTOP  600  // ns Setup time for stop condition
#define I2C_FAST_TIDLE  1300

#include <pins_arduino.h>
#include <util/delay_basic.h>   // for _delay_loop_1



typedef uint8_t u8;
void i2c_init();
void i2cWrite(u8 data);
void i2cStart();
void i2cStop();
void sdaReleaseLine();
void sdaLow();
void sclReleaseLine();
void sclLow(); 
void sdaWrite(u8 bit);
bool readAck();


#endif