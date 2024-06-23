#include <cctype>
#include "hid/SensorEvent.h"
#include "ui/HomeScreen.h"
#include "ui/ScreenEvent.h"
#include "globals.h"

HomeScreen::HomeScreen() {
  _listenerId = dispatcher.subscribe(SensorEvent::descriptor,
    std::bind(&HomeScreen::onSensorEvent, this, std::placeholders::_1)
  );
}

HomeScreen::~HomeScreen() {
  dispatcher.unsubscribe(_listenerId);
}

void HomeScreen::onButton(ButtonSignal signal, int index) {
  if (signal != ButtonSignal::PRESS && signal != ButtonSignal::REPEAT) {
    return;
  }

  Sensor* sensor = getCurrentSensor();
  std::pair<char, char> keys = getAvailableKeys();

  if (sensor != nullptr && keys.first != ' ' && keys.second != ' ') {
    settings.setKeyMapping(index == 0 ? keys.first : keys.second, sensor->getSensorId(), sensor->getCardId());
  } else {
    auto type = index == 0 ? ScreenType::Settings : ScreenType::Empty;
    dispatcher.trigger(ScreenEvent(type));
  }
}

void HomeScreen::onSensorEvent(const Event& event) {
  if (event.type() != SensorEvent::descriptor) return;
  const auto& sensorEvent = static_cast<const SensorEvent&>(event);

  if (sensorEvent.eventType == SensorEventType::Release) {
    _currentSensorId = -1;
  } else {
    _currentSensorId = (signed)sensorEvent.sensorId;
  }
}

void HomeScreen::render() {
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);

  for (int index = 0; index < NUM_SENSORS; index++) {
    renderSensor(&sensors[index], index * 16);
  }

  std::pair<char, char> keys = getAvailableKeys();
  if (keys.first != ' ' && keys.second != ' ') {
    renderActions(Label(toupper(keys.first)), Label(toupper(keys.second)));
  } else {
    renderActions(Label(sprites::settings), Label(sprites::power));
  }

  u8g2.sendBuffer();
}

std::pair<char, char> HomeScreen::getAvailableKeys() {
  Sensor* sensor = getCurrentSensor();
  if (sensor != nullptr && sensor->getCardId() != 0) {
    return settings.getAvailableKeys(sensor->getCardId(), sensor->getSensorId());
  }

  return std::make_pair(' ', ' ');
}

Sensor* HomeScreen::getCurrentSensor() const {
  return _currentSensorId == -1 ? nullptr : &sensors[_currentSensorId];
}

void HomeScreen::renderSensor(Sensor *sensor, int y) {
  unsigned long cardId = sensor->getCardId();
  unsigned long sensorId = sensor->getSensorId();
  int textY = y + 11;

  u8g2.setDrawColor(1);
  if (_currentSensorId == sensorId) {
    u8g2.drawRBox(0, y, 108, 15, 2);
    u8g2.setDrawColor(0);
  }

  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.drawUTF8(3, textY, String(sensorId + 1).c_str());

  if (cardId != 0) {
    auto mapping = settings.findKeyMapping(cardId, sensorId);
    if (mapping.second) {
      int width = u8g2.getStrWidth(&mapping.first.display);
      u8g2.drawStr(105 - width, textY, &mapping.first.display);
    }
  }

  if (cardId != 0) {
    u8g2.drawUTF8(30, textY, String(cardId).c_str());
  } else if (sensor->isConnected()) {
    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.drawUTF8(30, textY, "(kein Chip)");
  }

  renderSensorIcon(sensor, 13, y + 2);
}

void HomeScreen::renderSensorIcon(Sensor *sensor, int x, int y) const {
  if (!sensor->isConnected()) {
    u8g2.drawXBM(x, y, 11, 11, sprites::emptySensor);
  } else if (sensor->getCardId() != 0) {
    u8g2.drawXBM(x, y, 11, 11, sprites::usedSensor);
  } else {
    unsigned long sensorPhase = _frame + sensor->getSensorId() * 4;
    while (sensorPhase >= 16) {
      sensorPhase -= 16;
    }

    u8g2.drawXBM(x, y, 11, 11, sprites::activeSensor[sensorPhase]);
  }
}

void HomeScreen::update() {
  _frame += 1;
  if (_frame >= 16) {
    _frame = 0;
  }
}