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

void teleport(PLAYER *player, MAP *map, char location){
    switch(location){
    case 'l':
        player->x=player->x-3;
        break;
    case 'r':
        player->x=player->x+3;
        break;
    case 't':
        player->y=player->y-3;
        break;
    case 'b':
        player->y=player->y+3;
        break;
    default:
        break;
    }
}

void move_player(PLAYER *player, MAP *map, int ch) {
    // player->current_room=find_player_room(map, player);
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
        default:
            return;
            break;
    }

    // Check if the new position is within the room boundaries
    int height=room->height;
    int width=room->width;
    int room_y=room->co_room.y;
    int room_x=room->co_room.x;
    if (new_x >= room_x && new_x < room_x+width && new_y >= room_y && new_y < room_y+height) {
        if (room->data[new_y][new_x] == 'd') {
            for(int i=0; i<MAX_DOOR; i++){
                if((new_y==0 && new_x==room->door[i].gap_x) ||
                (new_y==height-1 && new_x==room->door[i].gap_x) ||
                (new_y==room->door[i].gap_y && new_x==0) ||
                (new_y==room->door[i].gap_y && new_x==width-1)){
                    if(room->door[i].closed==0){
                        teleport(player, map, room->door[i].location);
                        player->current_room=room->door[i].track;
                        break;
                    }
                    else if(room->door[i].closed==1){
                        teleport(player, map, room->door[i].location);
                        if (!map->room[player->current_room].data) {
                            printw("before\n");
                            refresh();
                            exit(-5);
                        }
                        player->current_room=room->door[i].track;
                        if (!map->room[player->current_room].data) {
                            printw("after_\n");
                            refresh();
                            exit(-5);
                        }
                        room->door[i].closed=0;
                        createDoors(map, &map->room[room->door[i].track], room->door[i].location);
                        Display_room(player, map, room->door[i].track);
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