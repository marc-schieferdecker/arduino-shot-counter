#include <Arduino.h>
#include <SingleButton.h>
#include <Wire.h>

#ifndef BUTTON_PIN_REG
#define BUTTON_PIN_REG PINB
#endif

SingleButton::SingleButton(unsigned int _pressedMillisForLongpress, byte _buttonPressVal, byte _buttonReleaseVal) {
    pressedMillisForLongpress = _pressedMillisForLongpress;
    buttonPressVal = _buttonPressVal;
    buttonReleaseVal = _buttonReleaseVal;
}

void SingleButton::loop() {
    // Change page via button press (trigger on release)
    if (BUTTON_PIN_REG == buttonPressVal && !pressed) {
        onPress();
    }
    // Check for long press
    if (pressed && pressedStart < (millis() - pressedMillisForLongpress)) {
        onLongPress();
    }
    if (BUTTON_PIN_REG == buttonReleaseVal && pressed) {
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
