#ifndef GyroMeasure_h
#define GyroMeasure_h

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

class GyroMeasure {
    private:
        bool print_debug;
        int16_t gyro_g_x;
        int16_t gyro_g_y;
        int16_t gyro_g_z;
        float g_max = 0;
        float g_counted_last = 0;
        Adafruit_SSD1306 *display;

    public:
        // Constructor
        GyroMeasure(Adafruit_SSD1306 *display, bool print_debug);

        void init();

        // Get actual measurement and calculate acc max
        float getAccelerationMax();

        // Get the last acc max value measured
        float getLastAccelerationMax();

        // Set last counted g value
        void setGCountedLast(float g_counted_last);

        // Get last counted g value
        float getGCountedLast();
};

#endif