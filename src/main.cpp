#include <myssd1306.h>
#include <wiring.c> // for millis()



void setup(){
    S1306_init();
    updateDisplay();
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
