#include <Arduino.h>
#include <EEPROM.h>
#include <EEPromCRC.h>

EEPromCRC::EEPromCRC(uint16_t _crcAddress, int _dataStorageStart, int _dataStorageLength) {
    // Set EEPROM address for crc storage
    crcAddress = _crcAddress;
    dataStorageStart = _dataStorageStart;
    dataStorageEnd = _dataStorageStart + _dataStorageLength;
    // Init
    crcCheck();
}

void EEPromCRC::crcPut() {
    crcActive = calculateCrc();
    EEPROM.put(crcAddress, crcActive);
}

void EEPromCRC::crcCheck() {
    crcGet();
    crcValid = crcActive == calculateCrc();
}

bool EEPromCRC::crcIsValid() {
    return crcValid;
}
