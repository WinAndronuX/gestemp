//
// Created by itane on 03/11/2025.
//
//Librerias Estandar
#include <stdio.h>
#include <conio.h>

#include <gestemp/tempctrl.h>
#include <gestemp/zone.h>
#include <gestemp/fan.h>
#include <gestemp/utils.h>
#include <gestemp/menu.h>
#include <listview/listview.h> //Libreria
#include "../include/gestemp/tempctrl.h"

void tempShowCurrent()
{
    clearConsole();
    printf("--- Ver Temperatura Actual ---\n\n");
    int numZones = 0;
    Zone* allZones = NULL; //Faltan las zonas

    if (allZones == NULL || numZones == 0) {
        printf("No hay zonas registradas. Registre una zona primero.\n");
    } else {

        ListView* lv = listviewCreate("Zonas", 4);

        listviewHeadAdd(lv, "ID", 5);
        listviewHeadAdd(lv, "Nombre", 20);
        listviewHeadAdd(lv, "Temp. Actual", 15);
        listviewHeadAdd(lv, "Ventilador", 10);

        char tempStr[16];
        for (int i = 0; i < numZones; i++) {
            sprintf(tempStr, "%d", allZones[i].zoneId);
            listviewAdd(lv, tempStr);
            listviewAdd(lv, allZones[i].zoneName);
            sprintf(tempStr, "%.2f C", allZones[i].currentTemperature);
            listviewAdd(lv, tempStr);
            listviewAdd(lv, (allZones[i].fanStatus == FanOn) ? "ENCENDIDO" : "APAGADO");
        }

        listviewFootPrint(lv);

    }

    printf("\nPresione Enter para continuar...");
    clearBuffer();
    getchar();
    //falta el arreglo con las zonas por eso esta en gris

}

void tempShowHistory()
{
    clearConsole();
    printf("---  Historial de Eventos ---\n");


}

void tempManualControl()
{
    clearConsole();

    printf("---  Activar Ventilador Manualmente ---\n");
    scanf("%d", &zoneId);//revisar depsues

    Zone* zone = NULL;
    if (Zone == NULL)
    {
        printf("\nZona No encontrada \n");
        sleepSec(3);
        clearConsole();
    }else
    {
        printf("\nZona: %c \n%i \n" zone->zoneName, zone->zoneId);
        printf("Ventilador Estatus: %c\n", (zone->fanStatus == FanOn) ? "ENCENDIDO" : "APAGADO");
        int op = menuInputOpt(0,2);

        bool changed = false;
        if (op == 1 && zone->fanStatus == FanOff) {
            zone->fanStatus = FanOn;
            changed = true;
        } else if (op == 2 && zone->fanStatus == FanOn) {
            zone->fanStatus = FanOff;
            changed = true;
        }
        if (changed)
        {
            printf("\nGuardando Cambios ...\n");
            sleepSec(2);
        }else
        {
            printf("\nSaliendo Sin Cambios ...\n");
            sleepSec(2);
        }
    }


}

void tempRealTime()
{
    clearConsole();
    printf("--- Monitoreo en Tiempo Real ---\n");

    while (1)
    {
        int numZones= 0;
        Zone *zonenum = NULL;
        if (allZones == NULL || numZones == 0)
        {
            printf("No hay zonas registradas...\n");
            sleepSec(2);
            break;
        }
        ListView* lv = listviewCreate("Estado de Zonas", 4);

        listviewHeadAdd(lv, "ID", 5);
        listviewHeadAdd(lv, "Nombre", 20);
        listviewHeadAdd(lv, "Temp. Actual", 15);
        listviewHeadAdd(lv, "Ventilador", 10);

        char temp[16];
        for (int i = 0; i < numZones; i++) {

            sprintf(tempStr, "%d", allZones[i].zoneId);
            listviewAdd(lv, tempStr);
            listviewAdd(lv, allZones[i].zoneName);
            sprintf(tempStr, "%.2f C", allZones[i].currentTemperature);
            listviewAdd(lv, tempStr);

            listviewAdd(lv, (allZones[i].fanStatus == FanOn) ? "ON" : "OFF");
        }

        listviewFootPrint(lv);

        for (int i = 0; i < 10; i++)
        {
            if (_kbhit()) {
                goto end_monitor;
            }
        }
    }

    end_monitor:

    clearBuffer();
    printf("\n\nMonitoreo detenido ...\n");
    sleepSec(1);

}

//  Falta zones, las funciones no estan probadas