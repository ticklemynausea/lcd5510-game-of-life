#ifndef _routine_h_
#define _routine_h_

#include "Display.h"
#include "Matrix.h"
#include "LED.h"
#include "Buttons.h"

/* Loop Routines */
class Routine {
  public:

  static void (*DisplayUpdate)();
  static void (*UpdateMatrix)();
  static void (*Led)();
  static void (*Buttons)();

  static void Initialize();
};

#endif
