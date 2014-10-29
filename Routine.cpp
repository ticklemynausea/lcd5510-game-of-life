#include "Routine.h"

void (*Routine::DisplayUpdate)() = Display::UpdateFromMatrix;
void (*Routine::UpdateMatrix)() = Matrix::GameOfLife;
void (*Routine::Led)() = LED::Toggle;
void (*Routine::Buttons)() = Buttons::Read;

void Routine::Initialize() {
  // random seed
  randomSeed(analogRead(0)); 
}
