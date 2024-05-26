#include "struct.h"
#include "gen.h"

// Function to find the room the player is in
int find_player_room(MAP *map, PLAYER *player) {
  // Browse all card parts
    for (int i = 0; i < map->nb_rooms; i++) {
        ROOM *room = &map->room[i];
        
        // Check that the player's coordinates are within the room limits
        if (player->x >= room->co_room.x && player->x < room->co_room.x + room->width &&
            player->y >= room->co_room.y && player->y < room->co_room.y + room->height) {
            // Retourner l'ID de la pièce si le joueur s'y trouve
            return room->room_ID;
        }
    }
    // return -1 if the player is not in any room
    return -1;
}

// Function to teleport the player a certain distance in a given direction
void teleport(PLAYER *player, int dx, int dy){
    if(dx != 0){
        player->x = (dx == -1 ? player->x - 3 : player->x + 3);
    }
    if(dy != 0){
        player->y = (dy == -1 ? player->y - 3 : player->y + 3);
    }
}

// Function to determine direction based on dx and dy
int direction(int dx, int dy){
    int direction;
    if(dx != 0){
        // Set direction as left or right
        direction = (dx == -1 ? LEFT : RIGHT);
    }
    if(dy != 0){
       // Set direction as high or low
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
    default: // in case of unknown location
        perror("Erreur d'inversion de localisation");
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
    int dx = 0, dy = 0;

    // Determine new position based on user input
    switch (ch) {
        case 'z': 
            new_y--;
            dy = -1;
            break;
        case 's': 
            new_y++;
            dy = +1;
            break;
        case 'q': 
            new_x--;
            dx = -1;
            break;
        case 'd': 
            new_x++;
            dx = +1;
            break;
        default: 
            return;
            break;
    }

    int abs_x = abs(new_x - room->co_room.x);
    int abs_y = abs(new_y - room->co_room.y);

    // Check that the new position is within the part limits
    if (new_x >= room->co_room.x && new_x < room->co_room.x + room->width && new_y >= room->co_room.y && new_y < room->co_room.y + room->height) {
        // Check if the new position is a door
        if (room->data[abs_y][abs_x] == 'd') {
            int dir = direction(dx, dy); // Déterminer la direction
            int next_room = room->door[dir].track; // Obtenir la pièce suivante
            player->current_room = next_room; // Mettre à jour la pièce actuelle du joueur

            // Check that the door is closed
            if (room->door[dir].closed == 1) {
                map->visited[map->nb_visited] = next_room;
                map->nb_visited++;
                room->door[dir].closed = 0; // Ouvrir la porte
                // Create doors in the next room
                createDoors(map, &map->room[next_room], invertLocation(room->door[dir].location));
            }
            // Teleport the player
            teleport(player, dx, dy);
        }
    
        else if (room->data[abs_y][abs_x] != '#') {
            // Update player details if there is a wall 
            player->x = new_x;
            player->y = new_y;
        }
    }
}
