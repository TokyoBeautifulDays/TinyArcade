
#define maxOne(a,b) ((a)>(b)?(a):(b))
#define minOne(a,b) ((a)<(b)?(a):(b))

uint8_t maxValue(uint8_t num1, uint8_t num2) {
    uint8_t max_value; 
    if (num1 > num2) { max_value = num1; }
       else { max_value = num2;}
    return max_value;
}
void DEBUGprintChar(char c) {
  display.print(c);  
}
void DEBUGprintString(const char * c) {
  display.print(c);  
}
void DEBUGprintInt(int n) {
  display.print(n, DEC);  
}
void DEBUGprintHEX(int n) {
  display.print(n, HEX);  
}

// delay sprite move
bool frameWait (uint16_t frame, uint16_t modNumber) {
  if (frame % modNumber == 0) return true;
    else return false;
}

// 1time wait
void timeStart(uint32_t * startTime) {
  (*startTime) = millis();
}
void nSecondWait(uint32_t * startTime, uint16_t n) {
  while (millis() < (*startTime) + n) {}
  (*startTime) = millis();
}
void oneSecondWait(uint32_t * startTime) {
  while (millis() < (*startTime) + 1000 ) {}
  (*startTime) = millis();
}
bool nSecond(uint32_t * startTime, uint16_t n) {
  if (millis() < (*startTime) + n ) return false;
  (*startTime) = millis();
  return true;
}
bool oneSecond(uint32_t * startTime) {
  if (millis() < (*startTime) + 1000 ) return false;
  (*startTime) = millis();
  return true;
}
void oneMinuteWait(uint32_t * startTime) {
  for ( uint8_t i = 0; i < 60; i++ ) { oneSecondWait(startTime); }
}
bool oneMinute(uint32_t * startTime) {
  if (millis() < (*startTime) + (1000 * 60) ) return false;
  (*startTime) = millis();
  return true;
}

// DisplayValue
//
//  After call displayValue, need to claim setX, setY if necessary.
//
//  display.setX(SCREEN_WIDTH_OFFSET, SCREEN_WIDTH -1);
//  display.setY(0, SCREEN_HEIGHT - SCREEN_HEIGHT_OFFSET - 1);
//
void displayValue(uint8_t x, uint8_t y, String message, uint16_t score) {
  display.setCursor(x, y);
  display.print(message);
  display.print(score, DEC);
}
void displayMessage(uint8_t x, uint8_t y, String message) {
  display.setCursor(x, y);
  display.print(message);
}
// clean  x - SCREEN_WIDTH(96), y - SCREEN_HEIGHT(64) area.
void displayMessageClear(uint8_t x, uint8_t y) {
  display.clearWindow(x, y, SCREEN_WIDTH - x, SCREEN_HEIGHT - y);
}
void setBitmapArea(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2) {
  display.setX(x1, x2);
  display.setY(y1, y2);
}
void writeBorder(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, uint16_t borderColor) {
  if ( borderColor != EDGE) {
    display.drawLine(x1, y1, x2, y1, borderColor);
    display.drawLine(x1, y1, x1, y2, borderColor);
    display.drawLine(x1, y2, x2, y2, borderColor);
    display.drawLine(x2, y1, x2, y2, borderColor);
  }
}