#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <DataProfiles.h>
#include <PageContentHelper.h>
#include <ShotCounterLang.h>

PageContentHelper::PageContentHelper(Adafruit_SSD1306 *_display, ShotCounterLang *_shotCounterLang) {
    display = _display;
    shotCounterLang = _shotCounterLang;
}

void PageContentHelper::versionPage(char const *version) {
    display->setCursor(2, 0);
    display->println(F("Shot Counter"));
    display->setCursor(2, 12);
    display->println(version);
    display->setCursor(2, 24);
    display->println(F("letsshootshow.de"));
}

void PageContentHelper::counterPage(ShotCounterData shotCounter) {
    display->print(shotCounterLang->getKey(SCLangProfilePrepend));
    display->println(shotCounter.profileName);
    display->println();
    display->print(shotCounterLang->getKey(SCLangTotal));
    display->println(shotCounter.shotsTotal);
    display->print(shotCounterLang->getKey(SCLangSeries));
    display->println(shotCounter.shotsSeries);
}

void PageContentHelper::waitingForShotsPage() {
    display->println(shotCounterLang->getKey(SCLangWaitingForShots));
}

void PageContentHelper::enterProfilePage() {
    display->print(shotCounterLang->getKey(SCLangSetup));
}

void PageContentHelper::setupGforcePage(float countGforce, float gforceLast) {
    display->print(shotCounterLang->getKey(SCLangMinGToCountShot));
    display->print(countGforce);
    display->println(F(" g"));
    display->print(shotCounterLang->getKey(SCLangLastMeasured));
    display->print(gforceLast);
    display->print(F(" g"));
}

void PageContentHelper::setupShotDelayPage(int shotDelay) {
    display->print(shotCounterLang->getKey(SCLangMinimumTimeBetweenShots));
    display->print(shotDelay);
    display->println(F(" ms"));
}

void PageContentHelper::resetProfilePage() {
    display->print(shotCounterLang->getKey(SCLangResetProfile));
}

void PageContentHelper::calibrationPage(float gforceMaxMeasured) {
    display->print(shotCounterLang->getKey(SCLangCalibrationHelper));
    display->print(shotCounterLang->getKey(SCLangMaximumValueMeasured));
    display->print(gforceMaxMeasured);
    display->println(F(" g"));
}

void PageContentHelper::sensorErrorPage() {
    display->clearDisplay();
    display->setCursor(2, 0);
    display->println(F("Problem with gyro"));
    display->println();
    display->println(F("Sleep still active"));
    display->display();
}

void PageContentHelper::sensorResetErrorPage() {
    display->clearDisplay();
    display->setCursor(2, 0);
    display->println(F("Problem with gyro"));
    display->println();
    display->print(F("Could not reset gyro"));
    display->display();
}

void PageContentHelper::sensorSensSetupErrorPage() {
    display->clearDisplay();
    display->setCursor(2, 0);
    display->println(F("Problem with gyro"));
    display->println();
    display->print(F("Could not set gyro sensitivity"));
    display->display();
}

void PageContentHelper::sensorAccSetupErrorPage() {
    display->clearDisplay();
    display->setCursor(2, 0);
    display->println(F("Problem with gyro"));
    display->println();
    display->print(F("Could not set acceleration sensitivity"));
    display->display();
}

void PageContentHelper::powerOffDevicePage() {
    display->print(shotCounterLang->getKey(SCLangPowerOff));
}
