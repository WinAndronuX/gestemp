#include <stdio.h>
#include <gestemp/fan.h>


typedef struct {
    char* brand;
    char* model;
    float airflow;
} Fan;

static Fan list[] = { {"Abanic", "IFan", 0.04},  {"Abanic", "IFan Plus", 0.12},  {"Abanic", "IFan Pro", 0.25} };


void fanPrintInfo(FanType t) {
    printf("%s %s | Airflow rate: %.2f m³/s", t[list].brand, t[list].model, t[list].airflow);
}

float fanGetAirflowRate(FanType t) {
    return t[list].airflow;
}
