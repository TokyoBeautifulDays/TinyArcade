//
// x,y .. Screen Coordinate (Xcoordinate, Ycoordinate)
// width, height
// borderline .. on/off
// spriteList .. sprite pointer array (NULL end)
// pullDown .. true (pull down), false (pull up)
// wait .. delay parameter
//
// remark: y-value (spriteAttributes) .. should be outside drawing area. if it's inside, draw it overlappedly.
//
void showLoopSprite(slotAttribute * slot, bool borderline, spriteAttribute ** spriteList, bool pullDown, uint16_t wait) {
  bool selected = false;

  setBitmapArea((*slot).xcoordinate, (*slot).xcoordinate +(*slot).width - 1, (*slot).ycoordinate, (*slot).ycoordinate + (*slot).height - 1);

  spriteAttribute *sprite0, *sprite1; 
  uint8_t items;

  for (items = 0; spriteList[items] != NULL; items++) {} // find item amount

      sprite0 = spriteList[(*slot).index];

      if (spriteList[(*slot).index + 1] != NULL) sprite1 = spriteList[(*slot).index + 1 ]; else sprite1 = spriteList[0];
      
      for (int8_t step = 0; step <= (*slot).height; step += 1) {

            if (pullDown) { (*sprite0).y = step + 2; (*sprite1).y = -((*sprite1).height - step); }
              else { (*sprite0).y = -(step) - 2; (*sprite1).y = (*sprite1).height - step; }

            drawSpritesOnLine(linebuffer16b, spriteList, (*slot).width, (*slot).height, TS_16b_Black, borderline);
            delay(wait);
      }
      if ((*slot).index < (items - 1)) (*slot).index++;
       else (*slot).index = 0; 
}