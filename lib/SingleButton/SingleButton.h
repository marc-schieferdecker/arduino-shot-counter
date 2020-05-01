#ifndef SingleButton_h
#define SingleButton_h

class SingleButton {
   private:
    bool pressed = false;
    bool isLongpress = false;
    bool triggerShortpress = false;
    bool triggerLongpress = false;
    unsigned long pressedStart = 0;
    unsigned int pressedMillisForLongpress = 1000;
    volatile uint8_t *buttonPin;
    byte bitmaskForCheck;

    void onPress() {
        this->pressed = true;
        this->pressedStart = millis();
    }

    void onLongPress() {
        this->pressedStart = millis();
        this->isLongpress = true;
        this->triggerLongpress = true;
    }

    void onRelease() {
        this->pressed = false;
        // Short press: Change page
        if (!this->isLongpress) {
            this->triggerShortpress = true;
        } else {
            this->isLongpress = false;
        }
    }

   public:
    // Constructor
    SingleButton(unsigned int pressedMillisForLongpress, volatile uint8_t *buttonPin, byte bitmaskForCheck);

    // Method for loop
    void loop();
    // Activate short press trigger
    bool shortPressTrigger();
    // Deactivate short press trigger
    void shortPressTriggerDone();
    // Activate long press trigger
    bool longPressTrigger();
    // Deactivate long press trigger
    void longPressTriggerDone();
};

#endif