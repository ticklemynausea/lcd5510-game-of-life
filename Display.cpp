#include "Matrix.h"
#include "Display.h"

Adafruit_PCD8544 Display::controller = Adafruit_PCD8544(PIN_CLK, PIN_DIN, PIN_DC, PIN_CE, PIN_RST);

void Display::Initialize() {
  // initialize controller
  Display::controller.begin();

  // set contrast
  Display::controller.setContrast(60);
  
  // clears the screen and buffer
  Display::controller.clearDisplay(); 
}


void Display::UpdateFromDisplay() {
  Display::controller.display();
}

void Display::UpdateFromMatrix() {
  FOR_i {
    FOR_j {
      FOR_k {
        PIXEL_MAP_ijk(i, j, k);
      }
    }
  }

  Display::UpdateFromDisplay();
}

void Display::Set(uint8_t x, uint8_t y) {
  Display::controller.drawPixel(x, y, BLACK);
}

void Display::Unset(uint8_t x, uint8_t  y) {
  Display::controller.drawPixel(x, y, WHITE);
}

void Display::DrawLine(uint8_t  x0, uint8_t  y0, uint8_t  x1, uint8_t  x2, uint8_t  color) {
  Display::controller.drawLine(x0, y0, x1, x2, color);
};

void Display::DrawText(uint8_t x, uint8_t y, String &text) {
  Display::controller.setCursor(x, y);
  Display::controller.setTextColor(WHITE, BLACK);
  Display::controller.print(text);
};
