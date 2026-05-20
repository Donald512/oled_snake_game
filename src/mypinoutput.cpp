#include "mypinoutput.h"

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

bool dRead(u8 pin){
	if (pin > A5)	return false;
	if (pin < 8)	return (PIND & (1 << pin)) != false;
	else if (pin < A0) return (PINB & (1 << (pin - 8))) != false;
	else return (PINC & (1 << (pin - A0))) != false;
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
