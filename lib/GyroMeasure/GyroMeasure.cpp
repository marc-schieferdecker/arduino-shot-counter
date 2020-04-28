#include <Arduino.h>
#include <GyroMeasure.h>
#include <PageContentHelper.h>
#include <Wire.h>

#ifndef GYRO_ADDR
#define GYRO_ADDR 0x68  // AD0 low 0x68, AD0 high 0x69
#endif
#ifndef GYRO_ACC_REGISTER_START
#define GYRO_ACC_REGISTER_START 0x43
#endif

GyroMeasure::GyroMeasure(PageContentHelper *_pageContentHelper, bool _printDebug) {
    pageContentHelper = _pageContentHelper;
    printDebug = _printDebug;
}

void GyroMeasure::init() {
    // Setup gyro (MPU6050)
    Wire.begin();
    Wire.beginTransmission(GYRO_ADDR);
    Wire.write(0x6B);
    Wire.write(0b10000000);  // Reset gyro to default
    Wire.endTransmission(true);
    delay(50);
    if (printDebug) {
        Serial.println(F("Gyro reseted"));
    }

    // Wake sensor
    sensorWake();

    Wire.beginTransmission(GYRO_ADDR);
    Wire.write(0x1B);
    Wire.write(0b00011000);  // Set max gyro scale
    Wire.endTransmission(true);
    delay(50);
    if (printDebug) {
        Serial.println(F("Gyro set to max scale"));
    }

    Wire.beginTransmission(GYRO_ADDR);
    Wire.write(0x1C);
    Wire.write(0b00011000);  // Set acc to 16g
    Wire.endTransmission(true);
    delay(50);
    if (printDebug) {
        Serial.print(F("Gyro acc sensitivity set to 16g"));
    }

    // Let sensor sleep until it is used
    sensorSleep();
}

float GyroMeasure::getAccelerationMax() {
    // Measure gyro
    Wire.beginTransmission(GYRO_ADDR);
    Wire.write(GYRO_ACC_REGISTER_START);
    Wire.endTransmission(false);
    Wire.requestFrom(GYRO_ADDR, 6, true);
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

void GyroMeasure::sensorSleep() {
    Wire.beginTransmission(GYRO_ADDR);
    Wire.write(0x6B);
    Wire.write(0b01000000);  // Set sleep to 1
    Wire.endTransmission(true);
    delay(50);
    if (printDebug) {
        Serial.println(F("Gyro waked"));
    }
}

void GyroMeasure::sensorWake() {
    Wire.beginTransmission(GYRO_ADDR);
    Wire.write(0x6B);
    Wire.write(0);  // Set sleep to 0
    Wire.endTransmission(true);
    delay(50);
    if (printDebug) {
        Serial.println(F("Gyro waked"));
    }

    // Test if gyro is sleeping
    while (1) {
        Wire.beginTransmission(GYRO_ADDR);
        Wire.write(0x6B);
        Wire.endTransmission(false);
        Wire.requestFrom(GYRO_ADDR, 1, true);
        byte testSleepMode = Wire.read();
        Wire.endTransmission(true);
        delay(50);
        if (printDebug) {
            Serial.print(F("Gyro is sleeping -> "));
            Serial.println(testSleepMode);
        }
        if (testSleepMode & 0x01000000) {
            pageContentHelper->sensorErrorPage();
            delay(1000);
        } else {
            break;
        }
    }
}
