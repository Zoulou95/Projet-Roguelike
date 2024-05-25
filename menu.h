#ifndef MENU_H
#define MENU_H
#include "struct.h"
#include <stdlib.h>
#include <ncurses.h>
#include <stdio.h>

FICHIER create_file();
void print_menu(FICHIER file, int choice);
void choice_menu(int choice);
FICHIER create_file();
void game_loop(PLAYER *player, MAP *map, int room_ID);

#endif
