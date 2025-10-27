#ifndef GESTEMP_UTILS_H
#define GESTEMP_UTILS_H
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <Windows.h>
#define sleepSec(s) Sleep(s * 1000)
#define clearConsole() system("cls")
#else
#include <unistd.h>
#define sleepSec(s) sleep(s)
#define clearConsole() system("clear")
#endif

void fileValidation (FILE **pf);

#endif //GESTEMP_UTILS_H