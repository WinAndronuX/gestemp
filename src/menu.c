#include <stdio.h>
#include <gestemp/menu.h>
#include <gestemp/utils.h>


const char* menuHeadStart = {
    "+--------------------------------------------+\n"
    "|                  GESTEMP                   |\n"
    "|                                            |\n"
    "|               Menu Principal               |\n"
    "|                                            |\n"
    "| v0.0.1                                     |\n"
    "+--------------------------------------------+\n"
};

const char* menuStart[] = {
    "1 ) Zonas",
    "2 ) Control de temperaturas",
    "3 ) Consultas",
    "4 ) Usuarios",
    "5 ) Configuracion",
    "",
    "0 ) Salir",
    NULL
};

const char* menuHeadZones = {
    "+--------------------------------------------+\n"
    "|                  GESTEMP                   |\n"
    "|                                            |\n"
    "|                   Zonas                    |\n"
    "|                                            |\n"
    "| v0.0.1                                     |\n"
    "+--------------------------------------------+\n"
};

const char* menuZones[] = {
    "1 ) Ver zonas",
    "2 ) Agregar zona",
    "3 ) Borrar zona",
    "4 ) Modificar zona",
    "",
    "0 ) Regresar",
    NULL
};

const char* menuHeadTempCtrl = {
    "+--------------------------------------------+\n"
    "|                  GESTEMP                   |\n"
    "|                                            |\n"
    "|          Control de Temperaturas           |\n"
    "|                                            |\n"
    "| v0.0.1                                     |\n"
    "+--------------------------------------------+\n"
};

const char* menuTempCtrl[] = {
    "1 ) Ver temperatura actual",
    "2 ) Activar ventilador manualmente",
    "3 ) Ver historial de eventos",
    "4 ) Monitoreo en tiempo real",
    "",
    "0 ) Regresar",
    NULL
};

const char* menuHeadQueries = {
    "+--------------------------------------------+\n"
    "|                  GESTEMP                   |\n"
    "|                                            |\n"
    "|                 Consultas                  |\n"
    "|                                            |\n"
    "| v0.0.1                                     |\n"
    "+--------------------------------------------+\n"
};

const char* menuQueries[] = {
    "1 ) Buscar eventos por rango de temperatura o fecha",
    "2 ) Generar reporte estadistico",
    "3 ) Exportar historial a CSV",
    "",
    "0 ) Regresar",
    NULL
};

const char* menuHeadUsers = {
    "+--------------------------------------------+\n"
    "|                  GESTEMP                   |\n"
    "|                                            |\n"
    "|                  Usuarios                  |\n"
    "|                                            |\n"
    "| v0.0.1                                     |\n"
    "+--------------------------------------------+\n"
};

const char* menuUsers[] = {
    "1 ) Ver usuarios",
    "2 ) Agregar usuario",
    "3 ) Borrar usuario",
    "4 ) Cambiar contrasena",
    "",
    "0 ) Regresar",
    NULL
};

const char* menuHeadConfig = {
    "+--------------------------------------------+\n"
    "|                  GESTEMP                   |\n"
    "|                                            |\n"
    "|               Configuracion                |\n"
    "|                                            |\n"
    "| v0.0.1                                     |\n"
    "+--------------------------------------------+\n"
};

const char* menuConfig[] = {
    "1 ) Configurar umbral de temperatura por zona",
    "2 ) Restaurar configuracion por defecto por zona",
    "3 ) Cambiar tema de la aplicacion",
    "",
    "0 ) Regresar",
    NULL
};


void menuPrint(const char* menu[]) {
    printf("\nOpciones:\n");

    int i;
    for (i = 0; menu[i] != NULL; i++)
        printf("\t%s\n", menu[i]);
}

int menuInputOpt(int a, int b) {
    int x = a - 1;

    while (1) {

        clearBuffer();

        printf("%c ", PROMPT);
        scanf("%d", &x);

        if (isInRange(x, a, b)) break;

        printf("Opcion invalida.\n");
    }

    return x;
}