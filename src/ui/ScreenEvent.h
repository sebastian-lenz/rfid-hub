#ifndef RFID_HUB_SCREEN_EVENT_H
#define RFID_HUB_SCREEN_EVENT_H

#include "base/Event.h"

enum class ScreenType
{
  Empty,
  Home,
  Settings,
};

class ScreenEvent : public Event
{
public:
  const ScreenType screenType;

  static constexpr EventType descriptor = "ScreenEvent";

  explicit ScreenEvent(const ScreenType screenType) :
    screenType(screenType)
  { }

  EventType type() const override {
    return descriptor;
  }
};

#endif //RFID_HUB_SCREEN_EVENT_H
