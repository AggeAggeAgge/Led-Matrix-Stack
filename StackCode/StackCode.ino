
#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC


// Declare all variables
int screenWidth = 64;
int platformWidth = 20;
int positionX = 0;
bool direction = true;
int speed = 4;
int buttonPin = 2;
int platformHeight = 4;
int level = 0;
int oldPlatformWidth = 20;

int blockIndex = 0;
int blockPos[32] = { 0 };
int blockWidth[32] = { 10 };


// function to draw stuff to the oled screen, parameters(a: blockPosition) returns: void 
void draw(int a) {

  // Set the font text on the screen for later
  u8g.setFont(u8g_font_unifont);


  u8g.firstPage();
  do {
    for (int i = 0; i < 32; i++) {
      blockWidth[i] = platformWidth;
      u8g.drawFrame(4 * i, blockPos[i], 4, blockWidth[i]);
    }
  } while (u8g.nextPage());
}

void setup(void) {
  // Start serial Monitor
  Serial.begin(9600);

  // Assign button Input pin
  pinMode(buttonPin, INPUT);
  
  // assign default color value
  if (u8g.getMode() == U8G_MODE_R3G3B2) {
    u8g.setColorIndex(255);  // white
  } else if (u8g.getMode() == U8G_MODE_GRAY2BIT) {
    u8g.setColorIndex(3);  // max intensity
  } else if (u8g.getMode() == U8G_MODE_BW) {
    u8g.setColorIndex(1);  // pixel on
  } else if (u8g.getMode() == U8G_MODE_HICOLOR) {
    u8g.setHiColorByRGB(255, 255, 255);
  }
}

void loop(void) {
  // picture loop
  Serial.println(digitalRead(buttonPin));

  // Draw block att block position



  // if the blocks hits the right wall, flip the direction
  if (blockPos[blockIndex] >= screenWidth - platformWidth) {
    direction = false;
  }

  // if the blocks hits the left wall, flip the direction
  if (blockPos[blockIndex]  <= 0) {
    direction = true;
  }

  // Move the block in its designated direction
  if (direction == true) {
    blockPos[blockIndex]  = blockPos[blockIndex]  + speed;
  }

  if (direction == false) {
    blockPos[blockIndex]  = blockPos[blockIndex]  - speed;
  }

  draw(blockPos);


  if (digitalRead(buttonPin) == HIGH) {
    

  if (blockPos[blockIndex] < blockPos[blockIndex - 1]) {
    int diff = blockPos[blockIndex - 1] - blockPos[blockIndex];
    blockPos[blockIndex] = blockPos[blockIndex] + diff;
    blockWidth[blockIndex] = blockPos[blockIndex] - diff;
  }

  blockIndex = blockIndex + 1;

    delay(300);
  
    

  
  }
  //------------------------



  // rebuild the picture after some delay
}
