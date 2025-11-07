#ifndef GESTEMP_UTILS_H
#define GESTEMP_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32
#include <Windows.h>
#define sleepMSec(s) Sleep(s)
#define sleepSec(s) Sleep(s * 1000)
#define clearConsole() system("cls")
#else
#include <unistd.h>
#define sleepMSec(s) usleep(s * 0.001)
#define sleepSec(s) sleep(s)
#define clearConsole() system("clear")
#endif

#define PROMPT '>'
#define clearBuffer() int bfc; while ((bfc = getchar()) != '\n' && bfc != EOF)

void fileValidation (FILE **pf);

void logEvent(unsigned int id, int status, int mode);
bool isInRange(int x, int a, int b);

#endif //GESTEMP_UTILS_H