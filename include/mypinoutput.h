#ifndef MYPINOUTPUT_H
#define MYPINOUTPUT_H

#include <pins_arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t u8;
typedef int8_t i8;
typedef uint32_t u32;


typedef struct {
    u8 dataPin, clockPin, latchPin;
} my595;

void dWrite(u8 pin, bool value);
bool dRead(u8 pin);
void dToggle(u8 pin);


#ifdef __cplusplus
}
#endif

#endif