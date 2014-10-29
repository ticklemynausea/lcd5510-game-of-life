#include "Buttons.h"
#include "Matrix.h"
#include "Routine.h"
#include "GFX.h"

/* button pins */
uint8_t Buttons::pinout[NUM_BUTTONS] = {8, 9, 10, 11, 12};

/* button states (to be used) */
uint8_t Buttons::state[NUM_BUTTONS] = {LOW};

void (*Buttons::callbacks[NUM_BUTTONS])() = {
  &Buttons::callbackButton1,
  &Buttons::callbackButton2,
  &Buttons::callbackButton3,
  &Buttons::callbackButton4,
  &Buttons::callbackButton5
};


/* Buttons */
void Buttons::Initialize() {
  FOR_b {
    pinMode(Buttons::pinout[b], INPUT_PULLUP);  
  }
}

void Buttons::Read() {
  FOR_b {
    int state = digitalRead(Buttons::pinout[b]);
    
    // check if the pushbutton is pressed.
    // if it is, the state is HIGH:
    if (state == HIGH) {
      0;  
    } else if (state == LOW) {
      Buttons::callbacks[b]();
    }
  }
}

void Buttons::callbackButton1() {
  Matrix::InitializeProbability(random(32, 128));
  GFX::HorizontalSwipe();
}

void Buttons::callbackButton2() {
  Matrix::InitializeProbability(random(32, 128));
  GFX::VerticalSwipe();
}

void Buttons::callbackButton3() {
  Matrix::Flip(); 
}

void Buttons::callbackButton4() {
  Routine::SleepDecrease(10);
  String n = String(Routine::sleep);
  String s = String("Sleep: " + n + "ms");
  GFX::Message(10, 10, s);
}

void Buttons::callbackButton5() {
  Routine::SleepIncrease(10);
  String n = String(Routine::sleep);
  String s = String("Sleep: " + n + "ms");
  GFX::Message(10, 10, s);
}
