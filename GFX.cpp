#include "gfx.h"

void GFX::HorizontalSwipe() {
  delay(50);

  FOR_x {
    Display::DrawLine(x, 0, x, LCD_HEIGHT-1, BLACK);
    if (x >= 5) {
      uint16_t x2 = x - 5;
      Display::DrawLine(x2, 0, x2, LCD_HEIGHT-1, WHITE);
    }
    Display::UpdateFromDisplay();
  }

  for (uint8_t  x = LCD_WIDTH - 5; x < LCD_WIDTH; x++) {
    Display::DrawLine(x, 0, x, LCD_HEIGHT-1, WHITE);
    Display::UpdateFromDisplay();
  }
  
  delay(50);  
}

void GFX::VerticalSwipe() {
  delay(50);

  FOR_y {
    Display::DrawLine(0, y, LCD_WIDTH-1, y, BLACK);
    if (y >= 5) {
      uint16_t y2 = y - 5;
      Display::DrawLine(0, y2, LCD_WIDTH-1, y2, WHITE);
    }
    Display::UpdateFromDisplay();
  }

  for (uint8_t y = LCD_HEIGHT - 5; y < LCD_HEIGHT; y++) {
    Display::DrawLine(0, y, LCD_WIDTH-1, y, WHITE);
    Display::UpdateFromDisplay();
  }
  
  delay(50);  
}
