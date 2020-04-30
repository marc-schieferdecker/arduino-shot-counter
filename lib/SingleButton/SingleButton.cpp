#include <Arduino.h>
#include <SingleButton.h>
#include <Wire.h>

SingleButton::SingleButton(unsigned int _pressedMillisForLongpress, volatile uint8_t *_buttonPin, byte _bitmaskForCheck) {
    pressedMillisForLongpress = _pressedMillisForLongpress;
    buttonPin = _buttonPin;
    bitmaskForCheck = _bitmaskForCheck;
}

void SingleButton::loop() {
    // Change page via button press (trigger on release)
    if (!(*buttonPin & bitmaskForCheck) && !pressed) {
        onPress();
    }
    // Check for long press
    if (pressed && pressedStart < (millis() - pressedMillisForLongpress)) {
        onLongPress();
    }
    if ((*buttonPin & bitmaskForCheck) && pressed) {
        onRelease();
    }
}

bool SingleButton::shortPressTrigger() {
    return triggerShortpress;
}

void SingleButton::shortPressTriggerDone() {
    triggerShortpress = false;
}

bool SingleButton::longPressTrigger() {
    return triggerLongpress;
}

void SingleButton::longPressTriggerDone() {
    triggerLongpress = false;
}
