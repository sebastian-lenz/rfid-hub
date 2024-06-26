#ifndef RFID_HUB_GLOBALS_H
#define RFID_HUB_GLOBALS_H

#include <U8g2lib.h>
#include "base/EventDispatcher.h"
#include "hid/Settings.h"
#include "hid/Sensor.h"
#include "ui/Button.h"

#define DEBUG false

#define NUM_SENSORS 4
#define SENSOR_0  3, 10
#define SENSOR_1  0,  6
#define SENSOR_2 28, 27
#define SENSOR_3 22, 17

#define NUM_BUTTONS 2
#define BUTTON_0 8
#define BUTTON_1 7

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

extern U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;
extern EventDispatcher dispatcher;
extern Sensor sensors[NUM_SENSORS];
extern Button buttons[NUM_BUTTONS];
extern Settings settings;

void log(const String &message);

#endif //RFID_HUB_GLOBALS_H
