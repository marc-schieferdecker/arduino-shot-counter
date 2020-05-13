#ifndef DataProfiles_h
#define DataProfiles_h

#include <ShotCounterData.h>
#include <EEProm.h>
#include <EEPromCRC.h>

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
    ShotCounterData setup(bool forceDefault);
    // Get active shot counter
    ShotCounterData getShotCounter();
    // Store active shot counter
    void putShotCounter(ShotCounterData shotCounter);
    // Reset active shot counter to default
    ShotCounterData resetShotCounter();
    // Switch to next shot counter
    void nextShotCounter();
};

#endif