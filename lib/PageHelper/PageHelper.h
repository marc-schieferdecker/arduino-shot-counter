#ifndef PageHelper_h
#define PageHelper_h

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

class PageHelper {
   private:
    byte pageIndex = 0;
    byte maxMainPages;
    bool pageChanged = false;
    Adafruit_SSD1306 *display;

   public:
    // Constructor
    PageHelper(Adafruit_SSD1306 *display, byte maxMainPages);
    // Get page index
    byte getPageIndex();
    // Set page index
    void setPageIndex(byte pageIndex);
    // Switch to next page
    void nextPage();
    // Get page changed
    bool getPageChanged();
    // Set page changed
    void setPageChanged(bool pageChanged);
    // Get max main pages
    byte getMaxMainPages();
};

#endif