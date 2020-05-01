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

    void nextMainPage() {
        pageHelper->nextPage();
        displayHelper->setDisplayChanged(true);
        singleButton->shortPressTriggerDone();
    }

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
    // Main loop
    void loop();
    // Show shot counter page
    void counterPage();
    // Show waiting for shots page
    void waitingForShotsPage(const uint8_t *bitmap);
    // Show enter profile page
    void enterProfilePage(int subMenuPageIndex);
    // Show gyro sensor calibration page
    void calibrationPage(int nextPageIndex);
    // Show setup g force page
    void setupGforcePage(int nextPageIndex);
    // Show setup shot delay page
    void setupShotDelayPage(int nextPageIndex);
    // Show reset profile page
    void resetProfilePage(int nextPageIndex);
};

#endif