#include "struct.h"
#include "gen.h"

// Function to teleport the player a certain distance in a given direction when taking a door
void teleport(PLAYER *player, int dx, int dy){
    if(dx!=0){
        player->x = (dx == -1 ? player->x-3 : player->x+3);
    }
    if(dy!=0){
        player->y = (dy == -1 ? player->y-3 : player->y+3);
    }
}
// Function to determine position of the door taken
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
// Function to invert location
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
    default:// in case of unknown location
        perror("Invert location error");
        exit(EXIT_FAILURE);
        break;
    }
}

// Function to move the player
void move_player(PLAYER *player, MAP *map, int ch) {
   // Retrieve the room where the player is located
    ROOM *room = &map->room[player->current_room]; 
    int new_x = player->x;
    int new_y = player->y;
    int dx=0, dy=0;
    // Determine new position based on user input
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
    // relative position of the player inside the room (disance from the top left corner)
    int abs_x=abs(new_x-room->co_room.x);
    int abs_y=abs(new_y-room->co_room.y);
    // Check if the new position is within the room boundaries
    if (new_x >= room->co_room.x && new_x < room->co_room.x + room->width && new_y >= room->co_room.y && new_y < room->co_room.y + room->height){
        // Check if the new position is a door
        if (room->data[abs_y][abs_x] == 'd'){
            int dir=direction(dx, dy); //determine the direction of the door taken
            int next_room=room->door[dir].track; // room_ID of the room past the door
            player->current_room=next_room; // set the player positon
            // Check that the door is closed
            if(room->door[dir].closed==1){
                map->visited[map->nb_visited]=next_room;
                map->nb_visited++;
                room->door[dir].closed=0; // open the door
                // Create doors in the next room
                createDoors(map, &map->room[next_room], invertLocation(room->door[dir].location));
            }
            // Teleport the player
            teleport(player, dx, dy);
        }
        else if (room->data[abs_y][abs_x] != '#'){ // move player if it's not a wall or a door
            player->x=new_x;
            player->y=new_y;
        }
    }
}