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

void logEvent(unsigned int id, int status, char*name, float temperature, int mode)
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

        fprintf(arch, "[%04d-%02d-%02d %02d:%02d:%02d] Zona %d \"%s\": %s %s -> Temperatura registrada: %.2f C\n",
        tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
        tm->tm_hour, tm->tm_min, tm->tm_sec,
        id, name,
        (status == 0) ? "APAGADO" : "ENCENDIDO",
        (mode == 0) ? "AUTOMATICO" : "MANUAL", temperature);
    }
    fclose(arch);
}


void clearBuffer() {
    int __bfc__var;
    while ((__bfc__var = getchar()) != '\n' && __bfc__var != EOF);
}
