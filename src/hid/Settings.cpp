#include <functional>
#include <string>
#include <LittleFS.h>
#include "hid/Settings.h"
#include "globals.h"

#define DATA_HEADER "RFID_HUB_SETTINGS_V1"
#define DATA_FILE "/rfid-hub.dat"

using namespace std;

struct StoredData {
  char         header[21] = DATA_HEADER;
  bool         useIndividualSensors = true;
  KeyboardMode keyboardMode = KeyboardMode::NONE;
  KeyMapping   keyMappings[NUM_KEY_MAPPINGS] = {};
};

int normalizeIndex(int value) {
  while (value >= NUM_KEY_MAPPINGS) value -= NUM_KEY_MAPPINGS;
  while (value < 0) value += NUM_KEY_MAPPINGS;
  return value;
}

void Settings::begin(Timeouts* timeouts) {
  if (LittleFS.begin()) {
    log("LittleFS ready");
  } else {
    log("Could not start LittleFS!");
  }

  _timeouts = timeouts;
  clearKeyMappings();
  loadSettings();
}

std::pair<KeyMapping, bool> Settings::findKeyMapping(unsigned long cardId, unsigned long sensorId) {
  int index = findKeyMappingIndex(cardId, sensorId, _useIndividualSensors);
  return index != -1
    ? std::make_pair(_keyMappings[index], true)
    : std::make_pair(KeyMapping{}, false);
}

void Settings::clearKeyMappings() {
  string characters = KEY_MAPPINGS;
  string displays = KEY_DISPLAYS;

  for (int8_t index = 0; index < NUM_KEY_MAPPINGS; index++) {
    this->_keyMappings[index] = {
      .cardId = 0,
      .sensorId = 0,
      .key = characters.at(index),
      .display = displays.at(index),
    };
  }
}

std::pair<char, char> Settings::getAvailableKeys(unsigned long cardId, unsigned long sensorId) {
  int baseIndex = findKeyMappingIndex(cardId, sensorId, _useIndividualSensors);

  char availableLowerChar = ' ';
  char availableUpperChar = ' ';

  for (int offset = 0; offset < NUM_KEY_MAPPINGS; offset++) {
    if (availableLowerChar == ' ') {
      int index = normalizeIndex(baseIndex == -1 ? -offset - 1 : baseIndex - offset);
      if (_keyMappings[index].cardId == 0) {
        availableLowerChar = _keyMappings[index].key;
      }
    }

    if (availableUpperChar == ' ') {
      int index = normalizeIndex(baseIndex == -1 ? offset : baseIndex + offset);
      if (_keyMappings[index].cardId == 0) {
        availableUpperChar = _keyMappings[index].key;
      }
    }
  }

  return std::make_pair(availableLowerChar, availableUpperChar);
};

int Settings::findKeyMappingIndex(unsigned long cardId, unsigned long sensorId, bool useIndividualSensors) {
  for (int index = 0; index < NUM_KEY_MAPPINGS; index++) {
    if (
      (_keyMappings[index].cardId == cardId) &&
      (_keyMappings[index].sensorId == sensorId || !useIndividualSensors)
    ) {
      return index;
    }
  }

  return -1;
};

KeyboardMode Settings::getKeyboardMode() {
  return _keyboardMode;
};

void Settings::setKeyboardMode(KeyboardMode value) {
  if (this->_keyboardMode != value) {
    this->_keyboardMode = value;
    saveSettings();
  }
}

bool Settings::useIndividualSensors() {
  return _useIndividualSensors;
}

void Settings::useIndividualSensors(bool value) {
  if (this->_useIndividualSensors != value) {
    this->_useIndividualSensors = value;
    saveSettings();
  }
}

void Settings::setKeyMapping(char key, unsigned long sensorId, unsigned long cardId) {
  for (auto& keyMapping : this->_keyMappings) {
    if (
      (keyMapping.cardId == cardId) &&
      (keyMapping.sensorId == sensorId || !_useIndividualSensors)
    ) {
      keyMapping.cardId = 0;
      keyMapping.sensorId = 0;
    }

    if (keyMapping.key != key) {
      continue;
    }

    keyMapping.cardId = cardId;
    keyMapping.sensorId = sensorId;
  }

  saveSettings();
}

void Settings::saveSettings() {
  if (_timeouts == nullptr) {
    onSaveSettings();
  } else {
    if (_saveTimout != 0) {
      _timeouts->clearTimeout(_saveTimout);
    }

    _saveTimout = _timeouts->setTimeout(std::bind(&Settings::onSaveSettings, this), 5000);
  }
}

void Settings::onSaveSettings() {
  _saveTimout = 0;

  StoredData data;
  data.useIndividualSensors = _useIndividualSensors;
  data.keyboardMode = _keyboardMode;
  for (int index = 0; index < NUM_KEY_MAPPINGS; index++) {
    data.keyMappings[index] = _keyMappings[index];
  }

  char* dataAsBytes = reinterpret_cast<char*>(&data);

  File file = LittleFS.open(DATA_FILE, "w");
  file.write(dataAsBytes, sizeof(StoredData));
  log("Setting written");
  file.close();
}

void Settings::loadSettings() {
  if (!LittleFS.exists(DATA_FILE)) {
    log("No setting file found");
    return;
  }

  File file = LittleFS.open(DATA_FILE, "r");
  if (!file) {
    log("Could not open setting file");
    return;
  }

  StoredData data;
  char* dataAsBytes = reinterpret_cast<char*>(&data);
  file.readBytes(dataAsBytes, sizeof(StoredData));
  file.close();

  if (strcmp(data.header, DATA_HEADER) != 0) {
    log("Setting file has wrong header: " + String(data.header));
    return;
  }

  this->_useIndividualSensors = data.useIndividualSensors;
  this->_keyboardMode = data.keyboardMode;
  for (int index = 0; index < NUM_KEY_MAPPINGS; index++) {
    this->_keyMappings[index] = data.keyMappings[index];
  }
}

void Settings::reset() {
  _useIndividualSensors = true;
  _keyboardMode = DEFAULT_KEYBOARD_MODE;
  clearKeyMappings();
  saveSettings();
}
