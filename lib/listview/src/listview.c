#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <listview/listview.h>


ListView* listviewCreate(char* title, int nCols) {

    ListView* data = malloc(sizeof(ListView));

    data->title = title;
    data->nCols = nCols;
    data->colsSizes = calloc(sizeof(int), nCols);
    data->heads = calloc(sizeof(char*), nCols);
    data->iCols = 0;
    data->counter = 0;

    return data;
}

void listviewHeadAdd(ListView* lv, char* text, int size) {

    if (lv->iCols >= lv->nCols) return;

    lv->colsSizes[lv->iCols] = size;
    lv->heads[lv->iCols] = text;

    lv->iCols += 1;

    if (lv->iCols < lv->nCols) return;

    int i, j;

    int tableWidth = 0;

    for (i = 0; i < lv->nCols; i++) tableWidth += lv->colsSizes[i];
    tableWidth++;

    int center = (tableWidth / 2) - (strlen(lv->title) / 2);

    printf("%*c** %s **\n\n", center + 2 , ' ', lv->title);

    for (i = 0; i < lv->nCols; i++) {

        if (i == 0) {
            printf("┌");
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printf("─");
        } else if (i == lv->nCols - 1) {
            printf("┬");
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printf("─");
            printf("┐\n");
        } else {
            printf("┬");
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printf("─");
        }
    }

    for (i = 0; i < lv->nCols; i++) {

        printf("| %-*.*s ", lv->colsSizes[i], lv->colsSizes[i], lv->heads[i]);

        if (i == lv->nCols - 1) printf("|\n");
    }

    for (i = 0; i < lv->nCols; i++) {

        if (i == 0) {
            printf("├");
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printf("─");
        } else if (i == lv->nCols - 1) {
            printf("┼");
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printf("─");
            printf("┤\n");
        } else {
            printf("┼");
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printf("─");
        }
    }
}

void listviewHeadAddAuto(ListView* lv,char* text) {
    listviewHeadAdd(lv, text, strlen(text));
}

void listviewAdd(ListView* lv,char* text) {

    if (lv->iCols >= lv->nCols) lv->iCols = 0;

    printf("| %-*.*s ", lv->colsSizes[lv->iCols], lv->colsSizes[lv->iCols], text);

    if (lv->iCols == lv->nCols - 1) printf("|\n");

    lv->iCols += 1;
    lv->counter += 1;
}

void listviewFootPrint(ListView* lv) {


    int i, j;
    for (i = 0; i < lv->nCols; i++) {

        if (i == 0) {
            printf("└");
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printf("─");
        } else if (i == lv->nCols - 1) {
            printf("┴");
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printf("─");
            printf("┘\n");
        } else {
            printf("┴");
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printf("─");
        }
    }
    printf("Total: %d registros\n", lv->counter);

    free(lv->colsSizes);
    free(lv->heads);
    free(lv);
}

