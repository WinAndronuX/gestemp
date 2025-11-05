#ifndef GESTEMP_ZONE_H
#define GESTEMP_ZONE_H
#include <gestemp/fan.h>
#include <stdbool.h>

typedef struct {
    unsigned int zoneId;
    char zoneName[16];
    float zoneVolume;
    float temperatureThreshold;
    float currentTemperature;
    float internalHeat;
    FanStatus fanStatus;
    FanType fanType;
    int fanNum;
} Zone;

extern bool zonesLoaded;

static int zoneModValidation(char* string, bool isAdding);

static int zoneSearchId(unsigned int id);

static int loadZones();

static int writeZones();

static unsigned int getZoneId();

static unsigned int zoneNameVal(char zoneName[16]);

void zoneInit();

Zone *zoneRegistration();

void zoneAdd();

int zoneRemove();

int zoneModification();

int zoneThreshold();

void zonePrint();

void zoneTempCheck();

#endif //GESTEMP_ZONE_H