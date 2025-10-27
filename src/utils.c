#include <stdio.h>
#include <stdlib.h>
#include <gestemp/utils.h>

void fileValidation(FILE **pF) {
    if (*pF == NULL) {
        printf("El archivo no se puede acceder\n");
        exit(1);
    }
}
