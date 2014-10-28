#ifndef _display_h_
#define _display_h_

#include "Nokia_5110_LCD_GameOfLife.h"

#ifndef _include_libs_adafruit_
#define _include_libs_adafruit_
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#endif _include_libs_adafruit_

/* LCD Parameters */
#define LCD_WIDTH 84
#define LCD_HEIGHT 48
#define LCD_HEIGHT8 (LCD_HEIGHT >> 3)

/* LCD Display Pinout:
   D7 - Serial clock out (CLK oder SCLK)
   D6 - Serial data out (DIN)
   D5 - Data/Command select (DC oder D/C)
   D4 - LCD chip select (CE oder CS)
   D3 - LCD reset (RST) */
#define PIN_CLK 7
#define PIN_DIN 6
#define PIN_DC 5
#define PIN_CE 4
#define PIN_RST 3

/* * set/unset pixel in screen using buffer coordinates */
#define PIXEL_SET(i, j, k) Display::controller.drawPixel(i, j*8+k, BLACK)
#define PIXEL_UNSET(i, j, k) Display::controller.drawPixel(i, j*8+k, WHITE)
#define PIXEL_MAP(Matrix, i, j, k) Display::controller.drawPixel(i, j*8+k, BIT_READ(Matrix, i, j, k) == 1 ? BLACK : WHITE)

class Display {
  public:

  static Adafruit_PCD8544 controller;
  
  /* Initialize the LED display */
  static void Initialize();

  /* Standard "Update Display" routine: Maps the bitmap in Matrix to the LCD screen */
  static void Update();
};

#endif