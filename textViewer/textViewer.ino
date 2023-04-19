
#include <string.h>
#include <TinyScreen.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include "TinyArcade.h"

#define SCREEN_WIDTH 96
#define SCREEN_HEIGHT 64
#define THRESHOLD 1000
const uint16_t EDGE = 0x0000; 

#define EXTENSION ".txt"
String fileExtension = EXTENSION;

TinyScreen display = TinyScreen(TinyScreenPlus);

void setup() {
  arcadeInit();
  display.begin();
  display.setBitDepth(TSBitDepth16);
  display.setBrightness(15);
  display.setFlip(false);

  USBDevice.init();
  USBDevice.attach();
  SerialUSB.begin(9600);

  display.setFont(liberationSansNarrow_8ptFontInfo);
	display.fontColor(TS_16b_White, TS_16b_Black);
  initialMessage();
}

void loop(){

  opening();
  operation();
  displayMessage(16, 16, "TextViewer");
  
}
void operation() {

    // Joystick and Button Opertation
  if (checkJoystick(TAJoystickLeft)) { display.clearScreen(); help();}
  if (checkJoystick(TAJoystickRight)) {}
  if (checkJoystick(TAJoystickUp)) { }
  if (checkJoystick(TAJoystickDown)) {}
  if (checkButton(TAButton1)) { display.clearScreen(); showFileContents(fileExtension); }
  if (checkButton(TAButton2)) { display.clearScreen(); showDirectoryList(fileExtension); }
  
}
void initialMessage() {
  display.clearScreen();
  displayMessage(20, 3, "Welcome");
  displayMessage(16, 16, "TextViewer");
}
void opening() {
  displayMessage(0, 37, "B1: TextView");
  displayMessage(0, 50, "B2: Directory List");
}
void help() {

displayMessage(0, 2,  "Push Button2(R)");
displayMessage(0, 16, "for turning page");
while((button1ReadOut() < THRESHOLD) and (button2ReadOut() < THRESHOLD)) {} 
display.clearScreen();
}
