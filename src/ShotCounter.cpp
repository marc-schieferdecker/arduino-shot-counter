/**
 * Includes
 */
#include <Arduino.h>
#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// Project includes
#include "Grafics.h"
#include <SingleButton.h>
#include <DataProfiles.h>
#include <GyroMeasure.h>
#include <DisplayHelper.h>

/**
 * Definitions
 */
#define OLED_RESET 4
#define OLED_ADDR 0x3C
#define EEPROM_CRC_ADDR 1019
#define PROFILES_EEADDR_START 0
#define PROFILES_MAX 6
#define PRINT_DEBUG false

/**
 * Global variables
 */
// Page pointer and change status
short page_index = 0;
bool page_changed = false;
// Active shot counter
ShotCounter shotCounter;

/**
 * Instanciate classes
 */
// Display class
Adafruit_SSD1306 display(OLED_RESET);
// Display helper
DisplayHelper displayHelper(&display, SSD1306_SWITCHCAPVCC, OLED_ADDR);
// Gyro measurement
GyroMeasure gyroMeasure(&display);
// EEPROM crc
EEPromCRC eepromCrc(EEPROM_CRC_ADDR, PROFILES_EEADDR_START, sizeof(ShotCounter) * PROFILES_MAX);
// Data profiles
DataProfiles dataProfiles(PROFILES_MAX, PROFILES_EEADDR_START, &eepromCrc);
// Button handler
SingleButton singleButton(1500);

// Setup programm
void setup() {
  // Setup display (SBC-OLED01)
  displayHelper.setup();
  displayHelper.bitmapFullscreen(Logo);
  delay(1000);

  // Setup serial console in debug mode
  if (PRINT_DEBUG) {
    Serial.begin(9600);
    delay(50);
    Serial.println("Setup serial with 9600 baud");
  }

  // Init gyro senson
  gyroMeasure.init();

  // Init data & load shot counter from EEPROM
  shotCounter = dataProfiles.init();

  // Print state of EEPROM
  if (PRINT_DEBUG) {
    Serial.print("CRC EEPROM STATE ");
    Serial.print(eepromCrc.crcIsValid());
    Serial.println();
  }

  // Display test
  displayHelper.clear();
  display.setCursor(2,0);
  display.println("Shot Counter");
  display.setCursor(2,12);
  display.println("Version 0.01a");
  display.setCursor(2,24);
  display.println("letsshootshow.de");
  displayHelper.render();
  delay(1000);
}

// Main loop
void loop() {
  // Button handler
  singleButton.loop();
  // Check for short press
  if (singleButton.shortPressTrigger() && page_index <= 2) {
      page_index = (page_index + 1) > 2 ? 0 : (page_index + 1);
      page_changed = true;
      displayHelper.setDisplayChanged(true);
      singleButton.shortPressTriggerDone();
  }

  // Main page (displays data of active preset)
  if (page_index == 0) {
    // Show profile values
    if (displayHelper.getDisplayChanged()) {
      displayHelper.clear();
      display.println(shotCounter.profileName);
      display.println("");
      display.print("Total      ");
      display.println(shotCounter.shotsTotal);
      display.print("Series     ");
      display.println(shotCounter.shotsSeries);
      displayHelper.render();
      displayHelper.setDisplayChanged(false);
    }
    // Longpress handler
    if (singleButton.longPressTrigger()) {
      dataProfiles.nextShotCounter();
      shotCounter = dataProfiles.getShotCounter();
      displayHelper.setDisplayChanged(true);
      displayHelper.blink();
      singleButton.longPressTriggerDone();
    }
  }
  else if (page_index == 1) {
    if (displayHelper.getDisplayChanged()) {
      displayHelper.clear();
      display.println("  Waiting for shots");
      displayHelper.bitmapIcon(44, 10, Aim, 40, 20);
      displayHelper.render();
      displayHelper.setDisplayChanged(false);
    }
    // Longpress handler
    if (singleButton.longPressTrigger()) {
      shotCounter.shotsSeries = 0;
      dataProfiles.putShotCounter(shotCounter);
      displayHelper.blink();
      singleButton.longPressTriggerDone();
    }

    // Get value from sesnsor
    float g_max = gyroMeasure.getAccelerationMax();

    // Count shots depending on g force setting of profile
    if (g_max >= shotCounter.countGforce) {
      // Store counted g value for later usage
      gyroMeasure.setGCountedLast(g_max);

      // Debug
      if (PRINT_DEBUG) {
        Serial.print("SHOT COUNTED ");
        Serial.println(g_max);
      }

      // Delay to prevent multiple counts
      shotCounter.shotsSeries++;
      shotCounter.shotsTotal++;
      dataProfiles.putShotCounter(shotCounter);
      displayHelper.blink(shotCounter.shotDelay);
    }
  }
  else if (page_index == 2) {
    if (displayHelper.getDisplayChanged()) {
      displayHelper.clear();
      display.println("Setup profile");
      display.println("");
      display.println("To enter setup");
      display.println("hold button");
      displayHelper.render();
      displayHelper.setDisplayChanged(false);
    }
    // Longpress handler
    if (singleButton.longPressTrigger()) {
      page_index = 3;
      page_changed = true;
      displayHelper.setDisplayChanged(true);
      displayHelper.blink();
      delay(500);
      singleButton.longPressTriggerDone();
    }
  }
  else if (page_index == 3) {
    if (displayHelper.getDisplayChanged()) {
      displayHelper.clear();
      display.println("Minimum G force");
      display.println("to count a shot:");
      display.print(shotCounter.countGforce);
      display.println(" g");
      display.print("Last measured: ");
      display.print(gyroMeasure.getGCountedLast());
      display.print(" g");
      displayHelper.render();
      displayHelper.setDisplayChanged(false);
    }
    // Shortpress handler
    if (singleButton.shortPressTrigger()) {
      // If settings changed, save profile
      ShotCounter shotCounterStored = dataProfiles.getShotCounter();
      if (shotCounterStored.countGforce != shotCounter.countGforce) {
        dataProfiles.putShotCounter(shotCounter);
      }
      page_index = 4;
      page_changed = true;
      displayHelper.setDisplayChanged(true);
      singleButton.shortPressTriggerDone();
    }
    // Longpress handler
    if (singleButton.longPressTrigger()) {
      shotCounter.countGforce = shotCounter.countGforce + 0.5 > 16 ? 0.5 : shotCounter.countGforce + 0.5;
      displayHelper.setDisplayChanged(true);
      displayHelper.blink();
      singleButton.longPressTriggerDone();
    }
  }
  else if (page_index == 4) {
    if (displayHelper.getDisplayChanged()) {
      displayHelper.clear();
      display.println("Minimum time");
      display.println("between shots:");
      display.println("");
      display.print(shotCounter.shotDelay);
      display.println(" ms");
      displayHelper.render();
      displayHelper.setDisplayChanged(false);
    }
    // Shortpress handler
    if (singleButton.shortPressTrigger()) {
      // If settings changed, save profile
      ShotCounter shotCounterStored = dataProfiles.getShotCounter();
      if (shotCounterStored.shotDelay != shotCounter.shotDelay) {
        dataProfiles.putShotCounter(shotCounter);
      }
      page_index = 5;
      page_changed = true;
      displayHelper.setDisplayChanged(true);
      singleButton.shortPressTriggerDone();
    }
    // Longpress handler
    if (singleButton.longPressTrigger()) {
      shotCounter.shotDelay = shotCounter.shotDelay + 50 > 2000 ? 50 : shotCounter.shotDelay + 50;
      displayHelper.setDisplayChanged(true);
      displayHelper.blink();
      singleButton.longPressTriggerDone();
    }
  }
  else if (page_index == 5) {
    if (displayHelper.getDisplayChanged()) {
      displayHelper.clear();
      display.println("Reset profile?");
      display.println("");
      display.println("To reset this profile");
      display.println("hold button");
      displayHelper.render();
      displayHelper.setDisplayChanged(false);
    }
    // Shortpress handler
    if (singleButton.shortPressTrigger()) {
      page_index = 0;
      page_changed = true;
      displayHelper.setDisplayChanged(true);
      singleButton.shortPressTriggerDone();
    }
    // Longpress handler
    if (singleButton.longPressTrigger()) {
      shotCounter = dataProfiles.resetShotCounter();
      displayHelper.blink();
      singleButton.longPressTriggerDone();
    }
  }

  // Delay after page change to disable multiple page changes
  if (page_changed) {
    delay(150);
    page_changed = false;
  }
}
