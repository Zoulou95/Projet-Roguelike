#ifndef PLAYER_H
#define PLAYER_H
#include "struct.h"

void move_player(PLAYER *player, MAP *map, int ch);
int find_player_room(MAP *map, PLAYER *player);
void teleport(PLAYER *player, MAP *map, char location);

#endif