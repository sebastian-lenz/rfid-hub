#include "ui/Screen.h"
#include "globals.h"
#include "sprites.h"

#define BUTTON_WIDTH 19
#define BUTTON_HEIGHT 32
#define BUTTON_RADIUS 4


Label::Label(char text) : _text(text) { };

Label::Label(const uint8_t* bitmap) {
  _bitmap = bitmap;
};

void Label::draw(int x, int y) {
  if (_text != ' ') {
    int width = u8g2.getStrWidth(&_text);
    u8g2.drawStr(x + 9 - width / 2, y + 20, &_text);
  } else if (_bitmap != nullptr) {
    u8g2.setBitmapMode(1);
    u8g2.drawXBM(x + 4, y + 11, 11, 11, _bitmap);
  }
};


void Screen::renderActions(Label firstLabel, Label secondLabel) {
  u8g2.setDrawColor(1);

  for (int index = 0; index < NUM_BUTTONS; index++) {
    renderAction(index, &buttons[index], index == 0 ? &firstLabel : &secondLabel);
  }
}

void Screen::renderAction(int index, Button* button, Label* label) {
  int height = BUTTON_HEIGHT + index;
  int x = SCREEN_WIDTH - BUTTON_WIDTH;
  int y = index * 32 - index;

  u8g2.setDrawColor(1);
  if (button->isPressed()) {
    u8g2.drawRBox(x, y, BUTTON_WIDTH, height, BUTTON_RADIUS);
    u8g2.setDrawColor(0);
  } else {
    u8g2.drawRFrame(x, y, BUTTON_WIDTH, height, BUTTON_RADIUS);
  }

  label->draw(x, y);
}