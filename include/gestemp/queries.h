//
// Created by itane on 09/11/2025.
//

#ifndef GESTEMP_QUERIES_H
#define GESTEMP_QUERIES_H

#include <time.h>

typedef struct {
    char name[100];
    int num;
    float minTemp;
    float maxTemp;
    float avg;
} ReportElement;

void exportToCSV();

static int isValidDate(struct tm* date);

static int differenceValidation(char *sDate, time_t *sT, time_t *eT);

void searchEventByRange();

void queriesGenReport();

#endif //GESTEMP_QUERIES_H