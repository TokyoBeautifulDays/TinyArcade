#include <TinyScreen.h>
#include <SD.h>
#include <Wire.h>
#include <SPI.h>
#include "TinyArcade.h"
#include "NumberM.h"
#include "analogClock.h"


#define SCREEN_WIDTH 96
#define SCREEN_HEIGHT 64
#define SCREEN_HEIGHT_OFFSET 14

#define BUFFERSIZE 64    // MAX line buffer size

#define THRESHOLD 100   // chattering check

#define XCenter 48   // analog clock center
#define YCenter 32   // analog clock center
#define TIMESETMESSAGE " Set Time (24H)"
#define TIME24HMESSAGE "      24H"

typedef struct {
  uint8_t xcoordinate, ycoordinate;  // screen wide coordinate
  uint8_t x, y, width, height;       // sprite area coordinate(window coordinate) and size defined by display.setX, setY
  bool beShot;
  uint8_t index;                     // index for bitmapList what bitmap are selected
  const uint16_t *bitmap;           //  16bit color depth bitmap
} spriteAttribute; 

const uint16_t EDGE = 0x0000;             // transparent image pixel of bitmap(no color filled)
uint8_t linebuffer16b[BUFFERSIZE * 2];    // 16 bit color depth
bool borderline = false;

const uint16_t * bitmapList[] = {n0MBitmap, n1MBitmap, n2MBitmap, n3MBitmap, n4MBitmap, n5MBitmap, n6MBitmap, n7MBitmap, n8MBitmap, n9MBitmap} ;

spriteAttribute *number;
spriteAttribute number1 = { 3,  10, 0, 0, 18, 30, false, 0, n0MBitmap};  // Hour upper
spriteAttribute number2 = { 24, 10, 0, 0, 18, 30, false, 1, n1MBitmap};  // Hour lower
spriteAttribute number3 = { 54, 10, 0, 0, 18, 30, false, 2, n2MBitmap};  // Minute upper
spriteAttribute number4 = { 75, 10, 0, 0, 18, 30, false, 3, n3MBitmap};  // Minute lower
spriteAttribute column =  { 42, 15, 0, 0, 12, 20, false, 4, columnBitmap};  // ":"

spriteAttribute * spriteList[] = {&number1, NULL };

uint8_t digitindex = 1;         // showing which number sprite (number1, number2, number3, number4) is setecled
bool numberSelectMode = false;  // number editing mode(true) or clock mode(false)

bool timeSetDone = false;       // current time is et or not 
uint32_t initialTime;           // millis time stamp for scaling one second

bool digitalClock;              // digital clock(true) or analog clock(false)


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

  digitalClock = true;
 if (digitalClock) showDigitalClock();     // digital clock interface or analog clock interface
             else  showAnalogClock();
  digitindex =1;
  digitChange(digitindex);

  //display.drawLine(0, SCREEN_HEIGHT - SCREEN_HEIGHT_OFFSET, SCREEN_WIDTH - 1, SCREEN_HEIGHT - SCREEN_HEIGHT_OFFSET, TS_16b_White);
  timeSetDone = false;       // current time is not set
  numberSelectMode = false;  // clock mode

  if (!timeSetDone && digitalClock) displayMessage(10, 53, TIMESETMESSAGE);  // no current time set, show warning message
  timeStart(&initialTime);  // clock counter start
}

void loop() {
      if (button1ReadOut() > THRESHOLD && digitalClock) numberSelectMode = !numberSelectMode;   // check number edit-mode request
      if (numberSelectMode) {   // number edit mode

            timeSetDone = true;  // no ncecsarry time-setting warning message
            digitSelect(number, &digitindex, &numberSelectMode);
            displayMessageClear(10, 53);    // clear time-set warning message  
     
      } else {    // time mode
       
            if (button2ReadOut() > THRESHOLD) {
                        digitalClock = !digitalClock; 
                        if (!digitalClock) showAnalogClock();
                        else {
                            display.clearScreen();  // clean analog clock interface and re-construct digital clock interface
                            if (!timeSetDone) displayMessage(10, 53, TIMESETMESSAGE); 
                                else displayMessage(10, 53, TIME24HMESSAGE );
                            showDigitalClock();
                        }
            }
            clockStart(&initialTime);
            digitindex =1;                // reset to be ready for number selection mode 
            digitChange(digitindex);        
      }
}
// number sprite selection
void digitSelect(spriteAttribute *number, uint8_t *digitindex, bool *numberSelectMode) {

          if (numberSelect(number)) {
               // joystick Right
                if ((*digitindex) == 4) { (*digitindex) = 1; (*numberSelectMode) = !(*numberSelectMode); } // number selection mode OFF
                else { (*digitindex) += 1;}
                digitChange((*digitindex));
          } else {
                // joystick left
                if ((*digitindex) == 1) { (*digitindex) = 4;} else { (*digitindex) -= 1;}
                digitChange((*digitindex));
          }
}
// number edit on a number sprite
bool numberSelect(spriteAttribute *number) {
  bool show = false;
  bool joystickRight = false;

    writeBorder((*number).xcoordinate - 3, (*number).xcoordinate +  (*number).width - 1 + 3, (*number).ycoordinate - 3, (*number).ycoordinate + (*number).height - 1 + 3, TS_16b_Green);
    while(1) {
        if (JoystickUpReadOut() > THRESHOLD)  {
              if ((*number).index == 9) { (*number).index = 0;} else { (*number).index++;}
              show = true;
        }
        if (JoystickDownReadOut() > THRESHOLD)  {
              if ((*number).index == 0) { (*number).index = 9;} else { (*number).index--;}
              show = true;
        }
        if (show) {
               (*number).bitmap = bitmapList[(*number).index];
               showNumber((*number).xcoordinate, (*number).ycoordinate, (*number).width, (*number).height, false); // for bordeline adjustment
               writeBorder((*number).xcoordinate - 3, (*number).xcoordinate +  (*number).width - 1 + 3, (*number).ycoordinate - 3, (*number).ycoordinate + (*number).height - 1 + 3, TS_16b_Green);
               show = !show;
       }
       if (JoystickLeftReadOut() > THRESHOLD) { joystickRight = false; break;}
       if (JoystickRightReadOut() > THRESHOLD) { joystickRight = true; break;}
    } 
    clearWindow(linebuffer16b, (*number).xcoordinate - 3, (*number).xcoordinate + (*number).width - 1 + 3, (*number).ycoordinate - 3, (*number).ycoordinate + (*number).height + 3, TS_16b_Black);
    showNumber((*number).xcoordinate, (*number).ycoordinate, (*number).width, (*number).height, false);
    return joystickRight;
}
// draw number bitmap
void showNumber(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool borderline) {
  setBitmapArea(x, x + width - 1, y, y + height - 1);
  drawSpritesOnLine(linebuffer16b, spriteList, width, height, TS_16b_Black, borderline);
}
// number sprite change
void digitChange(uint8_t index) {
  switch(index) {
    case 1:
          number = &number1;
          spriteList[0] = &number1;
          break;
    case 2:
          number = &number2;
          spriteList[0] = &number2;
          break;
    case 3:
          number = &number3;
          spriteList[0] = &number3;
          break;
    case 4:
          number = &number4;
          spriteList[0] = &number4;
          break;
    case 5: //column
          number = &column;
          spriteList[0] = &column;
          break;

  }
}
void showDigitalClock() {

    digitChange(1);
    showNumber((*number).xcoordinate, (*number).ycoordinate, (*number).width, (*number).height, false);
    digitChange(2);
    showNumber((*number).xcoordinate, (*number).ycoordinate, (*number).width, (*number).height, false);
    digitChange(5);
    showNumber((*number).xcoordinate, (*number).ycoordinate, (*number).width, (*number).height, false);
    digitChange(3);
    showNumber((*number).xcoordinate, (*number).ycoordinate, (*number).width, (*number).height, false);
    digitChange(4);
    showNumber((*number).xcoordinate, (*number).ycoordinate, (*number).width, (*number).height, false);
}

void clockStart(uint32_t * startTime) {
  
  if (oneMinute(startTime)) {  // one minute passed!!

      if (number4.index == 9) {
              number4.index = 0;
              if (number3.index >= 5) {
                    number3.index = 0;

                    if ((number1.index >= 2) && ( number2.index == 3)) {  // 23 hour
                          number1.index = 0;
                          number2.index = 0;
                    } else if (number2.index == 9)  {
                          number1.index += 1;                          
                          number2.index = 0;
                    } else number2.index += 1;
                    number1.bitmap = bitmapList[number1.index]; 
                    number2.bitmap = bitmapList[number2.index]; 

              } else number3.index += 1;
              number3.bitmap = bitmapList[number3.index];  // minutes higher digit update

      } else number4.index += 1;
      number4.bitmap = bitmapList[number4.index];  // minutes lower digit update

      if (digitalClock) showDigitalClock();     // digital clock interface or analog clock interface
        else showAnalogClock();
  }
}

void showAnalogClock() {
  uint8_t hour;
  uint8_t minute;
  uint16_t clockColor = TS_16b_Yellow;  // day color

      display.clearScreen();
      setBitmapArea(0, SCREEN_WIDTH - 1, 0, SCREEN_HEIGHT - 1);


      minute = number3.index * 10 + number4.index;
      hour = number1.index * 10 + number2.index;
      if ( hour > 4 and hour < 12) clockColor = TS_16b_White;     // morning color
      if ( hour >= 12 and hour < 18) clockColor = TS_16b_Yellow;  // afternoon color
      if ( hour >= 18 or hour <= 4) clockColor = TS_16b_Green;    // night color


      if (hour >= 12) hour -= 12;
      SerialUSB.print(hour, DEC); SerialUSB.print(":"); SerialUSB.println(minute, DEC);


      // Minutes Clock Circle
      for (uint8_t angle = 0; angle < 60; angle++ ) {
        display.drawPixel(XCenter + minutesPoint[angle][0], YCenter +  minutesPoint[angle][1], clockColor);
        if ( angle == 0)  display.drawRect(XCenter - 2, 0, 4, 4, TSRectangleFilled ,TS_16b_White);
        if ( angle == 15) display.drawRect(XCenter + 32 - 2, YCenter - 2, 4, 4, TSRectangleFilled ,TS_16b_White);
        if ( angle == 30) display.drawRect(XCenter - 2, SCREEN_HEIGHT - 4, 4, 4, TSRectangleFilled ,TS_16b_White);
        if ( angle == 45) display.drawRect(XCenter - 32, YCenter - 2, 4, 4, TSRectangleFilled ,TS_16b_White);
      }

      // Hours Clock Circle
      for (uint8_t angle = 0; angle < 120; angle++ ) {
       // if (angle % 5 == 0) display.drawPixel(XCenter + hoursPoint[angle][0], YCenter +  hoursPoint[angle][1], TS_16b_Green);
        if ( angle == 0)  display.drawRect(XCenter - 2, YCenter - 24 - 0, 4, 4, TSRectangleFilled ,TS_16b_White);
        if ( angle == 15) display.drawRect(XCenter + 24 - 2, YCenter - 2, 4, 4, TSRectangleFilled ,TS_16b_White);
        if ( angle == 30) display.drawRect(XCenter - 2, YCenter + 24 - 4, 4, 4, TSRectangleFilled ,TS_16b_White);
        if ( angle == 45) display.drawRect(XCenter - 24, YCenter - 2, 4, 4, TSRectangleFilled ,TS_16b_White);
      }

      display.drawLine(XCenter, YCenter, XCenter + hoursPoint[(hour*60 + minute)/6][0], YCenter + hoursPoint[(hour*60 + minute)/6][1], clockColor);
      display.drawLine(XCenter, YCenter, XCenter + minutesPoint[minute][0], YCenter + minutesPoint[minute][1], clockColor);

}
