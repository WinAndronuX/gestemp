#include <stdio.h>
#include <gestemp/zone.h>
#include <gestemp/utils.h>
#include <gestemp/menu.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <gestemp/queries.h>
#include <listview/listview.h>

#define MAX_LINE_LENGTH 256

void exportToCSV() {


    char filterType[20] = "";
    char filterDate[11] = "";
    char outputFilename[50] = "reporte.csv";

    while(1) {

        clearConsole();

        printf("--- Exportar Historial a CSV ---\n\n");
        printf("Configuracion\n\n");
        printf("    1 ) Filtro de Tipo    : %s\n", (strlen(filterType) == 0) ? "Todos" : filterType);
        printf("    2 ) Filtro de Fecha   : %s\n", (strlen(filterDate) == 0) ? "Todas" : filterDate);
        printf("    3 ) Nombre de Archivo : %s\n", outputFilename);
        printf("\n");
        printf("    4 ) Exportar\n");
        printf("    0 ) Cancelar \n" );

        int opc = menuInputOpt(0, 4);

        if (opc == 0) return;

        if (opc == 4) break;

        if (opc == 1) {

            printf("\nFiltrar por tipo:\n   1 ) Todos\n    2 ) AUTOMATICO\n    3 ) MANUAL\n");
            int typeOpc = menuInputOpt(1, 3);
            if (typeOpc == 1) strcpy(filterType, "");
            if (typeOpc == 2) strcpy(filterType, "AUTOMATICO");
            if (typeOpc == 3) strcpy(filterType, "MANUAL");
        }
        else if (opc == 2) {

            printf("\nIngrese la fecha (YYYY-MM-DD) o '0' para todas: \n%c ",PROMPT );
            scanf("%10s", filterDate);
            clearBuffer();
            if (strcmp(filterDate, "0") == 0) strcpy(filterDate, "");
        }
        else if (opc == 3) {

             printf("\nNuevo nombre de archivo (ej. reporte.csv): \n%c ",PROMPT );
             scanf("%49s", outputFilename);
             clearBuffer();

            int len = strlen(outputFilename);


            if (len < 4 || strcmp(&outputFilename[len - 4], ".csv") != 0) {
                strcat(outputFilename, ".csv");

            }
        }
    }


    printf("\nExportando ");

    FILE* logFile = fopen("historial.log", "r");
    if (logFile == NULL) {
        printf("ERROR: No se pudo leer historial.log\n");
        sleepSec(2);
        return;
    }

    FILE* csvFile = fopen(outputFilename, "w");
    if (csvFile == NULL) {
        printf("ERROR: No se pudo crear %s\n", outputFilename);
        fclose(logFile);
        sleepSec(2);
        return;
    }


    fprintf(csvFile, "Fecha,Hora,Zona,Mensaje\n");

    char line[MAX_LINE_LENGTH];
    char date[11], time[9], message[100];
    int zoneId, itemsRead;
    int linesExported = 0;


    while (fgets(line, sizeof(line), logFile) != NULL) {


        itemsRead = sscanf(line, "[%10s %8s] Zona %d: %[^\n]", date, time, &zoneId, message);

        if (itemsRead == 4) {


            bool dateMatch = (strlen(filterDate) == 0) || (strcmp(date, filterDate) == 0);
            bool typeMatch = (strlen(filterType) == 0) || (strstr(message, filterType) != NULL);


            if (dateMatch && typeMatch) {
                fprintf(csvFile, "%s,%s,%d,%s\n", date, time, zoneId, message);
                linesExported++;
            }
        }
    }

    fclose(logFile);
    fclose(csvFile);

    printf("Completado %d líneas a %s\n", linesExported, outputFilename);

    //Abrir
    printf("|| Abriendo archivo ||\n");
    char command[100];

    #ifdef _WIN32

        sprintf(command, "start %s", outputFilename);
    #else

        sprintf(command, "xdg-open %s", outputFilename);
    #endif

    system(command);

    sleepSec(3);
}

static int isValidDate(struct tm* date) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    if (date->tm_year < 1970 || date->tm_year > (1900 + t->tm_year)) return false;
    if (date->tm_mon < 1 || date->tm_mon > 12) return false;
    if (date->tm_mday < 1) return false;
    if (date->tm_hour < 0 || date->tm_hour > 23) return false;
    if (date->tm_min < 0 || date->tm_min > 59) return false;
    if (date->tm_sec < 0 || date->tm_sec > 59) return false;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (date->tm_mon == 2 && ((date->tm_year % 4 == 0 && date->tm_year % 100 != 0) || (date->tm_year % 400 == 0))) {
        if (date->tm_mday > 29) {
            return false;
        }
    } else {
        if (date->tm_mday > daysInMonth[date->tm_mon - 1]) {
            return false;
        }
    }

    date->tm_year = date->tm_year - 1900;
    return true;
}

void searchEventByRange() {
    FILE* file = fopen("historial.log", "r");
    if (file == NULL) {
        printf("El historial esta vacio o no se pudo abrir.\n");
        return;
    }

    int numZones;
    Zone* listZones = zoneLoadAll(&numZones);
    if (listZones == NULL) {
        printf("No hay zonas registradas. Registre una zona primero.\n");
        fclose(file);
        return;
    }

    unsigned int searchId;
    char zoneName[16];
    clearBuffer();
    printf("Ingrese el nombre de la zona, escriba 'all' para todas:\n%c ", PROMPT);
    scanf("%[^\n]%*c", zoneName);

    if (strcmp(zoneName, "all") == 0) {
        searchId = 1;
    }else {
        searchId = zoneSearchName(zoneName);
    }

    if (searchId == -1) {
        printf("Error. ID de zona invalido.\n");
        zoneFree(listZones);
        fclose(file);
        return;
    }

    printf("\nSeleccione el tipo de filtro:\n");
    printf("\t1) Rango de fechas\n");
    printf("\t2) Mostrar todos (solo filtrar por zona)\n\n");
    printf("\t0) Salir\n");

    int filterType = menuInputOpt(0, 2);
    clearBuffer();

    if (filterType == 0) {
        zoneFree(listZones);
        fclose(file);
        return;
    }
    struct tm minDate, maxDate;
    time_t startDate, endDate;
    if (filterType == 1) {
        do {
            do {
                printf("Ingrese fecha de inicio con el formato YYYY-MM-DD HH:MM:SS\n%c ", PROMPT);
                scanf("%d-%d-%d %d:%d:%d", &minDate.tm_year, &minDate.tm_mon, &minDate.tm_mday, &minDate.tm_hour, &minDate.tm_min, &minDate.tm_sec);
                clearBuffer();

                if (isValidDate(&minDate)) break;

                printf("Error. Fecha invalida.\n");
            }while (true);

            do {
                printf("Ingrese fecha de fin con el formato YYYY-MM-DD HH:MM:SS\n%c ", PROMPT);
                scanf("%d-%d-%d %d:%d:%d", &maxDate.tm_year, &maxDate.tm_mon, &maxDate.tm_mday, &maxDate.tm_hour, &maxDate.tm_min, &maxDate.tm_sec);
                clearBuffer();

                if (isValidDate(&maxDate)) break;

                printf("Error. Fecha invalida.\n");
            }while (true);

            startDate = mktime(&minDate); endDate = mktime(&maxDate);
            if (!(startDate > endDate)) break;
            printf("Error. Rango de fechas invalidos, intente de nuevo por favor.\n");
        }while (true);
    }

    printf("\nEventos Filtrados\n\n");

    char line[MAX_LINE_LENGTH];
    int eventsFound = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        struct tm tmDate;
        time_t ttDate;
        int zoneId;

        int parsed = sscanf(line, "[%d-%d-%d %d:%d:%d] Zona %d:", &tmDate.tm_year, &tmDate.tm_mon, &tmDate.tm_mday,
                            &tmDate.tm_hour, &tmDate.tm_min, &tmDate.tm_sec, &zoneId);

        tmDate.tm_year -= 1900;
        ttDate = mktime(&tmDate);

        if (parsed < 7) {
            continue;
        }
        bool zoneMatch = (strcmp(zoneName, "all") == 0 || searchId == zoneId);

        bool dateMatch = true;
        if (filterType == 1) {
            dateMatch = (ttDate >= startDate && ttDate <= endDate);
        }

        if (zoneMatch && dateMatch) {
            printf("%s", line);
            eventsFound++;
        }
    }

    fclose(file);
    zoneFree(listZones);

    if (eventsFound == 0) {
        printf("No se encontraron eventos con esos criterios.\n");
    }

}

static int findReportElementByName(ReportElement* list, int n, char* name) {

    int i;
    for(i = 0; i < n; i++)
        if (strcmp(list[0].name, name) == 0) return i;

    return -1;
}

void queriesGenReport() {
    FILE* history = fopen("historial.log", "r");
    if (history == NULL) {
        printf("Error. No se pudo abrir el archivo de historial\n\n");
        return;
    }

    char zoneName[100];
    double zoneTemp;

    ReportElement* list = (ReportElement*) malloc(sizeof(ReportElement));
    int numZ = 0;

    while (1) {
        int x = fscanf(history, "%*[^\"]\"%99[^\"]\"%*[^-]-> Temperatura registrada: %lf C", zoneName, &zoneTemp);

        if (x != 2) {
            if (x == EOF) {
                break;
            }
            fscanf(history, "%*[^\n]\n");
            continue;
        }

        int id = findReportElementByName(list, numZ, zoneName);
        if (id == -1) {
            list = (ReportElement*) realloc(list, sizeof(ReportElement) * (numZ + 1));

            strcpy(list[0].name, zoneName);
            list[numZ].maxTemp = zoneTemp;
            list[numZ].minTemp = zoneTemp;
            list[numZ].avg = zoneTemp;
            list[numZ].num = 1;

            numZ++;
        } else {
            
            if (zoneTemp > list[id].maxTemp)
                list[id].maxTemp = zoneTemp;
            else if (zoneTemp < list[id].minTemp)
                list[id].minTemp = zoneTemp;
            
            list[id].avg += zoneTemp;
            list[id].num += 1;
        }
    }

    ListView* lv = listviewCreate("Reporte de Zonas", 4);

    listviewHeadAdd(lv, "Nombre", 15);
    listviewHeadAddAuto(lv, "Temp. Min.");
    listviewHeadAddAuto(lv, "Temp. Max.");
    listviewHeadAddAuto(lv, "Temp. Prom.");

    float totalAvg = 0, totalMin, totalMax;

    if (numZ > 0) {
        totalMin = list[0].minTemp;
        totalMax = list[0].maxTemp;
    }

    int i;
    for(i = 0; i < numZ; i++) {

        listviewAdd(lv, list[i].name);

        if (list[i].minTemp < totalMin) totalMin = list[i].maxTemp;

        char min[8];
        sprintf(min, "%.2f C", list[i].minTemp);
        listviewAdd(lv, min);

        if (list[i].maxTemp > totalMax) totalMax = list[i].maxTemp;

        char max[8];
        sprintf(max, "%.2f C", list[i].maxTemp);
        listviewAdd(lv, max);

        float avg = list[i].avg / (float) list[i].num;
        totalAvg += avg;

        char avgS[8];
        sprintf(avgS, "%.2f C", avg);
        listviewAdd(lv, avgS);
    }
    
    listviewFootPrint(lv);

    printf("\nTemp. Minima General: %.2f C\n", totalMin);
    printf("Temp. Maxima General: %.2f C\n", totalMin);
    printf("Temp. Promedio General: %.2f C\n", totalAvg / (float) numZ);

    fclose(history);
}
