<h>Embedded snake game | AVR Bit-Banged I2C & SSD1306 Driver</h>

A fully from-scratch Snake game for the ATmega328P. No Wire.h, no digitalWrite(), no graphic libraries.


Continuation of my other repo, win32 snake, which I built to first understand how the snake game and pattern works. 
Built it on windows first because it is easier to debug on laptop. Just change a variable and hit compile and see the results, but on the hardware, you don't know if it's a floating pin, NACK problem or logic problem, so, building on Windows was to make sure the game worked. 


I had to change a few things though, like changing most of my bool32 or u32 (for memory efficency on modern CPUs) to u8's (due to the scarce storage on MCU). 


https://github.com/user-attachments/assets/4002135c-00d2-4ca0-8b6c-5727dfdab71e



<img width="620" height="472" alt="image" src="https://github.com/user-attachments/assets/c393f947-44d3-49a8-9a43-e6fcc925c3ce" />

I built this as a hobby project - now I have a retro, nostalgic snake game - but also to understand how I2C communication and display drivers work. 
The i2c driver was built and debugged with my logic analyzer to ensure it worked as it should.
<img width="1737" height="757" alt="b88e7c55-78c7-4969-9242-db5eab81f25b_image" src="https://github.com/user-attachments/assets/532f66d9-9374-4d69-ae05-9dff84ebc7b2" />
The SSD1306 driver was built by using the datasheet, and referencing the Adafruit's version when I was stuck.

  - main.cpp          -> Actual game
  - myssd1306.cpp     -> Display init, pixel buffer, and display logic
  - my12c.cpp         -> Bit-banged I2C master (start, stop, write, ack)
  - mybtn.cpp         -> Button debouncing logic
  - mypinoutput.cpp   -> Direct port control (Replaces digitalRead())

Challenges:
- I2C NACKS on init. Caused by wrong I2C logic. Forgot the rule that SDA is only allowed to change when SCL is LOW.
<img width="1402" height="193" alt="375338d6-a260-4e5a-b70e-4250428c2086_image" src="https://github.com/user-attachments/assets/831027af-0999-4def-897d-037d80099deb" />

- Some display driver issues which I fixed by looking at how the Adafruit library works.

