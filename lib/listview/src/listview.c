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
            printSymbol("┌", 218);
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printSymbol("─", 196);
        } else if (i == lv->nCols - 1) {
            printSymbol("┬", 194);
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printSymbol("─", 196);
            printSymbol("┐", 191);
            printf("\n");
        } else {
            printSymbol("┬", 194);
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printSymbol("─", 196);
        }
    }

    for (i = 0; i < lv->nCols; i++) {

        printSymbol("│", 179);
        printf(" \e[1m%-*.*s\e[22m ", lv->colsSizes[i], lv->colsSizes[i], lv->heads[i]);

        if (i == lv->nCols - 1) {
            printSymbol("│", 179);
            printf("\n");
        }
    }

    for (i = 0; i < lv->nCols; i++) {

        if (i == 0) {
            printSymbol("├", 195);
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printSymbol("─", 196);
        } else if (i == lv->nCols - 1) {
            printSymbol("┼", 197);
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printSymbol("─", 196);
            printSymbol("┤", 180);
            printf("\n");
        } else {
            printSymbol("┼", 197);
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printSymbol("─", 196);
        }
    }
}

void listviewHeadAddAuto(ListView* lv,char* text) {
    listviewHeadAdd(lv, text, strlen(text));
}

void listviewAdd(ListView* lv,char* text) {

    if (lv->iCols >= lv->nCols) lv->iCols = 0;

    printSymbol("│", 179);
    printf(" %-*.*s ", lv->colsSizes[lv->iCols], lv->colsSizes[lv->iCols], text);

    if (lv->iCols == lv->nCols - 1) {
        printSymbol("│", 179);
        printf("\n");
    }

    if (lv->iCols == 0) lv->counter += 1;
    lv->iCols += 1;
}

void listviewFootPrint(ListView* lv) {


    int i, j;
    for (i = 0; i < lv->nCols; i++) {

        if (i == 0) {
            printSymbol("└", 192);
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printSymbol("─", 196);
        } else if (i == lv->nCols - 1) {
            printSymbol("┴", 193);
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printSymbol("─", 196);
            printSymbol("┘", 217);
            printf("\n");
        } else {
            printSymbol("┴", 193);
            for (j = 0; j < lv->colsSizes[i] + 2; j++) printSymbol("─", 196);
        }
    }
    printf("Total: %d registros\n", lv->counter);

    free(lv->colsSizes);
    free(lv->heads);
    free(lv);
}

