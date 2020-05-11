#include <Arduino.h>
#include <GyroMeasure.h>
#include <PageContentHelper.h>
#include <Wire.h>

#include "../../include/Debug.h"

GyroMeasure::GyroMeasure(int _gyroAddress, int _gyroAccRegisterStart, PageContentHelper *_pageContentHelper) {
    gyroAddress = _gyroAddress;
    gyroAccRegisterStart = _gyroAccRegisterStart;
    pageContentHelper = _pageContentHelper;
}

void GyroMeasure::setup() {
    // Wake sensor
    sensorWake();

    // Reset sensor to default settings
    setupResetSensor();

    // Wake sensor
    sensorWake();

    // Set acceleration measuring to lowest sensitivity (16g)
    setupAccelerationSensitivity();

    // Set scale of gyro to lowest sensitivity
    setupGyroSensitivity();

    // Let sensor sleep until it is used
    sensorSleep();
}

float GyroMeasure::getAccelerationMax() {
    // Measure gyro
    Wire.beginTransmission(gyroAddress);
    Wire.write(gyroAccRegisterStart);
    Wire.endTransmission(false);
    Wire.requestFrom(gyroAddress, 6, true);
    gyroGX = Wire.read() << 8 | Wire.read();
    gyroGY = Wire.read() << 8 | Wire.read();
    gyroGZ = Wire.read() << 8 | Wire.read();
    Wire.endTransmission(true);

    // Always use positive values, calculate real g values from 0 to 16 (sensor max)
    float gyro_acc_x = (float)abs(gyroGX) / 2048;
    float gyro_acc_y = (float)abs(gyroGY) / 2048;
    float gyro_acc_z = (float)abs(gyroGZ) / 2048;
    gMax = max(max(gyro_acc_x, gyro_acc_y), gyro_acc_z);

    return gMax;
}

float GyroMeasure::getLastAccelerationMax() {
    return gMax;
}

void GyroMeasure::setGCountedLast(float _gMaxCountedLast) {
    gMaxCountedLast = _gMaxCountedLast;
}

float GyroMeasure::getGCountedLast() {
    return gMaxCountedLast;
}

void GyroMeasure::setupResetSensor() {
    while (1) {
        Wire.begin();
        Wire.beginTransmission(gyroAddress);
        Wire.write(0x6B);
        Wire.write(0b10000000);  // Reset gyro to default
        Wire.endTransmission(true);
        delay(50);
#ifdef SCDebug
        Serial.println(F("Gyro reseted"));
#endif

        // Test for success (reset bit must have changed to 0)
        Wire.beginTransmission(gyroAddress);
        Wire.write(0x6B);
        Wire.endTransmission(false);
        Wire.requestFrom(gyroAddress, 1, true);
        byte testGyroReset = Wire.read();
        Wire.endTransmission(true);
        delay(50);
#ifdef SCDebug
        Serial.print(F("Gyro reset bits -> "));
        Serial.println(testGyroReset & 0b10000000);
#endif
        if ((testGyroReset & 0b10000000) == 0b10000000) {
            pageContentHelper->sensorResetErrorPage();
            delay(1000);
        } else {
            // Exit loop
            break;
        }
    }
}

void GyroMeasure::setupGyroSensitivity() {
    while (1) {
        Wire.beginTransmission(gyroAddress);
        Wire.write(0x1B);
        Wire.write(0b00011000);  // Set max gyro scale
        Wire.endTransmission(true);
        delay(50);
#ifdef SCDebug
        Serial.println(F("Gyro set to max scale"));
#endif

        // Test for success
        Wire.beginTransmission(gyroAddress);
        Wire.write(0x1B);
        Wire.endTransmission(false);
        Wire.requestFrom(gyroAddress, 1, true);
        byte testGyroSens = Wire.read();
        Wire.endTransmission(true);
        delay(50);
#ifdef SCDebug
        Serial.print(F("Gyro sens bits -> "));
        Serial.println(testGyroSens & 0b00011000);
#endif
        if ((testGyroSens & 0b00011000) != 0b00011000) {
            pageContentHelper->sensorSensSetupErrorPage();
            delay(1000);
        } else {
            // Exit loop
            break;
        }
    }
}

void GyroMeasure::setupAccelerationSensitivity() {
    while (1) {
        Wire.beginTransmission(gyroAddress);
        Wire.write(0x1C);
        Wire.write(0b00011000);  // Set acc to 16g
        Wire.endTransmission(true);
        delay(50);
#ifdef SCDebug
        Serial.println(F("Gyro acc sensitivity set to 16g"));
#endif

        // Test for success
        Wire.beginTransmission(gyroAddress);
        Wire.write(0x1C);
        Wire.endTransmission(false);
        Wire.requestFrom(gyroAddress, 1, true);
        byte testAccSens = Wire.read();
        Wire.endTransmission(true);
        delay(50);
#ifdef SCDebug
        Serial.print(F("Gyro acc sens bits -> "));
        Serial.println(testAccSens & 0b00011000);
#endif
        if ((testAccSens & 0b00011000) != 0b00011000) {
            pageContentHelper->sensorAccSetupErrorPage();
            delay(1000);
        } else {
            // Exit loop
            break;
        }
    }
}

void GyroMeasure::sensorSleep() {
    Wire.beginTransmission(gyroAddress);
    Wire.write(0x6B);
    Wire.write(0b01000000);  // Set sleep to 1
    Wire.endTransmission(true);
    delay(50);
#ifdef SCDebug
    Serial.println(F("Gyro sleep"));
#endif
}

void GyroMeasure::sensorWake() {
    // Wake the gyro and test for success
    while (1) {
        // Wake up
        Wire.beginTransmission(gyroAddress);
        Wire.write(0x6B);
        Wire.write(0);  // Set sleep to 0
        Wire.endTransmission(true);
        delay(50);
#ifdef SCDebug
        Serial.println(F("Gyro waked"));
#endif

        // Test if sensor is woke
        Wire.beginTransmission(gyroAddress);
        Wire.write(0x6B);
        Wire.endTransmission(false);
        Wire.requestFrom(gyroAddress, 1, true);
        byte testSleepMode = Wire.read();
        Wire.endTransmission(true);
        delay(50);
#ifdef SCDebug
        Serial.print(F("Gyro is sleeping -> "));
        Serial.println(testSleepMode);
#endif
        if (testSleepMode & 0x01000000) {
            pageContentHelper->sensorErrorPage();
            delay(1000);
        } else {
            // Sensor is woke so end the loop
            break;
        }
    }
}
