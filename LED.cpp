#include "LED.h"

uint8_t LED::state = 0;
void LED::Initialize() {
  pinMode(13, OUTPUT);
}

void LED::Toggle() {
  if (LED::state == 1) {
    digitalWrite(13, HIGH); 
    LED::state = 0;
  } else {
    digitalWrite(13, LOW);
    LED:state = 1;
  }
}
