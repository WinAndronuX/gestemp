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

    if (lv->iCols < lv->nCols - 1) return;

    int i, j;
    for (i = 0; i < lv->nCols; i++) {

        if (i == 0) {
            printf("┌");
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printf("─");
        } else if (i == lv->nCols - 1) {
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printf("─");
            printf("┐\n");
        } else {
            printf("┬");
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printf("─");
        }
    }

    for (i = 0; i < lv->nCols; i++) {

        char ss[lv->colsSizes[i]];

        int s = lv->colsSizes[i];
        if (strlen(lv->heads[i]) > s) {
            strncpy(ss, lv->heads[i], lv->colsSizes[i]);
            ss[lv->colsSizes[i]] = '\0';
        } else
            strcpy(ss, lv->heads[i]);

        printf("| %s ", ss);

        if (i == lv->nCols - 1) printf("|\n");
    }

    for (i = 0; i < lv->nCols; i++) {

        if (i == 0) {
            printf("├");
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printf("─");
        } else if (i == lv->nCols - 1) {
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

    int j;
    char ss[lv->colsSizes[lv->iCols]];

    int s = lv->colsSizes[lv->iCols];
    if (strlen(text) > s) {
        strncpy(ss, text, lv->colsSizes[lv->iCols]);
        ss[lv->colsSizes[lv->iCols]] = '\0';
    } else
        strcpy(ss, text);

    printf("| %s ", ss);

    if (lv->iCols == lv->nCols - 1) printf("|\n");

    lv->iCols += 1;
}

void listviewFootPrint(ListView* lv) {


    int i, j;
    for (i = 0; i < lv->nCols; i++) {

        if (i == 0) {
            printf("└");
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printf("─");
        } else if (i == lv->nCols - 1) {
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printf("─");
            printf("┘\n");
        } else {
            printf("┴");
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printf("─");
        }
    }

    free(lv->colsSizes);
    free(lv->heads);
    free(lv);
}

