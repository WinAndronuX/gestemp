#ifndef TEMPSENSOR_H
#define TEMPSENSOR_H

#include <gestemp/zone.h>

void tempsensorInit();
float tempsensorRead(Zone *z);

#endif