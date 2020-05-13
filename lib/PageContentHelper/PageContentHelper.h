#ifndef PageContentHelper_h
#define PageContentHelper_h

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DataProfiles.h>
#include <ShotCounterLang.h>

class PageContentHelper {
   private:
    Adafruit_SSD1306 *display;
    ShotCounterLang *shotCounterLang;

   public:
    // Constructor
    PageContentHelper(Adafruit_SSD1306 *display, ShotCounterLang *shotCounterLang);
    // Show info page with version
    void versionPage(char const *version);
    // Show shot counter page
    void counterPage(ShotCounterData shotCounter);
    // Show waiting for shots page
    void waitingForShotsPage();
    // Show enter profile page
    void enterProfilePage();
    // Show setup g force page
    void setupGforcePage(float countGforce, float gforceLast);
    // Show setup shot delay page
    void setupShotDelayPage(int shotDelay);
    // Show reset profile page
    void resetProfilePage();
    // Show gyro sensor calibration page
    void calibrationPage(float gforceMaxMeasured);
    // Show gyro sensor error page
    void sensorErrorPage();
    // Show gyro reset eror page
    void sensorResetErrorPage();
    // Show gyro sensitivity setup error page
    void sensorSensSetupErrorPage();
    // Show acceleration sensitivity setup error page
    void sensorAccSetupErrorPage();
    // Show enter device setup page
    void enterDeviceSetupPage();
    // Show language selection page
    void languageSelectionPage(char activeLanguage);
    // Show reset device page
    void resetDevicePage();
    // Show power off page
    void powerOffDevicePage();
};

#endif