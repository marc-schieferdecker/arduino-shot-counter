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
  display -> println("Shot Counter");
  display -> setCursor(2,12);
  display -> println(version);
  display -> setCursor(2,24);
  display -> println("letsshootshow.de");
}

void PageContentHelper::counterPage(ShotCounter shotCounter) {
    display -> println(shotCounter.profileName);
    display -> println("");
    display -> print("Total      ");
    display -> println(shotCounter.shotsTotal);
    display -> print("Series     ");
    display -> println(shotCounter.shotsSeries);
}

void PageContentHelper::waitingForShotsPage() {
    display -> println("  Waiting for shots");
}

void PageContentHelper::enterProfilePage() {
    display -> println("Setup profile");
    display -> println("");
    display -> println("To enter setup");
    display -> println("hold button");
}

void PageContentHelper::setupGforcePage(float countGforce, float gforcelast) {
    display -> println("Minimum G force");
    display -> println("to count a shot:");
    display -> print(countGforce);
    display -> println(" g");
    display -> print("Last measured: ");
    display -> print(gforcelast);
    display -> print(" g");
}

void PageContentHelper::setupShotDelayPage(int shotDelay) {
    display -> println("Minimum time");
    display -> println("between shots:");
    display -> println("");
    display -> print(shotDelay);
    display -> println(" ms");
}

void PageContentHelper::resetProfilePage() {
    display -> println("Reset profile?");
    display -> println("");
    display -> println("To reset this profile");
    display -> println("hold button");
}
