//*******
// Display small sprite
//
//*********

//#include <string.h>
#include <TinyScreen.h>
#include <SD.h>
#include <Wire.h>
#include <SPI.h>
#include "TinyArcade.h"
#include "sprites.h"

#define SCREEN_WIDTH 96
#define SCREEN_WIDTH_OFFSET 0
#define SCREEN_HEIGHT 64
#define SCREEN_HEIGHT_OFFSET 10
#define BALL_WIDTH 8
#define BALL_HEIGHT 8
#define BALLSIZE BALL_WIDTH * BALL_HEIGHT
#define WINDOW_MIN_LENGTH BALL_WIDTH + 2

#define BUFFERSIZE16B SCREEN_WIDTH * 2  // good enough size seince multi use purpose

uint8_t x, y, p, q;  // window1, window2 (x,y)-position

typedef struct {
  uint8_t x, y, width, height;
  bool beShot;
  const uint16_t *bitmap;  // 16bit color depth
} spriteAttribute;

spriteAttribute ball1 = { 0, 0, BALL_WIDTH, BALL_HEIGHT, false, ball1Bitmap }; spriteAttribute ball2 = { 0, 0, BALL_WIDTH, BALL_HEIGHT, false, ball2Bitmap };
spriteAttribute *spriteList1[] = { &ball1, NULL }; spriteAttribute *spriteList2[] = { &ball2, NULL };

const uint16_t EDGE = 0x0000;          // transparent image oixel of bitmap
uint8_t lineBuffer16b[BUFFERSIZE16B];  // color depth : 16bit (SCREEN_WIDTH * 2)  shoud be good enough big for multi purpose

// window bounce direction
uint8_t direction1 = 0, direction2 = 3;  // move directtion 0..NE, 1..SE, 2..SW, 3..NW
bool window1Larger = false, window2Larger = true; // Window zoom ( true .. larger  false.. smaller)
uint8_t window1Length, window2Length;  // window size (x length = y length)

TinyScreen display = TinyScreen(TinyScreenPlus);

void setup() {
  arcadeInit();
  display.begin();
  display.setBitDepth(TSBitDepth16);
  display.setBrightness(15);
  display.setFlip(false);

  display.setFont(liberationSansNarrow_8ptFontInfo);
  display.fontColor(TS_16b_White, TS_16b_Black);

  USBDevice.init();
  USBDevice.attach();
  SerialUSB.begin(9600);

  windowInitialize();
  writeBorderlines();

  while (!checkButton(TAButton1)) {} 
  displayMessage(0, 55, "            ");
}

void windowInitialize() {
  // window1 initial position, size and location
  window1Length = SCREEN_HEIGHT - SCREEN_HEIGHT_OFFSET - 10;
  window1Larger = false;
  x = random(SCREEN_WIDTH_OFFSET + 2, SCREEN_WIDTH - window1Length); y = 0;
  // window2 initial position, size and location
  window2Length = WINDOW_MIN_LENGTH;
  window2Larger = true;
  p = random(SCREEN_WIDTH_OFFSET, SCREEN_WIDTH - window2Length); q = random(0, SCREEN_HEIGHT - SCREEN_HEIGHT_OFFSET - window2Length);
}
void writeBorderlines() {
  // Vertical offset border line
  if (SCREEN_WIDTH_OFFSET != 0) display.drawLine(SCREEN_WIDTH_OFFSET - 1, 0, SCREEN_WIDTH_OFFSET - 1, SCREEN_HEIGHT - SCREEN_HEIGHT_OFFSET - 1, TS_16b_White);
  // Holizontal offset border line
  if (SCREEN_HEIGHT_OFFSET != 0) display.drawLine(0, SCREEN_HEIGHT - SCREEN_HEIGHT_OFFSET - 1, SCREEN_WIDTH - 1, SCREEN_HEIGHT - SCREEN_HEIGHT_OFFSET - 1, TS_16b_White);
  if (SCREEN_HEIGHT_OFFSET >= 10) displayMessage(0, 55, "Push Button1");
}

void loop() {

  delay(50);

  clearWindow(lineBuffer16b, x, x + window1Length - 1, y, y + window1Length - 1, TS_16b_Black); // clear previous window
  windowMove(&x, &y, &direction1, window1Length);
  windowZoom(&x, &y, &window1Length, &window1Larger);
  drawWindow(lineBuffer16b, x, x + window1Length - 1, y, y + window1Length - 1, spriteList1, &ball1);
  
  clearWindow(lineBuffer16b, p, p + window2Length - 1, q, q + window2Length - 1, TS_16b_Black); // clear previous window
  windowMove(&p, &q, &direction2, window2Length);
  windowZoom(&p, &q, &window2Length, &window2Larger);
  drawWindow(lineBuffer16b, p, p + window2Length - 1, q, q + window2Length - 1, spriteList2, &ball2);
}

void drawWindow(uint8_t *linebuffer, uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, spriteAttribute **spriteList, spriteAttribute *sprite) {
  // draw sprites on background color
  setBitmapArea(x1, x2, y1, y2);

  // ball moving inside window
  sprite->x = (x2 - x1 - sprite->width)/2;  // always center of window
  if (sprite->y + sprite->height < (y2 - y1)) sprite->y += 3; else { sprite->y = 0; }

  // write
  drawSpritesOnLine(linebuffer, spriteList, (y2 - y1 + 1), TS_16b_Black);  // y2 - y1 + 1 ..total line number
}

void drawSpritesOnLine(uint8_t *linebuffer, spriteAttribute **spriteList, uint8_t length, uint16_t backgroundColor) {

  display.startData();
  for (uint8_t lineNumber = 0; lineNumber < length; lineNumber++) {

    // setup background color
    for (uint8_t index = 0; index < length; index++) {
      if (lineNumber == 0 or lineNumber == (length - 1)) {  // top and bottom border line
        linebuffer[index * 2] = TS_16b_White >> 8;
        linebuffer[index * 2 + 1] = TS_16b_White;
      } else {
          if (index == 0 or index == (length - 1)) {      // left and right border line
              linebuffer[index * 2] = TS_16b_White >> 8;
              linebuffer[index * 2 + 1] = TS_16b_White;
          } else {                                        // other pixel filled with background color
              linebuffer[index * 2] = backgroundColor >> 8;
              linebuffer[index * 2 + 1] = backgroundColor;
        }
      }
    }
    spriteSynthesis(linebuffer, spriteList, lineNumber, length);
    display.writeBuffer(linebuffer, length * 2);  // 16bit color depth. size : uint8_t * 2
  }
  display.endTransfer();
}