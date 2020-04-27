#ifndef GyroMeasure_h
#define GyroMeasure_h

#include <PageContentHelper.h>

class GyroMeasure {
    private:
        bool print_debug;
        int16_t gyro_g_x;
        int16_t gyro_g_y;
        int16_t gyro_g_z;
        float g_max = 0;
        float g_counted_last = 0;
        PageContentHelper *pageContentHelper;

    public:
        // Constructor
        GyroMeasure(PageContentHelper *pageContentHelper, bool print_debug);

        // Init gyro sensor
        void init();

        // Get actual measurement and calculate acc max
        float getAccelerationMax();

        // Get the last acc max value measured
        float getLastAccelerationMax();

        // Set last counted g value
        void setGCountedLast(float g_counted_last);

        // Get last counted g value
        float getGCountedLast();

        // Send sensor to sleep mode
        void sensorSleep();

        // Make sensor woke
        void sensorWake();
};

#endif