#ifndef _gfx_h_
#define _gfx_h_

#include "matrix.h"
#include "display.h"

class GFX {
  public:
 
  static void HorizontalSwipe();
  static void VerticalSwipe();
  static void Message(uint8_t x, uint8_t y, String &text);

};

#endif 
