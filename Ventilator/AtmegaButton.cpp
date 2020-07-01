#include "AtmegaButton.h"
//https://arduino.stackexchange.com/questions/48243/detecting-if-a-push-button-was-pressed-and-released
AtmegaButton::AtmegaButton(uint8_t pin) : _pin(pin) {}

void AtmegaButton::begin() {
  pinMode(_pin, INPUT);
  _state = digitalRead(_pin);
}

bool AtmegaButton::isReleased() {
  bool v = digitalRead(_pin);
  if (v != _state) {
    _state = v;
    if (!_state) {
      return true;
    }
  }
  return false;
}
