#ifndef DisplayHelper_h
#define DisplayHelper_h

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class DisplayHelper {
    private:
        Adafruit_SSD1306 *display;
        bool display_changed = true;
        byte voltage;
        byte oled_addr;

    public:
        // Constructor
        DisplayHelper(Adafruit_SSD1306 *display, byte voltage, byte oled_addr);
        // Setup
        void setup();
        // Clear display
        void clear();
        // Blink display
        void blink(int waitTime = 100);
        // Set display changed
        void setDisplayChanged(bool changed);
        // Get display changed
        bool getDisplayChanged();
        // Draw a full screen image
        void bitmapFullscreen(const uint8_t bitmap[]);
        // Draw an icon
        void bitmapIcon(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h);
        // Render view
        void render();
};

#endif