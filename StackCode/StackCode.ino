#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

// Variables
int screenHeight = 64;   
int platformWidth = 20;  
bool direction = true;
int speed = 3;
int buttonPin = 2;
int blockIndex = 0;
bool gameOver = false;

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

void setup(void) {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  blockWidth[0] = 20; // Initial size
  
  // Set display color
  u8g.setColorIndex(1); 
}

void loop(void) {
  if (gameOver) {
    u8g.firstPage();
    do { draw(); } while (u8g.nextPage());
    return;
  }

  if (blockPos[blockIndex] >= screenHeight - platformWidth) direction = false;
  if (blockPos[blockIndex] <= 0) direction = true;

  if (direction) blockPos[blockIndex] += speed;
  else blockPos[blockIndex] -= speed;

  u8g.firstPage();
  do {
    draw();
  } while (u8g.nextPage());

  if (digitalRead(buttonPin) == HIGH) {
    if (blockIndex > 0) {
      int prevStart = blockPos[blockIndex - 1];
      int prevEnd   = blockPos[blockIndex - 1] + blockWidth[blockIndex - 1];
      int currStart = blockPos[blockIndex];
      int currEnd   = blockPos[blockIndex] + platformWidth;

      int finalStart = max(prevStart, currStart);
      int finalEnd   = min(prevEnd, currEnd);
      int newSize    = finalEnd - finalStart;

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
  }
}