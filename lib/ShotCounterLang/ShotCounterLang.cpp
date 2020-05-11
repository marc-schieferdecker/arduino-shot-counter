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
    if (key == SCLangWaitingForShots) {
        return lang == 'D' ? getProgmemCharAsPointer(SCLangWaitingForShotsDE) : getProgmemCharAsPointer(SCLangWaitingForShotsEN);
    } else if (key == SCLangTotal) {
        return lang == 'D' ? getProgmemCharAsPointer(SCLangTotalDE) : getProgmemCharAsPointer(SCLangTotalEN);
    } else if (key == SCLangSeries) {
        return lang == 'D' ? getProgmemCharAsPointer(SCLangSeriesDE) : getProgmemCharAsPointer(SCLangSeriesEN);
    } else if (key == SCLangSetup) {
        return lang == 'D' ? getProgmemCharAsPointer(SCLangSetupDE) : getProgmemCharAsPointer(SCLangSetupEN);
    } else if (key == SCLangPowerOff) {
        return lang == 'D' ? getProgmemCharAsPointer(SCLangPowerOffDE) : getProgmemCharAsPointer(SCLangPowerOffEN);
    } else if (key == SCLangResetProfile) {
        return lang == 'D' ? getProgmemCharAsPointer(SCLangResetProfileDE) : getProgmemCharAsPointer(SCLangResetProfileEN);
    } else if (key == SCLangMinGToCountShot) {
        return lang == 'D' ? getProgmemCharAsPointer(SCLangMinGToCountShotDE) : getProgmemCharAsPointer(SCLangMinGToCountShotEN);
    } else if (key == SCLangLastMeasured) {
        return lang == 'D' ? getProgmemCharAsPointer(SCLangLastMeasuredDE) : getProgmemCharAsPointer(SCLangLastMeasuredEN);
    } else if (key == SCLangMinimumTimeBetweenShots) {
        return lang == 'D' ? getProgmemCharAsPointer(SCLangMinimumTimeBetweenShotsDE) : getProgmemCharAsPointer(SCLangMinimumTimeBetweenShotsEN);
    } else if (key == SCLangCalibrationHelper) {
        return lang == 'D' ? getProgmemCharAsPointer(SCLangCalibrationHelperDE) : getProgmemCharAsPointer(SCLangCalibrationHelperEN);
    } else if (key == SCLangMaximumValueMeasured) {
        return lang == 'D' ? getProgmemCharAsPointer(SCLangMaximumValueMeasuredDE) : getProgmemCharAsPointer(SCLangMaximumValueMeasuredEN);
    } else if (key == SCLangProfilePrepend) {
        return lang == 'D' ? getProgmemCharAsPointer(SCLangProfilePrependDE) : getProgmemCharAsPointer(SCLangProfilePrependEN);
    }

    return getProgmemCharAsPointer(LangError);
}
