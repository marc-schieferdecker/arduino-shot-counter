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
#include <PageHelper.h>
#include <PageContentHelper.h>

/**
 * Definitions
 */
#define SHOT_COUNTER_VERSION "Version 0.02a"
#define PRINT_DEBUG false
#define OLED_RESET 4
#define OLED_ADDR 0x3C
#ifdef GYRO_ADDR
  #undef GYRO_ADDR
  #define GYRO_ADDR 0x68 // AD0 low 0x68, AD0 high 0x69
#endif
#ifdef GYRO_ACC_REGISTER_START
  #undef GYRO_ACC_REGISTER_START
  #define GYRO_ACC_REGISTER_START 0x43
#endif
#define EEPROM_CRC_ADDR 1019
#define PROFILES_EEADDR_START 0
#define PROFILES_MAX 6
#define PAGES_MAIN_TOTAL 3
#ifdef BUTTON_PIN_REG
  #undef BUTTON_PIN_REG
  #define BUTTON_PIN_REG PINB
#endif
#define BUTTON_PRESS_VAL 0
#define BUTTON_RELEASE_VAL 4

/**
 * Global variables
 */
// Active shot counter
ShotCounter shotCounter;

/**
 * Instanciate classes
 */
// Display class
Adafruit_SSD1306 display(OLED_RESET);
// Display helper
DisplayHelper displayHelper(&display, SSD1306_SWITCHCAPVCC, OLED_ADDR);
// Page content helper
PageContentHelper pageContentHelper(&display);
// Gyro measurement
GyroMeasure gyroMeasure(&pageContentHelper, PRINT_DEBUG);
// EEPROM crc
EEPromCRC eepromCrc(EEPROM_CRC_ADDR, PROFILES_EEADDR_START, sizeof(ShotCounter) * PROFILES_MAX);
// Data profiles
DataProfiles dataProfiles(PROFILES_MAX, PROFILES_EEADDR_START, &eepromCrc);
// Page helper
PageHelper pageHelper(&display, PAGES_MAIN_TOTAL);
// Button handler
SingleButton singleButton(1500, BUTTON_PRESS_VAL, BUTTON_RELEASE_VAL);

// Setup
void setup() {
  // Setup display (SBC-OLED01)
  displayHelper.setup();
  displayHelper.bitmapFullscreen(Logo);
  delay(1000);

  // Setup serial console in debug mode
  if (PRINT_DEBUG) {
    Serial.begin(9600);
    delay(50);
    Serial.println(F("Setup serial with 9600 baud"));
  }

  // Init gyro senson
  gyroMeasure.init();

  // Init data & load shot counter from EEPROM
  shotCounter = dataProfiles.init();

  // Print state of EEPROM
  if (PRINT_DEBUG) {
    Serial.print(F("CRC EEPROM STATE "));
    Serial.print(eepromCrc.crcIsValid());
    Serial.println();
  }

  // Display version page
  displayHelper.clear();
  pageContentHelper.versionPage(SHOT_COUNTER_VERSION);
  displayHelper.render();
  delay(1000);
}

// Main loop
void loop() {
  // Button handler
  singleButton.loop();

  // Check for short press
  if (singleButton.shortPressTrigger() && pageHelper.getPageIndex() < PAGES_MAIN_TOTAL) {
      pageHelper.nextPage();
      displayHelper.setDisplayChanged(true);
      singleButton.shortPressTriggerDone();
  }

  // Main page (displays data of active preset)
  if (pageHelper.getPageIndex() == 0) {
    // Show profile values
    if (displayHelper.getDisplayChanged()) {
      displayHelper.clear();
      pageContentHelper.counterPage(shotCounter);
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
  else if (pageHelper.getPageIndex() == 1) {
    if (displayHelper.getDisplayChanged()) {
      // Wake sensor
      gyroMeasure.sensorWake();
      // Display waiting for shots page
      displayHelper.clear();
      pageContentHelper.waitingForShotsPage();
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
        Serial.print(F("SHOT COUNTED "));
        Serial.println(g_max);
      }

      // Delay to prevent multiple counts
      shotCounter.shotsSeries++;
      shotCounter.shotsTotal++;
      dataProfiles.putShotCounter(shotCounter);
      displayHelper.blink(shotCounter.shotDelay);
    }
  }
  else if (pageHelper.getPageIndex() == 2) {
    if (displayHelper.getDisplayChanged()) {
      // Set sensor to sleep for lower power consumption
      gyroMeasure.sensorSleep();
      // Display enter setup page
      displayHelper.clear();
      pageContentHelper.enterProfilePage();
      displayHelper.render();
      displayHelper.setDisplayChanged(false);
    }
    // Longpress handler
    if (singleButton.longPressTrigger()) {
      pageHelper.setPageIndex(3);
      pageHelper.setPageChanged(true);
      displayHelper.setDisplayChanged(true);
      displayHelper.blink();
      delay(500);
      singleButton.longPressTriggerDone();
    }
  }
  else if (pageHelper.getPageIndex() == 3) {
    if (displayHelper.getDisplayChanged()) {
      displayHelper.clear();
      pageContentHelper.setupGforcePage(shotCounter.countGforce, gyroMeasure.getGCountedLast());
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
      pageHelper.setPageIndex(4);
      pageHelper.setPageChanged(true);
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
  else if (pageHelper.getPageIndex() == 4) {
    if (displayHelper.getDisplayChanged()) {
      displayHelper.clear();
      pageContentHelper.setupShotDelayPage(shotCounter.shotDelay);
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
      pageHelper.setPageIndex(5);
      pageHelper.setPageChanged(true);
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
  else if (pageHelper.getPageIndex() == 5) {
    if (displayHelper.getDisplayChanged()) {
      displayHelper.clear();
      pageContentHelper.resetProfilePage();
      displayHelper.render();
      displayHelper.setDisplayChanged(false);
    }
    // Shortpress handler
    if (singleButton.shortPressTrigger()) {
      pageHelper.setPageIndex(0);
      pageHelper.setPageChanged(true);
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
  if (pageHelper.getPageIndex()) {
    delay(150);
    pageHelper.setPageChanged(false);
  }
}
