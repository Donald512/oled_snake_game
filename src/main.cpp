#include <myssd1306.h>
#include <stdbool.h>
#include <mybtn.h>
#include <Arduino.h>    // for random and millis()


#define rightWall 127
#define bottomWall 63

#define size 4

#define maxNumPartsSnake (rightWall/size * bottomWall/size)
#define maxNumPartsSnake 100    // overwrites but done to not overallocate

#define black 0
#define white 1

#define numBtns 4

#define _left 5
#define _up 6
#define _down 7
#define _right 9

typedef uint32_t u32;
typedef int8_t i8;
// typedef uint16_t u16;

typedef struct{
    u8 x;
    u8 y;
}   squareShape;

typedef struct{
    i8 right;
    i8 down;
}   direction;

u32 snakeSize = maxNumPartsSnake * sizeof(squareShape);

void drawSquareShape(squareShape sqr);
void eraseSquareShape(squareShape sqr);
bool addStructs(squareShape head[], direction dir);
void createSnake();
void drawSnake(squareShape snake[], u16 length);
void moveSnake();
void generateApple();
void drawApple();
bool headOnApple();
void changeDir(u8 pin);
void drawBorder();

direction headDir = {0, 0};
u16 currentLevel = 1;
squareShape apple;
squareShape snake[maxNumPartsSnake] = {0};

u8 gameSpeedMs = 200;

Button moveBtns[numBtns] = {Button(_left), Button(_up), Button(_down), Button(_right)};    // alloc space for 4 buttons
bool RUNNING;

void setup(){
    S1306_init();
    
    randomSeed(analogRead(A0));
    // ! show game logo
    // delay(2000); // Pause for 2 seconds
    clearDisplay();
    createSnake();
    generateApple();
    RUNNING = true;

    // while (true){
    //     drawSquareShape((squareShape){64, 16});
    //     updateDisplay();
    // }
    Serial.begin(9600);
}


u32 lastTime = millis();


void loop(){
    if (RUNNING){
        u32 now = millis();
        for (u8 i = 0; i < numBtns; i++){
            moveBtns[i].watch(now);
            if (moveBtns[i].wasPressed()){
                changeDir(moveBtns[i].pin);
            }
        }
        if (now - lastTime > gameSpeedMs){
            if (headOnApple()){
                currentLevel++;
                eraseSquareShape(apple);
                generateApple();
            }
            eraseSquareShape(snake[currentLevel - 1]);
            drawBorder();
            moveSnake();
            lastTime = now;
            updateDisplay();
        }
        Serial.print(snake[0].x);
        Serial.print(", ");
        Serial.println(snake[0].y);
    }
    else{
        currentLevel = 1;
        clearDisplay();
        createSnake();
        generateApple();
        headDir.down = 0;
        headDir.right = 0;
        RUNNING = true;
        updateDisplay();
    }

}

void changeDir(u8 pin){
    switch (pin){
        case _up:{
            if (headDir.down <= 0){  // cant move in opposite direction blocker
                headDir.right = 0;
                headDir.down = -size;
            }
        }   break;
        case _down:{
            if (headDir.down >= 0){ 
                headDir.right = 0;
                headDir.down = size;
            }
        }   break;
        case _left:{
            if (headDir.right <= 0){ 
                headDir.right = -size;
                headDir.down = 0;
            }
        }   break;
        case _right:{
            if (headDir.right >= 0){ 
                headDir.right = size;
                headDir.down = 0;
            }
        }   break;        
    }

}

void drawSquareShape(squareShape sqr){

    for (u8 i = sqr.x; i < sqr.x + 1*size; i++ ){
        for (u8 j = sqr.y; j < sqr.y + size; j++){
            drawPixel(i, j, white);
        }
    }
}

void eraseSquareShape(squareShape sqr){

    for (u8 i = sqr.x; i < sqr.x + 1*size; i++){
        for (u8 j = sqr.y; j < sqr.y + size; j++ ){
            drawPixel(i, j, black);
        }
    }
}

void drawSnake(squareShape snake[], u16 length){
    for (u16 i = 0; i < length; i++){
        drawSquareShape(snake[i]);
    }
}

bool addStructs(squareShape head[], direction dir){
    i8 resultX = head->x + dir.right;
    i8 resultY = head->y + dir.down;
    if (dir.right){
        if (resultX >= 0 && resultX <= rightWall + 1 - size){
            head->x = resultX;
        }
        else {
            Serial.println(resultX);
            return false;
        }
    }
    else if (dir.down){
        if (resultY >= 0 && resultY <= bottomWall + 1 - size ){     // eg if size is 4, 127 - 4 = 123, 123, 124, 125, 126, skips 127, so have to shift it up 1 
            head->y = resultY;
        }
        else {
            Serial.print("head y position: ");
            Serial.print(head->y);
            Serial.print(", Direction down: ");
            Serial.println(dir.down);
            return false;
        }
    }
    return true;
}


void createSnake(){
    snake[0].x = random(0, (rightWall/size) + 1 ) * size;  // makes sure its a multiple of size, also better if size is a factor of rightwall - 1 (0 indexed)
    snake[0].y = random(0, (bottomWall/size) + 1) * size; 
    drawSquareShape(snake[0]);
}



void moveSnake(){
    squareShape headCopy = snake[0];
    if (!addStructs(&headCopy, headDir)){
        // Serial.print(headCopy.x);
        // Serial.println(", ");
        // Serial.println(headCopy.y);
            Serial.print("Died at: ");
            Serial.print(headCopy.x);
            Serial.print(",");
            Serial.println(headCopy.y);
            // Add a loop to print the whole snake array here
            for(int i=0; i<currentLevel; i++) {
                Serial.print("Part "); Serial.print(i); Serial.print(": ");
                Serial.print(snake[i].x); Serial.print(","); Serial.println(snake[i].y);
            }
        RUNNING = false;
        return;
    }
    u16 i = currentLevel;
    for ( ; i > 1; i--){
        snake[i - 1] = snake[i - 2];
    }
    snake[0] = headCopy;  // copy the results
    // ! must do body collision check inside this function, instead of addStructs because it takes a copy
    for (u16 i = 1; i < currentLevel; i++){
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y){
            RUNNING = false;
            return;
        }
    }
    
    drawSnake(snake, currentLevel);
}


void drawApple(){
    drawSquareShape(apple);
}


void generateApple(){
    while (true){
        u8 x = random(0, (rightWall/size) + 1) * size;  // makes sure its a multiple of size
        u8 y = random(0, (bottomWall/size) + 1) * size;
        u8 i = 0; 
        for ( ; i < currentLevel; i++){
            if (snake[i].x == x && snake[i].y == y ){
                break;
            }
        }
        if (i == currentLevel){ // that means the loop finished and no overlap was found
            apple.x = x;
            apple.y = y;
            drawApple();
            break;
        }
    }
}


bool headOnApple(){
    if (snake[0].x == apple.x && snake[0].y == apple.y){
        return true;
    }
    return false;
}

void drawBorder(){
    for (u8 i = 0; i < 128; i++){
        drawPixel(i, 1, white);
        drawPixel(i, bottomWall, white);

    }
    for (u8 j = 0; j < bottomWall; j++){
        drawPixel(0, j, white);
        drawPixel(127, j, white);
    }
}
