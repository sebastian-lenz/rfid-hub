#ifndef RFID_HUB_SCREEN_H
#define RFID_HUB_SCREEN_H

#include "ui/Button.h"

class Label
{
private:
  const char _text = ' ';
  const uint8_t* _bitmap = nullptr;
public:
  explicit Label(char text);
  explicit Label(const uint8_t* bitmap);
  void draw(int x, int y);
};


class Screen
{
public:
  virtual ~Screen() = default;
  virtual void render() { }
  virtual void update() { }
  virtual void onButton(ButtonSignal signal, int index) {}

protected:
  void renderActions(Label firstLabel, Label secondLabel);

private:
  void renderAction(int index, Button* button, Label* label);
};

#endif //RFID_HUB_SCREEN_H
