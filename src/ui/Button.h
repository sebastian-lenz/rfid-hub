#ifndef RFID_HUB_BUTTON_H
#define RFID_HUB_BUTTON_H

#include <Arduino.h>

enum class ButtonSignal {
  NONE,
  PRESS,
  RELEASE,
  REPEAT,
};

class Button {
private:
  const uint _buttonPin;
  bool       _isPressed = false;
  long       _pressedFrames = 0;

public:
  explicit Button(uint buttonPin)
    : _buttonPin(buttonPin)
  { }

  void begin() const {
    gpio_init(_buttonPin);
    gpio_set_dir(_buttonPin, GPIO_IN);
    gpio_pull_up(_buttonPin);
  }

  ButtonSignal update() {
    bool isPressed = !gpio_get(_buttonPin);
    bool didChange = _isPressed != isPressed;
    _isPressed = isPressed;

    if (didChange) {
      _pressedFrames = -6;
      return isPressed
        ? ButtonSignal::PRESS
        : ButtonSignal::RELEASE;
    }

    if (isPressed) {
      _pressedFrames += 1;
      return _pressedFrames > 0 && _pressedFrames % 4 == 0
        ? ButtonSignal::REPEAT
        : ButtonSignal::NONE;
    }

    return ButtonSignal::NONE;
  }

  [[nodiscard]]
  bool isPressed() const {
    return _isPressed;
  }
};

#endif //RFID_HUB_BUTTON_H
