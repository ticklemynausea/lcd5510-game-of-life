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

/* LCD Parameters */
#define LCD_WIDTH 84
#define LCD_HEIGHT 48
#define LCD_HEIGHT8 (LCD_HEIGHT >> 3)

/* mapping macros */
#define map(i, j) (i + LCD_WIDTH*j)
#define map_x(i, j, k) (i)
#define map_y(i, j, k) (j*8+k)
#define map_i(x, y) (x)
#define map_j(x, y) (y/8)
#define map_k(x, y) (y%8)

/* cycles */
/* * over Matrix buffer width */
#define for_i for (int i = 0; i < LCD_WIDTH; i++)
/* * over Matrix buffer height */
#define for_j for (int j = 0; j < LCD_HEIGHT8; j++)
/* * bits over a byte */
#define for_k for (int k = 0; k < 8; k++)
#define for_n for (int n = 0; n < 8; n++)

/* * Buttons * */
#define for_b for (int b = 0; b < 5; b++)
/* * over screen width * */
#define for_x for (int x = 0; x < LCD_WIDTH; x++)
/* * over screen height * */
#define for_y for (int y = 0; y < LCD_HEIGHT; y++)

/* * read/set/clear bit k in buffer position i j * */
#define bit_read(Matrix, i, j, k) bitRead(Matrix[map(i, j)], k)
#define bit_set(Matrix, i, j, k) bitSet(Matrix[map(i, j)], k)
#define bit_clear(Matrix, i, j, k) bitClear(Matrix[map(i, j)], k)

/* * read/set/clear bits using screen position coordinates * */
#define bit_set_xy(Matrix, x, y) bit_set(Matrix, map_i(x, y),  map_j(x, y), map_k(x, y))
#define bit_read_xy(Matrix, x, y) bit_read(Matrix, map_i(x, y),  map_j(x, y), map_k(x, y))
#define bit_clear_xy(Matrix, x, y) bit_clear(Matrix, map_i(x, y),  map_j(x, y), map_k(x, y));

/* * set/unset pixel in screen using buffer coordinates) */
#define pixel_set(i, j, k) display.drawPixel(i, j*8+k, BLACK)
#define pixel_unset(i, j, k) display.drawPixel(i, j*8+k, WHITE)
#define pixel_map(i, j, k) display.drawPixel(i, j*8+k, bit_read(Matrix, i, j, k) == 1 ? BLACK : WHITE)

/* * checks if n is in the interval [a, b] * */
#define between(n, a, b) ((n >= a) && (n <= b))

/* * neighbour directions */
#define NORTH 0
#define NORTHEAST 1
#define EAST 2
#define SOUTHEAST 3
#define SOUTH 4
#define SOUTHWEST 5
#define WEST 6
#define NORTHWEST 7

/* Initialize LCD Display
   D7 - Serial clock out (CLK oder SCLK)
   D6 - Serial data out (DIN)
   D5 - Data/Command select (DC oder D/C)
   D4 - LCD chip select (CE oder CS)
   D3 - LCD reset (RST) */
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

/* Matrix and temporary Matrix */
unsigned char Matrix[LCD_WIDTH * LCD_HEIGHT8] = {0};
unsigned char Matrix_new[LCD_WIDTH * LCD_HEIGHT8] = {0};

/* LED toggle */
unsigned char LED_State = 0;

/* button pins */
unsigned char Buttons_Pinout[5] = {8, 9, 10, 11, 12};

/* button states (to be used) */
unsigned char Buttons_ButtonStates[5] = {LOW};

/* button callbacks */
/* * forward declarations * */
void Buttons_CallbackButton1();
void Buttons_CallbackButton2();
void Buttons_CallbackButton3();
void Buttons_CallbackButton4();
void Buttons_CallbackButton5();

/* * callback array */
void (*button_callbacks[5])() = {
  Buttons_CallbackButton1, Buttons_CallbackButton2, Buttons_CallbackButton3, Buttons_CallbackButton4, Buttons_CallbackButton5
};

/* Initialize the LED display */
void Display_Initialize();

/* Standard "Update Display" routine: Maps the bitmap in Matrix to the LCD screen */
void Display_Update();


/* Matrix object: initializations */
void Matrix_InitializeRandom();
void Matrix_InitializePattern();
void Matrix_InitializeBitmap();

/* Implements Game of Life in using the main Matrix object */
void Matrix_GameOfLife_dubaiss();

/* Initialize LED GPIO Pin */
void LED_Initialize();

/* Toggles the standard LED on the arduino */
void LED_Toggle();

/* Reads the input from the tactile switches connected to the digital input pins in Buttons_Pinout[] */
void Buttons_Read();

/* Loop Routines */
void (*loop_DisplayUpdate)() = Display_Update;
void (*loop_UpdateMatrix)() = Matrix_GameOfLife_dubaiss;
void (*loop_Led)() = LED_Toggle;
void (*loop_Buttons)() = Buttons_Read;


/* Data strucutre initialization & Game of Life algorithm */
void Matrix_InitializeRandom() {
  for_i {
    for_j {
      Matrix[map(i, j)] = random(256);
    }
  }
}

void Matrix_InitializePattern() {
  for_i {
    for_j {
      if ((i + j) % 2 == 0)
        Matrix[map(i, j)] = random(256);
    }
  }
}

void Matrix_InitializeBitmap() {
  for_i {
    for_j {
      Matrix[map(i, j)] = random(256);
    }
  }
}

/* implemented using dubaiss' "neighbours XXX" algorithm */
void Matrix_GameOfLife_dubaiss() {
  
  // new state
  unsigned char neighbour[8] = {0};
  unsigned char byte_cell;
  unsigned char byte_cell_new;
  unsigned char byte_cell_count;
  // calculate new state
  
  for_i { 
    for_j {

      byte_cell = Matrix[map(i, j)];

      /* * get each bit's neighbour one byte at a time * */
      /* east and west */
      neighbour[WEST] = 0b00000000;
      if (i > 0) {
        neighbour[WEST] = Matrix[map((i - 1), j)];
      }

      neighbour[EAST] = 0b00000000;
      if (i < (LCD_WIDTH - 1)) {
        neighbour[EAST] = Matrix[map((i + 1), j)];
      }
      
      /* north */
      neighbour[NORTH] = 0b00000000;      
      neighbour[NORTHEAST] = 0b00000000;      
      neighbour[NORTHWEST] = 0b00000000;      
      if (j > 0) {
        neighbour[NORTH] = Matrix[map(i, j - 1)];
        if (i > 0) {
          neighbour[NORTHWEST] = Matrix[map(i - 1, j - 1)];
        }
        if (i < (LCD_WIDTH - 1)) {
          neighbour[NORTHEAST] = Matrix[map(i + 1, j - 1)];
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
        neighbour[SOUTH] = Matrix[map(i, j + 1)];
        if (i > 0) {
          neighbour[SOUTHWEST] = Matrix[map(i - 1, j + 1)];
        }
        if (i < (LCD_WIDTH - 1)) {
          neighbour[SOUTHEAST] = Matrix[map(i + 1, j + 1)];
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
      
      Matrix_new[map(i, j)] = byte_cell_new;
    }
  }
  
  // apply new state
  for_i {
    for_j {
      Matrix[map(i, j)] = Matrix_new[map(i, j)];
    }
  }

}

void Matrix_Flip() {
  for_i {
    for_j {
      Matrix[map(i, j)] = ~Matrix[map(i, j)];
    }
  }
}

void Display_Initialize() {
  // initialize display
  display.begin();

  // set contrast
  display.setContrast(60);
  
  // clears the screen and buffer
  display.clearDisplay(); 
}

void Display_Update() {

  for_i {
    for_j {
      for_k {
        pixel_map(i, j, k);
      }
    }
  }

  display.display();
}


void LED_Initialize() {
  pinMode(13, OUTPUT);
}

void LED_Toggle() {
  if (LED_State == 1) {
    digitalWrite(13, HIGH); 
    LED_State = 0;
  } else {
    digitalWrite(13, LOW);
    LED_State = 1;
  }
}

/* Buttons */
void Buttons_Initialize() {
  for_b {
    pinMode(Buttons_Pinout[b], INPUT_PULLUP);  
  }
}

void Buttons_Read() {
  for_b {
    int Buttonstate = digitalRead(Buttons_Pinout[b]);
    
    // check if the pushbutton is pressed.
    // if it is, the Buttonstate is HIGH:
    if (Buttonstate == HIGH) {
      0;  
    } else if (Buttonstate == LOW) {
      button_callbacks[b]();
    }
  }
}

void Buttons_CallbackButton1() {
  Matrix_InitializeRandom();
}

void Buttons_CallbackButton2() {
  Matrix_InitializePattern();
}

void Buttons_CallbackButton3() {
  Matrix_InitializeBitmap();
}

void Buttons_CallbackButton4() {
  Matrix_Flip(); 
}

void Buttons_CallbackButton5() {
  delay(250);  
}

/* Setup & Loop */

void setup()   {

  // random seed
  randomSeed(analogRead(0));

  Display_Initialize();
  
  // gpio pins
  LED_Initialize();

  // initialize Buttons
  Buttons_Initialize();

  // initialize Matrix
  Matrix_InitializeRandom();
}


void loop() {
 
  loop_DisplayUpdate();

  loop_UpdateMatrix();
  
  loop_Led();

  loop_Buttons();
 
}
