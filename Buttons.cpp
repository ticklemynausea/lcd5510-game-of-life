#include "Buttons.h"
#include "Routine.h"

/* button pins */
uint8_t Buttons::pinout[NUM_BUTTONS] = {8, 9, 10, 11, 12};

/* button state */
uint8_t Buttons::state[NUM_BUTTONS];

void (*Buttons::callbacks[NUM_BUTTONS])() = {
  &Routine::Reset,
  &Routine::Pause,
  &Routine::Flip,
  &Routine::DecreaseDelay,
  &Routine::IncreaseDelay
};


/* Buttons */
void Buttons::Initialize() {
  FOR_b {
    pinMode(Buttons::pinout[b], INPUT_PULLUP);
    Buttons::state[b] = HIGH;
  }
}

void Buttons::Read() {
  FOR_b {
    uint8_t new_state = digitalRead(Buttons::pinout[b]);
    uint8_t previous_state = Buttons::state[b];

    // check if the pushbutton is pressed.
    // if it is, the state is HIGH:
    if (new_state == LOW && previous_state == HIGH) {
      Buttons::callbacks[b]();
      Buttons::state[b] = new_state;
    }
    
    if (previous_state == LOW && new_state == HIGH) {
      Buttons::state[b] = new_state;      
    }
  }
}
