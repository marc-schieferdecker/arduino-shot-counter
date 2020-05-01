#ifndef GyroMeasure_h
#define GyroMeasure_h

#include <PageContentHelper.h>

class GyroMeasure {
   private:
    int gyroAddress;
    int gyroAccRegisterStart;
    bool printDebug;
    int16_t gyroGX;
    int16_t gyroGY;
    int16_t gyroGZ;
    float gMax = 0;
    float gMaxCountedLast = 0;
    PageContentHelper *pageContentHelper;

   public:
    // Constructor
    GyroMeasure(int gyroAddress, int gyroAccRegisterStart, PageContentHelper *pageContentHelper, bool printDebug);
    // Init gyro sensor
    void setup();
    // Get actual measurement and calculate acc max
    float getAccelerationMax();
    // Get the last acc max value measured
    float getLastAccelerationMax();
    // Set last counted g value
    void setGCountedLast(float gMaxCountedLast);
    // Get last counted g value
    float getGCountedLast();
    // Reset sensor to default settings
    void setupResetSensor();
    // Setup sensor sensitivity
    void setupGyroSensitivity();
    // Setup acceleration sensor sensitivity
    void setupAccelerationSensitivity();
    // Send sensor to sleep mode
    void sensorSleep();
    // Make sensor woke
    void sensorWake();
};

#endif