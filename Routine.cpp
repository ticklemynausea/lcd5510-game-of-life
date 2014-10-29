#include "Routine.h"
#include "Matrix.h"
#include "GFX.h"

void (*Routine::DisplayUpdate)() = Display::UpdateFromMatrix;
void (*Routine::UpdateMatrix)() = Matrix::GameOfLife;
void (*Routine::Led)() = LED::Toggle;
void (*Routine::Buttons)() = Buttons::Read;

uint16_t Routine::sleep;

uint16_t Routine::pause;

void Routine::Initialize() {
  // random seed
  randomSeed(analogRead(0));

  Routine::sleep = 0;
  Routine::pause = 0;
}

void Routine::Nop() {
    
}

void Routine::Sleep() {
  if (Routine::sleep > 0) {
    delay(Routine::sleep); 
  }
}

void Routine::Reset() {
  Matrix::InitializeProbability(random(32, 128));
  GFX::RandomSwipe();
  delay(300);
}

void Routine::Pause() {

  String str, count_live_str, count_dead_str, percent_str;
  uint16_t count_live, count_dead, percent;
  uint16_t len_padding;
    
  if (Routine::pause == 0) {

    Routine::pause = 1;

    Routine::DisplayUpdate = Routine::Nop;
    Routine::UpdateMatrix = Routine::Nop;
    Routine::Led = Routine::Nop;
   
    Matrix::GetInfo(count_live, count_dead, percent);
    
    count_live_str = String(count_live);
    count_dead_str = String(count_dead);
    percent_str = String(percent);
    
    str = String("    Paused    ");  
    GFX::Message(0, 0, str);

    str = String(count_live_str + "/" + count_dead_str + "=" + percent_str + "%   ");
    str = str.substring(0, 14);
    GFX::Message(0, 41, str);

    Display::UpdateFromDisplay();

  } else {

    Routine::pause = 0;
    Routine::DisplayUpdate = Display::UpdateFromMatrix;
    Routine::UpdateMatrix = Matrix::GameOfLife;
    Routine::Led = LED::Toggle;

  }
}

void Routine::Flip() {
  Matrix::Flip(); 
}

void Routine::DecreaseDelay() {
  // achtung Routine::sleep overflows when under 0
  if (BETWEEN(Routine::sleep - SLEEP_STEP, 0, SLEEP_MAX)) {
    Routine::sleep -= SLEEP_STEP;
    String n = String(Routine::sleep);
    String s = String("Delay: " + n + "ms");
    GFX::Message(10, 10, s);
    Display::UpdateFromDisplay();
    delay(500);
  }
}

void Routine::IncreaseDelay() {
  if (Routine::sleep + SLEEP_STEP <= SLEEP_MAX) {
    Routine::sleep += SLEEP_STEP;
    String n = String(Routine::sleep);
    String s = String("Delay: " + n + "ms");
    GFX::Message(10, 10, s);
    Display::UpdateFromDisplay();
    delay(500);
  }
}
