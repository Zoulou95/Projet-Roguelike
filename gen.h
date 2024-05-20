#ifndef GEN.H
#define GEN.H
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define MAX_SIZE_ROOM_WIDHT 12
#define MAX_SIZE_ROOM_HEIGHT 12
#define MIN_SIZE_ROOM_WIDHT 5
#define MIN_SIZE_ROOM_HEIGHT 5
#define MAX_DOOR 4
#define LEFT 0
#define RIGHT 1
#define TOP 2
#define BOTTOM 3
#define CHECK 10

typedef struct{ // structure for coordinates
    int x;
    int y;
}COORD;

typedef struct{
    int door_ID; // door ID
    COORD co_door; // room coordinates
    char location;
}DOOR;

typedef struct{
    int room_ID; // room ID
    COORD co_room; // room coordinates
    int width;
    int height;
    int doors;
    DOOR door[MAX_DOOR];
}ROOM;

typedef struct{
    int max_room;
    int nb_rooms;
    ROOM *room;
}MAP;

#endif

// chercher (MODIF)