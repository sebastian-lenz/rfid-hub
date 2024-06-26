#ifndef RFID_HUB_HOME_SCREEN_H
#define RFID_HUB_HOME_SCREEN_H

#include "base/Event.h"
#include "hid/Sensor.h"
#include "ui/Screen.h"

class HomeScreen : public Screen
{
private:
  long          _currentSensorId = -1;
  unsigned long _frame = 0;
  unsigned long _listenerId = 0;

public:
  HomeScreen();
  ~HomeScreen() override;
  void render() override;
  void update() override;
  void onButton(ButtonSignal signal, int index) override;
  void onSensorEvent(const Event& event);

private:
  std::pair<char, char> getAvailableKeys();
  Sensor* getCurrentSensor() const;
  void renderSensor(Sensor *sensor, int y);
  void renderSensorIcon(Sensor *sensor, int x, int y) const;
};

#endif //RFID_HUB_HOME_SCREEN_H
