#include "globals.h"

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
Settings settings;
EventDispatcher dispatcher;

Sensor sensors[NUM_SENSORS] = {
  Sensor(0, SENSOR_0),
  Sensor(1, SENSOR_1),
  Sensor(2, SENSOR_2),
  Sensor(3, SENSOR_3),
};

Button buttons[NUM_BUTTONS] = {
  Button(BUTTON_0),
  Button(BUTTON_1),
};

void log(const String &message) {
  #if DEBUG
    Serial.println(message);
  #endif
}