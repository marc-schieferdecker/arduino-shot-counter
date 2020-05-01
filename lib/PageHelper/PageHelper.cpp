#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <PageHelper.h>

PageHelper::PageHelper(Adafruit_SSD1306 *_display, short _maxMainPages) {
    display = _display;
    maxMainPages = _maxMainPages;
}

short PageHelper::getPageIndex() {
    return pageIndex;
}

void PageHelper::setPageIndex(short _pageIndex) {
    pageChanged = pageIndex != _pageIndex;
    pageIndex = _pageIndex;
}

void PageHelper::nextPage() {
    pageIndex = (pageIndex + 1) >= maxMainPages ? 0 : (pageIndex + 1);
    pageChanged = true;
}

bool PageHelper::getPageChanged() {
    return pageChanged;
}

void PageHelper::setPageChanged(bool _pageChanged) {
    pageChanged = _pageChanged;
}

short PageHelper::getMaxMainPages() {
    return maxMainPages;
}
