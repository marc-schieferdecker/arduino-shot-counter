#include <Arduino.h>
#include <Wire.h>
#include <SingleButton.h>

#ifndef BUTTON_PIN_REG
    #define BUTTON_PIN_REG PINB
#endif

SingleButton::SingleButton(unsigned int _pressed_millis_for_longpress, byte _button_press_val, byte _button_release_val) {
    pressed_millis_for_longpress = _pressed_millis_for_longpress;
    button_press_val = _button_press_val;
    button_release_val = _button_release_val;
}

void SingleButton::loop() {
    // Change page via button press (trigger on release)
    if (BUTTON_PIN_REG == button_press_val && !pressed) {
        onPress();
    }
    // Check for long press
    if (pressed && pressed_start < (millis() - pressed_millis_for_longpress)) {
        onLongPress();
    }
    if (BUTTON_PIN_REG == button_release_val && pressed) {
        onRelease();
    }
}

bool SingleButton::shortPressTrigger() {
    return trigger_shortpress;
}

void SingleButton::shortPressTriggerDone() {
    trigger_shortpress = false;
}

bool SingleButton::longPressTrigger() {
    return trigger_longpress;
}

void SingleButton::longPressTriggerDone() {
    trigger_longpress = false;
}
