#ifndef _buttons_h_
#define _buttons_h_

#ifndef _include_libs_arduino_
#define _include_libs_arduino_
#include <Arduino.h>
#endif

#define FOR_b for (int b = 0; b < 5; b++)

#define NUM_BUTTONS 5


class Buttons {
  public:
  
  static unsigned char pinout[NUM_BUTTONS];
  static unsigned char state[NUM_BUTTONS];
  
  /* Initialize Buttons */
  static void Initialize();
  
  /* Reads the input from the tactile switches connected to the digital input pins in Buttons_Pinout[] */
  static void Read();
  
  /* button callbacks */
  static void callbackButton1();
  static void callbackButton2();
  static void callbackButton3();
  static void callbackButton4();
  static void callbackButton5();
  
  static void (*callbacks[NUM_BUTTONS])();
  
};



#endif
