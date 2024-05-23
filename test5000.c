#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#define ROOM_WIDTH 12
#define ROOM_HEIGHT 12
#define MIN_SIZE_ROOM_WIDTH 5
#define MIN_SIZE_ROOM_HEIGHT 5
#define DISPLAY_WIDTH 11
#define DISPLAY_HEIGHT 11

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
    srand(time(NULL));

    MAP *map = (MAP *)malloc(sizeof(MAP));
    if (map == NULL) {
        perror("Memory allocation error for the map\n");
        exit(EXIT_FAILURE);
    }
    map->max_room = getMaxRooms();
    map->rooms = (ROOM *)malloc(sizeof(ROOM) * map->max_room);
    if (map->rooms == NULL) {
        perror("Memory allocation error for the map rooms\n");
        exit(EXIT_FAILURE);
    }

    int width_spawn = ROOM_WIDTH;
    int height_spawn = ROOM_HEIGHT;
    map->rooms[0] = *create_room(width_spawn, height_spawn, 0, 0);
    map->rooms[0].explored = 1;
    map->room_count = 1;

    for (int i = 1; i < map->max_room; i++) {
        int width = rand() % (ROOM_WIDTH - MIN_SIZE_ROOM_WIDTH + 1) + MIN_SIZE_ROOM_WIDTH;
        int height = rand() % (ROOM_HEIGHT - MIN_SIZE_ROOM_HEIGHT + 1) + MIN_SIZE_ROOM_HEIGHT;
        int x = rand() % 3 - 1;
        int y = rand() % 3 - 1;
        map->rooms[i] = *create_room(width, height, x, y);
        map->room_count++;
    }
    return map;
}

void Display_room(PLAYER *player, MAP *map);
void display_room_view(PLAYER *player, MAP *map);
void move_player(PLAYER *player, MAP *map, int ch);

int main() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    MAP *map = create_map();
    PLAYER player = {map->rooms[0].width / 2, map->rooms[0].height / 2, 0};

    Display_room(&player, map);

    endwin();
    return 0;
}

void Display_room(PLAYER *player, MAP *map) {
    int ch;

    while (1) {
        display_room_view(player, map);

        ch = getch();
        if (ch == 27) { // Escape key
            timeout(-1);
            clear();
            refresh();
            free(map->rooms);
            free(map);
            break;
        }
        move_player(player, map, ch);

        napms(50);
    }

    endwin();
}

void display_room_view(PLAYER *player, MAP *map) {
    clear();
    ROOM *current_room = &map->rooms[player->current_room];

    for (int i = 0; i < DISPLAY_HEIGHT; i++) {
        for (int j = 0; j < DISPLAY_WIDTH; j++) {
            int y = player->y - DISPLAY_HEIGHT / 2 + i;
            int x = player->x - DISPLAY_WIDTH / 2 + j;

            int global_x = current_room->x * ROOM_WIDTH + x;
            int global_y = current_room->y * ROOM_HEIGHT + y;

            char c = ' ';
            for (int k = 0; k < map->room_count; k++) {
                ROOM *room = &map->rooms[k];
                int local_x = global_x - room->x * ROOM_WIDTH;
                int local_y = global_y - room->y * ROOM_HEIGHT;
                if (local_x >= 0 && local_x < room->width && local_y >= 0 && local_y < room->height) {
                    if (local_x == player->x && local_y == player->y && k == player->current_room) {
                        c = 'P';
                    } else {
                        c = room->data[local_y][local_x];
                    }
                }
            }
            printw("%c", c);
        }
        printw("\n");
    }
    refresh();
}

void move_player(PLAYER *player, MAP *map, int ch) {
    ROOM *room = &map->rooms[player->current_room];
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
            for (int i = 0; i < map->room_count; i++) {
                ROOM *adj_room = &map->rooms[i];
                if (adj_room->x == room->x + (new_x == 0 ? -1 : (new_x == room->width - 1 ? 1 : 0)) &&
                    adj_room->y == room->y + (new_y == 0 ? -1 : (new_y == room->height - 1 ? 1 : 0))) {
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
