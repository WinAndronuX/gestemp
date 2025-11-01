#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <gestemp/utils.h>
#include <gestemp/tempsensor.h>
#include <gestemp/users.h>
#include <gestemp/menu.h>

bool programFinished = false;

void *executeEverySecond() {

    while (!programFinished) {

//        printf("Ejecutando..\n");

        sleepSec(1);
    }

    return NULL;
}

void menu() {

    int opc = -1;

START:
    clearConsole();
    printf("%s\n", menuHeadStart);
    menuPrint(menuStart);
    opc = menuInputOpt(0, 5);

    switch (opc) {
    case 0: {
        programFinished = true;
        printf("Saliendo...\n");
        return;
    }
    case 1: goto ZONES;     break;
    case 2: goto TEMP;      break;
    case 3: goto CONSULT;   break;
    case 4: goto USERS;     break;
    case 5: goto CONFIG;    break;
    }

    goto START;

ZONES:
    clearConsole();
    printf("%s\n", menuHeadZones);

    while (1) {
        menuPrint(menuZones);
        opc = menuInputOpt(0, 1);

        switch (opc) {
        case 0:
            goto START;
            break;
        case 1:

            break;
        }
    }

TEMP:
    clearConsole();
    printf("%s\n", menuHeadTempCtrl);

    while (1) {
        menuPrint(menuTempCtrl);
        opc = menuInputOpt(0, 4);

        switch (opc) {
        case 0:
            goto START;
            break;
        case 1:

            break;
        case 2:

            break;
        case 3:

            break;
        case 4:

            break;
        }
    }

CONSULT:
    clearConsole();
    printf("%s\n", menuHeadQueries);

    while (1) {
        menuPrint(menuQueries);
        opc = menuInputOpt(0, 3);

        switch (opc) {
        case 0:
            goto START;
            break;
        case 1:

            break;
        case 2:

            break;
        case 3:

            break;
        }
    }

USERS:
    clearConsole();
    printf("%s\n", menuHeadUsers);

    while (1) {
        menuPrint(menuUsers);
        opc = menuInputOpt(0, 4);

        switch (opc) {
        case 0:
            goto START;
            break;
        case 1:

            break;
        case 2:

            break;
        case 3:

            break;
        case 4:

            break;
        }
    }

CONFIG:
    clearConsole();
    printf("%s\n", menuHeadConfig);

    while (1) {
        menuPrint(menuConfig);
        opc = menuInputOpt(0, 2);

        switch (opc) {
        case 0:
            goto START;
            break;
        case 1:

            break;
        case 2:

            break;
        }
    }
}

int main() {

    tempsensorInit();
    usersInit();

    clearConsole();
    pthread_t thread;
    const int error = pthread_create(&thread, NULL, executeEverySecond, NULL);
    if (error) {
        printf("Error al crear el hilo de ejecucion.\nSaliendo...\n");
        return 1;
    }

    if (!usersLogin()) {
        printf("Credenciales incorrectas\n");
        programFinished = true;

        pthread_join(thread, NULL);
        return 1;
    }

    menu();

    pthread_join(thread, NULL);

    return 0;
}