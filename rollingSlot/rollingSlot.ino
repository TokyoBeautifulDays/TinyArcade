#include <TinyScreen.h>
#include <SD.h>
#include <Wire.h>
#include <SPI.h>
#include "TinyArcade.h"
#include "NumberM.h"

#define SCREEN_WIDTH 96
#define SCREEN_HEIGHT 64
#define SCREEN_HEIGHT_OFFSET 10

#define BUFFERSIZE 64    // MAX line buffer size
#define THRESHOLD 1000   // chattering check


typedef struct {
  int8_t x, y, width, height;       // sprite area coordinate(window coordinate) and size defined by display.setX, setY
  bool beShot;                      // no use
  uint8_t index;                    // index for bitmapList what bitmap are selected
  const uint16_t *bitmap;           // 16bit color depth bitmap
} spriteAttribute; 

typedef struct {
  uint8_t id;
  uint8_t xcoordinate, ycoordinate, width, height;  // SCREEN WIDE Coordinate
  bool beShot; 
  uint8_t index;    // spriteList index (point to number)
} slotAttribute; 

const uint16_t EDGE = 0x0000;             // transparent image pixel of bitmap(no color filled)
uint8_t linebuffer16b[BUFFERSIZE * 2];    // 16 bit color depth
bool borderline = false;

spriteAttribute number0, number1, number2, number3, number4, number5, number6, number7, number8, number9;
spriteAttribute * spriteList[] = {&number0, &number1, &number2, &number3, &number4, &number5, &number6, &number7, &number8, &number9, NULL };
uint8_t shotCount[] = {0,0,0,0,0,0,0,0,0,0};  // amount of shots for each number
const uint16_t * bitmapList[] = {n0MBitmap, n1MBitmap, n2MBitmap, n3MBitmap, n4MBitmap, n5MBitmap, n6MBitmap, n7MBitmap, n8MBitmap, n9MBitmap};

slotAttribute *slot;
slotAttribute slot0 = { 0,  5, 10, 18, 30, false, 0};
slotAttribute slot1 = { 1, 28, 10, 18, 30, false, 1};
slotAttribute slot2 = { 2, 51, 10, 18, 30, false, 2};
slotAttribute slot3 = { 3, 74, 10, 18, 30, false, 3};
slotAttribute * slotList[] = { &slot0, &slot1, &slot2, &slot3, NULL};
uint8_t slotIndex = 0, targetSlot = 0;  // targetSlot is assigned to target of user's button action

TinyScreen display = TinyScreen(TinyScreenPlus);

void numberInitialize() {
    number0 = {0, 100, 18, 30, false, 0, n0MBitmap}; number1 = {0, 100, 18, 30, false, 1, n1MBitmap}; number2 = {0, 100, 18, 30, false, 2, n2MBitmap};
    number3 = {0, 100, 18, 30, false, 2, n3MBitmap}; number4 = {0, 100, 18, 30, false, 4, n4MBitmap}; number5 = {0, 100, 18, 30, false, 5, n5MBitmap};
    number6 = {0, 100, 18, 30, false, 6, n6MBitmap}; number7 = {0, 100, 18, 30, false, 7, n7MBitmap}; number8 = {0, 100, 18, 30, false, 8, n8MBitmap};
    number9 = {0, 100, 18, 30, false, 9, n9MBitmap};
}
void allInitialize() {
    slot = &slot0; slotIndex = 0; targetSlot = 0;
    numberInitialize();

    spriteList[0] = &number0; spriteList[1] = &number1; spriteList[2] = &number2; spriteList[3] = &number3; spriteList[4] = &number4; spriteList[5] = &number5;
    spriteList[6] = &number6; spriteList[7] = &number7; spriteList[8] = &number8; spriteList[9] = &number9; spriteList[10] = NULL;
    shotCount[0] = 0; shotCount[1] = 0; shotCount[2] = 0; shotCount[3] = 0; shotCount[4] = 0;
    shotCount[5] = 0; shotCount[6] = 0; shotCount[7] = 0; shotCount[8] = 0; shotCount[9] = 0;
    slot0 = {0,  5, 10, 18, 30, false, 0}; slot1 = {1, 28, 10, 18, 30, false, 1}; slot2 = {2, 51, 10, 18, 30, false, 2}; slot3 = {3, 74, 10, 18, 30, false, 3};
}

void setup() {

  arcadeInit();
  display.begin();display.setBitDepth(TSBitDepth16); display.setBrightness(15);display.setFlip(false); display.setFont(liberationSansNarrow_8ptFontInfo);
  display.fontColor(TS_16b_White, TS_16b_Black);
  USBDevice.init(); USBDevice.attach(); SerialUSB.begin(9600);
}

void loop() {

  // Initialize for Game Start
  display.clearScreen(); allInitialize();displayMessage(25, 20, "Welcome");  ;displayMessage(20, 40, "Rolling Slot!");
  while (button1ReadOut() < THRESHOLD) { }  // wait for game start

  display.clearScreen(); slotGameLoop();

  // Game Result
  while (button1ReadOut() < THRESHOLD) {}
    String resultMessage = "Try again!";

    display.clearScreen();
    for (uint8_t index = 0; spriteList[index] != NULL; index++) {
      switch (shotCount[index]) {
        case 0:case 1:case 2:  break;
        case 3: resultMessage = " 3 shots! Great!"; break;
        case 4: resultMessage = " 4 shots! Super!"; break;
      }
    }
    displayMessage(0, 10, resultMessage); displayMessage(0, 30, "Btn1 for new game");
    while (button1ReadOut() < THRESHOLD) {}  // wait for new game
}
void slotGameLoop() {

    while(slotList[targetSlot] != NULL) {
            numberInitialize();  // on purpose of synthesis number sprites.
            slotRun(slot);
            if (button1ReadOut() > THRESHOLD) {   // number of target slot is selected!
                    (*slotList[targetSlot]).beShot = true; shotCount[(*slotList[targetSlot]).index] += 1; targetSlot++; 
            } 
            switch(slotIndex) {
                case 0: case 1:
                case 2: slotIndex++; break;
                case 3: slotIndex = 0; break;
            }
            slotChange(slotIndex);
    }
}
void slotRun(slotAttribute *slot) {

  if ((*slot).beShot) return;
    else showLoopSprite(slot, false, spriteList, false, 3);
}
void slotChange(uint8_t slotIndex) {

  switch(slotIndex) {
    case 0: slot = &slot0; break;
    case 1: slot = &slot1; break;
    case 2: slot = &slot2; break;
    case 3: slot = &slot3; break;
  }
}
