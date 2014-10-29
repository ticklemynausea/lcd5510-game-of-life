#ifndef _matrix_h_
#define _matrix_h_

#ifndef _include_libs_arduino_
#define _include_libs_arduino_
#include <Arduino.h>
#endif

#include "Nokia_5110_LCD_GameOfLife.h"
#include "Display.h"

/* * checks if n is in the interval [a, b] * */
#define BETWEEN(n, a, b) ((n >= a) && (n <= b))

/* * over matrix buffer width */
#define FOR_i for (uint8_t i = 0; i < LCD_WIDTH; i++)
/* * over matrix buffer height */
#define FOR_j for (uint8_t j = 0; j < LCD_HEIGHT8; j++)
/* * bits over a byte */
#define FOR_k for (uint8_t k = 0; k < 8; k++)
#define FOR_n for (uint8_t n = 0; n < 8; n++)
/* * over screen width * */
#define FOR_x for (uint8_t x = 0; x < LCD_WIDTH; x++)
/* * over screen height * */
#define FOR_y for (uint8_t y = 0; y < LCD_HEIGHT; y++)


/* mapping macros */
#define MAP(i, j) (i + LCD_WIDTH*j)
#define MAP_i(x, y) (x)
#define MAP_j(x, y) (y/8)
#define MAP_k(x, y) (y%8)

/* * read/set/clear bit k in buffer position i j * */
#define BIT_READ(i, j, k) bitRead(Matrix::buffer[MAP(i, j)], k)
#define BIT_SET(i, j, k) bitSet(Matrix::buffer[MAP(i, j)], k)
#define BIT_CLEAR(i, j, k) bitClear(Matrix::buffer[MAP(i, j)], k)

/* * read/set/clear bits using screen position coordinates * */
#define BIT_SET_xy(x, y) BIT_SET(MAP_i(x, y),  MAP_j(x, y), MAP_k(x, y))
#define BIT_READ_xy(x, y) BIT_READ(MAP_i(x, y),  MAP_j(x, y), MAP_k(x, y))
#define BIT_CLEAR_xy(x, y) BIT_CLEAR(MAP_i(x, y),  MAP_j(x, y), MAP_k(x, y))

/* * neighbour directions */
#define NORTH 0
#define NORTHEAST 1
#define EAST 2
#define SOUTHEAST 3
#define SOUTH 4
#define SOUTHWEST 5
#define WEST 6
#define NORTHWEST 7

class Matrix {
  private:
  static uint8_t buffer_new[LCD_WIDTH * LCD_HEIGHT8];

  public:
  /* Matrix and temporary Matrix */
  static uint8_t buffer[LCD_WIDTH * LCD_HEIGHT8];

  Matrix();
  
  /* Matrix object: initializations */
  static void InitializeRandom();
  static void InitializeProbability(uint8_t chance_in_256);
  static void InitializePattern();
  static void InitializeBitmap();
  
  /* Implements Game of Life in using the main Matrix object */
  /* Game of Life uses dubaiss' "neighbours XXX" algorithm */
  static void GameOfLife();
  
  /* Bitwise Flip */
  static void Flip();
};

#endif
