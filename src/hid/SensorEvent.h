#ifndef RFID_HUB_SENSOR_EVENT_H
#define RFID_HUB_SENSOR_EVENT_H

#include "base/Event.h"

enum class SensorEventType
{
  Bind,
  Release,
};

class SensorEvent : public Event
{
public:
  const unsigned long   cardId;
  const SensorEventType eventType;
  const unsigned long   sensorId;

  static constexpr EventType descriptor = "SensorEvent";

  SensorEvent(
    const SensorEventType eventType,
    const unsigned long sensorId,
    const unsigned long cardId
  ) :
    eventType(eventType),
    sensorId(sensorId),
    cardId(cardId)
  { }

  EventType type() const override {
    return descriptor;
  }
};

#endif //RFID_HUB_SENSOR_EVENT_H
