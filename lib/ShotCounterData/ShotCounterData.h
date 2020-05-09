#ifndef ShotCounterData_h
#define ShotCounterData_h

struct ShotCounterData {
    unsigned long shotsTotal;
    unsigned int shotsSeries;
    float countGforce;
    int shotDelay;
    char profileName[15];
};

#endif