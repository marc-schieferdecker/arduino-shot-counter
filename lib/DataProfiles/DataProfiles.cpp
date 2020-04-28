#include <Arduino.h>
#include <DataProfiles.h>
#include <EEPROM.h>
#include <EEPromCRC.h>

DataProfiles::DataProfiles(unsigned short _profilesMax, unsigned int _eeAddress, EEPromCRC *_eepromCrc) {
    profilesMax = _profilesMax;
    eeAddress = _eeAddress;
    eepromCrc = _eepromCrc;
}

ShotCounter DataProfiles::init() {
    // Check crc
    if (!eepromCrc->crcIsValid()) {
        for (int i = 0; i < profilesMax; i++) {
            // Write default data to EEPROM
            ShotCounter defaultShotCounter = {
                0,
                0,
                3,
                250,
                ""};

            // Set profile name
            sprintf(defaultShotCounter.profileName, "Preset %d", i + 1);

            // Store data and update crc
            EEPROM.put(eeAddress + (sizeof(ShotCounter) * i), defaultShotCounter);
        }
        eepromCrc->crcPut();
    }
    return getShotCounter();
}

ShotCounter DataProfiles::getShotCounter() {
    ShotCounter shotCounter;
    EEPROM.get(eeAddress, shotCounter);
    return shotCounter;
}

void DataProfiles::putShotCounter(ShotCounter shotCounter) {
    EEPROM.put(eeAddress, shotCounter);
    eepromCrc->crcPut();
}

ShotCounter DataProfiles::resetShotCounter() {
    ShotCounter defaultShotCounter = {
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
    eeAddress = (sizeof(ShotCounter) * selectedProfile);
}