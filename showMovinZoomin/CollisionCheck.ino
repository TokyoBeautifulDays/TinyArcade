

#define zmax(a,b) ((a)>(b)?(a):(b))
#define zmin(a,b) ((a)<(b)?(a):(b))

//
// Bump check between bitmaps
//
bool bitmapBump(spriteAttribute *s1, spriteAttribute *s2) {
  if ((s1->x < s2->x + s2->width) && (s1->x + s1->width > s2->x))
    if ((s2->y < s1->y + s1->height) && (s2->y + s2->height > s1->y))
      return true;
  return false;
}

//
// Bump check between EDGE pixels
//
bool pixelBump(spriteAttribute *s1, spriteAttribute *s2) {
  if (!bitmapBump(s1, s2))return false;
  int startX = zmax(s1->x, s2->x);
  int endX = zmin(s1->x + s1->width, s2->x + s2->width);
  int startY = zmax(s1->y, s2->y);
  int endY = zmin(s1->y + s1->height, s2->y + s2->height);
  for (int y = startY; y < endY; y++) {
    for (int x = startX; x < endX; x++) {
      if (s1->bitmap[(y - s1->y)*s1->width + (x - s1->x)] != EDGE && s2->bitmap[(y - s2->y)*s2->width + (x - s2->x)] != EDGE)
        return true;
    }
  }
  return false;
}
