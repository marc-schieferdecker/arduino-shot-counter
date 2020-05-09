#include <Arduino.h>
#include <ShotCounterData.h>
#include <DataProfiles.h>
#include <EEPROM.h>
#include <EEPromCRC.h>

DataProfiles::DataProfiles(byte _profilesMax, unsigned int _eeAddress, EEPromCRC *_eepromCrc) {
    profilesMax = _profilesMax;
    eeAddress = _eeAddress;
    eepromCrc = _eepromCrc;
}

ShotCounterData DataProfiles::setup() {
    // Check crc
    if (!eepromCrc->crcIsValid()) {
        for (int i = 0; i < profilesMax; i++) {
            // Write default data to EEPROM
            ShotCounterData defaultShotCounter = {
                0,
                0,
                3,
                250,
                ""};

            // Set profile name
            sprintf(defaultShotCounter.profileName, "Preset %d", i + 1);

            // Store data and update crc
            EEPROM.put(eeAddress + (sizeof(ShotCounterData) * i), defaultShotCounter);
        }
        eepromCrc->crcPut();
    }
    return getShotCounter();
}

ShotCounterData DataProfiles::getShotCounter() {
    ShotCounterData shotCounter;
    EEPROM.get(eeAddress, shotCounter);
    return shotCounter;
}

void DataProfiles::putShotCounter(ShotCounterData shotCounter) {
    EEPROM.put(eeAddress, shotCounter);
    eepromCrc->crcPut();
}

ShotCounterData DataProfiles::resetShotCounter() {
    ShotCounterData defaultShotCounter = {
        0,
        0,
        3,
        250,
        ""};
    // Set profile name
    sprintf(defaultShotCounter.profileName, "Preset %d", selectedProfile + 1);
    putShotCounter(defaultShotCounter);
    return defaultShotCounter;
}

void DataProfiles::nextShotCounter() {
    selectedProfile++;
    if (selectedProfile > (profilesMax - 1)) {
        selectedProfile = 0;
    }
    eeAddress = (sizeof(ShotCounterData) * selectedProfile);
}