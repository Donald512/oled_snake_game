#ifndef MYBTN_H
#define MYBTN_H

#include <Arduino.h> // for pinMode()
#include <mypinoutput.h>   // for dRead()
#include <stdint.h>

#define u32 uint32_t
#define u8 uint8_t

#define globalVariable static

class Button{
    private:
        u32 lastCheckedTime = 0;
        bool lastCheckedState = HIGH;
        bool pressEvent = false; 
        u8 debounceDelay = 50;  // will make this public if case requires it 
    public:
        u8 pin;
        
        
    Button(u8 p);
    void watch(u32 now);
    bool wasPressed();
};



#ifdef __cplusplus

#endif

#endif