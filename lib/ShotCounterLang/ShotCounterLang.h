#include <Arduino.h>

#ifndef ShotCounterLang_h
#define ShotCounterLang_h

// String not translated error message
const char LangError[] PROGMEM = {"translation error"};

#define waitingForShots 0
const char waitingForShotsEN[] PROGMEM = {"  Waiting for shots"};
const char waitingForShotsDE[] PROGMEM = {"  Warte auf Schuesse"};

class ShotCounterLang {
   private:
    unsigned int eeAddress;
    char lang = 'E';

    // Convert PROGMEM char to char*
    char* getProgmemCharAsPointer(const char* str) {
        char* a = 0;
        a = (char*)realloc(a, strlen_P(str) + 1);
        strcpy_P(a, str);
        return a;
    }

   public:
    // Constructor
    ShotCounterLang(unsigned int eeAddress);
    // Set language and store lang char in EEPROM
    void setLang(char lang);
    // Get language from EEPROM
    void getLang();
    // Get language string by key
    char* getKey(int key);
};

#endif