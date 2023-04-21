//
// WIndow moving
// x (pointer) .. window's x location to Screen area
// y (pointer) .. window's y location to Screen area
// direction (pointer) .. currect bounce direction
//    0.. North East, 1..South East, 2..South West, 3..North West
//
// x, y, direction is changed by reason of SCREEN WIDE and SCREEN HEIGHT collision
//
// SCREEN_WIDE 96
// SCREEN_WIDE_OFFSET  left side offset
// SCREEN_HEIGHT 64
// SCREEN_HEIGHT_OFFSET bottom side offset
//

void windowMove(uint8_t *x, uint8_t *y, uint8_t *direction, uint8_t length) {

  switch ((*direction)) {
    case 0:
            if ( (*x) >= SCREEN_WIDTH_OFFSET && ((*x) + length) < (SCREEN_WIDTH - 1)) {
              if ( (*y) > 0 ) { (*x) += 1; (*y) -= 1; } else { (*direction) = 1; (*x) += 1; (*y) += 1;}
            } else {  // (x + WINDOW_WIDTH) >= (SCREEN_WIDTH - 1)
              if ( (*y) > 0 ) { (*direction) = 3; (*x) -= 1; (*y) -= 1; } else { (*direction) = 2; (*x) -= 1; (*y) += 1;}             
            }
            break;
    case 1:
            if ( (*x) >= SCREEN_WIDTH_OFFSET && ((*x) + length) < (SCREEN_WIDTH - 1)) {
              if (((*y) + length) >= (SCREEN_HEIGHT - SCREEN_HEIGHT_OFFSET - 1)) { (*direction) = 0; (*x) += 1; (*y) -= 1;} else { (*x) += 1; (*y) += 1;}
            } else {  // (x + WINDOW_WIDTH) >= (SCREEN_WIDTH - 1)
              if (((*y) + length) >= (SCREEN_HEIGHT - SCREEN_HEIGHT_OFFSET - 1)) { (*direction) = 3; (*x) -= 1; (*y) -= 1;} else { (*direction) = 2; (*x) -= 1; (*y) += 1;}
            }
            break;
    case 2:
            if ( (*x) > SCREEN_WIDTH_OFFSET) {
              if (((*y) + length) >= (SCREEN_HEIGHT - SCREEN_HEIGHT_OFFSET - 1)) { (*direction) = 3; (*x) -= 1; (*y) -= 1;} else { (*x) -= 1; (*y) += 1;}
            } else { //x <= SCREEN_WIDTH_OFFSET
              if (((*y) + length) >= (SCREEN_HEIGHT - SCREEN_HEIGHT_OFFSET - 1)) { (*direction) = 0; (*x) += 1; (*y) -= 1;} else { (*direction) = 1; (*x) += 1; (*y) += 1;}
            }
            break;
    case 3:
          if ( (*x) > SCREEN_WIDTH_OFFSET) {
            if ( (*y) > 0) { (*x) -= 1; (*y) -= 1;} else { (*direction) = 2; (*x) -= 1; (*y) += 1;}
          } else { // x <= SCREEN_WIDTH_OFFSET
            if ( (*y) > 0) { (*direction) = 0; (*x) += 1; (*y) -= 1;} else { (*direction) = 1; (*x) += 1; (*y) += 1;}
          }
          break;
  }
}
