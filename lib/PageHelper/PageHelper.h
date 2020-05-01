#ifndef PageHelper_h
#define PageHelper_h

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

class PageHelper {
   private:
    short pageIndex = 0;
    short maxMainPages;
    bool pageChanged = false;
    Adafruit_SSD1306 *display;

   public:
    // Constructor
    PageHelper(Adafruit_SSD1306 *display, short maxMainPages);
    // Get page index
    short getPageIndex();
    // Set page index
    void setPageIndex(short pageIndex);
    // Switch to next page
    void nextPage();
    // Get page changed
    bool getPageChanged();
    // Set page changed
    void setPageChanged(bool pageChanged);
    // Get max main pages
    short getMaxMainPages();
};

#endif