#include "Routine.h"

void (*Routine::DisplayUpdate)() = Display::UpdateFromMatrix;
void (*Routine::UpdateMatrix)() = Matrix::GameOfLife;
void (*Routine::Led)() = LED::Toggle;
void (*Routine::Buttons)() = Buttons::Read;

uint16_t Routine::sleep;

void Routine::Initialize() {
  // random seed
  randomSeed(analogRead(0));

  Routine::sleep = 0;
}

void Routine::Sleep() {
  if (Routine::sleep > 0) {
    delay(Routine::sleep); 
  }
}

void Routine::SleepDecrease(uint16_t d) {
  // achtung Routine::sleep overflows when under 0
  if (BETWEEN(Routine::sleep - d, 0, SLEEP_MAX)) {
     Routine::sleep -= d;
  }
}

void Routine::SleepIncrease(uint16_t d) {
  if (Routine::sleep + d <= SLEEP_MAX) {
     Routine::sleep += d;
  }
}
