#ifndef GEN_H
#define GEN_H

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "player.h"

#define ROOM_WIDTH 11
#define ROOM_HEIGHT 11
#define MIN_SIZE_ROOM_HEIGHT 5
#define MIN_SIZE_ROOM_WIDTH 5
#define DISPLAY_HEIGHT 11
#define DISPLAY_WIDTH 11

typedef struct {
    int width;
    int height;
} ROOM;

typedef struct {
    int max_room;
    ROOM *room;
} MAP;



void give_seed(int *seed);
int getMaxRooms();
ROOM *create_room(int width, int height);
MAP *create_map();
void display_room_view(PLAYER *player, int width, int height, char room[][width]);
void move_player(PLAYER *player, int width, int height, char room[][width], int ch);
void Display_room(PLAYER *player, MAP *map, int room_index);
#endif