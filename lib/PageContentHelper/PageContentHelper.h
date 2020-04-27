#ifndef PageContentHelper_h
#define PageContentHelper_h

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DataProfiles.h>

class PageContentHelper {
    private:
        Adafruit_SSD1306 *display;

    public:
        // Constructor
        PageContentHelper(Adafruit_SSD1306 *display);
        // Show info page with version
        void versionPage(char const *version);
        // Show shot counter page
        void counterPage(ShotCounter shotCounter);
        // Show waiting for shots page
        void waitingForShotsPage();
        // Show enter profile page
        void enterProfilePage();
        // Show setup g force page
        void setupGforcePage(float countGforce, float gforcelast);
        // Show setup shot delay page
        void setupShotDelayPage(int shotDelay);
        // Show reset profile page
        void resetProfilePage();
        // Show gyro sensor error page
        void sensorErrorPage();
};

#endif