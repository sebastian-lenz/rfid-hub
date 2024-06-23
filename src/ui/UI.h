#ifndef RFID_HUB_UI_H
#define RFID_HUB_UI_H

#include "base/Core.h"
#include "base/Event.h"
#include "ui/Screen.h"
#include "ui/ScreenEvent.h"
#include "globals.h"

class UI : public Core
{
private:
  unsigned long _lastButtonPress = 0;
  Screen* _screen;

public:
  UI();
  void onScreenEvent(const Event& event);

protected:
  void onLoop() override;
  void onSetup() override;

private:
  void setScreen(ScreenType type);
};

#endif //RFID_HUB_UI_H
