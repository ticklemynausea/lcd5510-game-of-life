/*  _   ______  __ __ _______       ___________________     __    __________
   / | / / __ \/ //_//  _/   |     / ____/ ____<  / __ \   / /   / ____/ __ \
  /  |/ / / / / ,<   / // /| |    /___ \/___ \ / / / / /  / /   / /   / / / /
 / /|  / /_/ / /| |_/ // ___ |   ____/ /___/ // / /_/ /  / /___/ /___/ /_/ /
/_/ |_/\____/_/ |_/___/_/  |_|  /_____/_____//_/\____/  /_____/\____/_____/

   ______                              ____   __    _ ____
  / ____/___ _____ ___  ___     ____  / __/  / /   (_) __/__
 / / __/ __ `/ __ `__ \/ _ \   / __ \/ /_   / /   / / /_/ _ \
/ /_/ / /_/ / / / / / /  __/  / /_/ / __/  / /___/ / __/  __/
\____/\__,_/_/ /_/ /_/\___/   \____/_/    /_____/_/_/  \___/ 

Nokia 5510 / Arduino Demo: Game of Life 
Uses Adafruit PCD8544 and GFX libraries

igotbugs@ticklemynausea.net */

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define LCD_WIDTH 84
#define LCD_HEIGHT 48
#define LCD_HEIGHT8 (LCD_HEIGHT >> 3)

#define map(i, j) (i + LCD_WIDTH*j)
#define map_x(i, j, k) (i)
#define map_y(i, j, k) (j*8+k)
#define map_i(x, y) (x)
#define map_j(x, y) (y/8)
#define map_k(x, y) (y%8)

#define for_i for (int i = 0; i < LCD_WIDTH; i++)
#define for_j for (int j = 0; j < LCD_HEIGHT8; j++)
#define for_k for (int k = 0; k < 8; k++)
#define for_n for (int n = 0; n < 8; n++)
#define for_b for (int b = 0; b < 5; b++)

#define for_x for (int x = 0; x < LCD_WIDTH; x++)
#define for_y for (int y = 0; y < LCD_HEIGHT; y++)

#define bit_read(matrix, i, j, k) bitRead(matrix[map(i, j)], k)
#define bit_set(matrix, i, j, k) bitSet(matrix[map(i, j)], k)
#define bit_clear(matrix, i, j, k) bitClear(matrix[map(i, j)], k)

#define bit_set_xy(matrix, x, y) bit_set(matrix, map_i(x, y),  map_j(x, y), map_k(x, y))
#define bit_read_xy(matrix, x, y) bit_read(matrix, map_i(x, y),  map_j(x, y), map_k(x, y))
#define bit_clear_xy(matrix, x, y) bit_clear(matrix, map_i(x, y),  map_j(x, y), map_k(x, y));

#define pixel_set(i, j, k) display.drawPixel(i, j*8+k, BLACK)
#define pixel_unset(i, j, k) display.drawPixel(i, j*8+k, WHITE)
#define pixel_map(i, j, k) display.drawPixel(i, j*8+k, bit_read(matrix, i, j, k) == 1 ? BLACK : WHITE)

#define between(n, a, b) ((n >= a) && (n <= b))

#define NORTH 0
#define NORTHEAST 1
#define EAST 2
#define SOUTHEAST 3
#define SOUTH 4
#define SOUTHWEST 5
#define WEST 6
#define NORTHWEST 7

#define RAND_RANGE 255

// Initialize LCD Display
// D7 - Serial clock out (CLK oder SCLK)
// D6 - Serial data out (DIN)
// D5 - Data/Command select (DC oder D/C)
// D4 - LCD chip select (CE oder CS)
// D3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

unsigned char matrix[LCD_WIDTH * LCD_HEIGHT8] = {0};
unsigned char matrix_new[LCD_WIDTH * LCD_HEIGHT8] = {0};
unsigned char toggle = 0;

/* button pins */
unsigned char button_pins[5] = {8, 9, 10, 11, 12};

/* button states */
unsigned char button_states[5] = {LOW};

/* button callbacks */
void callback_button_1();
void callback_button_2();
void callback_button_3();
void callback_button_4();
void callback_button_5();
void (*button_callbacks[5])() = {
  callback_button_1, callback_button_2, callback_button_3, callback_button_4, callback_button_5
};

/*   ______            __
    / ____/  ____     / /
   / / __   / __ \   / /
  / /_/ /  / /_/ /  / /___
  \____/   \____/  /_____/ */

void initialize_matrix(char limit) {
  for_x {
    for_y {
      char rand = random(RAND_RANGE);
      if (between(rand, 0, limit)) {
        bit_set_xy(matrix, x, y);
      }
    }
  }
}

// implemented using dubaiss' "neighbours XXX" algorithm
void evolve_matrix() {
  
  // new state
  unsigned char neighbour[8] = {0};
  unsigned char byte_cell;
  unsigned char byte_cell_new;
  unsigned char byte_cell_count;
  // calculate new state
  
  for_i { 
    for_j {

      byte_cell = matrix[map(i, j)];

      /* * get each bit's neighbour one byte at a time * */
      /* east and west */
      neighbour[WEST] = 0b00000000;
      if (i > 0) {
        neighbour[WEST] = matrix[map((i - 1), j)];
      }

      neighbour[EAST] = 0b00000000;
      if (i < (LCD_WIDTH - 1)) {
        neighbour[EAST] = matrix[map((i + 1), j)];
      }
      
      /* north */
      neighbour[NORTH] = 0b00000000;      
      neighbour[NORTHEAST] = 0b00000000;      
      neighbour[NORTHWEST] = 0b00000000;      
      if (j > 0) {
        neighbour[NORTH] = matrix[map(i, j - 1)];
        if (i > 0) {
          neighbour[NORTHWEST] = matrix[map(i - 1, j - 1)];
        }
        if (i < (LCD_WIDTH - 1)) {
          neighbour[NORTHEAST] = matrix[map(i + 1, j - 1)];
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
        neighbour[SOUTH] = matrix[map(i, j + 1)];
        if (i > 0) {
          neighbour[SOUTHWEST] = matrix[map(i - 1, j + 1)];
        }
        if (i < (LCD_WIDTH - 1)) {
          neighbour[SOUTHEAST] = matrix[map(i + 1, j + 1)];
        }
      }
      neighbour[SOUTH] = ((neighbour[SOUTH] & 0b00000001) << 7) | (byte_cell >> 1);
      neighbour[SOUTHEAST] = ((neighbour[SOUTHEAST] & 0b00000001) << 7) | (neighbour[EAST] >> 1);
      neighbour[SOUTHWEST] = ((neighbour[SOUTHWEST] & 0b00000001) << 7) | (neighbour[WEST] >> 1);

      /* calculate each bit of next gen */
      byte_cell_new = 0b00000000;
      
      for_k {
        byte_cell_count = 0;  
        for_n {
          byte_cell_count += (neighbour[n] & 0b00000001);
        }
        
        byte_cell_new >>= 1;
        if ((byte_cell_count == 3) || ((byte_cell_count == 2) && (byte_cell & 0b00000001))) {
          byte_cell_new |= 0b10000000;
        }
        
        for_n {
          neighbour[n] >>= 1;
        }
        byte_cell >>= 1;
      }
      
      matrix_new[map(i, j)] = byte_cell_new;
    }
  }
  
  // apply new state
  for_i {
    for_j {
      matrix[map(i, j)] = matrix_new[map(i, j)];
    }
  }

}

void update_display() {

  for_i {
    for_j {
      for_k {
        pixel_map(i, j, k);
      }
    }
  }

  display.display();
}

void toggle_pin() {
  if (toggle == 1) {
    digitalWrite(13, HIGH); 
    toggle = 0;
  } else {
    digitalWrite(13, LOW);
    toggle = 1;
  }
}

/*    ____        __  __
     / __ )__  __/ /_/ /_____  ____  _____
    / __  / / / / __/ __/ __ \/ __ \/ ___/
   / /_/ / /_/ / /_/ /_/ /_/ / / / (__  )
  /_____/\__,_/\__/\__/\____/_/ /_/____/  */

void initialize_buttons() {
  for_b {
    pinMode(button_pins[b], INPUT_PULLUP);  
  }
}

void read_buttons() {
  for_b {
    int buttonState = digitalRead(button_pins[b]);
    
    // check if the pushbutton is pressed.
    // if it is, the buttonState is HIGH:
    if (buttonState == HIGH) {
      digitalWrite(13, HIGH); 
  
    } else if (buttonState == LOW) {
      button_callbacks[b]();
      digitalWrite(13, LOW);
    }
  }
}

void callback_button_1() {
  initialize_matrix(RAND_RANGE / 2);
}

void callback_button_2() {
  initialize_matrix(RAND_RANGE * (2/3));
}

void callback_button_3() {
  initialize_matrix(RAND_RANGE * (1/3));
}

void callback_button_4() {
  delay(200);  
}

void callback_button_5() {
  delay(500);  
}

/* _____      __                 ___        __  ___      _
  / ___/___  / /___  ______     ( _ )      /  |/  /___ _(_)___
  \__ \/ _ \/ __/ / / / __ \   / __ \/|   / /|_/ / __ `/ / __ \
 ___/ /  __/ /_/ /_/ / /_/ /  / /_/  <   / /  / / /_/ / / / / /
/____/\___/\__/\__,_/ .___/   \____/\/  /_/  /_/\__,_/_/_/ /_/
                   /_/ */

void setup()   {

  // random seed
  randomSeed(analogRead(0));

  // initialize display
  display.begin();

  // set contrast
  display.setContrast(60);
  
  // clears the screen and buffer
  display.clearDisplay();
  
  // gpio pins
  pinMode(13, OUTPUT);

  // initialize buttons
  initialize_buttons();

  // initialize matrix
  initialize_matrix(RAND_RANGE / 2);
}


void loop() {
 
  update_display();

  evolve_matrix();
  
  toggle_pin();

  read_buttons();
 
}
