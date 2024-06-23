#ifndef RFID_HUB_SENSOR_H
#define RFID_HUB_SENSOR_H

#include <MFRC522.h>
#include "base/EventDispatcher.h"
#include "hid/SensorEvent.h"

const int checkInterval = 20;
const int numSensors = 4;

class Sensor 
{
private:
  unsigned long _cardId = 0;
  byte          _chipSelectPin;
  bool          _hasReadError = false;
  bool          _isConnected = false;
  byte          _resetPowerDownPin;
  MFRC522       _rfid;
  unsigned long _sensorId;
  unsigned long _updateIndex = 0;

public:
  Sensor(unsigned long sensorId, byte chipSelectPin, byte resetPowerDownPin);
  void begin();
  void update();

  [[nodiscard]]
  unsigned long getCardId() const { return _cardId; }

  [[nodiscard]]
  unsigned long getSensorId() const { return _sensorId; }

  [[nodiscard]]
  bool hasReadError() const { return _hasReadError; }

  [[nodiscard]]
  bool isConnected() const { return _isConnected; }

private:
  void bindCard(unsigned long cardId);
  bool checkConnection();
  unsigned long findNewCardId();
  bool isCardPresent();
  void releaseCard();
  void reportConnection();
};

#endif //RFID_HUB_SENSOR_H