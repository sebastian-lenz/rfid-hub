#ifndef RFID_HUB_HID_H
#define RFID_HUB_HID_H

#include "base/Core.h"
#include "hid/Emitter.h"

class HID : public Core
{
private:
  Emitter _emitter;

protected:
  void onLoop() override;
  void onSetup() override;
};

#endif //RFID_HUB_HID_H
