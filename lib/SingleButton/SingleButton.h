#ifndef SingleButton_h
#define SingleButton_h

class SingleButton {
    private:
        bool pressed = false;
        bool is_longpress = false;
        bool trigger_shortpress = false;
        bool trigger_longpress = false;
        unsigned long pressed_start = 0;
        unsigned int pressed_millis_for_longpress = 1000;
        byte button_pin;
        byte button_press_val;
        byte button_release_val;

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
        SingleButton(unsigned int pressed_millis_for_longpress, byte button_pin, byte button_press_val, byte button_release_val);

        // Method for loop
        void loop();

        bool shortPressTrigger();

        void shortPressTriggerDone();

        bool longPressTrigger();

        void longPressTriggerDone();
};

#endif