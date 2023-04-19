//
// write Text on display
// in: buffer .. content
//     size .. buffer size
//     xOffset .. x-asix offset for display area
//     yOffset .. y-axis offset for display area
// out: displayed character count
//
int displayBufferCharCount(char * buffer, int size, int xOffset, int yOffset) {
  // displayed column count
  int displayedCharCount = 0;

  // X-axis available space calculation
  uint8_t availableWidth = SCREEN_WIDTH;
  #define WIDESTXSIZE 8;
  uint8_t widest = WIDESTXSIZE; // longest pixel size of font width (default value)
  // Y-axis available space calculation
  uint8_t availableHeight = SCREEN_HEIGHT;

  uint8_t fontHeight = display.getFontHeight(); 

  bool firstChar = true; // 1st charater of line or not
  bool space = false;    // multiple spaces check. previous char is space or not.

  availableWidth = SCREEN_WIDTH - xOffset; availableHeight = SCREEN_HEIGHT - yOffset; 

  display.setCursor(xOffset,yOffset);
  uint8_t cWidth = widest;  // temporary character pixel wide
  for ( int index = 0; index < size && availableHeight >= fontHeight; index ++ )  {

    // calc width pixel size
    if ( ((index+1) < size) && (buffer[index] != '\n') && (buffer[index+1] != '\n') ) {
          cWidth = (display.getPrintWidth(&buffer[index]) - display.getPrintWidth(&buffer[index+1]));
          widest =  maxValue(cWidth, widest);
      } else { cWidth = widest; }

    // calc auto-linefeed
    if (availableWidth < cWidth ) {
          availableHeight -= fontHeight ;
          display.setCursor(xOffset, (SCREEN_HEIGHT - availableHeight)); // line feed for character height
          firstChar = true;
          space = false;     // previous character is space or not for space-suppressing purpose
          availableWidth = SCREEN_WIDTH - xOffset;
          if (availableHeight < fontHeight) continue;
    }
     // character operation (space, new-line, others)
    switch (buffer[index]) {
            case '\n':
                if (!firstChar) availableHeight -= fontHeight ;  // line-feed! Possible to be short of remaining height
                display.setCursor(xOffset, (SCREEN_HEIGHT - availableHeight)); // line feed for character height
                firstChar = true;
                space = false;
                availableWidth = SCREEN_WIDTH - xOffset;
                displayedCharCount++;
                break;
            case ' ':                                     // space is suppressed cause first character of line.
                displayedCharCount++;
                if (firstChar) continue;
                      else if (space == true) continue;   // previous character is Space, continue
                space = true;
                display.print(buffer[index]);
                availableWidth -= cWidth ;
                break;
            default:
                firstChar = false;
                space = false;
                display.print(buffer[index]);
                availableWidth -= cWidth;
                displayedCharCount++;
                break;
    }
  }
  return displayedCharCount;
}