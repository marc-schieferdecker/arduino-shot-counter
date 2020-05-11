#include <Arduino.h>
#include <EEPROM.h>
#include <ShotCounterLang.h>

ShotCounterLang::ShotCounterLang(unsigned int _eeAddress) {
    // Set EEPROM address and get language from EEPROM
    eeAddress = _eeAddress;
    loadLang();

    // Check if lang is valid, if not apply english as default
    if (lang != 'E' && lang != 'D') {
        setLang('E');
    }
}

char ShotCounterLang::getLang() {
    return lang;
}

void ShotCounterLang::switchLang() {
    setLang(lang == 'D' ? 'E' : 'D');
}

char* ShotCounterLang::getKey(int key) {
    if (key == SCLangWaitingForShots) {
        return lang == 'D' ? getProgmemCharAsPointer(SCLangWaitingForShotsDE) : getProgmemCharAsPointer(SCLangWaitingForShotsEN);
    } else if (key == SCLangTotal) {
        return lang == 'D' ? getProgmemCharAsPointer(SCLangTotalDE) : getProgmemCharAsPointer(SCLangTotalEN);
    } else if (key == SCLangSeries) {
        return lang == 'D' ? getProgmemCharAsPointer(SCLangSeriesDE) : getProgmemCharAsPointer(SCLangSeriesEN);
    } else if (key == SCLangProfileSetup) {
        return lang == 'D' ? getProgmemCharAsPointer(SCLangProfileSetupDE) : getProgmemCharAsPointer(SCLangProfileSetupEN);
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
    } else if (key == SCLangDeviceSetup) {
        return lang == 'D' ? getProgmemCharAsPointer(SCLangDeviceSetupDE) : getProgmemCharAsPointer(SCLangDeviceSetupEN);
    } else if (key == SCLangLanguage) {
        return lang == 'D' ? getProgmemCharAsPointer(SCLangLanguageDE) : getProgmemCharAsPointer(SCLangLanguageEN);
    } else if (key == SCLangChangeLanguage) {
        return lang == 'D' ? getProgmemCharAsPointer(SCLangChangeLanguageDE) : getProgmemCharAsPointer(SCLangChangeLanguageEN);
    }

    return getProgmemCharAsPointer(LangError);
}
