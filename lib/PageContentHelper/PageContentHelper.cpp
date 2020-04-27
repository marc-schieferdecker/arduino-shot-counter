#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DataProfiles.h>
#include <PageContentHelper.h>

PageContentHelper::PageContentHelper(Adafruit_SSD1306 *_display) {
    display = _display;
}

void PageContentHelper::versionPage(char const *version) {
    display -> setCursor(2,0);
    display -> println(F("Shot Counter"));
    display -> setCursor(2,12);
    display -> println(version);
    display -> setCursor(2,24);
    display -> println(F("letsshootshow.de"));
}

void PageContentHelper::counterPage(ShotCounter shotCounter) {
    display -> println(shotCounter.profileName);
    display -> println(F(""));
    display -> print(F("Total      "));
    display -> println(shotCounter.shotsTotal);
    display -> print(F("Series     "));
    display -> println(shotCounter.shotsSeries);
}

void PageContentHelper::waitingForShotsPage() {
    display -> println("  Waiting for shots");
}

void PageContentHelper::enterProfilePage() {
    display -> println(F("Setup profile"));
    display -> println(F(""));
    display -> println(F("To enter setup"));
    display -> println(F("hold button"));
}

void PageContentHelper::setupGforcePage(float countGforce, float gforcelast) {
    display -> println(F("Minimum G force"));
    display -> println(F("to count a shot:"));
    display -> print(countGforce);
    display -> println(F(" g"));
    display -> print(F("Last measured: "));
    display -> print(gforcelast);
    display -> print(F(" g"));
}

void PageContentHelper::setupShotDelayPage(int shotDelay) {
    display -> println(F("Minimum time"));
    display -> println(F("between shots:"));
    display -> println(F(""));
    display -> print(shotDelay);
    display -> println(F(" ms"));
}

void PageContentHelper::resetProfilePage() {
    display -> println(F("Reset profile?"));
    display -> println(F(""));
    display -> println(F("To reset this profile"));
    display -> println(F("hold button"));
}

void PageContentHelper::sensorErrorPage() {
    display -> clearDisplay();
    display -> setTextColor(WHITE);
    display -> setTextSize(1);
    display -> setCursor(2,0);
    display -> println(F("Problem with gyro"));
    display -> println(F(""));
    display -> println(F("Sleep still active"));
    display -> display();
}