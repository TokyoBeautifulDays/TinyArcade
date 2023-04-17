//
// linebuffer  .. buffer for line
// spriteList .. array of sprites. NULL terminate
// xlength .. drawing width
// ylength .. drawing height
// backgroundColor .. back color
// borderline .. true(white borderline is drawn)/false( no borderline is drawn)
//
//
void drawSpritesOnLine(uint8_t *linebuffer, spriteAttribute **spriteList, uint8_t xlength, uint8_t ylength, uint16_t backgroundColor, bool borderline) {

  display.startData();
  for (uint8_t lineNumber = 0; lineNumber < ylength; lineNumber++) {

    // setup background color
    for (uint8_t index = 0; index < xlength; index++) {
      if (borderline && (lineNumber == 0 or lineNumber == (ylength - 1))) {  // top and bottom border line
        linebuffer[index * 2] = TS_16b_White >> 8;
        linebuffer[index * 2 + 1] = TS_16b_White;
      } else {
          if (borderline && (index == 0 or index == (xlength - 1))) {      // left and right border line
              linebuffer[index * 2] = TS_16b_White >> 8;
              linebuffer[index * 2 + 1] = TS_16b_White;
          } else {                                        // other pixel filled with background color
              linebuffer[index * 2] = backgroundColor >> 8;
              linebuffer[index * 2 + 1] = backgroundColor;
        }
      }
    }
    spriteSynthesis(linebuffer, spriteList, lineNumber, xlength);
    display.writeBuffer(linebuffer, xlength * 2);  // 16bit color depth. size : uint8_t * 2
  }
  display.endTransfer();
}