#ifndef RFID_HUB_EVENT_H
#define RFID_HUB_EVENT_H

#include <functional>

using EventType = const char*;

class Event {
public:
  [[nodiscard]]
  virtual EventType type() const = 0;
};

#endif //RFID_HUB_EVENT_H
