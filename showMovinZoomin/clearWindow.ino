//
// window area clear with specified color
//

void clearWindow(uint8_t *linebuffer, uint8_t x1, uint8_t x2,  uint8_t y1, uint8_t y2, uint16_t backgroundColor) {

  setBitmapArea(x1, x2, y1, y2);
  display.startData();
  for (uint8_t lineNumber = 0; lineNumber <= (y2 - y1); lineNumber++) {
    for (uint8_t index = 0; index <= (x2 - x1); index++) { linebuffer[index * 2] = backgroundColor >> 8; linebuffer[index * 2 + 1] = backgroundColor;}
    display.writeBuffer(linebuffer, (x2 - x1 + 1) * 2);  // 16bit color depth. size : uint8_t * 2
  }
  display.endTransfer();
}