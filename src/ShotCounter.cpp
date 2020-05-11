/**
 * Includes
 */
// Libs
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
// Project includes
#include <ShotCounterData.h>
#include <DataProfiles.h>
#include <DisplayHelper.h>
#include <GyroMeasure.h>
#include <PageContentHelper.h>
#include <PageController.h>
#include <PageHelper.h>
#include <PowerDevice.h>
#include <SingleButton.h>
#include <ShotCounterLang.h>
// Bitmaps
#include "Grafics.h"

/**
 * Definitions
 */
// Version and debug
#define SHOT_COUNTER_VERSION "Version 0.05a"
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
#define PAGES_MAIN_TOTAL 5
// Single button pin and bitmask
#define BUTTON_PIN_REG PINB
#define BUTTON_PIN_BITMASK 0b00000100  // Pin D10 = bit 2 of PINB
// Power device pin and bitmasks
#define POWERPIN_PIN PIND
#define POWERPIN_BITMASK_ON 0b00100000  // PIN D5 = bit 5 of PIND
#define POWERPIN_BITMASK_OFF 0b00000000

/**
 * Instanciate classes
 */
// Display class
Adafruit_SSD1306 display(OLED_RESET);
// Display helper
DisplayHelper displayHelper(&display, SSD1306_SWITCHCAPVCC, OLED_ADDR);
// Localization
ShotCounterLang shotCounterLang(PROFILES_EEADDR_START + (sizeof(ShotCounterData) * PROFILES_MAX) + 1);
// Page content helper
PageContentHelper pageContentHelper(&display, &shotCounterLang);
// Gyro measurement
GyroMeasure gyroMeasure(GYRO_ADDR, GYRO_ACC_REGISTER_START, &pageContentHelper, PRINT_DEBUG);
// EEPROM crc
EEPromCRC eepromCrc(EEPROM_CRC_ADDR, PROFILES_EEADDR_START, sizeof(ShotCounterData) * PROFILES_MAX);
// Data profiles
DataProfiles dataProfiles(PROFILES_MAX, PROFILES_EEADDR_START, &eepromCrc);
// Page helper
PageHelper pageHelper(&display, PAGES_MAIN_TOTAL);
// Button handler
SingleButton singleButton(1500, &BUTTON_PIN_REG, BUTTON_PIN_BITMASK);
// Power device
PowerDevice powerDevice(&POWERPIN_PIN, POWERPIN_BITMASK_ON, POWERPIN_BITMASK_OFF);
// Main controller
PageController pageController(
    &dataProfiles,
    &displayHelper,
    &gyroMeasure,
    &pageContentHelper,
    &pageHelper,
    &singleButton,
    &powerDevice,
    &shotCounterLang,
    PRINT_DEBUG);

// Setup
void setup() {
    // Setup serial console in debug mode
    if (PRINT_DEBUG) {
        Serial.begin(9600);
        delay(50);
        Serial.println(F("Debug serial with 9600 baud on"));
    }

    // Keep device on and set power pin to HIGH
    powerDevice.keepDeviceOn();
    if (PRINT_DEBUG) {
        Serial.print(F("POWER PIN STATE "));
        Serial.print((POWERPIN_PIN & POWERPIN_BITMASK_ON) >> 5);
        Serial.println();
    }

    // Setup display (SBC-OLED01)
    displayHelper.setup();
    displayHelper.bitmapFullscreen(Logo);
    delay(1000);

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
    // Control loop
    pageController.loop();
}
