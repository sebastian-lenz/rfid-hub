#ifndef RFID_HUB_SIMULATOR_H
#define RFID_HUB_SIMULATOR_H

#include <Keyboard.h>
#include "base/Event.h"
#include "hid/SensorEvent.h"
#include "hid/Settings.h"

class Emitter
{
private:
  Keyboard_ _keyboard;

public:
  void begin();

private:
  void handlePressKey(const SensorEventType type, const KeyMapping keyMapping);
  void handleHoldKey(const SensorEventType type, const KeyMapping keyMapping);
  void handleSendCommand(const SensorEvent& sensorEvent);
  void onSensorEvent(const Event& event);
};

#endif //RFID_HUB_SIMULATOR_H
