#include <stdio.h>

#include <gestemp/tempctrl.h>
#include <gestemp/zone.h>
#include <gestemp/fan.h>
#include <gestemp/utils.h>
#include <gestemp/menu.h>

#include <string.h>
#include <listview/listview.h>

#define MAX_LOG_LINES 10
#define MAX_LINE_LENGTH 256


void tempShowCurrent()
{
    int numZones;
    Zone* listZones = zoneLoadAll(&numZones);

    char zoneName[16];
    if (listZones == NULL)
    {
        puts("No hay zonas registradas. Registre una zona primero.\n");
        return;
    }


    clearConsole();
    printf("\n\n--- Ver Temperatura Actual ---\n\n");
    clearBuffer();
    printf("Ingrese el nombre de la zona: \n%c ",PROMPT);
    scanf("%[^\n]%*c", zoneName);

    unsigned int idSearch = zoneSearchName(zoneName);
    if (idSearch == -1) {
        printf("Error. No se encontro la zona.\n");
        return;
    }


    clearConsole();
    printf("\n\n--- Ver Temperatura Actual ---\n\n");
    ListView* lv = listviewCreate("Zonas", 4);
    listviewHeadAdd(lv, "ID", 5);
    listviewHeadAdd(lv, "Nombre", 20);
    listviewHeadAdd(lv, "Temp. Actual", 15);
    listviewHeadAdd(lv, "Ventilador", 10);

    char tempStr[16];

    sprintf(tempStr, "%d", listZones[idSearch].zoneId);
    listviewAdd(lv, tempStr);

    listviewAdd(lv, listZones[idSearch].zoneName);

    sprintf(tempStr, "%.2f C", listZones[idSearch].currentTemperature);
    listviewAdd(lv, tempStr);

    listviewAdd(lv, (listZones[idSearch].fanStatus == FanOn) ? "ENCENDIDO" : "APAGADO");

    listviewFootPrint(lv);

    printf("\nPresione Enter para continuar...");
    clearBuffer();
    getchar();
    zoneFree(listZones);
    clearConsole();

}

void tempShowHistory() {

    clearConsole();
    printf("--- 3) Ver Historial de Eventos ---\n\n");
    printf("Seleccione una opcion:\n");
    printf(" 1) Ver el historial completo\n");
    printf(" 2) Ver los ultimos 10 eventos\n");
    printf(" 0) Regresar\n");

    int opc = menuInputOpt(0, 2);

    if (opc == 0) {
        return;
    }

    FILE* file = fopen("historial.log", "r");

    if (file == NULL) {
        printf("El historial esta vacio o no se pudo abrir.\n");
    } else {

        char lineBuffer[MAX_LINE_LENGTH];
        int totalLines = 0;

        if (opc == 1) {

            printf("\n--- Historial Completo ---\n");


            while (fgets(lineBuffer, sizeof(lineBuffer), file) != NULL) {
                printf("%s", lineBuffer);
                totalLines++;
            }
            if (totalLines == 0) {
                printf("El historial esta vacio.\n");
            }

        } else if (opc == 2) {

            printf("\n--- Ultimos 10 Eventos ---\n");


            while (fgets(lineBuffer, sizeof(lineBuffer), file) != NULL) {
                totalLines++;
            }

            if (totalLines == 0) {
                 printf("El historial esta vacio.\n");
            } else {

                rewind(file);


                int linesToSkip = totalLines - 10;
                if (linesToSkip < 0) { 
                    linesToSkip = 0;
                }


                for (int i = 0; i < linesToSkip; i++) {
                    fgets(lineBuffer, sizeof(lineBuffer), file);
                }


                while (fgets(lineBuffer, sizeof(lineBuffer), file) != NULL) {
                    printf("%s", lineBuffer);
                }
            }
        }

        fclose(file);
    }

    printf("\n--- Fin del Historial ---\n");
    printf("Presione Enter para continuar...");
    clearBuffer(); 
    getchar();
    clearConsole();
}

void tempManualControl() {
    int numZones;
    Zone* listZones = zoneLoadAll(&numZones);
    Zone* currentZone = listZones;
    clearConsole();
    char zoneName[16];
    int Id;
    printf("---  Activar Ventilador Manualmente ---\n");
    do {
        printf("Ingrese el nombre de la zona: \n%c ", PROMPT);
        clearBuffer();
        scanf("%[^\n]%*c", zoneName);

        Id = zoneSearchName(zoneName);

        if (Id == -1) {
            printf("\nZona No encontrada \n");
            sleepSec(3);
            clearConsole();
        } else {
            currentZone = listZones + Id;

            printf("\nZona: %s \n%i \n", currentZone->zoneName, currentZone->zoneId);
            printf("Ventilador Estatus: %s\n", (currentZone->fanStatus == FanOn) ? "ENCENDIDO" : "APAGADO");
            printf("\n\n1) Encender\n2) Apagar\n3) Automatico\n0) Salir\n\n");
            int op = menuInputOpt(0,3);

            bool changed = false;
            if (op == 1) {
                currentZone->fanStatus = FanOn;
                currentZone->forced = true;
                changed = true;
                logEvent(currentZone->zoneId, currentZone->fanStatus, currentZone->zoneName, currentZone->currentTemperature, currentZone->forced);
            } else if (op == 2) {
                currentZone->fanStatus = FanOff;
                currentZone->forced = true;
                changed = true;
                logEvent(currentZone->zoneId, currentZone->fanStatus, currentZone->zoneName, currentZone->currentTemperature, currentZone->forced);
            } else if (op == 3) {
                currentZone->forced = false;
                changed = true;
            }

            if (changed)
            {
                printf("\nGuardando Cambios ...\n");
                zoneSaveAll(listZones, numZones);
                sleepSec(2);
            }
            else
            {
                printf("\nSaliendo Sin Cambios ...\n");
                sleepSec(2);
            }
        }
    } while (Id == -1);

    zoneFree(listZones);
    clearConsole();
}

void tempRealtimeMonitor() {
    int numZones = 0;
    Zone* listZones = zoneLoadAll(&numZones);

    clearConsole();
    printf("--- Monitoreo en Tiempo Real  ---\n\n");
    printf("Presione 'Enter' para DETENER el monitoreo");

    ListView* lv = listviewCreate("Estado de Zonas", 4);
    listviewHeadAdd(lv, "ID", 5);
    listviewHeadAdd(lv, "Nombre", 20);
    listviewHeadAdd(lv, "Temp. Actual", 15);
    listviewHeadAdd(lv, "Ventilador", 10);

    char tempStr[16];
    for (int i = 0; i < numZones; i++) {
        sprintf(tempStr, "%d", listZones[i].zoneId);
        listviewAdd(lv, tempStr);
        listviewAdd(lv, listZones[i].zoneName);
        sprintf(tempStr, "%.2f C", listZones[i].currentTemperature);
        listviewAdd(lv, tempStr);
        listviewAdd(lv, (listZones[i].fanStatus == FanOn) ? "ON" : "OFF");
    }

    listviewFootPrint(lv);
    zoneFree(listZones);
}

