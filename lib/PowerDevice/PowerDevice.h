#ifndef PowerDevice_h
#define PowerDevice_h

class PowerDevice {
   private:
    volatile uint8_t *powerPin;
    byte bitmaskOn;
    byte bitmaskOff;

    void on() {
        *powerPin = *powerPin | bitmaskOn;
    }

    void off() {
        *powerPin = *powerPin & bitmaskOn;
    }

   public:
    // Constructor
    PowerDevice(volatile uint8_t *powerPin, byte bitmaskOn, byte bitmaskOff);

    // Power on
    void keepDeviceOn();
    // Power off
    void turnDeviceOff();
};

#endif