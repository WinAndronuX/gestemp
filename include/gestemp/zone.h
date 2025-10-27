#ifndef GESTEMP_ZONE_H
#define GESTEMP_ZONE_H
#include <gestemp/fan.h>

typedef struct {
    int zoneId;
    char zoneName[50];
    float temperatureThreshold;
    float currentTemperature;
    FanStatus fanStatus;
    FanType fanType;
    int fanNum;
} Zone;

//void zoneRegistration();

#endif //GESTEMP_ZONE_H