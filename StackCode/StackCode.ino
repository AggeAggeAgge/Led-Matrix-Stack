
#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC

int screenWidth = 64;
int platformWidth = 20;
int positionX = 0;
bool direction = true;
int speed = 4;
int buttonPin = 2;
int platformHeight = 4;
int level = 0;

int blockIndex = 0;
int blockPos[32] = { 0 };
int blockWidth[32] = { 10 };

void draw(int a) {

  // graphic commands to redraw the complete screen should be placed here

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

  Serial.begin(9600);

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


  pinMode(8, OUTPUT);
}

void loop(void) {
  // picture loop
  Serial.println(digitalRead(buttonPin));


  draw(blockPos);

  if (blockPos[blockIndex] >= screenWidth - platformWidth) {
    direction = false;
  }

  if (blockPos[blockIndex]  <= 0) {
    direction = true;
  }


  if (direction == true) {
    blockPos[blockIndex]  = blockPos[blockIndex]  + speed;
  }

  if (direction == false) {
    blockPos[blockIndex]  = blockPos[blockIndex]  - speed;
  }

  if (digitalRead(buttonPin) == HIGH) {
    blockIndex = blockIndex + 1;
    blockPos[blockIndex]  = 0;
  }



  // rebuild the picture after some delay
  delay(50);
}
