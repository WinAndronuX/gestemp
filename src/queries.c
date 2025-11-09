#include <stdio.h>


#include <gestemp/zone.h>
#include <gestemp/utils.h>
#include <gestemp/menu.h>

#include <string.h>

#include <gestemp/queries.h>
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