#ifndef PageHelper_h
#define PageHelper_h

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class PageHelper {
    private:
        short page_index = 0;
        short max_main_pages;
        bool page_changed = false;
        Adafruit_SSD1306 *display;

    public:
        // Constructor
        PageHelper(Adafruit_SSD1306 *display, short max_main_pages);
        // Show info page with version
        void versionPage(char const *version);
        // Get page index
        short getPageIndex();
        // Set page index
        void setPageIndex(short page_index);
        // Switch to next page
        void nextPage();
        // Get page changed
        bool getPageChanged();
        // Set page changed
        void setPageChanged(bool page_changed);
};

#endif