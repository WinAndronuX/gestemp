#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <gestemp/utils.h>
#include <gestemp/tempsensor.h>
#include <gestemp/users.h>

bool programFinished = false;

void *executeEverySecond() {

    while (!programFinished) {

//        printf("Ejecutando..\n");

        sleepSec(1);
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

    clearConsole();

    pthread_join(thread, NULL);

    return 0;
}