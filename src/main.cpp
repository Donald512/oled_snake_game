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

#define t_cycle 2500    // 1/2.5 * 10-6 = 400kHz the cycle shouldnt be faster than this
#define t_LOW 1250      
#define t_HIGH 1250     
// t_LOW + _tHIGH is 2500, these are not actually defined in the datasheet 
#define t_HSTART 600
#define t_SD    100 // Data setup time, SDA should be stable during this period
#define t_SSTOP 600
#define t_IDLE  1300    // time before new transmission can start 


// 31 is 62.5/2, to round up the number
#define NS_TO_CYCLES(ns) (((ns) + 31)/62.5) 
// convert cycles to the delay_loop_1 
// total cycles = 1 (to load the variable) + (3 * loop count)
// loop count = (total cycles - 1)/3
// 1 is 3/2, to round up the number 
#define CYCLES_TO_LOOP1(cycles) (((cycles) + 1)/3)
#define NS_TO_LOOP1(ns) CYCLES_TO_LOOP1(NS_TO_CYCLES(ns))


#include <Arduino.h>


void i2c_init();
void S1306_init();
void i2cWrite(u8 data);
void i2cStart();
void i2cStop();
void sdaReleaseLine();
void sdaLow();
void sclReleaseLine();
void sclLow();
void sdaWrite(u8 bit);
bool readAck();
void clearDisplay();
void updateDisplay();
void drawPixel(u8 x, u8 y, u8 color);




// #include <func.cpp>


void i2c_init(){
    pinMode(A4, INPUT_PULLUP);   // Enable SDA and SCL
    pinMode(A5, INPUT_PULLUP);
}

void S1306_init(){
    // i2c_init();
    i2cStart();
    i2cWrite(SSD_1306_ADDR1_WRITE);
    i2cWrite(CMD_MODE);


    i2cWrite(DISPLAY_OFF_CMD);
    // set oscillator freq  
    i2cWrite(FRQ_OUT_CMD);
    i2cWrite((8 << 4) | 0); // 0x80 so osc freq of 8, max is 15, (prescaler/divide ratio is 0 + 1 )// ! tweak osc freq
    // set MUX ratio
    i2cWrite(MUX_RATIO_CMD);
    i2cWrite(0x1F);     // mux ratio is 31 + 1 , full display enable

    
    // Set display offset
    i2cWrite(DISP_OFFSET_CMD);
    i2cWrite(0x0);  // display offset is 0, no vertical shifting
    // Set display start line
    i2cWrite(DISP_STRT_LINE_CMD | 0);   // 0 offset
    // enable charge pump since arduino can only provide 5v and it needs 7v to 15v, sacrifices current to boost voltage
    i2cWrite(CHARGE_PUMP_CMD);
    i2cWrite(0x14); // enable charge pump


    i2cWrite(ADDRESS_MODE_CMD);
    i2cWrite(HORIZ_ADDRESS_MODE_CMD);    
    // Set segment remap
    i2cWrite(SEG_REMAP_CMD | 0x01); 
    // set com output scan direction
    i2cWrite(COM_OUT_SCAN_DIR_CMD_NORMAL | 0x08);


    // set com pins hardware config
    i2cWrite(0xDA);
    i2cWrite(0x02);
    // set contrast
    i2cWrite(0x81);
    i2cWrite(127);  // 50% brightness, 0x75, goes from 0 to 255 // ! tweak contrast


    // Set precharge period 0xD9
    i2cWrite(0xD9);
    i2cWrite(0xF1);
    // Set V_com Deselect 0xDB
    i2cWrite(0xDB);
    i2cWrite(0x40);
    // disable entire display on
    i2cWrite(DISABLE_ENTIRE_DISPLAY_ON_CMD);
    // set normal display, 1 is on, 0 is off
    i2cWrite(NORMAL_DISPLAY_CMD);
    i2cWrite(DEACTIVATE_SCROLL_CMD);
    // display on
    i2cWrite(DISPLAY_ON_CMD);



    // i2cWrite(COLUMN_ADDRESS_CMD);
    // i2cWrite(0);    // start
    // i2cWrite(127);  // end
    
    // i2cWrite(PG_ADDRESS_CMD);
    // i2cWrite(0);
    // i2cWrite(7);


    
    i2cStop();
}

void i2cWrite(u8 data){
    // make sure sda is output
	DDRC |= (1 << (A4 - A0));
    for (u8 i = 0; i < 8; i++){
        sclLow();
        _delay_loop_1(NS_TO_LOOP1(t_LOW));
        sdaWrite(!!(data & (1 << 7)));
        _delay_loop_1(NS_TO_LOOP1(t_SD));
        sclReleaseLine();
        data <<= 1;
        _delay_loop_1(NS_TO_LOOP1(t_HIGH));
    }
    if (readAck()); // ion really care if ACK or not // todo: check if any problems 
}


void i2cStart(){
    sclReleaseLine();  // make sure scl is high
    sdaLow();
    _delay_loop_1(NS_TO_LOOP1(t_HSTART));
    sclLow();
}

void i2cStop(){
    sdaLow();   // make sure sda already low 
    sclReleaseLine();  // scl goes high first
    _delay_loop_1(NS_TO_LOOP1(t_SSTOP)); 
    sdaReleaseLine();
    _delay_loop_1(NS_TO_LOOP1(t_IDLE));
}

void sdaReleaseLine(){
	// Make input to release line and be pulled high
	DDRC &= ~(1 << (A4 - A0));
}

void sdaLow(){
	DDRC |= (1 << (A4 - A0));	// make output first so it can drive low
	PORTC &= ~(1 << (A4 - A0));
}

void sclReleaseLine(){
	// Make input to release line and be pulled high
	DDRC &= ~(1 << (A5 - A0));
}

void sclLow(){
	DDRC |= (1 << (A5 - A0));	// make output first so it can drive low
	PORTC &= ~(1 << (A5 - A0));
}

void sdaWrite(u8 bit){
	if (bit){
		sdaReleaseLine();
	}
	else{
		sdaLow();
	}
}

bool readAck(){
    sclLow();
    _delay_loop_1(NS_TO_LOOP1(t_LOW));
	sdaReleaseLine();	// release line
	sclReleaseLine();
    _delay_loop_1(NS_TO_LOOP1(t_HIGH));
	bool isLow =  !(PINC & (1 << (A4 - A0)));
	sclLow();
	return isLow;
	// when clock is high, data is valid, when clock low, data is allwoed to chang
	// which is why this is read on the falling edge
}


u8 globalBackbuffer[128][64 / 8] = {0};

void clearDisplay(){
    memset((void*)globalBackbuffer, 0, sizeof(u8) * 128 * 8);
}

void updateDisplay(){
    i2cStart();
    i2cWrite(SSD_1306_ADDR1_WRITE);
    i2cWrite(CMD_MODE);

    i2cWrite(PG_ADDRESS_CMD);   // triple byte
    i2cWrite(0);    // page start address
    i2cWrite(7);

    i2cWrite(COLUMN_ADDRESS_CMD);
    i2cWrite(0);
    i2cWrite(127);
    i2cStop();

    i2cStart();
    i2cWrite(SSD_1306_ADDR1_WRITE);
    i2cWrite(DATA_MODE);
    for (u8 y = 0; y < 8; y++){
        for (u8 x = 0; x < 128; x++){  // fill horizontally, column 0 to column 127
            i2cWrite(globalBackbuffer[x][y]);  
        }
    }
    i2cStop();
}


void drawPixel(u8 x, u8 y, u8 color){
    if (x > 127 || y > 63) return;
    u8 page = y/8;
    u8 bitPos = y % 8;
    
    if (color){
        globalBackbuffer[x][page] |= (1 << bitPos);
    }
    else{
        globalBackbuffer[x][page] &= ~(1 << bitPos);
    }

}

void setup(){
    i2c_init();
    S1306_init();
    updateDisplay();
    Serial.begin(9600);
}

u8 x = 0, y = 0;
u8 lastx = 0, lasty = 0;

u32 lastTime = millis();

void loop(){
    if (millis() - lastTime > 10){
        drawPixel(lastx, lasty, 0); 
        drawPixel(x, y, 1);

        lastx = x;
        lasty = y;
        x++;
        if (x > 127){
            x = 0;
            y++;
            if (y > 31){
                y = 0;
            }
        }
        lastTime = millis();
        updateDisplay();
        Serial.print(x);
        Serial.print(",");
        Serial.println(y);

    }
}
