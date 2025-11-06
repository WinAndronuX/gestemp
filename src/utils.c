#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <gestemp/utils.h>

void fileValidation(FILE **pF) {
    if (*pF == NULL) {
        printf("El archivo no se puede acceder\n");
        exit(1);
    }
}

bool isInRange(int x, int a, int b) {
    return x >= a && x <= b;
}

void logEvent(unsigned int id, int status, int mode)
{
    FILE *arch = fopen("historial.log", "a");

    if (arch  == NULL)
    {
        printf("\n | Error al guardar cambios en el historial |\n");
        return;
    }else
    {
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);

        fprintf(arch, "%02d:%02d %i %s %s\n", tm->tm_hour, tm->tm_min, id,
            (status==0) ? "OFF" : "ON", (mode == 0) ? "MANUAL" : "AUTOMATICO");
    }
    fclose(arch);
}
//status 0:OFF     1:ON
//mode 0:manual    1:automatico