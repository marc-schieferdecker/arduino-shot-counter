#include <Arduino.h>
#include <EEPROM.h>
#include <ShotCounterLang.h>

ShotCounterLang::ShotCounterLang(unsigned int _eeAddress) {
    // Set EEPROM address and get language from EEPROM
    eeAddress = _eeAddress;
    getLang();

    // Check if lang is valid, if not apply english as default
    if (lang != 'E' && lang != 'D') {
        setLang('E');
    }
}

void ShotCounterLang::setLang(char _lang) {
    EEPROM.update(eeAddress, _lang);
    getLang();
}

void ShotCounterLang::getLang() {
    lang = EEPROM.read(eeAddress);
}

char* ShotCounterLang::getKey(int key) {
    if (key == waitingForShots) {
        switch (lang) {
            case 'E':
                return getProgmemCharAsPointer(waitingForShotsEN);
            case 'D':
                return getProgmemCharAsPointer(waitingForShotsDE);
            default:
                return getProgmemCharAsPointer(waitingForShotsEN);
        }
    }
    return getProgmemCharAsPointer(LangError);
}
