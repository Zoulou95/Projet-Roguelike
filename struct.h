#ifndef STRUCT_H
#define STRUCT_H
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<ncurses.h>
#define MAX_SIZE_ROOM_WIDTH 11
#define MAX_SIZE_ROOM_HEIGHT 11
#define MIN_SIZE_ROOM_HEIGHT 5
#define MIN_SIZE_ROOM_WIDTH 5
#define DISPLAY_HEIGHT 21
#define DISPLAY_WIDTH 21
#define MAX_DOOR 4
#define LEFT 0
#define RIGHT 1
#define TOP 2
#define BOTTOM 3
#define SPAWN_X 0
#define SPAWN_Y 0
#define MAX_ROOM 20
#define MIN_ROOM 15
#define maxMenu 2
#define CHECK 10
#define MAX_CHAR 50

typedef struct{ // structure for coordinates
    int x;
    int y;
}COORD;

typedef struct{
    int closed; // door ID
    char location;
    int gap_x;
    int gap_y;
    int track; // precedent room_ID
}DOOR;

typedef struct{
    int room_ID; // room ID
    COORD co_room; // room coordinates
    // int x;
    // int y;
    int width;
    int height;
    int doors; // number of doors
    DOOR door[MAX_DOOR];
    char **data;
    int explored;
}ROOM;

typedef struct{
    int max_room;
    int nb_rooms;
    int nb_visited;
    int *visited;
    ROOM *room;
}MAP;

typedef struct {
    int x;
    int y;
    int current_room;
} PLAYER;

typedef struct{ // créer un fichier pour retourner plusieurs variables dans les fonctions
    FILE * fp;
    char menu[maxMenu][20];
}FICHIER;

#endif