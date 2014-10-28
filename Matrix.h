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
#define FOR_i for (int i = 0; i < LCD_WIDTH; i++)
/* * over matrix buffer height */
#define FOR_j for (int j = 0; j < LCD_HEIGHT8; j++)
/* * bits over a byte */
#define FOR_k for (int k = 0; k < 8; k++)
#define FOR_n for (int n = 0; n < 8; n++)
/* * over screen width * */
#define FOR_x for (int x = 0; x < LCD_WIDTH; x++)
/* * over screen height * */
#define FOR_y for (int y = 0; y < LCD_HEIGHT; y++)


/* mapping macros */
#define MAP(i, j) (i + LCD_WIDTH*j)
#define MAP_i(x, y) (x)
#define MAP_j(x, y) (y/8)
#define MAP_k(x, y) (y%8)

/* * read/set/clear bit k in buffer position i j * */
#define BIT_READ(matrix, i, j, k) bitRead(matrix[MAP(i, j)], k)
#define BIT_SET(matrix, i, j, k) bitSet(matrix[MAP(i, j)], k)
#define BIT_CLEAR(matrix, i, j, k) bitClear(matrix[MAP(i, j)], k)

/* * read/set/clear bits using screen position coordinates * */
#define BIT_SET_xy(matrix, x, y) BIT_SET(matrix, MAP_i(x, y),  MAP_j(x, y), MAP_k(x, y))
#define BIT_READ_xy(matrix, x, y) BIT_READ(matrix, MAP_i(x, y),  MAP_j(x, y), MAP_k(x, y))
#define BIT_CLEAR_xy(matrix, x, y) BIT_CLEAR(matrix, MAP_i(x, y),  MAP_j(x, y), MAP_k(x, y))

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
  static unsigned char buffer_new[LCD_WIDTH * LCD_HEIGHT8];

  public:
  /* Matrix and temporary Matrix */
  static unsigned char buffer[LCD_WIDTH * LCD_HEIGHT8];

  Matrix();
  
  /* Matrix object: initializations */
  static void InitializeRandom();
  static void InitializePattern();
  static void InitializeBitmap();
  
  /* Implements Game of Life in using the main Matrix object */
  /* Game of Life uses dubaiss' "neighbours XXX" algorithm */
  static void GameOfLife();
  
  /* Bitwise Flip */
  static void Flip();
};

#endif
