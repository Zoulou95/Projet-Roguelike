#include "struct.h"
#include "gen.h"

int find_player_room(MAP *map, PLAYER *player) {
    for (int i = 0; i < map->nb_rooms; i++) {
        ROOM *room = &map->room[i];
        
        // Check if player is within the room's boundaries
        if (player->x >= room->co_room.x && player->x < room->co_room.x + room->width &&
            player->y >= room->co_room.y && player->y < room->co_room.y + room->height) {
            return room->room_ID; // Return the room ID
        }
    }
}

void teleport(PLAYER *player, MAP *map, int room_ID, char location){
    switch(location){
    case 'l':
        player->y=map->room[room_ID].door[LEFT].co_door.y;
        player->x=map->room[room_ID].door[LEFT].co_door.x-2;
        break;
    case 'r':
        player->y=map->room[room_ID].door[RIGHT].co_door.y;
        player->x=map->room[room_ID].door[RIGHT].co_door.x+2;
        break;
    case 't':
        player->y=map->room[room_ID].door[TOP].co_door.y-2;
        player->x=map->room[room_ID].door[TOP].co_door.x;
        break;
    case 'b':
        player->y=map->room[room_ID].door[BOTTOM].co_door.y+2;
        player->x=map->room[room_ID].door[BOTTOM].co_door.x;
        break;
    default:
        break;
    }
}

void move_player(PLAYER *player, MAP *map, int ch) {
    player->current_room=find_player_room(map, player);
    ROOM *room = &map->room[player->current_room];
    int new_x = player->x;
    int new_y = player->y;

    switch (ch) {
        case 'z':
            new_y--;
            break;
        case 's':
            new_y++;
            break;
        case 'q':
            new_x--;
            break;
        case 'd':
            new_x++;
            break;
    }

    // Check if the new position is within the room boundaries
    if (new_x >= room->co_room.x && new_x < room->co_room.x+room->width && new_y >= room->co_room.y && new_y < room->co_room.y+room->height) {
        if (room->data[new_y][new_x] == 'd') {
            for(int i=0; i<MAX_DOOR; i++){
                if(room->door[i].co_door.x==new_x && room->door[i].co_door.y==new_y){
                    if(room->door[i].closed==0){
                        teleport(player, map, map->room[player->current_room].room_ID, room->door[i].location);
                        player->current_room=find_player_room(map, player);
                        break;
                    }
                    else if(room->door[i].closed==1){
                        teleport(player, map, map->room[player->current_room].room_ID, room->door[i].location);
                        player->current_room=find_player_room(map, player);
                        Display_room(player, map, map->room[player->current_room].room_ID, room->door[i].location);
                        room->door[i].closed=0;
                        break;
                    }
                }
            }
        }
        else if (room->data[new_y][new_x] != '#') {
            // Move player within the same room
            player->x = new_x;
            player->y = new_y;
        }
    }
}