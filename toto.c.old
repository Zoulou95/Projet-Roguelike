#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#define ROOM_WIDTH 11
#define ROOM_HEIGHT 11
#define MIN_SIZE_ROOM_WIDTH 5
#define MIN_SIZE_ROOM_HEIGHT 5
#define DISPLAY_WIDTH 50
#define DISPLAY_HEIGHT 50

typedef struct {
    int width;
    int height;
    char **data;
    int x;
    int y;
    int explored;
} ROOM;

typedef struct {
    int max_room;
    ROOM *rooms;
    int room_count;
} MAP;

typedef struct {
    int x, y;
    int current_room;
} PLAYER;

typedef struct {
    int x, y;
    int health;
    int attack;
} MONSTER;

int getMaxRooms() {
    int minthRooms = 5;
    int maxthRooms = 20;
    return rand() % (maxthRooms - minthRooms + 1) + minthRooms;
}

ROOM *create_room(int width, int height, int x, int y) {
    ROOM *room = (ROOM *)malloc(sizeof(ROOM));
    if (room == NULL) {
        perror("Memory allocation error for the room\n");
        exit(EXIT_FAILURE);
    }
    room->width = width;
    room->height = height;
    room->x = x;
    room->y = y;
    room->explored = 0;
    room->data = (char **)malloc(height * sizeof(char *));
    for (int i = 0; i < height; i++) {
        room->data[i] = (char *)malloc(width * sizeof(char));
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                room->data[i][j] = '#'; // Borders
            } else {
                room->data[i][j] = ' '; // Empty space
            }
        }
    }
    room->data[0][width / 2] = 'd'; // Door at the top
    room->data[height - 1][width / 2] = 'd'; // Door at the bottom
    room->data[height / 2][0] = 'd'; // Door on the left
    room->data[height / 2][width - 1] = 'd'; // Door on the right
    return room;
}

MAP *create_map() {
    int seed = 45;
    srand(seed);

    MAP *map = (MAP *)malloc(sizeof(MAP));
    if (map == NULL) {
        perror("Memory allocation error for the map\n");
        exit(EXIT_FAILURE);
    }
    map->max_room = getMaxRooms();
    printw("%d", map->max_room);
    getch();
    map->rooms = (ROOM *)malloc(sizeof(ROOM) * map->max_room);
    if (map->rooms == NULL) {
        perror("Memory allocation error for the map rooms\n");
        exit(EXIT_FAILURE);
    }

    int width_spawn = ROOM_WIDTH;
    int height_spawn = ROOM_HEIGHT;
    map->rooms[0] = *create_room(width_spawn, height_spawn, 50, 100);
    map->rooms[0].explored = 1;
    map->room_count = 1;

    return map;
}

ROOM *find_room(MAP *map, int x, int y) {
    for (int i = 0; i < map->room_count; i++) {
        if (map->rooms[i].x == x && map->rooms[i].y == y) {
            return &map->rooms[i];
        }
    }
    return NULL;
}

void convert_unexplored_doors_to_walls(MAP *map) {
    for (int i = 0; i < map->room_count; i++) {
        ROOM *room = &map->rooms[i];
        for (int y = 0; y < room->height; y++) {
            for (int x = 0; x < room->width; x++) {
                if (room->data[y][x] == 'd') {
                    // Check if the adjacent room exists and is explored
                    int adj_x = room->x + (x == 0 ? -1 : (x == room->width - 1 ? 1 : 0));
                    int adj_y = room->y + (y == 0 ? -1 : (y == room->height - 1 ? 1 : 0));
                    ROOM *adj_room = find_room(map, adj_x, adj_y);
                    if (adj_room == NULL || !adj_room->explored) {
                        room->data[y][x] = '#'; // Convert door to wall
                    }
                }
            }
        }
    }
}

void create_adjacent_room(PLAYER *player, MAP *map, int dx, int dy) {
    int new_x = map->rooms[player->current_room].x + dx-1;
    int new_y = map->rooms[player->current_room].y + dy;
    printw("%d,%d", new_x, new_y);
    refresh();

    if (find_room(map, new_x, new_y) == NULL) {
        if (map->room_count < map->max_room) {
            int width = ROOM_WIDTH;
            int height = ROOM_HEIGHT;
            map->rooms[map->room_count] = *create_room(width, height, new_x, new_y);
            map->rooms[map->room_count].explored = 1;
            map->room_count++;
        } else {
            convert_unexplored_doors_to_walls(map);
        }
    }
}

MONSTER *spawn_monster(int room_width, int room_height, int player_x, int player_y, int health, int attack) {
    MONSTER *monster = (MONSTER *)malloc(sizeof(MONSTER));
    if (monster == NULL) {
        perror("Memory allocation error for the monster\n");
        exit(EXIT_FAILURE);
    }

    // Randomly generate coordinates within the room's boundaries
    do {
        monster->x = rand() % (room_width - 2) + 1; // Avoid spawning on the room's borders
        monster->y = rand() % (room_height - 2) + 1;
    } while (monster->x == player_x && monster->y == player_y); // Ensure the monster doesn't spawn on the player's position

    monster->health = health;
    monster->attack = attack;
    return monster;
}

void delete_monsters(MONSTER **monsters, int *monster_count) {
    for (int i = 0; i < *monster_count; i++) {
        free(monsters[i]);
    }
    *monster_count = 0;
}

void fight(PLAYER *player, MONSTER *monster) {
    int player_attack = 10; // Example player attack value
    int player_health = 100; // Example player health value

    player_health -= monster->attack;
    monster->health -= player_attack;

    if (monster->health <= 0) {
        mvprintw(0, 0, "Monster defeated!");
        refresh();
        napms(1000);
    } else if (player_health <= 0) {
        mvprintw(0, 0, "Player defeated!");
        refresh();
        napms(1000);
        endwin();
        exit(EXIT_FAILURE);
    } else {
        mvprintw(0, 0, "Fight continues! Player health: %d, Monster health: %d", player_health, monster->health);
        refresh();
        napms(1000);
    }
}

void Display_room(PLAYER *player, MAP *map, MONSTER **monsters, int *monster_count);
void display_visible_rooms(PLAYER *player, MAP *map, MONSTER **monsters, int monster_count);
void move_player(PLAYER *player, MAP *map, MONSTER **monsters, int *monster_count, int ch);

int main() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    MAP *map = create_map();
    PLAYER player = {map->rooms[0].width / 2, map->rooms[0].height / 2, 0};

    int max_monsters = 10;
    MONSTER *monsters[max_monsters];
    int monster_count = 0;

    // Example of spawning a monster
    monsters[monster_count++] = spawn_monster(map->rooms[0].width, map->rooms[0].height, player.x, player.y, 20, 5);

    Display_room(&player, map, monsters, &monster_count);

    endwin();
    return 0;
}

void Display_room(PLAYER *player, MAP *map, MONSTER **monsters, int *monster_count) {
    int ch;

    while (1) {
        display_visible_rooms(player, map, monsters, *monster_count);

        ch = getch();
        if (ch == 27) { // Escape key
            timeout(-1);
            clear();
            refresh();
            free(map->rooms);
            free(map);
            delete_monsters(monsters, monster_count);
            break;
        }
        move_player(player, map, monsters, monster_count, ch);

        napms(50);
    }

    endwin();
}

void display_visible_rooms(PLAYER *player, MAP *map, MONSTER **monsters, int monster_count) {
    clear();
    ROOM *current_room = &map->rooms[player->current_room];

    int term_height, term_width;
    getmaxyx(stdscr, term_height, term_width);

    int start_y = (term_height - DISPLAY_HEIGHT) / 2;
    int start_x = (term_width - DISPLAY_WIDTH) / 2;

    int offset_x = player->x - DISPLAY_WIDTH / 2;
    int offset_y = player->y - DISPLAY_HEIGHT / 2;

    for (int i = 0; i < DISPLAY_HEIGHT; i++) {
        for (int j = 0; j < DISPLAY_WIDTH; j++) {
            int global_x = current_room->x * ROOM_WIDTH + offset_x + j;
            int global_y = current_room->y * ROOM_HEIGHT + offset_y + i;

            char c = ' ';
            for (int k = 0; k < map->room_count; k++) {
                ROOM *room = &map->rooms[k];
                int local_x = global_x - room->x * ROOM_WIDTH;
                int local_y = global_y - room->y * ROOM_HEIGHT;
                if (local_x >= 0 && local_x < room->width && local_y >= 0 && local_y < room->height) {
                    if (local_x == player->x && local_y == player->y && k == player->current_room) {
                        c = 'P';
                    } 
                    else {
                        c = room->data[local_y][local_x];
                    }
                }
            }
            mvprintw(start_y + i, start_x + j, "%c", c);
        }
    }
    refresh();
}

void move_player(PLAYER *player, MAP *map, MONSTER **monsters, int *monster_count, int ch) {
    ROOM *room = &map->rooms[player->current_room];

    int new_x = player->x;
    int new_y = player->y;
    int dx = 0, dy = 0;

    switch (ch) {
        case KEY_UP:
            new_y--;
            dy = -1;
            break;
        case KEY_DOWN:
            new_y++;
            dy = 1;
            break;
        case KEY_LEFT:
            new_x--;
            dx = -1;
            break;
        case KEY_RIGHT:
            new_x++;
            dx = 1;
            break;
    }

    if (new_x >= 0 && new_x < room->width && new_y >= 0 && new_y < room->height) {
        if (room->data[new_y][new_x] == 'd') {
            ROOM *adj_room = find_room(map, room->x + dx, room->y + dy);
            if (adj_room == NULL) {
                create_adjacent_room(player, map, dx, dy);
                adj_room = &map->rooms[map->room_count - 1];
            }
            player->current_room = adj_room - map->rooms;

            // Delete monsters in the previous room
            delete_monsters(monsters, monster_count);

            // Align the player position with the door in the new room
            if (dx == -1) { // Moving left
                player->x = adj_room->width - 2;
                player->y = adj_room->height / 2;
            } else if (dx == 1) { // Moving right
                player->x = 1;
                player->y = adj_room->height / 2;
            } else if (dy == -1) { // Moving up
                player->x = adj_room->width / 2;
                player->y = adj_room->height - 2;
            } else if (dy == 1) { // Moving down
                player->x = adj_room->width / 2;
                player->y = 1;
            }
            adj_room->explored = 1;
        } else if (room->data[new_y][new_x] != '#') {
            // Check if there's a monster in the new position
            int monster_found = 0;
            for (int i = 0; i < *monster_count; i++) {
                if (monsters[i]->x == new_x && monsters[i]->y == new_y) {
                    fight(player, monsters[i]);
                    monster_found = 1;
                    if (monsters[i]->health <= 0) {
                        free(monsters[i]);
                        for (int j = i; j < *monster_count - 1; j++) {
                            monsters[j] = monsters[j + 1];
                        }
                        (*monster_count)--;
                    }
                    break;
                }
            }

            if (!monster_found) {
                player->x = new_x;
                player->y = new_y;
            }
        }
    }
}
