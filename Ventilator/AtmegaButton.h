#include <Arduino.h>

class AtmegaButton {
  private:
    bool _state;
    uint8_t _pin;

  public:
    AtmegaButton(uint8_t pin);

    void begin();
    bool isReleased();
};
