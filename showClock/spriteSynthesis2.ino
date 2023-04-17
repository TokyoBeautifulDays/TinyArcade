//
// Draw Sprite color on linebuffer
//
//      typedef struct {
//          uint8_t x;   // x coordinate of window sepcified display.setX
//          uint8_t y;   // y coordinate of window sepcified display.setY
//          uint8_t width;
//          uint8_t height;
//          bool beShot;        // true .. be shot!(no draw on display)  false .. alive(draw on display)
//          uint16_t *bitmap;   // 16bit color depth
//      } spriteAttribute; 
//
//    spriteAttribute ball = { 44, 28, BALL_WIDTH, BALL_HEIGHT, false, ballBitmap} ;
//    spriteAttribute * spriteList [] = { &ball, NULL };
//
//    linebuffer[SCREEN_WIDTH * 2] .. for 16bit color depth  (background color data should be filled before call)
//    lineNumber .. which Y-axis( 0 - 63 )
//
//
//      display.startData();
//      // line(Y-axis) loop
//      for ( uint8_t lineNumber = 0; lineNumber < SCREEN_HEIGHT; lineNumber ++ ) {
//
//            // draw background color
//            for (uint8_t index = 0; index < SCREEN_WIDTH; index++) 
//                    { linebuffer[index * 2] = backgroundColor >> 8; linebuffer[index * 2 + 1] = backgroundColor; }
//            // sprite synthesis
//            spriteSynthesis(linebuffer, spriteList, lineNumber);
//            // write linebuffer on display
//            display.writeBuffer(linebuffer,BUFFERSIZE16B);  // 16bit color depth. size : uint8_t * 2
//      }
//      display.endTransfer();

void spriteSynthesis(uint8_t * linebuffer, spriteAttribute ** spriteList, uint8_t lineNumber, uint8_t xlength) {
  
    for (int index = 0; spriteList[index] != NULL; index ++) {
        spriteAttribute * sprite = spriteList[index];
        if ( sprite->beShot ) continue;  // this sprite is shot! skip out!

        if (lineNumber >= sprite->y && lineNumber <= sprite->y + sprite->height - 1)
        {  // Y-Axis: sprite is overlapped.
            if (sprite->x >= 0 && ((sprite->x + sprite->width - 1) <= xlength - 1))
            { // X-Axis: sprite is overlapped.
          
                // copy color into linebuffer
                for ( int xIndex = sprite->x; xIndex < sprite->x + sprite->width; xIndex++) {
                    uint16_t color = sprite->bitmap[(lineNumber - sprite->y) * sprite->width + (xIndex - sprite->x)];
                    if (color != EDGE) { linebuffer[xIndex * 2 ] = color >>8; linebuffer[xIndex * 2 + 1] = color; }
                }
            }
        }
    }
}