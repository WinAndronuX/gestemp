#include <stdio.h>
#include <pthread.h>
#include <gestemp/utils.h>
#include <gestemp/tempsensor.h>


void *executeEverySecond() {

    while (1) {

        printf("Ejecutando..\n");

        sleepSec(1);
    }
}

int main() {

    tempsensorInit();

    clearConsole();
    pthread_t thread;
    const int error = pthread_create(&thread, NULL, executeEverySecond, NULL);
    if (error) {
        printf("Error al crear el hilo de ejecucion.\nSaliendo...\n");
        return 1;
    }

    pthread_join(thread, NULL);

    return 0;
}