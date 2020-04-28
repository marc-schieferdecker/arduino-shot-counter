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
    byte buttonPressVal;
    byte buttonReleaseVal;

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
    SingleButton(unsigned int pressedMillisForLongpress, byte buttonPressVal, byte buttonReleaseVal);

    // Method for loop
    void loop();

    bool shortPressTrigger();

    void shortPressTriggerDone();

    bool longPressTrigger();

    void longPressTriggerDone();
};

#endif