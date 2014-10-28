#include "Matrix.h"
#include "Display.h"

#ifndef _include_libs_adafruit_
#define _include_libs_adafruit_
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#endif _include_libs_adafruit_

Adafruit_PCD8544 Display::controller = Adafruit_PCD8544(PIN_CLK, PIN_DIN, PIN_DC, PIN_CE, PIN_RST);

void Display::Initialize() {
  // initialize controller
  Display::controller.begin();

  // set contrast
  Display::controller.setContrast(60);
  
  // clears the screen and buffer
  Display::controller.clearDisplay(); 
}

void Display::Update() {
  FOR_i {
    FOR_j {
      FOR_k {
        PIXEL_MAP(Matrix::buffer, i, j, k);
      }
    }
  }

  Display::controller.display();
}
