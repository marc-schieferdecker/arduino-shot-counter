#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <DisplayHelper.h>

DisplayHelper::DisplayHelper(Adafruit_SSD1306 *_display, byte _voltage, byte _oledAddr) {
    display = _display;
    voltage = _voltage;
    oledAddr = _oledAddr;
}

void DisplayHelper::setup() {
    display->begin(voltage, oledAddr);
    display->clearDisplay();
    display->setTextColor(WHITE);
    display->setTextSize(1);
    display->setCursor(0, 0);
}

void DisplayHelper::clear() {
    display->clearDisplay();
    display->setCursor(0, 0);
}

void DisplayHelper::blink(int waitTime) {
    display->invertDisplay(true);
    delay(waitTime);
    display->invertDisplay(false);
}

void DisplayHelper::setDisplayChanged(bool changed) {
    displayChanged = changed;
}

bool DisplayHelper::getDisplayChanged() {
    return displayChanged;
}

void DisplayHelper::bitmapFullscreen(const uint8_t bitmap[]) {
    display->drawBitmap(0, 0, bitmap, 128, 32, WHITE);
    display->display();
}

void DisplayHelper::bitmapIcon(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h) {
    display->drawBitmap(x, y, bitmap, w, h, WHITE);
}

void DisplayHelper::render() {
    display->display();
}