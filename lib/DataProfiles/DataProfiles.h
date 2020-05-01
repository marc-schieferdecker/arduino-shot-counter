#ifndef DataProfiles_h
#define DataProfiles_h

#include <EEProm.h>
#include <EEPromCRC.h>

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
    byte profilesMax = 6;
    byte selectedProfile = 0;
    EEPromCRC *eepromCrc;

   public:
    // Constructor
    DataProfiles(byte profilesMax, unsigned int eeAddress, EEPromCRC *eepromCrc);
    // Init functions
    ShotCounter setup();
    // Get active shot counter
    ShotCounter getShotCounter();
    // Store active shot counter
    void putShotCounter(ShotCounter shotCounter);
    // Reset active shot counter to default
    ShotCounter resetShotCounter();
    // Switch to next shot counter
    void nextShotCounter();
};

#endif