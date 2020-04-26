#include <Arduino.h>
#include <Wire.h>

#define BUTTON_PIN_REG PINB
#define BUTTON_PRESS_VAL 0
#define BUTTON_RELEASE_VAL 4

class SingleButton {
    private:
        bool pressed = false;
        bool is_longpress = false;
        bool trigger_shortpress = false;
        bool trigger_longpress = false;
        unsigned long pressed_start = 0;
        unsigned int pressed_millis_for_longpress = 1000;

        void onPress() {
            this -> pressed = true;
            this -> pressed_start = millis();
        }

        void onLongPress() {
            this -> pressed_start = millis();
            this -> is_longpress = true;
            this -> trigger_longpress = true;
        }

        void onRelease() {
            this -> pressed = false;
            // Short press: Change page
            if (!this -> is_longpress) {
                this -> trigger_shortpress = true;
            }
            else {
                this -> is_longpress = false;
            }
        }

    public:
        // Constructor
        SingleButton(unsigned int pressed_millis_for_longpress) {
            this -> pressed_millis_for_longpress = pressed_millis_for_longpress;
        }

        // Method for loop
        void loop() {
            // Change page via button press (trigger on release)
            if (BUTTON_PIN_REG == BUTTON_PRESS_VAL && !this -> pressed) {
                this -> onPress();
            }
            // Check for long press
            if (this -> pressed && this -> pressed_start < (millis() - this -> pressed_millis_for_longpress)) {
                this -> onLongPress();
            }
            if (BUTTON_PIN_REG == BUTTON_RELEASE_VAL && this -> pressed) {
                this -> onRelease();
            }
        }

        bool shortPressTrigger() {
            return this -> trigger_shortpress;
        }

        void shortPressTriggerDone() {
            this -> trigger_shortpress = false;
        }

        bool longPressTrigger() {
            return this -> trigger_longpress;
        }

        void longPressTriggerDone() {
            this -> trigger_longpress = false;
        }
};
