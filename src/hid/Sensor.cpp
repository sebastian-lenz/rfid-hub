#include "hid/Sensor.h"
#include "globals.h"

Sensor::Sensor(unsigned long sensorId, byte chipSelectPin, byte resetPowerDownPin) {
  _chipSelectPin = chipSelectPin;
  _resetPowerDownPin = resetPowerDownPin;
  _sensorId = sensorId;
  _updateIndex = sensorId * checkInterval;
}

void Sensor::begin() {
  checkConnection();
  reportConnection();
}

void Sensor::bindCard(unsigned long cardId) {
  releaseCard();
  log("Sensor #" + String(_sensorId) + ": Bind card with id " + String(cardId));
  _cardId = cardId;
  dispatcher.trigger(SensorEvent(SensorEventType::Bind, _sensorId, cardId));
}

bool Sensor::checkConnection() {
  bool wasConnected = _isConnected;
  if (!_isConnected) {
    _rfid.PCD_Init(_chipSelectPin, _resetPowerDownPin);
    _isConnected = _rfid.PCD_PerformSelfTest();
  } else {
    _isConnected = _rfid.PCD_GetAntennaGain() > 0;
  }

  return wasConnected != _isConnected;
}

unsigned long Sensor::findNewCardId() {
  _hasReadError = false;

  if (!_rfid.PICC_IsNewCardPresent()) {
    return 0;
  } else if (!_rfid.PICC_ReadCardSerial()) {
    _hasReadError = true;
    log("Sensor #" + String(_sensorId) + ": Error, could not read card cardId");
    return 0;
  }

  unsigned long cardId = 0;
  for (byte i = 0; i < _rfid.uid.size; i++) {
    cardId = ((cardId + _rfid.uid.uidByte[i]) * 10);
  }

  _rfid.PICC_HaltA();
  return cardId;
}

bool Sensor::isCardPresent() {
  byte bufferATQA[2];
  byte bufferSize = sizeof(bufferATQA);

  _rfid.PCD_WriteRegister(MFRC522::TxModeReg, 0x00);
  _rfid.PCD_WriteRegister(MFRC522::RxModeReg, 0x00);
  _rfid.PCD_WriteRegister(MFRC522::ModWidthReg, 0x26);

  if (_rfid.PICC_WakeupA(bufferATQA, &bufferSize) == MFRC522::STATUS_OK) {
    _rfid.PICC_HaltA();
    return true;
  } else {
    return false;
  }
}

void Sensor::releaseCard() {
  if (_cardId == 0) {
    return;
  }

  log("Sensor #" + String(_sensorId) + ": Released card");
  dispatcher.trigger(SensorEvent(SensorEventType::Release, _sensorId, _cardId));
  _cardId = 0;
}

void Sensor::reportConnection() {
  #if DEBUG
    if (_isConnected) {
      log("Sensor " + String(_sensorId) + " connected");
      _rfid.PCD_DumpVersionToSerial();
    } else {
      log("Sensor " + String(_sensorId) + " not connected");
    }
  #endif
}

void Sensor::update() {
  _updateIndex += 1;
  if (_updateIndex > checkInterval * numSensors) {
    _updateIndex = 0;

    if (checkConnection()) {
      reportConnection();
    }
  }

  if (_isConnected) {
    unsigned long cardId = findNewCardId();
    if (cardId != 0) {
      bindCard(cardId);
    } else if (_cardId != 0 && !isCardPresent()) {
      releaseCard();
    }
  }
}
