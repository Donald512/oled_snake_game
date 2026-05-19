#ifndef MYSSD1306_H
#define MYSSD1306_H

#define SSD_1306_ADDR1 0b0111100
#define SSD_1306_ADDR2 0b0111101

#define SA0      (1 << 1)
#define READ     (1 << 0)
#define WRITE    (0 << 0)

#define SSD_1306_ADDR1_READ  ((SSD_1306_ADDR1 << 1) | READ)
#define SSD_1306_ADDR1_WRITE ((SSD_1306_ADDR1 << 1) | WRITE)
// #define SSD_1306_ADDR1_WRITE 0x3D // !  test

#define SSD_1306_ADDR2_READ  ((SSD_1306_ADDR2 << 1) | READ)
#define SSD_1306_ADDR2_WRITE ((SSD_1306_ADDR2 << 1) | WRITE)


#define DISPLAY_TEST_CMD 0xA5
#define DISPLAY_ENDTEST_CMD 0xA4

#define INVERSE_DISPLAY_CMD 0xA7

#define DISPLAY_OFF_CMD 0xAE
#define DISPLAY_ON_CMD 0xAF

// ! Scroll instructions pg 28 

// to set the 

//  Init stuff
#define FRQ_OUT_CMD 0xD5
// clock divide ratio can be set by A[3:0], divide ratio can also be thought of as prescaler
// osc freq can be set by A[7:4]
#define MUX_RATIO_CMD 0xA8
#define DISP_OFFSET_CMD 0xD3
// display offset is set with A[5:0]
#define DISP_STRT_LINE_CMD 0x40
#define SEG_REMAP_CMD 0xA0  // address 0 is mapped to SEG0
#define COM_OUT_SCAN_DIR_CMD_NORMAL 0xC0
#define CONTRAST_CMD 0x81 
#define DISABLE_ENTIRE_DISPLAY_ON_CMD 0xA4
#define NORMAL_DISPLAY_CMD 0xA6
#define CHARGE_PUMP_CMD 0x8D
#define DEACTIVATE_SCROLL_CMD 0x2E

#define ADDRESS_MODE_CMD 0x20
#define HORIZ_ADDRESS_MODE_CMD 0x0

#define CMD_MODE 0x00
#define DATA_MODE 0x40

#define COLUMN_ADDRESS_CMD 0x21 // used to set start and end
#define PG_ADDRESS_CMD  0x22



// #define t_cycle 2500    // 1/2.5 * 10-6 = 400kHz the cycle shouldnt be faster than this
// #define t_LOW 1250      
// #define t_HIGH 1250     
// // t_LOW + _tHIGH is 2500, these are not actually defined in the datasheet 
// #define t_HSTART 600
// #define t_SD    100 // Data setup time, SDA should be stable during this period
// #define t_SSTOP 600
// #define t_IDLE  1300    // time before new transmission can start 




#include <myi2c.h>
#include <string.h> // for memset

void S1306_init();
void clearDisplay();
void updateDisplay();
void drawPixel(u8 x, u8 y, u8 color);


#endif