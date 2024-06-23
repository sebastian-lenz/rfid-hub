#include "hid/Emitter.h"
#include "globals.h"

void Emitter::begin() {
  _keyboard.begin();
  dispatcher.subscribe(SensorEvent::descriptor, std::bind(&Emitter::onSensorEvent, this, std::placeholders::_1));
}

void Emitter::onSensorEvent(const Event& event) {
  if (event.type() != SensorEvent::descriptor) {
    return;
  }

  const KeyboardMode mode = settings.getKeyboardMode();
  const auto& sensorEvent = static_cast<const SensorEvent&>(event);
  if (mode == KeyboardMode::SEND_COMMANDS) {
    return handleSendCommand(sensorEvent);
  }

  auto keyMappingResult = settings.findKeyMapping(sensorEvent.cardId, sensorEvent.sensorId);
  if (!keyMappingResult.second) {
    return;
  }

  switch (settings.getKeyboardMode()) {
    case KeyboardMode::PRESS_KEYS:
      handlePressKey(sensorEvent.eventType, keyMappingResult.first);
      break;
    case KeyboardMode::HOLD_KEYS:
      handleHoldKey(sensorEvent.eventType, keyMappingResult.first);
      break;
  }
}

void Emitter::handlePressKey(const SensorEventType type, const KeyMapping keyMapping) {
  if (type == SensorEventType::Bind) {
    _keyboard.write(keyMapping.key);
  } else {
    _keyboard.write(' ');
  }
}

void Emitter::handleHoldKey(const SensorEventType type, const KeyMapping keyMapping) {
  if (type == SensorEventType::Bind) {
    _keyboard.press(keyMapping.key);
  } else {
    _keyboard.release(keyMapping.key);
  }
}

void Emitter::handleSendCommand(const SensorEvent& sensorEvent) {
  String command =
    String(sensorEvent.sensorId) + " " +
    (sensorEvent.eventType == SensorEventType::Bind ? String(sensorEvent.cardId) : "0");

  _keyboard.println(command);
}
