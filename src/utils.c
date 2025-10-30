#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
