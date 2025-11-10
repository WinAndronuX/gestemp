#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <gestemp/utils.h>
#include <gestemp/tempsensor.h>
#include <gestemp/users.h>
#include <gestemp/menu.h>
#include <gestemp/tempctrl.h>
#include <gestemp/queries.h>

bool programFinished = false, tempMonitor = false;

void *executeEverySecond() {

    while (!programFinished) {

        zoneTempCheck();
        writeZones();
        if (tempMonitor) tempRealtimeMonitor();
        

        sleepSec(5);
    }

    writeZones();

    return NULL;
}

void *executeEvery5Minutes() {

    while (!programFinished) {

        // Aqui va la llamada al log de eventos

        sleepSec(120);
    }

    return NULL;
}

void menu() {

    int opc = -1;

START:
    clearConsole();
    printf("%s", menuHeadStart);
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
    printf("%s", menuHeadZones);

    while (1) {
        menuPrint(menuZones);
        opc = menuInputOpt(0, 4);

        switch (opc) {
        case 0:
            goto START;
            break;
        case 1:
            zonePrint();
            break;
        case 2:
            zoneAdd();
            break;
        case 3:
            zoneRemove();
            break;
        case 4:
            zoneModification();
            break;
        }
    }

TEMP:


    while (1) {
        clearConsole();
        printf("%s", menuHeadTempCtrl);
        menuPrint(menuTempCtrl);
        opc = menuInputOpt(0, 4);

        switch (opc) {
        case 0:
            goto START;
            break;
        case 1:
            tempShowCurrent();
            break;
        case 2:
            tempManualControl();
            break;
        case 3:
            tempShowHistory();
            break;
        case 4:
            tempMonitor = true;
            tempRealtimeMonitor();
            clearBuffer();
            getchar();
            tempMonitor = false;
            break;
        }
    }

CONSULT:
    clearConsole();
    printf("%s", menuHeadQueries);

    while (1) {
        menuPrint(menuQueries);
        opc = menuInputOpt(0, 3);

        switch (opc) {
        case 0:
            goto START;
            break;
        case 1:
            searchEventByRange();
            break;
        case 2:

            break;
        case 3:
            exportToCSV();
            break;
        }
    }

USERS:
    clearConsole();
    printf("%s", menuHeadUsers);

    while (1) {
        menuPrint(menuUsers);
        opc = menuInputOpt(0, 4);

        switch (opc) {
        case 0:
            goto START;
            break;
        case 1:
            usersPrint();
            break;
        case 2:
            usersAdd();
            break;
        case 3:
            usersRemove();
            break;
        case 4:
            usersChangePasswd();
            break;
        }
    }

CONFIG:
    clearConsole();
    printf("%s", menuHeadConfig);

    while (1) {
        menuPrint(menuConfig);
        opc = menuInputOpt(0, 2);

        switch (opc) {
        case 0:
            goto START;
            break;
        case 1:
            zoneThresholdModification();
            break;
        case 2:
            zoneDefaultThreshold();
            break;
        }
    }
}

int main() {

    tempsensorInit();
    usersInit();
    zoneInit();

    clearConsole();
    pthread_t thread;
    const int error = pthread_create(&thread, NULL, executeEverySecond, NULL);
    if (error) {
        printf("Error al crear el hilo de ejecucion.\nSaliendo...\n");
        return 1;
    }

    pthread_t thread2;
    const int err2 = pthread_create(&thread2, NULL, executeEvery5Minutes, NULL);
    if (err2) {
        printf("Error al crear el hilo de ejecucion.\nSaliendo...\n");
        return 1;
    }

    if (!usersLogin()) {
        printf("Credenciales incorrectas\n");
        programFinished = true;

        pthread_join(thread, NULL);
        pthread_join(thread2, NULL);
        return 1;
    }

    menu();

    pthread_join(thread, NULL);
    pthread_join(thread2, NULL);

    return 0;
}