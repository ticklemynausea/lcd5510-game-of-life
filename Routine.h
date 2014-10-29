#ifndef _routine_h_
#define _routine_h_

#include "Display.h"
#include "Matrix.h"
#include "LED.h"
#include "Buttons.h"

#define SLEEP_MAX 200
#define SLEEP_STEP 20

/* Loop Routines */
class Routine {
  public:
  static uint16_t sleep;
  static uint16_t pause;
  
  static void (*DisplayUpdate)();
  static void (*UpdateMatrix)();
  static void (*Led)();
  static void (*Buttons)();

  static void Initialize();
  static void Nop();

  static void Sleep();

  static void Reset();
  static void Pause();
  static void Flip();
  static void DecreaseDelay();
  static void IncreaseDelay();
  

};

#endif
