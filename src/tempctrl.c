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

#import "utils.c"
#import "zone.c"


void tempShowCurrent()
{
    clearConsole();
    printf("\n\n--- Ver Temperatura Actual ---\n\n");
    int idSearch;
    if (listZones == NULL)
    {
        puts("No hay zonas registradas. Registre una zona primero.\n");
        return;
    }

    do
    {
        puts("      0)Salir\n");
        puts("Ingrese id: ");
        scanf("%i",&idSearch);

        if (idSearch == 0)
            return;

        idSearch = zoneSearchId(idSearch);
    }while(idSearch != -1);

    clearConsole();
    printf("\n\n--- Ver Temperatura Actual ---\n\n");
    ListView* lv = listviewCreate("Zonas", 4);

    listviewHeadAdd(lv, "ID", 5);
    listviewHeadAdd(lv, "Nombre", 20);
    listviewHeadAdd(lv, "Temp. Actual", 15);
    listviewHeadAdd(lv, "Ventilador", 10);

    char tempStr[16];
    sprintf(tempStr, "%d", listZones[idSearch].zoneId);
    listviewAdd(lv, listZones[idSearch].zoneName);
    sprintf(tempStr, "%.2f C", listZones[idSearch].currentTemperature);
    listviewAdd(lv, (listZones[idSearch].fanStatus == FanOn) ? "ENCENDIDO" : "APAGADO");


    listviewFootPrint(lv);

    printf("\nPresione Enter para continuar...");
    clearBuffer();
    getchar();


}

void tempShowHistory()
{
    clearConsole();
    FILE *arch = fopen("historial.log", "r");
    int lineCount = 0;

    if (arch  == NULL)
    {
        printf("\n | Error al acceder al historial |\n");
        return;
    }else
    {

        printf("\n\n---  Historial de Eventos ---\n");

        char lineBuffer[256];
        while (fgets(lineBuffer, sizeof(lineBuffer), arch) != NULL) {
            printf("%s", lineBuffer);
            lineCount++;
        }

        if (lineCount == 0) {
            printf("\n| El historial esta vacio |\n");
        }

        fclose(arch);
    }
    printf("\nPresione Enter para continuar...");
    clearBuffer();
    getchar();


}

void tempManualControl()
{
    clearConsole();
    int Id;
    printf("---  Activar Ventilador Manualmente ---\n");
    scanf("%d", &Id);

    Id = zoneSearchId(Id);

    if (Id == NULL)
    {
        printf("\nZona No encontrada \n");
        sleepSec(3);
        clearConsole();
    }else
    {
        printf("\nZona: %s \n%i \n", listZones[Id].zoneName, listZones[Id].zoneId);
        printf("Ventilador Estatus: %s\n", (listZones[Id].fanStatus == FanOn) ? "ENCENDIDO" : "APAGADO");
        int op = menuInputOpt(0,2);

        bool changed = false;
        if (op == 1 && listZones[Id].fanStatus == FanOff) {
            listZones[Id].fanStatus = FanOn;
            changed = true;
        } else if (op == 2 && listZones[Id].fanStatus == FanOn) {
            listZones[Id].fanStatus = FanOff;
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
    return;

}

void tempRealTime()
{
    clearConsole();
    printf("--- Monitoreo en Tiempo Real ---\n");

    while (1)
    {
        int numZones= 0;
        Zone *zonenum = NULL;
        if (listZones == NULL || numZones == 0)
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