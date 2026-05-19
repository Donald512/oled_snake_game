#include <myssd1306.h>


u8 globalBackbuffer[128][64 / 8] = {0};

void S1306_init(){
    i2c_init();
    i2cStart();
    i2cWrite(SSD_1306_ADDR1_WRITE);
    i2cWrite(CMD_MODE);


    i2cWrite(DISPLAY_OFF_CMD);
    // set oscillator freq  
    i2cWrite(FRQ_OUT_CMD);
    i2cWrite((8 << 4) | 0); // 0x80 so osc freq of 8, max is 15, (prescaler/divide ratio is 0 + 1 )// ! tweak osc freq
    // set MUX ratio
    i2cWrite(MUX_RATIO_CMD);
    // i2cWrite(0x1F);     // mux ratio is 31 + 1 , half display enable
    i2cWrite(0x3F);     // mux ratio is 63 + 1 , full display enable

    
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
    i2cWrite(0x12);     // ! Using alternative layout, 0x02 made my pixels rectanfular instead of square
    // i2cWrite(0x02); 

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

    
    i2cStop();
}


void clearDisplay(){
    memset((void*)globalBackbuffer, 0, sizeof(globalBackbuffer));
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
