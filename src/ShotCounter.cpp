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
#include "classes\EEPromCRC.h"

/**
 * Definitions
 */
#define OLED_RESET 4
#define OLED_ADDR 0x3C
#define GYRO_ADDR 0x68
#define EEPROM_CRC_ADDR 1019

/**
 * Global variables
 */
// Gyro
int16_t gyro_g_x, gyro_g_y, gyro_g_z;
int16_t gyro_x, gyro_y, gyro_z;
int16_t gyro_temp;
// Selected profile
short profile_index = 0;

// EEPROM pointer, data structure & checksum class
int eeAddress = 0;
struct ShotCounter {
  unsigned long shotsTotal;
  unsigned int shotsSeries;
  unsigned int countGforce;
  char profileName[15];
};
EEPROM_crc eepromCrc(EEPROM_CRC_ADDR, eeAddress, sizeof(ShotCounter));

// Display class
Adafruit_SSD1306 display(OLED_RESET);

// Setup programm
void setup() {
  // Setup display (SBC-OLED01)
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.drawBitmap(0, 0, Logo, 128, 64, WHITE);
  delay(1000);
  display.display();

  // Setup gyro (MPU6050)
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(GYRO_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  // Init EEPROM
  if (!eepromCrc.crcIsValid()) {
    // Write default data to EEPROM
    ShotCounter defaultShotCounter = {
      0,
      0,
      3,
      "Preset 1"
    };
    EEPROM.put(eeAddress, defaultShotCounter);
    eepromCrc.crcPut();
  }
}

// Main loop
void loop() {
  // Display test
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(23,0);
  display.println("Shot Counter");
  display.setCursor(23,12);
  display.println("letsshootshow.de");
  display.setCursor(36,24);
  display.println("Ready!");
  display.display();
  delay(1000);
  display.clearDisplay();
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);

  // Gyro test
  Wire.beginTransmission(GYRO_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(GYRO_ADDR, 7*2, true);
  gyro_g_x = Wire.read()<<8 | Wire.read();
  gyro_g_y = Wire.read()<<8 | Wire.read();
  gyro_g_z = Wire.read()<<8 | Wire.read();
  gyro_temp = (Wire.read()<<8 | Wire.read()) / 340.00+36.53;
  gyro_x = Wire.read()<<8 | Wire.read();
  gyro_y = Wire.read()<<8 | Wire.read();
  gyro_z = Wire.read()<<8 | Wire.read();
}
