#ifndef FAN_H
#define FAN_H

typedef enum {
    IFan = 0,
    IFanPlus = 1,
    IFanPro = 2
} FanType;

typedef enum {
    FanOff = 0,
    FanOn = 1
} FanStatus;

void fanPrintInfo(FanType t);
float fanGetAirflowRate(FanType t);

#endif