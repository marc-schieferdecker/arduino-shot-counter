#include <Arduino.h>
#include <EEPROM.h>
#include "EEPromCRC.h"

struct ShotCounter {
  unsigned long shotsTotal;
  unsigned int shotsSeries;
  float countGforce;
  int shotDelay;
  char profileName[15];
};

class DataProfiles {
    private:
        unsigned int eeAddress = 0;
        int profilesMax = 6;
        int selectedProfile = 0;
        EEPROM_crc *eepromCrc;

    public:
        // Constructor
        DataProfiles(unsigned short profilesMax, unsigned int eeAddress, EEPROM_crc *eepromCrc) {
            this -> profilesMax = profilesMax;
            this -> eeAddress = eeAddress;
            this -> eepromCrc = eepromCrc;
        }

        void init() {
            // Check crc
            if (!this -> eepromCrc -> crcIsValid()) {
                for(int i = 0; i < this -> profilesMax; i++) {
                    // Write default data to EEPROM
                    ShotCounter defaultShotCounter = {
                        0,
                        0,
                        3,
                        250,
                        ""
                    };

                    // Set profile name
                    sprintf(defaultShotCounter.profileName, "Preset %d", i + 1);

                    // Store data and update crc
                    EEPROM.put(this -> eeAddress + (sizeof(ShotCounter) * i), defaultShotCounter);
                }
                this -> eepromCrc -> crcPut();
            }
        }

        ShotCounter getShotCounter() {
            ShotCounter shotCounter;
            EEPROM.get(this -> eeAddress, shotCounter);
            return shotCounter;
        }

        void putShotCounter(ShotCounter shotCounter) {
            EEPROM.put(this -> eeAddress, shotCounter);
            this -> eepromCrc -> crcPut();
        }

        ShotCounter resetShotCounter() {
            ShotCounter defaultShotCounter = {
                0,
                0,
                3,
                250,
                ""
            };
            // Set profile name
            sprintf(defaultShotCounter.profileName, "Preset %d", this -> selectedProfile + 1);
            this -> putShotCounter(defaultShotCounter);
            return defaultShotCounter;
        }

        void nextShotCounter() {
            this -> selectedProfile++;
            if (this -> selectedProfile > (this -> profilesMax - 1)) {
                this -> selectedProfile = 0;
            }
            this -> eeAddress = (sizeof(ShotCounter) * this -> selectedProfile);
        }
};
