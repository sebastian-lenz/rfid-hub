#ifndef RFID_HUB_CORE_H
#define RFID_HUB_CORE_H

#include <Arduino.h>
#include "Timeouts.h"

class Core : public Timeouts
{
public:
  void loop() {
    unsigned long startMillis = millis();
    processTimeouts();
    onLoop();

    long duration = (long)(_targetDuration - (millis() - startMillis));
    if (duration > 0) {
      delay(duration);
    }
  }

  void setup() {
    onSetup();
  }

protected:
  virtual void onLoop() {}
  virtual void onSetup() {}

private:
  unsigned long _targetDuration = 80;
};

#endif //RFID_HUB_CORE_H
