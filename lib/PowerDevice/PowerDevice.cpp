#include <Arduino.h>
#include <PowerDevice.h>

PowerDevice::PowerDevice(volatile uint8_t *_powerPin, byte _bitmaskOn, byte _bitmaskOff) {
    powerPin = _powerPin;
    bitmaskOn = _bitmaskOn;
    bitmaskOff = _bitmaskOff;
}

void PowerDevice::keepDeviceOn() {
    on();
}

void PowerDevice::turnDeviceOff() {
    off();
}
