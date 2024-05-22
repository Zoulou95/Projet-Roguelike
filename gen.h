#ifndef GEN.H
#define GEN.H
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<ncurses.h>
#define MAX_SIZE_ROOM_WIDTH 11
#define MAX_SIZE_ROOM_HEIGHT 11
#define MIN_SIZE_ROOM_WIDHT 5
#define MIN_SIZE_ROOM_HEIGHT 5
#define MIN_SIZE_ROOM_WIDTH 5
#define DISPLAY_HEIGHT 11
#define DISPLAY_WIDTH 11
#define MAX_DOOR 4
#define LEFT 0
#define RIGHT 1
#define TOP 2
#define BOTTOM 3
#define SPAWN_X 0
#define SPAWN_Y 0
#define MAX_ROOM 20
#define MIN_ROOM 15
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

void give_seed(int *seed);
int getMaxRooms();
MAP *create_map();
int isSpaceAvailable(MAP *map, ROOM *new_room);
ROOM *createRoom(MAP *map, ROOM *prev_room, char location);
int numberOfDoors(MAP *map);
int createLeftDoor(MAP *map, ROOM *room);
int createRightDoor(MAP *map, ROOM *room);
int createTopDoor(MAP *map, ROOM *room);
int createBottomDoor(MAP *map, ROOM *room);
void createDoors(MAP *map, ROOM *room, char existing_door);
ROOM *Spawn(MAP *map);

#endif