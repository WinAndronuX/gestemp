#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gestemp/fan.h>
#include <gestemp/tempsensor.h>
#include <gestemp/zone.h>


static float rho = 1.2;
static float cp = 1005;
static float tOut = 27;
static float tempMax = 38;
static float tempMin = 20;

static void loadConfig() {

    FILE *cfg = fopen("config.ini", "r");
    if (cfg == NULL) {
        printf("Error al leer el archivo de configuracion: config.ini\nUsando valores por defecto\n");
        return;
    }

    char line[256];

    while (fgets(line, sizeof(line), cfg) != NULL) {

        char *arg = strtok(line, "\t=");

        if (arg == NULL || arg[0] == '#' || arg[0] == ';') {
            continue;
        }

        char *sVal = strtok(NULL, "\t=\n\r");
        if (sVal == NULL) {
            printf("config.ini: Error en la linea de '%s'\n", arg);
            continue;
        }

        float value = atof(sVal);
        if (strcmp(arg, "AIR_DENSITY") == 0)
            rho = value;
        else if(strcmp(arg, "AIR_HEAT_CAPACITY") == 0)
            cp = value;
        else if(strcmp(arg, "EXTERN_TEMP") == 0)
            tOut = value;
        else if(strcmp(arg, "MAX_TEMP") == 0)
            tempMax = value;
        else if(strcmp(arg, "MIN_TEMP") == 0)
            tempMin = value;
    }

    fclose(cfg);
}

void tempsensorInit() {

    srand(time(NULL));

    loadConfig();
}

float tempsensorRead(Zone *z) {

    float newT;

    float qInt = z->internalHeat + (rand() % 21) + (-10);

    if (z->fanStatus == FanOn) {

        float qAir = fanGetAirflowRate(z->fanType) * z->fanNum;
        newT = z->currentTemperature  + 1 * ( (qAir / z->zoneVolume) * (tOut - z->currentTemperature) + qInt / (rho *cp *z->zoneVolume) );
    } else {
        newT = z->currentTemperature + 1 * (qInt / (rho * cp * z->zoneVolume));
    }

    if (newT > tempMax)
        newT = tempMax;
    if (newT < tempMin)
        newT = tempMin;

    return newT;
}
