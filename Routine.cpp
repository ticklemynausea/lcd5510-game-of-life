#include "Routine.h"

void (*Routine::DisplayUpdate)() = Display::Update;
void (*Routine::UpdateMatrix)() = Matrix::GameOfLife;
void (*Routine::Led)() = LED::Toggle;
void (*Routine::Buttons)() = Buttons::Read;
