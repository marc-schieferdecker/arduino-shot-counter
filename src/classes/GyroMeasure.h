#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#if !defined(GYRO_ADDR)
    #define GYRO_ADDR 0x68 // AD0 low 0x68, AD0 high 0x69
#endif
#if !defined(GYRO_ACC_REGISTER_START)
    #define GYRO_ACC_REGISTER_START 0x43
#endif
#if !defined(PRINT_DEBUG)
    #define PRINT_DEBUG false
#endif

class GyroMeasure {
    private:
        int16_t gyro_g_x;
        int16_t gyro_g_y;
        int16_t gyro_g_z;
        float g_max = 0;
        float g_counted_last = 0;
        Adafruit_SSD1306 *display;

    public:
        // Constructor
        GyroMeasure(Adafruit_SSD1306 *display) {
            this -> display = display;
        }

    void init() {
        // Setup gyro (MPU6050)
        Wire.begin();
        Wire.beginTransmission(GYRO_ADDR);
        Wire.write(0x6B);
        Wire.write(0b10000000); // Reset gyro to default
        Wire.endTransmission(true);
        delay(50);
        if (PRINT_DEBUG) {
            Serial.println("Gyro reseted");
        }

        Wire.beginTransmission(GYRO_ADDR);
        Wire.write(0x6B);
        Wire.write(0); // Set sleep to 0
        Wire.endTransmission(true);
        delay(50);
        if (PRINT_DEBUG) {
            Serial.println("Gyro waked");
        }

        Wire.beginTransmission(GYRO_ADDR);
        Wire.write(0x1B);
        Wire.write(0b00011000); // Set max gyro scale
        Wire.endTransmission(true);
        delay(50);
        if (PRINT_DEBUG) {
            Serial.println("Gyro set to max scale");
        }

        Wire.beginTransmission(GYRO_ADDR);
        Wire.write(0x1C);
        Wire.write(0b00011000); // Set acc to 16g
        Wire.endTransmission(true);
        delay(50);
        if (PRINT_DEBUG) {
            Serial.print("Gyro acc sensitivity set to 16g");
        }

        // Test if gyro is sleeping
        Wire.beginTransmission(GYRO_ADDR);
        Wire.write(0x6B);
        Wire.endTransmission(false);
        Wire.requestFrom(GYRO_ADDR, 1, true);
        byte testSleepMode = Wire.read();
        Wire.endTransmission(true);
        delay(50);
        if (PRINT_DEBUG) {
            Serial.print("Gyro is sleeping -> ");
            Serial.println(testSleepMode);
        }
        if (testSleepMode & 0x01000000) {
            this -> display -> clearDisplay();
            this -> display -> setTextColor(WHITE);
            this -> display -> setTextSize(1);
            this -> display -> setCursor(2,0);
            this -> display -> println("Problem with gyro");
            this -> display -> println("Sleep bit still active");
            delay(5000);
        }
    }

    // Get actual measurement and calculate acc max
    float getAccelerationMax() {
        // Measure gyro
        Wire.beginTransmission(GYRO_ADDR);
        Wire.write(GYRO_ACC_REGISTER_START);
        Wire.endTransmission(false);
        Wire.requestFrom(GYRO_ADDR, 6, true);
        this -> gyro_g_x = Wire.read()<<8 | Wire.read();
        this -> gyro_g_y = Wire.read()<<8 | Wire.read();
        this -> gyro_g_z = Wire.read()<<8 | Wire.read();
        Wire.endTransmission(true);

        // Always use positive values, calculate real g values from 0 to 16 (sensor max)
        float gyro_acc_x = (float)abs(this -> gyro_g_x) / 2048;
        float gyro_acc_y = (float)abs(this -> gyro_g_y) / 2048;
        float gyro_acc_z = (float)abs(this -> gyro_g_z) / 2048;
        this -> g_max = max(max(gyro_acc_x, gyro_acc_y), gyro_acc_z);
        
        return this -> g_max;
    }

    // Get the last acc max value measured
    float getLastAccelerationMax() {
        return this -> g_max;
    }

    // Set last counted g value
    void setGCountedLast(float g_counted_last) {
        this -> g_counted_last = g_counted_last;
    }

    // Get last counted g value
    float getGCountedLast() {
        return this -> g_counted_last;
    }
};
