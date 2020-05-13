#include <Arduino.h>

#ifndef ShotCounterLang_h
#define ShotCounterLang_h

// String not translated error message
const char LangError[] PROGMEM = {"translation error"};

// Include language header
#include "../../include/Lang.h"

class ShotCounterLang {
   private:
    unsigned int eeAddress;
    char lang = 'E';
    // Dynamic char buffer for converting progmem chars to char*
    char* progMemCharBuffer = 0;

    // Get language from EEPROM
    void loadLang() {
        lang = EEPROM.read(eeAddress);
    }

    // Convert PROGMEM char to char*
    char* getProgmemCharAsPointer(const char* str) {
        progMemCharBuffer = (char*)realloc(progMemCharBuffer, strlen_P(str) + 1);
        strcpy_P(progMemCharBuffer, str);
        return progMemCharBuffer;
    }

   public:
    // Constructor
    ShotCounterLang(unsigned int eeAddress);
    // Set language and store lang char in EEPROM
    void setLang(char lang);
    // Get language
    char getLang();
    // Change language
    void switchLang();
    // Get language string by key
    char* getKey(int key);
};

#endif