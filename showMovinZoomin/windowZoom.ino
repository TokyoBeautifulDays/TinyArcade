//
//   *x, *y .. pointer for x, y
//   * length .. length
//   * larger .. true(larger), false(smaller)
//
//    WINDOW_MIN_LENGTH  .. munimum size of window

void windowZoom(uint8_t *x, uint8_t *y, uint8_t *length, bool *larger) {
  
    switch ((*larger)) {
    case true:  // larger 
        if ((*x) > SCREEN_WIDTH_OFFSET  && 
            (*y) > 0 && 
            (*x) + (*length)  < SCREEN_WIDTH && 
            (*y) + (*length) < (SCREEN_HEIGHT - SCREEN_HEIGHT_OFFSET) - 1)  { (*x) -= 1; (*y) -= 1; (*length) += 2;} else { (*larger) = false; }
        break;

    case false:  // smaller
        if ((*length)  > WINDOW_MIN_LENGTH ) { (*x) += 1; (*y) += 1; (*length) -= 2; } else {  (*larger) = true; }
        break;
  }
}