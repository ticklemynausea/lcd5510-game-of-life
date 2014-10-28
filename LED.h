#ifndef _led_h_
#define _led_h_

#ifndef _include_libs_arduino_
#define _include_libs_arduino_
#include <Arduino.h>
#endif

class LED {
  private:
  /* LED toggle */
  static unsigned char state;
  
  public:
  /* Initialize LED GPIO Pin */
  static void Initialize();

  /* Toggles the standard LED on the arduino */
  static void Toggle();
};

#endif
