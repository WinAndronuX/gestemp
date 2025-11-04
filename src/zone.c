#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <gestemp/zone.h>
#include <gestemp/utils.h>
#include <gestemp/tempsensor.h>
#include <listview/listview.h>

#include "gestemp/users.h"

bool zonesLoaded = false;

static float tempMax = 50, tempMin = -10;

static Zone* listZones = NULL;
static int numZones = 0;

static int loadZones() {

    FILE* file = fopen("zones.dat","rb");
    if (file == NULL) {
        return false;
    }

    if (listZones == NULL) {
        free(listZones);
        listZones = NULL;
    }

    listZones = NULL;
    numZones = 0;
    Zone tempZone;

    while (fread(&tempZone, sizeof(Zone), 1, file)) {

        Zone* temp = (Zone*) realloc(listZones, (numZones + 1) * sizeof(Zone));
        if (temp == NULL) {
            fclose(file);
            return false;
        }
        listZones = temp;
        listZones[numZones] = tempZone;
        numZones++;
    }

    fclose(file);
    return true;
}

static int writeZones() {

    FILE* file = fopen("zones.dat","wb");

    if (file == NULL) {
        return false;
    }

    fwrite(listZones, sizeof(Zone), numZones, file);

    fclose(file);

    return true;
}

static unsigned int getZoneId() {
    bool inIn[numZones + 1];
    int i;
    for (i = 0; i < numZones + 1; i++) {
        inIn[i] = false;
    }
    for (i = 0; i < numZones; i++) {
        int id = listZones[i].zoneId;
        if (id > 0 && id <= numZones) {
            inIn[id] = true;
        }
    }
    for (i = 1; i <= numZones; i++) {
        if (inIn[i] == false) {
            return i;
        }
    }
    return numZones + 1;
}

static unsigned int zoneNameVal(char zoneName[16]) {
    int i;
    if (strlen(zoneName) > 15) {
        return true;
    }
    for (i = 0; i < numZones; i++) {
        if (strcmp(listZones[i].zoneName, zoneName) == 0) {
            return true;
        }
    }
    return false;
}

void zoneInit() {

    zonesLoaded = loadZones();

    if (!zonesLoaded) {
        printf("No se ha encontrado ninguna zona, antes de poder realizar algo, ingrese una zona\n");
    }

}

Zone *zoneRegistration() {
    loadZones();

    Zone* registeredZone = malloc(sizeof(Zone));

    do {
        printf("Ingrese el nombre de la zona: \n%c", PROMPT);
        scanf("%s", registeredZone->zoneName);
        clearBuffer();

        if (!zoneNameVal(registeredZone->zoneName)) break;

        printf("Error con el nombre de zona.\n");
    }while (true);

    registeredZone->zoneId = getZoneId();

    do {
        printf("Ingrese el volumen de la zona: \n%c", PROMPT );
        scanf("%f", &registeredZone->zoneVolume);

        if (registeredZone->zoneVolume <= 0) {
            printf("Error. El volumen no puede ser negativo o cero.\n");
        }
    }while (registeredZone->zoneVolume <= 0);


    do {
        printf("Ingrese la temperatura inicial de la zona: \n%c", PROMPT);
        scanf("%f", &registeredZone->currentTemperature);

        if (!(registeredZone->currentTemperature > tempMax || registeredZone->currentTemperature < tempMin)) break;

        printf("Error. Temperatura fuera de rango.\n");
    }while (true);

    do{
        printf("Ingrese el umbral de la zona: \n%c", PROMPT);
        scanf("%f", &registeredZone->temperatureThreshold);

        if (!(registeredZone->temperatureThreshold > tempMax || registeredZone->temperatureThreshold < tempMin)) break;

        printf("Error. Umbral fuera de rango.\n");
    }while (true);

    do {
        printf("Ingrese la cantidad de ventiladores de la zona: \n%c", PROMPT);
        scanf("%i", &registeredZone->fanNum);

        if (!(registeredZone->fanNum <= 0)) break;

        printf("Error. La cantidad de ventiladores no puede ser igual o menor a 0.\n");
    }while (true);
    FanType fanType;

    printf("Ingrese el tipo de ventilador de los siguientes tipos:\n0 ) ");
    fanPrintInfo(IFan);printf("\n1 ) ");
    fanPrintInfo(IFanPlus);printf("\n2 ) ");
    fanPrintInfo(IFanPro);printf("\n%c", PROMPT);
    do {
        scanf("%i", (int*)&fanType);
        switch (fanType) {
            case 0:registeredZone->fanType = IFan;break;
            case 1:registeredZone->fanType = IFanPlus;break;
            case 2:registeredZone->fanType = IFanPro;break;
            default:printf("Tipo de ventilador invalido, ingrese uno correcto e intente de nuevo\n%c", PROMPT);
        }
    }while(fanType != 0 && fanType != 1 && fanType != 2);

    int nPeople, nCellphones, nComputers;

    do {
        printf("Ingrese el numero de personas, telefonos y computadoras en la zona (separados por un espacio): \n%c", PROMPT);
        scanf("%i %i %i", &nPeople, &nCellphones, &nComputers);

        if (!(nPeople < 0 || nCellphones < 0 || nComputers < 0)) break;

        printf("Error. La cantidad no puede ser negativa.\n");
    }while (true);
    registeredZone->internalHeat = (float)((nPeople * 100) + (nCellphones * 4) + (nComputers * 140));

    zonesLoaded = loadZones();

    return registeredZone;
}

void zoneAdd() {
    Zone *registeredZone = zoneRegistration();
    Zone* temp = (Zone*) realloc(listZones, (numZones + 1) *sizeof(Zone));

    temp[numZones].zoneId = registeredZone->zoneId;
    strcpy(temp[numZones].zoneName, registeredZone->zoneName);
    temp[numZones].zoneVolume = registeredZone->zoneVolume;
    temp[numZones].temperatureThreshold = registeredZone->temperatureThreshold;
    temp[numZones].currentTemperature = registeredZone->currentTemperature;
    temp[numZones].internalHeat = registeredZone->internalHeat;
    temp[numZones].fanStatus = registeredZone->fanStatus;
    temp[numZones].fanType = registeredZone->fanType;
    temp[numZones].fanNum = registeredZone->fanNum;

    listZones = temp;

    numZones++;

    writeZones();
}

int zoneRemove() {

    if (actualUser.role != UserRoleOperator && actualUser.role != UserRoleAdmin) {
        printf("Accion no permitida. Permisos insuficientes");
        return false;
    }

    loadZones();

    if (numZones <= 0) {
        printf("Error. No hay zonas por borrar\n");
        return false;
    }

    unsigned int id;
    printf("Ingrese el id de la zona a eliminar:\n%c ", PROMPT);
    scanf("%ud", &id);

    Zone* zone;
    int indexToRemove = -1;

    for (int i = 0; i < numZones; i++) {
        if (listZones[i].zoneId == id) {
            zone = &listZones[i];
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove == -1) {
        printf("Error. Id de zona invalido\n");
        return false;
    }

    for (int i = indexToRemove; i < numZones - 1; i++) {
        listZones[i] = listZones[i + 1];
    }

    numZones--;

    listZones = (Zone*) realloc(listZones, numZones * sizeof(Zone));

    writeZones();

    return true;

}

int zoneModification() {

    if (actualUser.role != UserRoleOperator && actualUser.role != UserRoleAdmin) {
        printf("Accion no permitida. Permisos insuficientes\n");
        return false;
    }

    loadZones();

    if (numZones <= 0) {
        printf("Error. No hay zonas por modificar\n");
        return false;
    }

    unsigned int id;
    printf("Ingrese el id de la zona a modificar:\n%c ", PROMPT);
    scanf("%ud", &id);

    Zone* zone;
    int indexToMod = -1;

    for (int i = 0; i < numZones; i++) {
        if (listZones[i].zoneId == id) {
            zone = &listZones[i];
            indexToMod = i;
            break;
        }
    }

    if (indexToMod == -1) {
        printf("Error. Id de zona invalido\n");
        return false;
    }

    Zone *registeredZone = zoneRegistration();

    listZones[indexToMod] = *registeredZone;

    writeZones();

    return true;

}

void zonePrint() {

    ListView *lv = listviewCreate("Zonas", 9);

    listviewHeadAdd(lv, "Id", 3);
    listviewHeadAdd(lv, "Nombre", 17);
    listviewHeadAdd(lv, "Volumen (m³)", 13);
    listviewHeadAdd(lv, "Temperatura", 12);
    listviewHeadAdd(lv, "Umbral", 8);
    listviewHeadAdd(lv, "Calor (W)", 9);
    listviewHeadAdd(lv, "Estado", 10);
    listviewHeadAdd(lv, "Tipo", 10);
    listviewHeadAdd(lv, "Cantidad", 10);

    int i;
    for (i = 0; i < numZones; i++) {
        char sD[5];
        sprintf(sD, "%d", listZones[i].zoneId);

        listviewAdd(lv, sD);

        listviewAdd(lv, listZones[i].zoneName);

        char sF[20];
        sprintf(sF, "%f", listZones[i].zoneVolume);
        listviewAdd(lv, sF);

        strcpy(sF, "");
        sprintf(sF, "%f", listZones[i].currentTemperature);
        listviewAdd(lv, sF);

        strcpy(sF, "");
        sprintf(sF, "%f", listZones[i].temperatureThreshold);
        listviewAdd(lv, sF);

        strcpy(sF, "");
        sprintf(sF, "%f", listZones[i].internalHeat);
        listviewAdd(lv, sF);

        char status[10];
        switch (listZones[i].fanStatus) {
            case FanOff:strcpy(status, "Apagado");break;
            case FanOn: strcpy(status, "Encendido");break;
        }

        listviewAdd(lv, status);

        char type[10];
        switch (listZones[i].fanType) {
            case IFan:strcpy(type, "IFan");break;
            case IFanPlus:strcpy(type, "IFan Plus");break;
            case IFanPro:strcpy(type, "IFan Pro");break;
        }

        listviewAdd(lv, type);

        strcpy(sD, "");
        sprintf(sD, "%i", listZones[i].fanNum);
        listviewAdd(lv, sD);

    }
    listviewFootPrint(lv);
}

void zoneTempCheck() {
    int i;
    for (i = 0; i < numZones; i++) {
        listZones[i].currentTemperature = tempsensorRead(&listZones[i]);
        if (listZones[i].currentTemperature > listZones[i].temperatureThreshold) {
            listZones[i].fanStatus = FanOn;
        }else {
            listZones[i].fanStatus = FanOff;
        }
    }
    printf("\n");
}