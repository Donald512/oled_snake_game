#include <myi2c.h>




void i2c_init(){
    DDRC &= ~(1 << (A4 - A0));    // Make SDA output
    DDRC &= ~(1 << (A5 - A0));    // Make SCL output
    PORTC |= (1 << (A4 - A0)) | (1 << (A5 - A0));    // Enable pullup

}



void i2cWrite(u8 data){
    // make sure sda is output
	DDRC |= (1 << (A4 - A0));
    for (u8 i = 0; i < 8; i++){
        sclLow();
        _delay_loop_1(NS_TO_LOOP1(I2C_FAST_TLOW));
        sdaWrite(!!(data & (1 << 7)));
        _delay_loop_1(NS_TO_LOOP1(I2C_FAST_TSD));
        sclReleaseLine();
        data <<= 1;
        _delay_loop_1(NS_TO_LOOP1(I2C_FAST_THIGH));
    }
    if (readAck()); // ion really care if ACK or not // todo: check if any problems 
}

void i2cStart(){
    sclReleaseLine();  // make sure scl is high
    sdaLow();
    _delay_loop_1(NS_TO_LOOP1(I2C_FAST_THSTART));
    sclLow();
}

void i2cStop(){
    sdaLow();   // make sure sda already low 
    sclReleaseLine();  // scl goes high first
    _delay_loop_1(NS_TO_LOOP1(I2C_FAST_TSSTOP)); 
    sdaReleaseLine();
    _delay_loop_1(NS_TO_LOOP1(I2C_FAST_TIDLE));
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
    _delay_loop_1(NS_TO_LOOP1(I2C_FAST_TLOW));
	sdaReleaseLine();	// release line
	sclReleaseLine();
    _delay_loop_1(NS_TO_LOOP1(I2C_FAST_THIGH));
	bool isLow =  !(PINC & (1 << (A4 - A0)));
	sclLow();
	return isLow;
	// when clock is high, data is valid, when clock low, data is allwoed to chang
	// which is why this is read on the falling edge
}

