#include <Arduino.h>
#include <Wire.h>
#include <SingleButton.h>

#ifndef BUTTON_PIN_REG
    #define BUTTON_PIN_REG PINB
#endif
#ifndef BUTTON_PRESS_VAL
    #define BUTTON_PRESS_VAL 0
#endif
#ifndef BUTTON_RELEASE_VAL
    #define BUTTON_RELEASE_VAL 4
#endif

SingleButton::SingleButton(unsigned int _pressed_millis_for_longpress) {
    pressed_millis_for_longpress = _pressed_millis_for_longpress;
}

void SingleButton::loop() {
    // Change page via button press (trigger on release)
    if (BUTTON_PIN_REG == BUTTON_PRESS_VAL && !pressed) {
        onPress();
    }
    // Check for long press
    if (pressed && pressed_start < (millis() - pressed_millis_for_longpress)) {
        onLongPress();
    }
    if (BUTTON_PIN_REG == BUTTON_RELEASE_VAL && pressed) {
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
