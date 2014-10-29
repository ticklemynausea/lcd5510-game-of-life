#include "Matrix.h"

uint8_t Matrix::buffer_new[LCD_WIDTH * LCD_HEIGHT8];
uint8_t Matrix::buffer[LCD_WIDTH * LCD_HEIGHT8];

/* Data strucutre initialization & Game of Life algorithm */
void Matrix::InitializeRandom() {
  FOR_i {
    FOR_j {
      Matrix::buffer[MAP(i, j)] = random(256);
    }
  }
}

void Matrix::InitializePattern() {
  FOR_i {
    FOR_j {
      if ((i + j) % 2 == 0)
        Matrix::buffer[MAP(i, j)] = random(256);
    }
  }
}

void Matrix::InitializeBitmap() {
  FOR_i {
    FOR_j {
      Matrix::buffer[MAP(i, j)] = random(256);
    }
  }
}

void Matrix::GameOfLife() {
  
  // new state
  uint8_t neighbour[8] = {0};
  uint8_t byte_cell;
  uint8_t byte_cell_new;
  uint8_t byte_cell_count;
  // calculate new state
  
  FOR_i { 
    FOR_j {

      byte_cell = Matrix::buffer[MAP(i, j)];

      /* * get each bit's neighbour one byte at a time * */
      /* east and west */
      neighbour[WEST] = 0b00000000;
      if (i > 0) {
        neighbour[WEST] = Matrix::buffer[MAP((i - 1), j)];
      }

      neighbour[EAST] = 0b00000000;
      if (i < (LCD_WIDTH - 1)) {
        neighbour[EAST] = Matrix::buffer[MAP((i + 1), j)];
      }
      
      /* north */
      neighbour[NORTH] = 0b00000000;      
      neighbour[NORTHEAST] = 0b00000000;      
      neighbour[NORTHWEST] = 0b00000000;      
      if (j > 0) {
        neighbour[NORTH] = Matrix::buffer[MAP(i, j - 1)];
        if (i > 0) {
          neighbour[NORTHWEST] = Matrix::buffer[MAP(i - 1, j - 1)];
        }
        if (i < (LCD_WIDTH - 1)) {
          neighbour[NORTHEAST] = Matrix::buffer[MAP(i + 1, j - 1)];
        }
      }
      neighbour[NORTH] = (byte_cell << 1) | ((neighbour[NORTH] & 0b10000000) >> 7);
      neighbour[NORTHEAST] = (neighbour[EAST] << 1) | ((neighbour[NORTHEAST] & 0b1000000) >> 7);
      neighbour[NORTHWEST] = (neighbour[WEST] << 1) | ((neighbour[NORTHWEST] & 0b1000000) >> 7);
      
      /* south */
      neighbour[SOUTH] = 0b00000000;      
      neighbour[SOUTHEAST] = 0b00000000;      
      neighbour[SOUTHWEST] = 0b00000000;   
      if (j < (LCD_HEIGHT8 - 1)) {
        neighbour[SOUTH] = Matrix::buffer[MAP(i, j + 1)];
        if (i > 0) {
          neighbour[SOUTHWEST] = Matrix::buffer[MAP(i - 1, j + 1)];
        }
        if (i < (LCD_WIDTH - 1)) {
          neighbour[SOUTHEAST] = Matrix::buffer[MAP(i + 1, j + 1)];
        }
      }
      neighbour[SOUTH] = ((neighbour[SOUTH] & 0b00000001) << 7) | (byte_cell >> 1);
      neighbour[SOUTHEAST] = ((neighbour[SOUTHEAST] & 0b00000001) << 7) | (neighbour[EAST] >> 1);
      neighbour[SOUTHWEST] = ((neighbour[SOUTHWEST] & 0b00000001) << 7) | (neighbour[WEST] >> 1);

      /* calculate each bit of next gen */
      byte_cell_new = 0b00000000;
      
      FOR_k {
        byte_cell_count = 0;  
        FOR_n {
          byte_cell_count += (neighbour[n] & 0b00000001);
        }
        
        byte_cell_new >>= 1;
        if ((byte_cell_count == 3) || ((byte_cell_count == 2) && (byte_cell & 0b00000001))) {
          byte_cell_new |= 0b10000000;
        }
        
        FOR_n {
          neighbour[n] >>= 1;
        }
        byte_cell >>= 1;
      }
      
      Matrix::buffer_new[MAP(i, j)] = byte_cell_new;
    }
  }
  
  // apply new state
  FOR_i {
    FOR_j {
      Matrix::buffer[MAP(i, j)] = Matrix::buffer_new[MAP(i, j)];
    }
  }

}

void Matrix::Flip() {
  FOR_i {
    FOR_j {
      Matrix::buffer[MAP(i, j)] = ~Matrix::buffer[MAP(i, j)];
    }
  }
}
