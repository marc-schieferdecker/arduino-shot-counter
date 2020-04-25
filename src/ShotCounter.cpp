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
#define BUTTON_PIN 10

/**
 * Global variables
 */
// Gyro
int16_t gyro_g_x, gyro_g_y, gyro_g_z;
int16_t gyro_x, gyro_y, gyro_z;
float gyro_temp;
// Selected profile
short profile_index = 0;
// Menu pointer
short menu_index = 0;

// Display class
Adafruit_SSD1306 display(OLED_RESET);

// EEPROM pointer, data structure & checksum class
int eeAddress = 0;
struct ShotCounter {
  unsigned long shotsTotal;
  unsigned int shotsSeries;
  unsigned int countGforce;
  char profileName[15];
};
ShotCounter shotCounter;
EEPROM_crc eepromCrc(EEPROM_CRC_ADDR, eeAddress, sizeof(ShotCounter));

// Setup programm
void setup() {
  Serial.begin(9600);
  Serial.print("Setup()");
  Serial.println();

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
  Wire.write(0);
  Wire.endTransmission(true);

  // Setup button (Com-KY004TM)
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH);

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
  // Load shot counter from EEPROM
  EEPROM.get(eeAddress, shotCounter);

  // Print state of EEPROM
  Serial.print("CRC EEPROM STATE ");
  Serial.print(eepromCrc.crcIsValid());
  Serial.println();

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
  // Set menu handler via button
  if (digitalRead(BUTTON_PIN) == LOW) {
    menu_index = (menu_index + 1) > 4 ? 0 : (menu_index + 1);
  }

  if (menu_index == 0) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.println(shotCounter.profileName);
    display.print("Total  : ");
    display.println(shotCounter.shotsTotal);
    display.print("Series : ");
    display.println(shotCounter.shotsSeries);
    display.print("G force: ");
    display.println(shotCounter.countGforce);
    display.display();
  }
  else if (menu_index == 1) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Page 2");
    display.display();
  }
  else if (menu_index == 2) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Page 3");
    display.display();
  }
  else if (menu_index == 3) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Page 4");
    display.display();
  }
  else if (menu_index == 4) {
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Wating for shots!");
      display.display();
      // Get gyro data
      Wire.beginTransmission(GYRO_ADDR);
      Wire.write(0x3B);
      Wire.endTransmission(false);
      Wire.requestFrom(GYRO_ADDR, 14, true);
      gyro_x = Wire.read()<<8 | Wire.read();
      gyro_y = Wire.read()<<8 | Wire.read();
      gyro_z = Wire.read()<<8 | Wire.read();
      gyro_temp = Wire.read()<<8 | Wire.read();
      gyro_g_x = Wire.read()<<8 | Wire.read();
      gyro_g_y = Wire.read()<<8 | Wire.read();
      gyro_g_z = Wire.read()<<8 | Wire.read();
      Wire.endTransmission(true);
      Serial.print("GYRO TEMP ");
      Serial.println(gyro_temp / 340 + 36.53);
      Serial.print("GYRO G ");
      Serial.print(gyro_g_x); Serial.print(" ");
      Serial.print(gyro_g_y); Serial.print(" ");
      Serial.println(gyro_g_z);
      Serial.print("GYRO XYZ ");
      Serial.print(gyro_x); Serial.print(" ");
      Serial.print(gyro_y); Serial.print(" ");
      Serial.println(gyro_z);
  }
}
