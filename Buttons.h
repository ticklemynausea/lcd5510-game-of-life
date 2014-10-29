#ifndef _buttons_h_
#define _buttons_h_

#ifndef _include_libs_arduino_
#define _include_libs_arduino_
#include <Arduino.h>
#endif

#define FOR_b for (int b = 0; b < 5; b++)

#define BUTTON_A 0
#define BUTTON_B 1
#define BUTTON_C 2
#define BUTTON_D 3
#define BUTTON_E 4
#define BUTTON_F 5
#define NUM_BUTTONS 5


class Buttons {
  public:
  
  static uint8_t pinout[NUM_BUTTONS];
  static uint8_t state[NUM_BUTTONS];
  
  /* Initialize Buttons */
  static void Initialize();
  
  /* Reads the input from the tactile switches connected to the digital input pins in Buttons_Pinout[] */
  static void Read();
  
  /* button callbacks */
  static void (*callbacks[NUM_BUTTONS])();
  
};

#endif
