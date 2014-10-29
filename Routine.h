#ifndef _routine_h_
#define _routine_h_

#include "Display.h"
#include "Matrix.h"
#include "LED.h"
#include "Buttons.h"

#define SLEEP_MAX 100

/* Loop Routines */
class Routine {
  public:
  static uint16_t sleep;
  
  static void (*DisplayUpdate)();
  static void (*UpdateMatrix)();
  static void (*Led)();
  static void (*Buttons)();

  static void Initialize();

  static void Sleep();
  static void SleepIncrease(uint16_t d);  
  static void SleepDecrease(uint16_t d);

};

#endif
