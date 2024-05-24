#ifndef GEN_H 
#define GEN_H 
#include "struct.h"
#include "player.h"

// void give_seed(int *seed);
int getMaxRooms();
MAP *create_map();
int isSpaceAvailable(MAP *map, ROOM *new_room);
ROOM *createRoom(MAP *map, ROOM *prev_room, char location);
// ROOM *create_room(int width, int height, int x, int y);
int numberOfDoors(MAP *map);
int createLeftDoor(MAP *map, ROOM *room);
int createRightDoor(MAP *map, ROOM *room);
int createTopDoor(MAP *map, ROOM *room);
int createBottomDoor(MAP *map, ROOM *room);
void createDoors(MAP *map, ROOM *room, char existing_door);
ROOM *Spawn(MAP *map);
void Display_room(PLAYER *player, MAP *map, int room_ID, char location);
void display_room_view(PLAYER *player, MAP *map, int width, int height, int room_ID);
// void display_visible_rooms(PLAYER *player, MAP *map);
#endif