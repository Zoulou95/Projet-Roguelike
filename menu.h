#ifndef MENU_H
#define MENU_H
#include <stdlib.h>
#include <ncurses.h>
#include <stdio.h>

#define maxMenu 3

typedef struct{ // crÃ©er un fichier pour retourner plusieurs variables dans les fonctions
    FILE * fp;
    char menu[maxMenu][20];
}FICHIER;

FICHIER create_file();
void print_menu(FICHIER file, int *choice);

#endif
