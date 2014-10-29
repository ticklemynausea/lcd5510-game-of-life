#include "gfx.h"

void GFX::HorizontalSwipe() {

  FOR_x {
 
    Display::DrawLine(x, 0, x, LCD_HEIGHT - 1, BLACK);
    if (x >= 1) {
      uint16_t x2 = x - 1;
      Display::DrawLine(x2, 0, x2, LCD_HEIGHT - 1, WHITE);
    }

    if (x > 1) {
      FOR_y {
        uint16_t x2 = x - 2;
        PIXEL_MAP(x2, y);
      }  
    }

    Display::UpdateFromDisplay();
 
  }

  for (uint8_t x = LCD_WIDTH - 1; x < LCD_WIDTH; x++) {
    FOR_y {
      PIXEL_MAP(x, y);
    }
    Display::UpdateFromDisplay();
  }
  
  delay(200);  
}

void GFX::VerticalSwipe() {

  FOR_y {

    Display::DrawLine(0, y, LCD_WIDTH-1, y, BLACK);

    if (y >= 1) {
      uint16_t y2 = y - 1;
      Display::DrawLine(0, y2, LCD_WIDTH-1, y2, WHITE);
    }

    if (y > 1) {
      FOR_x {
        uint16_t y2 = y - 2;
        PIXEL_MAP(x, y2);
      }  
    }

    Display::UpdateFromDisplay();

  }

  for (uint8_t y = LCD_HEIGHT - 1; y < LCD_HEIGHT; y++) {
    FOR_x {
      PIXEL_MAP(x, y);
    }
    Display::UpdateFromDisplay();
  }
  
  delay(200);  
}
