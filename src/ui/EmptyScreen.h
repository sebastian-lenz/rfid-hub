#ifndef RFID_HUB_EMPTY_SCREEN_H
#define RFID_HUB_EMPTY_SCREEN_H

#include "ui/Screen.h"

class EmptyScreen : public Screen {
private:
  bool _didRender = false;

public:
  EmptyScreen();
  ~EmptyScreen() override;
  void onButton(ButtonSignal signal, int index) override;
};

#endif //RFID_HUB_EMPTY_SCREEN_H
