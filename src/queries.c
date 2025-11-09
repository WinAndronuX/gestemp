#include <gestemp/queries.h>
#include <gestemp/utils.h>
#include <gestemp/menu.h>
#include <gestemp/zone.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

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
