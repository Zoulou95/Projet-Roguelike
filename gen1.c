#include <stdlib.h>
#include <ncurses.h>

#define MAX_CHAR 100
#define MAX_ROOM 20
#define MIN_ROOM 5
#define MAX_HEIGHT_ROOM 11
#define MAX_WIDTH_ROOM 11
#define MIN_HEIGHT_ROOM 3
#define MIN_WIDTH_ROOM 3

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
    ROOM *room;
    int room_count;
    int roomID;
} MAP;

typedef struct {
    int x;
    int y;
    int current_room_index;
} PLAYER;

///////////////////////////////////////////////////////////////////////////////////////

int give_seed();
void gameloop(MAP *map, ROOM *room);
int GetMaxRoom(MAP *map);
MAP *create_map();
ROOM *create_room(MAP *map);
void get_random_room_size(MAP *map, ROOM *room);
void initRoom(MAP *map, ROOM *room, int height, int width);
void display_room(MAP* map, ROOM *room, int height, int width);

///////////////////////////////////////////////////////////////////////////////////////

int main(void){
    initscr();
    cbreak();
    curs_set(0);

    int seed = give_seed();
    srand(seed);
    noecho();

    MAP *map = create_map();
    ROOM *room = create_room(map);

    gameloop(map,room);
    
    free(map->room);
    free(map);
    free(room);

    endwin();
    exit(EXIT_SUCCESS);
}

int give_seed(){
    system("clear");
    
        int seed; 
    do{
        char buffer[MAX_CHAR];
        printw("Type your number ");
        getstr(buffer);
        seed = strtol(buffer, NULL, 10);
        clear();
        if(seed == 0){
            printw("Write a number and seed = 0 doesn't exist\n");
        }
    }while(seed == 0);

    return seed;
}

void gameloop(MAP *map, ROOM *room){
    int ch;
    do{
        clear();
        initRoom(map, room, room[map->roomID].height, room[map->roomID].width);
        display_room(map, room, room[map->roomID].height, room[map->roomID].width);
        mvprintw(room[map->roomID].height, 0,"(%d, %d), current_room : %d", room[map->roomID].height, room[map->roomID].width, map->roomID);
        map->roomID++;
        room = create_room(map);

    }while((ch = getch()) != 27);
}

int GetMaxRoom(MAP *map){
    map->max_room = rand() % ((MAX_ROOM + 1) - MIN_ROOM) + MIN_ROOM;
    return map->max_room;
}

MAP *create_map() {
    MAP *map = (MAP*)malloc(sizeof(MAP));
    if(map == NULL){
        perror("memory allocation error for the map\n");
    }
    GetMaxRoom(map);
    printw("The map contain %d max room", map->max_room);
    map->room = (ROOM*)malloc(sizeof(ROOM) * map->max_room);
    if(map->room == NULL){
        perror("memory allocation error for the max room of the map\n");
    }
    map->roomID = 0;
    refresh();
    clear();
    return map;
}

ROOM *create_room(MAP *map){
    ROOM *room = (ROOM*)malloc(sizeof(ROOM));
    if(room == NULL){
        perror("memory allocation error for the map\n");
    }
    if(map->roomID == 0){
        room[map->roomID].height = MAX_HEIGHT_ROOM;
        room[map->roomID].width = MAX_WIDTH_ROOM;
    }
    else{
        get_random_room_size(map, room);
    }

    room->explored = 0;
    return room;
}

void get_random_room_size(MAP *map, ROOM *room) {
    room[map->roomID].height = rand()% (MAX_HEIGHT_ROOM - MIN_HEIGHT_ROOM + 1) + MIN_HEIGHT_ROOM;
    room[map->roomID].width = rand()% (MAX_WIDTH_ROOM - MIN_WIDTH_ROOM + 1) + MIN_WIDTH_ROOM;
}

void initRoom(MAP *map, ROOM *room, int height, int width){
    room->data = (char **)malloc(height * sizeof(char *));
    for (int i = 0; i < height; i++) {
        room->data[i] = (char *)malloc(width * sizeof(char));
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                room[map->roomID].data[i][j] = '#'; // Borders
            } else {
                room[map->roomID].data[i][j] = ' '; // Empty space
            }
        }
    }
}

// void initDoor(MAP *map, ROOM *room, int height, int width){
//     if(map->roomID = 0){
//         room[]
//     }
// }

void display_room(MAP* map, ROOM *room, int height, int width){
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            printw("%c", room->data[i][j]);
        }
        printw("\n");
    }
    refresh();
}