#include "func.h"

void msbShiftOut(my595 &shiftReg, u8 pattern){

	for (uint8_t i = 0; i < 8; i++){		
		if (shiftReg.dataPin < 8){
			if ((pattern & (1 << 7)) != 0) PORTD |= (1 << shiftReg.dataPin);
			else PORTD &= ~(1 << shiftReg.dataPin);	
		}
		else if (shiftReg.dataPin < 14){
			if ((pattern & (1 << 7)) != 0) PORTB |= (1 << (shiftReg.dataPin - 8));
			else PORTB &= ~(1 << (shiftReg.dataPin - 8));
		}
		else if (shiftReg.dataPin < A6){
			if ((pattern & (1 << 7)) != 0) PORTC |= (1 << (shiftReg.dataPin - A0));
			else PORTC &= ~(1 << (shiftReg.dataPin - 8));
		}
		else return;
 
		pattern = pattern << 1;

		if (shiftReg.clockPin < 8) {
			PORTD |= (1 << shiftReg.clockPin);
			PORTD &= ~(1 << shiftReg.clockPin);
		}
		else if (shiftReg.clockPin < 14){
			PORTB |= (1 << (shiftReg.clockPin - 8));
			PORTB &= ~(1 << (shiftReg.clockPin - 8));
		}
		else {
			PORTC |= (1 << (shiftReg.clockPin - A0));
			PORTC &= ~(1 << (shiftReg.clockPin - A0));
		}
	}

}

void dWrite(u8 pin, bool value){
	if (pin > A5) return;
	if (pin < 8){
		if (value) 	PORTD |= ( 1 << pin);
		else 		PORTD &= ~(1 << pin);
	}
	else if (pin < 14){
		pin -= 8;
		if (value) 	PORTB |= ( 1 << (pin));
		else 		PORTB &= ~(1 << (pin));
	}
	else{
		pin -= A0;
		if (value) 	PORTC |= ( 1 << (pin));
		else 		PORTC &= ~(1 << (pin));
	} 
}

bool dRead(uint8_t pin){
	if (pin > A5)	return LOW;
	if (pin < 8)	return (PIND & (1 << pin)) != LOW;
	else if (pin < A0) return (PINB & (1 << (pin - 8))) != LOW;
	else return (PINC & (1 << (pin - A0))) != LOW;
}

void dToggle(u8 pin){
	if (pin > A5) return;
	if (pin < 8){
		PORTD ^= (1 << pin);
	}
	else if (pin < 14){
		pin -= 8;
		PORTB ^= ( 1 << (pin));
	}
	else{
		pin -= A0;
		PORTC ^= ( 1 << (pin));
	} 
}

void sdaHigh(){
	// Make input to release line and be pulled high
	DDRC &= ~(1 << (A4 - A0));
}

void sdaLow(){
	DDRC |= (1 << (A4 - A0));	// make output first so it can drive low
	PORTC &= ~(1 << (A4 - A0));
}

void sclHigh(){
	// Make input to release line and be pulled high
	DDRC &= ~(1 << (A5 - A0));
}

void sclLow(){
	DDRC |= (1 << (A5 - A0));	// make output first so it can drive low
	PORTC &= ~(1 << (A5 - A0));
}

void sdaWrite(u8 bit){
	if (bit){
		sdaHigh();
	}
	else{
		sdaLow();
	}
}


bool readAck(){
	sdaHigh();	// release line
	sclHigh();
	bool isLow =  !(PINC & (1 << (A4 - A0)));
	sclLow();
	return isLow;
	// when clock is high, data is valid, when clock low, data is allwoed to chang
	// which is why this is read on the falling edge
}