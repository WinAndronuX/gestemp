#ifndef MENU_H
#define MENU_H


extern const char* menuHeadStart;
extern const char* menuHeadZones;
extern const char* menuHeadTempCtrl;
extern const char* menuHeadQueries;
extern const char* menuHeadUsers;
extern const char* menuHeadConfig;

extern const char* menuStart[];
extern const char* menuZones[];
extern const char* menuTempCtrl[];
extern const char* menuQueries[];
extern const char* menuUsers[];
extern const char* menuConfig[];

void menuPrint(const char* menu[]);
int menuInputOpt(int a, int b);

#endif // MENU_H