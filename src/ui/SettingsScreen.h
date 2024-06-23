#ifndef RFID_HUB_SETTINGS_SCREEN_H
#define RFID_HUB_SETTINGS_SCREEN_H

#include "ui/Screen.h"

class SettingsScreen : public Screen {
private:
  long _currentSetting = 0;

public:
  void render() override;
  void onButton(ButtonSignal signal, int index) override;
};

#endif //RFID_HUB_SETTINGS_SCREEN_H
