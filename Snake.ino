#include <EEPROM.h>


// Shift register pins
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

// Joystick pins
int virtical = A0;
int horizontal = A1;

// Buttons
int buttonUp;
int buttonRight;
int buttonDown;
int buttonLeft;

// Directions
#define UP -8
#define RIGHT 1
#define DOWN 8
#define LEFT -1



// Game Variables

// Stores the location for each snake section
int snake[64];

// The direction the snake is moving
int direction;
int newDirection;

// Location of the food
int food;

// Snake Length
int size;

// Current Score
int points;

// Current Level
int level;

// Speed of each level in ms. Each level is 20% faster than the previous level
int levels[20] = {1000, 800, 614, 512, 410, 328, 262, 210, 168, 134, 107, 100, 80, 62, 51, 41, 33, 26, 21, 17};


// High Score to be retrieved from EEPROM
int highScore;



// Characters

// Numbers
byte zero[8]    = {0x00,0x3C,0x24,0x24,0x24,0x24,0x3C,0x00};
byte one[8]     = {0x00,0X10,0X18,0X10,0X10,0X10,0X10,0x00};
byte two[8]     = {0x00,0X18,0X24,0X20,0X10,0X08,0x3C,0x00};
byte three[8]   = {0x00,0x3C,0x20,0x20,0x38,0x20,0x3C,0x00};
byte four[8]    = {0x00,0x24,0x24,0x3C,0x20,0x20,0x20,0x00};
byte five[8]    = {0x00,0x3C,0x04,0x3C,0x20,0x20,0x3C,0x00};
byte six[8]     = {0x00,0x3C,0x04,0x3C,0x24,0x24,0x3C,0x00};
byte seven[8]   = {0x00,0x3C,0x20,0x20,0x20,0x20,0x20,0x00};
byte eight[8]   = {0x00,0x3C,0x24,0x3C,0x24,0x24,0x3C,0x00};
byte nine[8]    = {0x00,0x3C,0x24,0x3C,0x20,0x20,0x3C,0x00};

// Letters
byte a[8]       = {0x00,0x3C,0x24,0x24,0x3C,0x24,0x24,0x00};
byte b[8]       = {0x00,0x1C,0x24,0x1C,0x24,0x24,0x1C,0x00};
byte c[8]       = {0x00,0x3C,0x24,0x04,0x04,0x24,0x3C,0x00};
byte d[8]       = {0x00,0x1C,0x24,0x24,0x24,0x24,0x1C,0x00};
byte e[8]       = {0x00,0x3C,0x04,0x04,0x1C,0x04,0x3C,0x00};
byte f[8]       = {0x00,0x3C,0x04,0x1C,0x04,0x04,0x04,0x00};
byte g[8]       = {0x00,0x3C,0x24,0x04,0x34,0x24,0x3C,0x00};
byte h[8]       = {0x00,0x24,0x24,0x3C,0x24,0x24,0x24,0x00};
byte i[8]       = {0x00,0x38,0x10,0x10,0x10,0x10,0x38,0x00};
byte j[8]       = {0x00,0x30,0x20,0x20,0x20,0x24,0x18,0x00};
byte k[8]       = {0x00,0x24,0x14,0x0C,0x14,0x24,0x24,0x00};
byte l[8]       = {0x00,0x04,0x04,0x04,0x04,0x04,0x3c,0x00};
byte m[8]       = {0x00,0x44,0x6C,0x54,0x44,0x44,0x44,0x00};
byte n[8]       = {0x00,0x24,0x2C,0x34,0x24,0x24,0x24,0x00};
byte o[8]       = {0x00,0x3C,0x24,0x24,0x24,0x24,0x3C,0x00};
byte p[8]       = {0x00,0x3C,0x24,0x24,0x3C,0x04,0x04,0x00};
byte q[8]       = {0x00,0x18,0x24,0x24,0x24,0x34,0x38,0x00};
byte r[8]       = {0x00,0x1C,0x24,0x24,0x1C,0x24,0x24,0x00};
byte s[8]       = {0x00,0x3C,0x24,0x04,0x3C,0x20,0x3C,0x00};
byte t[8]       = {0x00,0x3C,0x08,0x08,0x08,0x08,0x08,0x00};
byte u[8]       = {0x00,0x24,0x24,0x24,0x24,0x24,0x3C,0x00};
byte v[8]       = {0x00,0x24,0x24,0x24,0x24,0x24,0x18,0x00};
byte w[8]       = {0x00,0x44,0x44,0x44,0x44,0x54,0x6C,0x00};
byte x[8]       = {0x00,0x44,0x44,0x28,0x10,0x28,0x44,0x00};
byte y[8]       = {0x00,0x44,0x44,0x28,0x10,0x10,0x10,0x00};
byte z[8]       = {0x00,0x3C,0x20,0x10,0x08,0x04,0x3C,0x00};

// Special Characters
byte blank[8]   = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
byte questionMark[8] = {0x00,0x18,0x24,0x10,0x08,0x00,0x08,0x00};
byte go[8]      = {0x00,0xEF,0xA9,0xA1,0xAD,0xA9,0xEF,0x00};

// Text variables (There's probably a better way to do this but it's fine for now)
byte* gameOverText[10] = {g,a,m,e,blank,o,v,e,r,blank}; 
byte* alphabet[37] = {a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z, blank, one, two, three, four, five, six, seven, eight, nine, zero};
byte* snakeText[6] = {s, n, a, k, e, blank};
byte* countDownText[4] = {three, two, one, go};
byte* scoreText[6] = {s,c,o,r,e,blank};
byte* continueGameText[8] = {c,o,n,t,i,n,u,e};
byte* continueQuestionMark[2] = {questionMark, blank};
byte* levelText[1] = {l};
byte* highScoreText[3] = {h,i,blank};

// Variables to measure time for snake movement speed
unsigned long time1;
unsigned long time2;

// LED Matrix
int ledMatrix[64];

// Setup the inital game conditions
void setup() {

    // set shift register lachPins to output
    pinMode(latchPin, OUTPUT);

    // get high score from EEPROM
    highScore = EEPROM.read(0);

    // display logo
    displayText(snakeText, 6, 400);

    // setup game
    reset();
}

// Run main game loop
void loop() {

    // Read buttons and set direction
    buttonUp = analogRead(A0);
    buttonRight = analogRead(A1);
    buttonDown = analogRead(A2);
    buttonLeft = analogRead(A3);

    // Save the last pressed button as the newDirection. I only want to use the last button press. 
    // Because I'm ignoring backwards movement and I want to only use the last pressed direction this is the best menthod.
    if (buttonUp > 1000){
        newDirection = UP;
    } else if (buttonRight > 1000){
        newDirection = RIGHT;
    } else if (buttonDown > 1000){
        newDirection = DOWN;
    } else if (buttonLeft > 1000){
        newDirection = LEFT;
    }

    // Get the current time
    time2 = millis();

    // If the time passed is the speed of the level move the snake
    if (time2 - time1 > levels[level-1]) {
        time1 = time2;

        // Ignore backwards direction. I'm sure there's a better way to write this.
        if (newDirection == UP && direction == DOWN){
        } else if (newDirection == RIGHT && direction == LEFT){
        } else if (newDirection == DOWN && direction == UP){
        } else if (newDirection == LEFT && direction == RIGHT){
        } else {
            direction = newDirection;
        }
        
        // Move the snake
        moveSnake();
    }


    // display game
    draw();
}

// Resets game to initial conditions
void reset() {
    // set initial game data
    for (int i = 0; i < 64; i++){
        snake[i] = 0xff;
    }
    snake[0] = 24;
    direction = RIGHT;
    newDirection = RIGHT;
    size = 1;
    
    // initial food location shouldn't be at the same spot as the snake head
    // set random seed by reading unconnected pin
    randomSeed(analogRead(A5));

    placeFood();

    // beginning level
    points = 0;
    level = 1;

    displayText(countDownText, 4, 1000);
    tone(6, 100, 250);

    // intial time
    time1 = millis();
}

// Update game
void moveSnake(){

    int head = snake[0] + direction;

    // increase points if snake eats food
    if (head == food){
        points = points + level;
        size ++;
        tone(6, 100*points, 250);
        
        // move and add segments if needed
        for (int i = size - 1; i > 0; i--){
            snake[i] = snake[i-1]; 
        }
    
        snake[0] = head;

        // move to next level if all spots taken
        if (size == 32) {

            size = 1;
            level++;

            // display "L" + level number
            displayText(levelText, 1, 400);
            displayNumber(level); 
            delay(400);
            displayText(countDownText, 4, 1000);

            // setup next level
            for (int i = 0; i < 64; i++){
                snake[i] = 0xff;
            }
            snake[0] = 24;
            direction = RIGHT;
        }
    
        // add new food
        placeFood();
    } else {
        // move and add segments if needed
        for (int i = size - 1; i > 0; i--){
            snake[i] = snake[i-1]; 
        }
    
        snake[0] = head;
    }

    // game over conditions
    if (head < 0 || head > 63) {
        gameOver();
    } else if (direction == RIGHT && head%8 == 0){
        gameOver();
    } else if (direction == LEFT && head%8 == 7){
        gameOver();
    } else {
        for (int i = 1; i < 64; i++){
            if (head == snake[i]){
                gameOver();
            }
        }
    }
}

// Place New Food
void placeFood(){

    // how many open spaces are there?
    int openSpaces = 64 - size;

    //pick a random spot
    int randomOpenSpace = random(openSpaces);

    //cycle through available spaces until we get to our random number. That's where we place the food.
    for (int i = 0; i <= randomOpenSpace; i++){
        for (int j = 0; j < 64; j++){
            if (snake[j] == i){
                randomOpenSpace++;
            }
        }
    }

    food = randomOpenSpace;
}

// Game Over Routine
void gameOver(){

    // update new high score
    if (points > highScore){
        highScore = points;
        EEPROM.write(0, highScore);
    }
    
    displayText(gameOverText, 10, 400);
    displayNumber(points);
    displayText(highScoreText, 3, 400);
    displayNumber(highScore);
    continueGame();  
}

// Displays "?". Press any button to continue.
void continueGame(){

    buttonUp = analogRead(A0);
    buttonRight = analogRead(A1);
    buttonDown = analogRead(A2);
    buttonLeft = analogRead(A3);

    int z = 0;

    while (z == 0) {

        for (int x = 0; x < 2; x++){

            time1 = millis();

            while(millis() - time1 < 1000){
                for (int i = 0; i < 8; i++){
                    digitalWrite(latchPin, 0);
                    shiftOut(dataPin, clockPin, 0x01<<i);
                    shiftOut(dataPin, clockPin, ~continueQuestionMark[x][i]);
                    digitalWrite(latchPin, 1); 

                    buttonUp = analogRead(A0);
                    buttonRight = analogRead(A1);
                    buttonDown = analogRead(A2);
                    buttonLeft = analogRead(A3);  

                    if (buttonUp == 1023 || buttonRight == 1023 || buttonDown == 1023 || buttonLeft == 1023){
                        z = 1;
                    }

                }
            }
        }

    }

    delay(400);

    reset();
}

// Display Text
void displayText(byte** text, int size, int speed){

    for (int x = 0; x < size; x++){

        time1 = millis();

        while(millis() - time1 < speed){
            for (int i = 0; i < 8; i++){
                digitalWrite(latchPin, 0);
                shiftOut(dataPin, clockPin, 0x01<<i);
                shiftOut(dataPin, clockPin, ~text[x][i]);
                digitalWrite(latchPin, 1); 

            }
        }
    }
}

// Display number 0-999
void displayNumber(int number){   
    int hundreds = number/100;
    int tens = (number%100)/10;
    int ones = number%10;

    byte* numberToDisplay[6];

    switch (hundreds) {
        case 0: numberToDisplay[0] = zero;
            break;
        case 1: numberToDisplay[0] = one;
            break;
        case 2: numberToDisplay[0] = two;
            break;
        case 3: numberToDisplay[0] = three;
            break;
        case 4: numberToDisplay[0] = four;
            break;
        case 5: numberToDisplay[0] = five;
            break;
        case 6: numberToDisplay[0] = six;
            break;
        case 7: numberToDisplay[0] = seven;
            break;
        case 8: numberToDisplay[0] = eight;
            break;
        case 9: numberToDisplay[0] = nine;
            break;
    }

    switch (tens) {
        case 0: numberToDisplay[2] = zero;
            break;
        case 1: numberToDisplay[2] = one;
            break;
        case 2: numberToDisplay[2] = two;
            break;
        case 3: numberToDisplay[2] = three;
            break;
        case 4: numberToDisplay[2] = four;
            break;
        case 5: numberToDisplay[2] = five;
            break;
        case 6: numberToDisplay[2] = six;
            break;
        case 7: numberToDisplay[2] = seven;
            break;
        case 8: numberToDisplay[2] = eight;
            break;
        case 9: numberToDisplay[2] = nine;
            break;
    }

    switch (ones) {
        case 0: numberToDisplay[4] = zero;
            break;
        case 1: numberToDisplay[4] = one;
            break;
        case 2: numberToDisplay[4] = two;
            break;
        case 3: numberToDisplay[4] = three;
            break;
        case 4: numberToDisplay[4] = four;
            break;
        case 5: numberToDisplay[4] = five;
            break;
        case 6: numberToDisplay[4] = six;
            break;
        case 7: numberToDisplay[4] = seven;
            break;
        case 8: numberToDisplay[4] = eight;
            break;
        case 9: numberToDisplay[4] = nine;
            break;
    }

    numberToDisplay[1] = blank;
    numberToDisplay[3] = blank;
    numberToDisplay[5] = blank;

    displayText(numberToDisplay, 6, 400);

    delay(1000);
}

// Draw game board
void draw(){

    // clear LED matrix
    for (int i = 0; i<64; i++){
        ledMatrix[i] = 0;
    }

    // Place snake
    for (int i = 0; i<64; i++){

        if (snake[i] != 0xff) {
            ledMatrix[snake[i]] = 1;
        }
    }

    // Place food
    ledMatrix[food] = 1;


    byte xData;

    // Draw screen
    for (byte y = 0; y < 8; y++){
        for (byte x = 0; x < 8; x++){
            if (ledMatrix[y * 8 + x]) {

                byte data = 0x01;

                xData = xData | data<<x;
            }

        }

        digitalWrite(latchPin, 0);
        shiftOut(dataPin, clockPin, 0x01<<y);
        shiftOut(dataPin, clockPin, ~(xData));
        digitalWrite(latchPin, 1); 

        // Clear xData
        xData = 0;
    }
}

// Shift out code. From arudino.cc Written by Carlyn Maw,Tom Igoe, David A. Mellis
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {


    //internal function setup
    int i=0;
    int pinState;
    pinMode(myClockPin, OUTPUT);
    pinMode(myDataPin, OUTPUT);

    //clear everything out just in case to
    //prepare shift register for bit shifting
    digitalWrite(myDataPin, 0);
    digitalWrite(myClockPin, 0);

    //for each bit in the byte myDataOut�
    //NOTICE THAT WE ARE COUNTING DOWN in our for loop
    //This means that %00000001 or "1" will go through such
    //that it will be pin Q0 that lights. 
    for (i=7; i>=0; i--)  {
        digitalWrite(myClockPin, 0);

        //if the value passed to myDataOut and a bitmask result 
        // true then... so if we are at i=6 and our value is
        // %11010100 it would the code compares it to %01000000 
        // and proceeds to set pinState to 1.
        if ( myDataOut & (1<<i) ) {
        pinState= 1;
        }
        else {  
        pinState= 0;
        }

        //Sets the pin to HIGH or LOW depending on pinState
        digitalWrite(myDataPin, pinState);
        //register shifts bits on upstroke of clock pin  
        digitalWrite(myClockPin, 1);
        //zero the data pin after shift to prevent bleed through
        digitalWrite(myDataPin, 0);
    }

    //stop shifting
    digitalWrite(myClockPin, 0);
}
