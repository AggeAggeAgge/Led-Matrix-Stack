#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

// Variables
int screenWidth = 64;   
int platformWidth = 20;  
bool direction = true;
float speed = 3;
int buttonPin = 2;
int blockIndex = 0;
bool gameOver = false;
const int buzzer = 11; //buzzer to arduino pin 9
float buzzerTone = 523.251;


// Arrayer 
int blockPos[32] = {0};   
int blockWidth[32] = {20}; 


// function that draws to the oled screen
void draw() {
  u8g.setFont(u8g_font_unifont);


  // Game over screen
  if (gameOver) {
    u8g.drawStr(20, 35, "GAME OVER");
    return;
  }
  //-----------------------------------


  // Draw every block placed so far
  for (int i = 0; i <= blockIndex; i++) {
    u8g.drawFrame(4 * i, blockPos[i], 4, blockWidth[i]);
  }
  //---------------------------------
}


// Setup function, runs once. parameters: none returns: none
void setup(void) {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  blockWidth[0] = 20; // Initial size
  pinMode(buzzer, OUTPUT); 
  
  // Set display color
  u8g.setColorIndex(1); 
}

//loop function, loops over and over. parameters: none returns: none
void loop(void) {
  // Restart
  if (gameOver) {
    u8g.firstPage();
    do { draw(); } while (u8g.nextPage());
    return;
  }


  // flip the direction of the block if it hits the endge of the screen.
  if (blockPos[blockIndex] >= screenWidth - platformWidth){
    direction = false;
  } 
  if (blockPos[blockIndex] <= 0) {
    direction = true;
  }
  if (direction) {
     blockPos[blockIndex] += speed;
  }
  else {
    blockPos[blockIndex] -= speed;
  } 
  // Draw each new page
  u8g.firstPage();
  do {
    draw();
  } while (u8g.nextPage());

  //If button is pressed all this happens, speed increases, piezo tone plays and block is placed at current position, changes block size,
  if (digitalRead(buttonPin) == HIGH) {
    speed += 0.2;
    tone(buzzer, buzzerTone);
    buzzerTone = buzzerTone * 1.0595;
    buzzerTone = buzzerTone * 1.0595;

    if (blockIndex > 0) {
      int prevStart = blockPos[blockIndex - 1];
      int prevEnd   = blockPos[blockIndex - 1] + blockWidth[blockIndex - 1];
      int currStart = blockPos[blockIndex];
      int currEnd   = blockPos[blockIndex] + platformWidth;

      int finalStart = max(prevStart, currStart);
      int finalEnd   = min(prevEnd, currEnd);
      int newSize    = finalEnd - finalStart;
      
      // Checks if platform is gone and ends game if its true.
      if (newSize <= 0) {
        gameOver = true;
      } else {
        blockPos[blockIndex] = finalStart;
        blockWidth[blockIndex] = newSize;
        
        platformWidth = newSize; 
        
        blockIndex++; 
        blockWidth[blockIndex] = platformWidth; 
        blockPos[blockIndex] = 0; 
      }
    } else {
      blockWidth[blockIndex] = platformWidth;
      blockIndex++;
      blockWidth[blockIndex] = platformWidth;
      blockPos[blockIndex] = 0;
    }
    
    delay(300); // Debounce to prevent multiple triggers
    noTone(buzzer); 

  }
}

