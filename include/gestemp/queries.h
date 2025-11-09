#ifndef GESTEMP_QUERIES_H
#define GESTEMP_QUERIES_H

#include <time.h>

static int isValidDate(struct tm* date);

static int differenceValidation(char *sDate, time_t *sT, time_t *eT);

void searchEventByRange();

#endif //GESTEMP_QUERIES_H