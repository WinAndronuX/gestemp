#ifndef GESTEMP_ZONE_H
#define GESTEMP_ZONE_H
#include <gestemp/fan.h>
#include <stdbool.h>

typedef struct {
    unsigned int zoneId;
    char zoneName[16];
    float zoneVolume;
    float temperatureThreshold;
    float defaultTemperatureThreshold;
    float currentTemperature;
    float internalHeat;
    bool forced;
    FanStatus fanStatus;
    FanType fanType;
    int fanNum;
} Zone;

extern bool zonesLoaded;

extern int nZones;

static int zoneModValidation(char* string, bool isAdding);

int zoneSearchName(char* name);

int loadZones();

static unsigned int getZoneId();

static unsigned int zoneNameVal(char zoneName[16]);

int writeZones();

void zoneInit();

Zone *zoneRegistration();

void zoneAdd();

int zoneRemove();

int zoneModification();

int zoneThresholdModification();

int zoneDefault();

void zonePrint();

void zoneTempCheck();

void zoneFree(Zone* zones);

void zoneLog();

void zoneSaveAll(Zone* listZones, int numZones);

Zone* zoneLoadAll(int* numZones);

#endif //GESTEMP_ZONE_H