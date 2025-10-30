#ifndef GESTEMP_UTILS_H
#define GESTEMP_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32
#include <Windows.h>
#define sleepSec(s) Sleep(s * 1000)
#define clearConsole() system("cls")
#else
#include <unistd.h>
#define sleepSec(s) sleep(s)
#define clearConsole() system("clear")
#endif

#define PROMPT '>'
#define clearBuffer() int bfc; while ((bfc = getchar()) != '\n' && bfc != EOF)

void fileValidation (FILE **pf);
bool isInRange(int x, int a, int b);

#endif //GESTEMP_UTILS_H