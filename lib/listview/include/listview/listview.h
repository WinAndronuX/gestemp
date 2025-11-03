#ifndef LISTVIEW_H
#define LISTVIEW_H

#ifdef _WIN32
#define printSymbol(u, w) printf("%c", w)
#else
#define printSymbol(u, w) printf(u)
#endif

typedef struct {
    char* title;
    int* colsSizes;
    char** heads;
    int nCols;
    int iCols;
    int counter;
} ListView;

ListView* listviewCreate(char* title, int nCols);
void listviewHeadAdd(ListView* lv, char* text, int size);
void listviewHeadAddAuto(ListView* lv,char* text);
void listviewAdd(ListView* lv,char* text);
void listviewFootPrint(ListView* lv);

#endif