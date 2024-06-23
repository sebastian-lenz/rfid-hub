#include "hid/HID.h"
#include "ui/UI.h"

HID hid;
UI ui;

void setup() {
  hid.setup();
}

void loop() {
  hid.loop();
}

void setup1() {
  ui.setup();
}

void loop1() {
  ui.loop();
}
