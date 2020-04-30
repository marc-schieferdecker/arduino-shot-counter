/**
 * Includes
 */
// Libs
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <EEPROM.h>
// Project includes
#include <DataProfiles.h>
#include <DisplayHelper.h>
#include <GyroMeasure.h>
#include <PageContentHelper.h>
#include <PageController.h>
#include <PageHelper.h>
#include <SingleButton.h>
// Includes
#include "Grafics.h"

/**
 * Definitions
 */
// Version and debug
#define SHOT_COUNTER_VERSION "Version 0.02a"
#define PRINT_DEBUG false
// Display
#define OLED_RESET 4
#define OLED_ADDR 0x3C
// Gyroscope
#define GYRO_ADDR 0x68  // AD0 low 0x68, AD0 high 0x69
#define GYRO_ACC_REGISTER_START 0x43
// Data storage
#define EEPROM_CRC_ADDR 1019
#define PROFILES_EEADDR_START 0
#define PROFILES_MAX 6
// Main pages available
#define PAGES_MAIN_TOTAL 3
// Single button pin and bitmask
#define BUTTON_PIN_REG PINB
#define BUTTON_PIN_BITMASK 0b00000100  // Pin D10 = bit 2

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
GyroMeasure gyroMeasure(GYRO_ADDR, GYRO_ACC_REGISTER_START, &pageContentHelper, PRINT_DEBUG);
// EEPROM crc
EEPromCRC eepromCrc(EEPROM_CRC_ADDR, PROFILES_EEADDR_START, sizeof(ShotCounter) * PROFILES_MAX);
// Data profiles
DataProfiles dataProfiles(PROFILES_MAX, PROFILES_EEADDR_START, &eepromCrc);
// Page helper
PageHelper pageHelper(&display, PAGES_MAIN_TOTAL);
// Button handler
SingleButton singleButton(1500, &BUTTON_PIN_REG, BUTTON_PIN_BITMASK);
// Main controller
PageController pageController(&dataProfiles, &displayHelper, &gyroMeasure, &pageContentHelper, &pageHelper, &singleButton, PRINT_DEBUG);

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

    // Init gyro sensor
    gyroMeasure.setup();

    // Init data storage
    dataProfiles.setup();

    // Init page controller
    pageController.setup();

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
        pageController.counterPage();
    } else if (pageHelper.getPageIndex() == 1) {
        pageController.waitingForShotsPage(Aim);
    } else if (pageHelper.getPageIndex() == 2) {
        pageController.enterProfilePage();
    } else if (pageHelper.getPageIndex() == 3) {
        pageController.calibrationPage();
    } else if (pageHelper.getPageIndex() == 4) {
        pageController.setupGforcePage();
    } else if (pageHelper.getPageIndex() == 5) {
        pageController.setupShotDelayPage();
    } else if (pageHelper.getPageIndex() == 6) {
        pageController.resetProfilePage();
    }

    // Delay after page change to disable multiple page changes
    if (pageHelper.getPageIndex()) {
        delay(150);
        pageHelper.setPageChanged(false);
    }
}
