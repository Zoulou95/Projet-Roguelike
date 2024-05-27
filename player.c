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
    return -1;
}

void teleport(PLAYER *player, int dx, int dy){
    if(dx!=0){
        player->x = (dx == -1 ? player->x-3 : player->x+3);
    }
    if(dy!=0){
        player->y = (dy == -1 ? player->y-3 : player->y+3);
    }
}

int direction(int dx, int dy){
    int direction;
    if(dx!=0){
        direction = (dx == -1 ? LEFT : RIGHT);
    }
    if(dy!=0){
        direction = (dy == -1 ? TOP : BOTTOM);
    }
    return direction;
}

char invertLocation(char location){
    switch(location){
    case 'l':
        return 'r';
        break;
    case 'r':
        return 'l';
        break;
    case 't':
        return 'b';
        break;
    case 'b':
        return 't';
        break;    
    default:
        perror("Invert location error");
        exit(EXIT_FAILURE);
        break;
    }
}

void move_player(PLAYER *player, MAP *map, int ch) {
    // player->current_room=find_player_room(map, player);
    ROOM *room = &map->room[player->current_room];
    int new_x = player->x;
    int new_y = player->y;
    int dx=0, dy=0;

    switch (ch) {
        case 'z':
            new_y--;
            dy=-1;
            break;
        case 's':
            new_y++;
            dy=+1;
            break;
        case 'q':
            new_x--;
            dx=-1;
            break;
        case 'd':
            new_x++;
            dx=+1;
            break;
        default:
            return;
            break;
    }
    int abs_x=abs(new_x-room->co_room.x);
    int abs_y=abs(new_y-room->co_room.y);
    // Check if the new position is within the room boundaries
    if (new_x >= room->co_room.x && new_x < room->co_room.x + room->width && new_y >= room->co_room.y && new_y < room->co_room.y + room->height){
        if (room->data[abs_y][abs_x] == 'd'){
            int dir=direction(dx, dy);
            int next_room=room->door[dir].track;
            player->current_room=next_room;
            if(room->door[dir].closed==1){
                map->visited[map->nb_visited]=next_room;
                map->nb_visited++;
                room->door[dir].closed=0;
                createDoors(map, &map->room[next_room], invertLocation(room->door[dir].location));
            }
            teleport(player, dx, dy);
        }
        else if (room->data[abs_y][abs_x] != '#'){
            player->x=new_x;
            player->y=new_y;
        }
    }
}