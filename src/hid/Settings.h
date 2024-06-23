#ifndef RFID_HUB_SETTINGS_H
#define RFID_HUB_SETTINGS_H

#include <tuple>
#include "base/Timeouts.h"

#define NUM_KEY_MAPPINGS 36
#define KEY_MAPPINGS "abcdefghijklmnopqrstuvwxyz0123456789"
#define KEY_DISPLAYS "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
#define DEFAULT_KEYBOARD_MODE KeyboardMode::HOLD_KEYS

enum class KeyboardMode {
  HOLD_KEYS,
  PRESS_KEYS,
  SEND_COMMANDS,
  NONE,
};

struct KeyMapping {
  unsigned long cardId;
  unsigned long sensorId;
  char key;
  char display;
};

class Settings {
private:
  bool          _useIndividualSensors = true;
  KeyboardMode  _keyboardMode = DEFAULT_KEYBOARD_MODE;
  KeyMapping    _keyMappings[NUM_KEY_MAPPINGS] = {};
  unsigned long _saveTimout = 0;
  Timeouts*     _timeouts = nullptr;

public:
  void begin(Timeouts* timeouts);
  void clearKeyMappings();
  void loadSettings();
  void saveSettings();
  std::pair<KeyMapping, bool> findKeyMapping(unsigned long cardId, unsigned long sensorId);
  std::pair<char, char> getAvailableKeys(unsigned long cardId, unsigned long sensorId);
  KeyboardMode getKeyboardMode();
  void reset();
  void setKeyboardMode(KeyboardMode value);
  void setKeyMapping(char key, unsigned long sensorId, unsigned long cardId);
  void useIndividualSensors(bool value);
  bool useIndividualSensors();
  void onSaveSettings();

private:
  int findKeyMappingIndex(unsigned long cardId, unsigned long sensorId, bool useIndividualSensors = true);
};

#endif //RFID_HUB_SETTINGS_H