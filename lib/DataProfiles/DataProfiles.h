#ifndef DataProfiles_h
#define DataProfiles_h

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
        int profilesMax = 6;
        int selectedProfile = 0;
        EEPromCRC *eepromCrc;

    public:
        // Constructor
        DataProfiles(unsigned short profilesMax, unsigned int eeAddress, EEPromCRC *eepromCrc);
        // Init functions
        void init();
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