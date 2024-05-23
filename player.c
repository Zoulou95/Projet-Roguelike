#include "struct.h"
#include "gen.h"

void move_player(PLAYER *player, MAP *map, int ch) {
    ROOM *room = &map->room[player->current_room];
    int new_x = player->x;
    int new_y = player->y;

    switch (ch) {
        case KEY_UP:
            new_y--;
            break;
        case KEY_DOWN:
            new_y++;
            break;
        case KEY_LEFT:
            new_x--;
            break;
        case KEY_RIGHT:
            new_x++;
            break;
    }

    // Check if the new position is within the room boundaries
    if (new_x >= 0 && new_x < room->width && new_y >= 0 && new_y < room->height) {
        if (room->data[new_y][new_x] == 'd') {
            // Transition to the adjacent room
            for (int i = 0; i < map->max_room; i++) {
                ROOM *adj_room = &map->room[i];
                if (adj_room->co_room.x == room->co_room.x + (new_x == 0 ? -1 : (new_x == room->width - 1 ? 1 : 0)) &&
                    adj_room->co_room.y == room->co_room.y + (new_y == 0 ? -1 : (new_y == room->height - 1 ? 1 : 0))) {
                    player->current_room = i;
                    player->x = (new_x == 0 ? adj_room->width - 2 : (new_x == room->width - 1 ? 1 : adj_room->width / 2));
                    player->y = (new_y == 0 ? adj_room->height - 2 : (new_y == room->height - 1 ? 1 : adj_room->height / 2));
                    adj_room->explored = 1;
                    break;
                }
            }
        } else if (room->data[new_y][new_x] != '#') {
            // Move player within the same room
            player->x = new_x;
            player->y = new_y;
        }
    }
}