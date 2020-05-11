#ifndef Lang_h
#define Lang_h

/**
 * Hex table for german umlauts (Codepage 850)
 * ä  ö  ü  ß  Ä  Ö  Ü
 * 84 94 81 E1 8E 99 9A
 */

#define SCLangWaitingForShots 0
const char SCLangWaitingForShotsEN[] PROGMEM = {"  Waiting for shots"};
const char SCLangWaitingForShotsDE[] PROGMEM = {"  Warte auf Sch\x81sse"};
#define SCLangTotal 1
const char SCLangTotalEN[] PROGMEM = {"Total      "};
const char SCLangTotalDE[] PROGMEM = {"Gesamt     "};
#define SCLangSeries 2
const char SCLangSeriesEN[] PROGMEM = {"Series     "};
const char SCLangSeriesDE[] PROGMEM = {"Serie      "};
#define SCLangSetup 3
const char SCLangSetupEN[] PROGMEM = {"Setup\n\nTo enter setup\nhold button"};
const char SCLangSetupDE[] PROGMEM = {"Einstellungen\n\nTaster halten\nzum \x94\x66\x66nen"};
#define SCLangPowerOff 4
const char SCLangPowerOffEN[] PROGMEM = {"Power off\n\nTo turn off device\nhold button"};
const char SCLangPowerOffDE[] PROGMEM = {"Ausschalten\n\nZum Ausschalten\nTaster halten"};
#define SCLangResetProfile 5
const char SCLangResetProfileEN[] PROGMEM = {"Reset profile?\n\nTo reset this profile\nhold button"};
const char SCLangResetProfileDE[] PROGMEM = {"Profil zur\x81\x63ksetzen?\n\nZum Zur\x81\x63ksetzen\nTaster halten"};
#define SCLangMinGToCountShot 6
const char SCLangMinGToCountShotEN[] PROGMEM = {"Minimum G force\nto count a shot:\n"};
const char SCLangMinGToCountShotDE[] PROGMEM = {"Minimale G Kraft\num Schuss zu z\x84hlen:\n"};
#define SCLangLastMeasured 7
const char SCLangLastMeasuredEN[] PROGMEM = {"Last measured: "};
const char SCLangLastMeasuredDE[] PROGMEM = {"Letzter Wert: "};
#define SCLangMinimumTimeBetweenShots 8
const char SCLangMinimumTimeBetweenShotsEN[] PROGMEM = {"Minimum time\nbetween shots:\n\n"};
const char SCLangMinimumTimeBetweenShotsDE[] PROGMEM = {"Minimale Zeit\nzwischen Sch\x81ssen:\n\n"};
#define SCLangCalibrationHelper 9
const char SCLangCalibrationHelperEN[] PROGMEM = {"Calibration helper\n\n"};
const char SCLangCalibrationHelperDE[] PROGMEM = {"Kalibrierhilfe\n\n"};
#define SCLangMaximumValueMeasured 10
const char SCLangMaximumValueMeasuredEN[] PROGMEM = {"Maximum value\nmeasured: "};
const char SCLangMaximumValueMeasuredDE[] PROGMEM = {"Gemessener\nMaximalwert: "};
#define SCLangProfilePrepend 11
const char SCLangProfilePrependEN[] PROGMEM = {"Gun "};
const char SCLangProfilePrependDE[] PROGMEM = {"Waffe "};

#endif