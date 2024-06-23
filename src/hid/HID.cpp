#include "hid/HID.h"
#include "globals.h"

void HID::onSetup() {
  #if DEBUG
    Serial.begin(9600);
    while (!Serial) {
      delay(100);
    }

    delay(200);
    Serial.println("RFID Hub ready");
  #endif

  SPI.begin();
  settings.begin(this);
  _emitter.begin();

  for (auto& sensor : sensors) {
    sensor.begin();
  }
}

void HID::onLoop() {
  for (auto& sensor : sensors) {
    sensor.update();
  }
}

