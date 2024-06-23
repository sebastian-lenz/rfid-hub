#include "EmptyScreen.h"
#include "ui/ScreenEvent.h"
#include "globals.h"

EmptyScreen::EmptyScreen() {
  u8g2.setPowerSave(1);
}

EmptyScreen::~EmptyScreen() {
  u8g2.setPowerSave(0);
  u8g2.setContrast(100);
}

void EmptyScreen::onButton(ButtonSignal signal, int index) {
  if (signal == ButtonSignal::PRESS) {
    dispatcher.trigger(ScreenEvent(ScreenType::Home));
  }
};
