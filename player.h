#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

typedef struct {
    int x;
    int y;
} PLAYER;

void move_player(PLAYER *player, int width, int height, char room[][width], int ch);

#endif 
