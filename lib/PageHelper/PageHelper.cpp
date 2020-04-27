#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PageHelper.h>

PageHelper::PageHelper(Adafruit_SSD1306 *_display, short _max_main_pages) {
    display = _display;
    max_main_pages = _max_main_pages;
}

void PageHelper::versionPage(char const *version) {
  display -> setCursor(2,0);
  display -> println("Shot Counter");
  display -> setCursor(2,12);
  display -> println(version);
  display -> setCursor(2,24);
  display -> println("letsshootshow.de");
}

short PageHelper::getPageIndex() {
    return page_index;
}

void PageHelper::setPageIndex(short _page_index) {
    page_index = _page_index;
}

void PageHelper::nextPage() {
    page_index = (page_index + 1) >= max_main_pages ? 0 : (page_index + 1);
    page_changed = true;
}


bool PageHelper::getPageChanged() {
    return page_changed;
}

void PageHelper::setPageChanged(bool _page_changed) {
    page_changed = _page_changed;
}
