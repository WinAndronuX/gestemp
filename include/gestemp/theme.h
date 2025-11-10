#ifndef THEME_H
#define THEME_H

#define CLI_COLOR_DEFAULT 39
#define CLI_COLOR_RED 31
#define CLI_COLOR_GREEN 32
#define CLI_COLOR_YELLOW 33
#define CLI_COLOR_BLUE 34
#define CLI_COLOR_MAGENTA 35
#define CLI_COLOR_CYAN 36

typedef struct {
    int code;
    char name[12];
} Theme;

void themeInit();
char* themeGetName();
void themeChange();
void themeRestoreCLI();

#endif