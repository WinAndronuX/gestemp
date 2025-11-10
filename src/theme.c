#include <stdio.h>
#include <string.h>
#include <gestemp/utils.h>
#include <gestemp/menu.h>
#include <gestemp/theme.h>


static int actualColor = CLI_COLOR_DEFAULT;
static char actualColorName[12] = "Por Defecto";

void themeInit() {


    FILE* file = fopen("theme.dat", "rb");
    if (file != NULL) {
    
        Theme theme;

        fread(&theme, sizeof(theme), 1, file);

        actualColor = theme.code;
        strcpy(actualColorName, theme.name);

        fclose(file);
    }
    
    printf("\x1b[%dm", actualColor);
}

char* themeGetName() {
    return actualColorName;
}

void themeChange() {

    printf("\nEscoja un color para el tema:\n\n");
    printf("\t1 ) Rojo\n\t2 ) Verde\n\t3 ) Amarillo\n\t4 ) Azul\n\t5 ) Magenta\n\t6 ) Cyan\n\n\t0 ) Por defecto\n%c ", PROMPT);
    int opc = menuInputOpt(0, 6);

    switch(opc) {
        case 0:
            actualColor = CLI_COLOR_DEFAULT;
            strcpy(actualColorName, "Por defecto");
            break;
        case 1:
            actualColor = CLI_COLOR_RED;
            strcpy(actualColorName, "Rojo");
            break;
        case 2:
            actualColor = CLI_COLOR_GREEN;
            strcpy(actualColorName, "Verde");
            break;
        case 3:
            actualColor = CLI_COLOR_YELLOW;
            strcpy(actualColorName, "Amarillo");
            break;
        case 4:
            actualColor = CLI_COLOR_BLUE;
            strcpy(actualColorName, "Azul");
            break;
        case 5:
            actualColor = CLI_COLOR_MAGENTA;
            strcpy(actualColorName, "Magenta");
            break;
        case 6:
            actualColor = CLI_COLOR_CYAN;
            strcpy(actualColorName, "Cyan");
            break;
    }

    Theme t;
    t.code = actualColor;
    strcpy(t.name, actualColorName);

    FILE *file = fopen("theme.dat", "wb");
    if (file == NULL) {
        printf("Error. No se pudo actualizar el tema de la aplicacion.\n");
        return;
    }
    
    fwrite(&t, sizeof(Theme), 1, file);

    fclose(file);

    printf("\nPor favor cierre la aplicacion y vuelvela a abrir para ver el cambio\n");
}

void themeRestoreCLI() {
    printf("\x1b[0m");
}
