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

#include "Nokia_5110_LCD_GameOfLife.h"
#include "Matrix.h"
#include "Display.h"
#include "LED.h"
#include "Buttons.h"
#include "Routine.h"

#ifndef _include_libs_adafruit_
#define _include_libs_adafruit_
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#endif _include_libs_adafruit_

/* Setup & Loop */

void setup() {
  // random seed
  randomSeed(analogRead(0));

  Display::Initialize();
  
  // gpio pins
  LED::Initialize();

  // initialize Buttons
  Buttons::Initialize();

  // initialize Matrix
  Matrix::InitializeRandom();
}


void loop() {
 
  Routine::DisplayUpdate();

  Routine::UpdateMatrix();
  
  Routine::Led();

  Routine::Buttons();
 
}
