/**
 * Includes
 */
#include <Arduino.h>
#include <EEPROM.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// Project includes
#include "Grafics.h"
#include "classes/SingleButton.h"
#include "classes/DataProfiles.h"

/**
 * Definitions
 */
#define OLED_RESET 4
#define OLED_ADDR 0x3C
#define GYRO_ADDR 0x68 // AD0 low 0x68, AD0 high 0x69
#define GYRO_ACC_REGISTER_START 0x43
#define EEPROM_CRC_ADDR 1019
#define PROFILES_EEADDR_START 0
#define PROFILES_MAX 6
#define DEBUG false

/**
 * Global variables
 */
// Gyro
int16_t gyro_g_x, gyro_g_y, gyro_g_z;
// Selected profile
short profile_index = 0;
// Page pointer and change status
short page_index = 0;
bool page_changed = false;

// Display needs update (reduce display draws for better performance)
bool display_changed = true;

// Display class
Adafruit_SSD1306 display(OLED_RESET);

// EEPROM pointer, data structure & checksum class
ShotCounter shotCounter;
EEPROM_crc eepromCrc(EEPROM_CRC_ADDR, PROFILES_EEADDR_START, sizeof(ShotCounter) * PROFILES_MAX);
DataProfiles dataProfiles(PROFILES_MAX, PROFILES_EEADDR_START, &eepromCrc);

// Button handler
SingleButton singleButton(1500);

// Setup programm
void setup() {
  if (DEBUG) {
    Serial.begin(9600);
    Serial.print("Setup()");
    Serial.println();
  }

  // Setup display (SBC-OLED01)
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.drawBitmap(0, 0, Logo, 128, 32, WHITE);
  display.display();
  delay(1000);

  // Setup gyro (MPU6050)
  Wire.begin();
  Wire.beginTransmission(GYRO_ADDR);
  Wire.write(0x6B);
  Wire.write(0b10000000); // Reset gyro to default
  Wire.endTransmission(true);

  Wire.begin();
  Wire.beginTransmission(GYRO_ADDR);
  Wire.write(0x6B);
  Wire.write(0); // Set sleep to 0
  Wire.endTransmission(true);

  Wire.beginTransmission(GYRO_ADDR);
  Wire.write(0x1B);
  Wire.write(0b00011000); // Set max gyro scale
  Wire.endTransmission(true);

  Wire.beginTransmission(GYRO_ADDR);
  Wire.write(0x1C);
  Wire.write(0b00011000); // Set acc to 16g
  Wire.endTransmission(true);

  // Init data
  dataProfiles.init();
  // Load shot counter from EEPROM
  shotCounter = dataProfiles.getShotCounter();

  // Print state of EEPROM
  if (DEBUG) {
    Serial.print("CRC EEPROM STATE ");
    Serial.print(eepromCrc.crcIsValid());
    Serial.println();
  }

  // Display test
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(2,0);
  display.println("Shot Counter");
  display.setCursor(2,12);
  display.println("Version 0.01a");
  display.setCursor(2,24);
  display.println("letsshootshow.de");
  display.display();
  delay(1000);
  display.invertDisplay(true);
  delay(100);
  display.invertDisplay(false);
  delay(100);
  display.clearDisplay();
  display.display();
}

// Main loop
void loop() {
  // Button handler
  singleButton.loop();
  // Check for short press
  if (singleButton.shortPressTrigger() && page_index <= 2) {
      page_index = (page_index + 1) > 2 ? 0 : (page_index + 1);
      page_changed = true;
      display_changed = true;
      singleButton.shortPressTriggerDone();
  }

  // Main page (displays data of active preset)
  if (page_index == 0) {
    // Show profile values
    if (display_changed) {
      display.clearDisplay();
      display.setCursor(0,0);
      display.println(shotCounter.profileName);
      display.println("");
      display.print("Total      ");
      display.println(shotCounter.shotsTotal);
      display.print("Series     ");
      display.println(shotCounter.shotsSeries);
      display.display();
    }
    // Longpress handler
    if (singleButton.longPressTrigger()) {
      dataProfiles.nextShotCounter();
      shotCounter = dataProfiles.getShotCounter();
      display_changed = true;
      display.invertDisplay(true);
      delay(100);
      display.invertDisplay(false);
      singleButton.longPressTriggerDone();
    }
  }
  else if (page_index == 1) {
    if (display_changed) {
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("  Waiting for shots");
      display.drawBitmap(44, 10, Aim, 40, 20, WHITE);
      display.display();
      display_changed = false;
    }
    // Longpress handler
    if (singleButton.longPressTrigger()) {
      shotCounter.shotsSeries = 0;
      dataProfiles.putShotCounter(shotCounter);
      display.invertDisplay(true);
      delay(100);
      display.invertDisplay(false);
      singleButton.longPressTriggerDone();
    }

    // Measure gyro
    Wire.beginTransmission(GYRO_ADDR);
    Wire.write(GYRO_ACC_REGISTER_START);
    Wire.endTransmission(false);
    Wire.requestFrom(GYRO_ADDR, 6, true);
    gyro_g_x = Wire.read()<<8 | Wire.read();
    gyro_g_y = Wire.read()<<8 | Wire.read();
    gyro_g_z = Wire.read()<<8 | Wire.read();
    Wire.endTransmission(true);

    // Always use positive values, calculate real g values from 0 to 16 (sensor max)
    float gyro_acc_x = (float)abs(gyro_g_x) / 2048;
    float gyro_acc_y = (float)abs(gyro_g_y) / 2048;
    float gyro_acc_z = (float)abs(gyro_g_z) / 2048;
    float gyro_max = max(max(gyro_acc_x, gyro_acc_y), gyro_acc_z);

    // Count shots depending on g force setting of profile
    if (gyro_max >= shotCounter.countGforce) {
      if (DEBUG) {
        Serial.print("SHOT COUNTED ");
        Serial.println(gyro_max);
      }
      // Delay to prevent multiple counts
      shotCounter.shotsSeries++;
      shotCounter.shotsTotal++;
      dataProfiles.putShotCounter(shotCounter);
      display.invertDisplay(true);
      delay(shotCounter.shotDelay);
      display.invertDisplay(false);
    }
  }
  else if (page_index == 2) {
    if (display_changed) {
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Setup profile");
      display.println("");
      display.println("To enter setup");
      display.println("hold button");
      display.display();
      display_changed = false;
    }
    // Longpress handler
    if (singleButton.longPressTrigger()) {
      page_index = 3;
      page_changed = true;
      display_changed = true;
      display.invertDisplay(true);
      delay(100);
      display.invertDisplay(false);
      delay(500);
      singleButton.longPressTriggerDone();
    }
  }
  else if (page_index == 3) {
    if (display_changed) {
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Minimum G force");
      display.println("to count a shot:");
      display.println();
      display.print(shotCounter.countGforce);
      display.println(" g");
      display.display();
      display_changed = false;
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
      display_changed = true;
      singleButton.shortPressTriggerDone();
    }
    // Longpress handler
    if (singleButton.longPressTrigger()) {
      shotCounter.countGforce = shotCounter.countGforce + 0.5 > 16 ? 0.5 : shotCounter.countGforce + 0.5;
      display_changed = true;
      display.invertDisplay(true);
      delay(100);
      display.invertDisplay(false);
      singleButton.longPressTriggerDone();
    }
  }
  else if (page_index == 4) {
    if (display_changed) {
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Minimum time");
      display.println("between shots:");
      display.println("");
      display.print(shotCounter.shotDelay);
      display.println(" ms");
      display.display();
      display_changed = false;
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
      display_changed = true;
      singleButton.shortPressTriggerDone();
    }
    // Longpress handler
    if (singleButton.longPressTrigger()) {
      shotCounter.shotDelay = shotCounter.shotDelay + 50 > 2000 ? 50 : shotCounter.shotDelay + 50;
      display_changed = true;
      display.invertDisplay(true);
      delay(100);
      display.invertDisplay(false);
      singleButton.longPressTriggerDone();
    }
  }
  else if (page_index == 5) {
    if (display_changed) {
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Reset profile?");
      display.println("");
      display.println("To reset this profile");
      display.println("hold button");
      display.display();
      display_changed = false;
    }
    // Shortpress handler
    if (singleButton.shortPressTrigger()) {
      page_index = 0;
      page_changed = true;
      display_changed = true;
      singleButton.shortPressTriggerDone();
    }
    // Longpress handler
    if (singleButton.longPressTrigger()) {
      shotCounter = dataProfiles.resetShotCounter();
      display.invertDisplay(true);
      delay(100);
      display.invertDisplay(false);
      singleButton.longPressTriggerDone();
    }
  }

  // Delay after page change to disable multiple page changes
  if (page_changed) {
    delay(150);
    page_changed = false;
  }
}
