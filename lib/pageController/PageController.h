#ifndef PageController_h
#define PageController_h

#include <DataProfiles.h>
#include <DisplayHelper.h>
#include <GyroMeasure.h>
#include <PageContentHelper.h>
#include <PageHelper.h>
#include <SingleButton.h>

class PageController {
   private:
    ShotCounter shotCounter;
    float gMaxCalibration = 0;
    unsigned long calibrationDisplayUpdateDelay = 0;
    DataProfiles *dataProfiles;
    DisplayHelper *displayHelper;
    GyroMeasure *gyroMeasure;
    PageContentHelper *pageContentHelper;
    PageHelper *pageHelper;
    SingleButton *singleButton;
    bool printDebug;

   public:
    // Constructor
    PageController(DataProfiles *dataProfiles,
                   DisplayHelper *displayHelper,
                   GyroMeasure *gyroMeasure,
                   PageContentHelper *pageContentHelper,
                   PageHelper *pageHelper,
                   SingleButton *singleButton,
                   bool printDebug);
    // Init shot counter
    void setup();
    // Show shot counter page
    void counterPage();
    // Show waiting for shots page
    void waitingForShotsPage(const uint8_t *bitmap);
    // Show enter profile page
    void enterProfilePage();
    // Show setup g force page
    void setupGforcePage();
    // Show setup shot delay page
    void setupShotDelayPage();
    // Show reset profile page
    void resetProfilePage();
    // Show gyro sensor calibration page
    void calibrationPage();
};

#endif