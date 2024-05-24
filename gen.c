#include"gen.h"
#include"menu.h"
#include"struct.h"

// void give_seed(int *seed) { // scanf for the seed
//     echo(); // disable hidden text
//     char str[100];
//     printw("Entrez la seed : ");
//     refresh(); // show text
//     getstr(str); // read the seed
//     *seed = atoi(str); // convert everything in int
//     noecho(); // enable hidden text
// }

int getMaxRooms() {
    int minthRooms = MIN_ROOM;
    int maxthRooms = MAX_DOOR;
    return rand() % (maxthRooms - minthRooms + 1) + minthRooms; // number of rooms
}
MAP *create_map(){
    // int seed;
    // give_seed(&seed); // ask the seed
    // srand(seed); // init random on the seed
    MAP *map = (MAP *)malloc(sizeof(MAP)); // memory allocation for the map
    if (map == NULL) { // security
        perror("Memory allocation error for the map\n");
        exit(2);
    }
    map->max_room = getMaxRooms(); // max_room
    map->room = (ROOM *)malloc(sizeof(ROOM) * map->max_room); // memory allocation for the table of rooms
    if (map->room == NULL) { // security
        perror("Memory allocation error for the map rooms\n");
        exit(3);
    }
    PLAYER *player;
    player->current_room=0;
    map->room[0]=*Spawn(map);
    Display_room(player, map, 0, 0);
    map->room[0].data[map->room[0].door[LEFT].co_door.y][map->room[0].door[LEFT].co_door.x]='d';
    map->room[0].data[map->room[0].door[RIGHT].co_door.y][map->room[0].door[RIGHT].co_door.x]='d';
    map->room[0].data[map->room[0].door[TOP].co_door.y][map->room[0].door[TOP].co_door.x]='d';
    map->room[0].data[map->room[0].door[BOTTOM].co_door.y][map->room[0].door[BOTTOM].co_door.x]='d';
    return map;
}

int isSpaceAvailable(MAP *map, ROOM *new_room){
    // Check for collisions with existing rooms
    for (int i = 0; i<map->nb_rooms; i++){ // compare new_room coordinate with an existing one
        ROOM existingRoom = map->room[i];
        if (!(new_room->co_room.x + new_room->width <= existingRoom.co_room.x ||
              new_room->co_room.x >= existingRoom.co_room.x + existingRoom.width || 
              new_room->co_room.y + new_room->height <= existingRoom.co_room.y || 
              new_room->co_room.y >= existingRoom.co_room.y + existingRoom.height)){
            free(new_room); // purge the new_room since it failed
            map->nb_rooms--; // total numbers of rooms -1
            return 0; // false
        }
    }
    return 1; // true
}

ROOM *createRoom(MAP *map, ROOM *prev_room, char location){ // create a room
    ROOM *new_room=(ROOM *)malloc(sizeof(ROOM)); // memory allocation for the new room
    if (new_room==NULL) { // security
        perror("Memory allocation error for the new room\n");
        exit(4);
    }
    new_room->data = (char **)malloc(new_room->height * sizeof(char *));
    if (new_room->data == NULL) {
        perror("Memory allocation error for room data");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < new_room->height; i++) {
        new_room->data[i] = (char *)malloc(new_room->width * sizeof(char));
        if (new_room->data[i] == NULL) {
            perror("Memory allocation error for room data");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < new_room->width; j++) {
            new_room->data[i][j] = ' ';
        }
    }
    for(int i=0; i<MAX_DOOR; i++){ // init doors ID to -1
        new_room->door[i].closed=-1;
    }
    char dir; // direction to create the new room
    new_room->doors=1;
    switch (location){ // create the door of entry of the new room
    case 'l': // new door location at the right
        new_room->door[RIGHT].co_door.x=prev_room->door[LEFT].co_door.x-1;
        new_room->door[RIGHT].co_door.y=prev_room->door[LEFT].co_door.y;
        new_room->door[RIGHT].closed=1;
        new_room->door[RIGHT].location='r';
        dir='l';
        break;
    case 'r': // new door location at the left
        new_room->door[LEFT].co_door.x=prev_room->door[RIGHT].co_door.x+1;
        new_room->door[LEFT].co_door.y=prev_room->door[RIGHT].co_door.y;
        new_room->door[LEFT].closed=1;
        new_room->door[LEFT].location='l';
        dir='r';
        break;
    case 't': // new door location at the bottom
        new_room->door[BOTTOM].co_door.x=prev_room->door[TOP].co_door.x;
        new_room->door[BOTTOM].co_door.y=prev_room->door[TOP].co_door.y-1;
        new_room->door[BOTTOM].closed=1;
        new_room->door[BOTTOM].location='b';
        dir='t';
        break;
    case 'b': // new door location at the top
        new_room->door[TOP].co_door.x=prev_room->door[BOTTOM].co_door.x;
        new_room->door[TOP].co_door.y=prev_room->door[BOTTOM].co_door.y+1;
        new_room->door[TOP].closed=1;
        new_room->door[TOP].location='t';
        dir='b';
        break;      
    default:
        break;
    }
    map->nb_rooms++;
    new_room->room_ID=prev_room->room_ID+1;
    new_room->width=(rand()%(MAX_SIZE_ROOM_WIDTH-MIN_SIZE_ROOM_WIDTH+1))+MIN_SIZE_ROOM_WIDTH;
    new_room->height=(rand()%(MAX_SIZE_ROOM_HEIGHT-MIN_SIZE_ROOM_HEIGHT+1))+MIN_SIZE_ROOM_HEIGHT;
    int gap; // variable for the position of the door in the wall
    switch (dir){ // create the position of the room depending of the door
    case 'l': // new room location at the left
        gap=1+rand()%(new_room->height-2);
        new_room->co_room.x=new_room->door[RIGHT].co_door.x-new_room->width;
        new_room->co_room.y=new_room->door[RIGHT].co_door.y-gap;
        break;
    case 'r': // new room location at the right
        gap=1+rand()%(new_room->height-2);
        new_room->co_room.x=new_room->door[LEFT].co_door.x;
        new_room->co_room.y=new_room->door[LEFT].co_door.y-gap;
        break;
    case 't': // new room location at the top
        gap=1+rand()%(new_room->width-2);
        new_room->co_room.x=new_room->door[BOTTOM].co_door.x-gap;
        new_room->co_room.y=new_room->door[BOTTOM].co_door.y-new_room->height;
        break;
    case 'b': // new room location at the bottom
        gap=1+rand()%(new_room->width-2);
        new_room->co_room.x=new_room->door[TOP].co_door.x-gap;
        new_room->co_room.y=new_room->door[TOP].co_door.y;
        break;
    default:
        break;
    }
    map->room[map->nb_rooms-1]=*new_room;
    return new_room;
}

int numberOfDoors(MAP *map){ // probability to create a new door
    switch (map->max_room-map->nb_rooms){
    case 0:
        return 0;
        break;
    case 1:
        return rand()%2;
        break;
    case 2:
        return rand()%3;
    default:
        if(map->nb_rooms<=(map->max_room)/3){
            return 2+rand()%2;
        }
        else if((map->nb_rooms<=(map->max_room)/2)&&(map->nb_rooms>(map->max_room)/3)){
            return 1+rand()%3;
        }
        else{
            return rand()%4;
        }
        break;
    }
}

int createLeftDoor(MAP *map, ROOM *room){
    room->door[LEFT].co_door.x=room->co_room.x;
    room->door[LEFT].co_door.y=room->co_room.y+1+rand()%(room->height-2);
    ROOM *new_room;
    for(int i=0; i<CHECK; i++){
        new_room=createRoom(map, room, 'l');
        if(isSpaceAvailable(map, new_room)){
            room->doors++;
            room->door[LEFT].closed=1;
            room->door[LEFT].location='l';
            room->data[room->door[LEFT].co_door.y][room->door[LEFT].co_door.x]='d';
            return 1; // success
        }
    }
    return 0; // failure
}
int createRightDoor(MAP *map, ROOM *room){
    room->door[RIGHT].co_door.x=room->co_room.x+room->width-1;
    room->door[RIGHT].co_door.y=room->co_room.y+1+rand()%(room->height-2);
    ROOM *new_room;
    for(int i=0; i<CHECK; i++){
        new_room=createRoom(map, room, 'r');
        if(isSpaceAvailable(map, new_room)){
            room->doors++;
            room->door[RIGHT].closed=1;
            room->door[RIGHT].location='r';
            room->data[room->door[RIGHT].co_door.y][room->door[RIGHT].co_door.x]='d';
            return 1; // success
        }
    }
    return 0; // failure
}
int createTopDoor(MAP *map, ROOM *room){
    room->door[TOP].co_door.x=room->co_room.x+1+rand()%(room->width);
    room->door[TOP].co_door.y=room->co_room.y;
    ROOM *new_room;
    for(int i=0; i<CHECK; i++){
        new_room=createRoom(map, room, 't');
        if(isSpaceAvailable(map, new_room)){
            room->doors++;
            room->door[TOP].closed=1;
            room->door[TOP].location='t';
            room->data[room->door[TOP].co_door.y][room->door[TOP].co_door.x]='d';
            return 1; // success
        }
    }
    return 0; // failure
}
int createBottomDoor(MAP *map, ROOM *room){
    room->door[BOTTOM].co_door.x=room->co_room.x+1+rand()%(room->width);
    room->door[BOTTOM].co_door.y=room->co_room.y+room->height-1;
    ROOM *new_room;
    for(int i=0; i<CHECK; i++){
        new_room=createRoom(map, room, 'b');
        if(isSpaceAvailable(map, new_room)){
            room->doors++;
            room->door[BOTTOM].closed=1;
            room->door[BOTTOM].location='b';
            room->data[room->door[BOTTOM].co_door.y][room->door[BOTTOM].co_door.x]='d';
            return 1; // success
        }
    }
    return 0; // failure
}

void createDoors(MAP *map, ROOM *room, char existing_door){
    int nb_doors=numberOfDoors(map);
    char possible_doors[]={'l', 'r', 't', 'b'};
    char new_doors[4]={0};
    for (int i=0; i<MAX_DOOR; i++){ // delete the existing door from the possible doors
        if (possible_doors[i]==existing_door){
            possible_doors[i]=0;
            room->door[i].closed=0;
            
        }
    }
    for (int i=0; i<nb_doors; i++){
        int chosen;
        do{
            chosen=rand()%MAX_DOOR; // choose a random wall for the new door
        }while(possible_doors[chosen]==0); // excluding the wall with an existing door

        new_doors[i]=possible_doors[chosen]; // add the new door
        possible_doors[chosen]=0; // delete the new door from the possible doors
    }
    for(int i=0; i<MAX_DOOR; i++){ // create the new doors
        switch(new_doors[i]){
        case 'l':
            if(!createLeftDoor(map, room)){ // in case the door can't be created
                possible_doors[LEFT]=0;
                new_doors[i]=0;
            }
            break;
        case 'r':
            if(!createRightDoor(map, room)){ // in case the door can't be created
                possible_doors[RIGHT]=0;
                new_doors[i]=0;
            }
            break;
        case 't':
            if(!createTopDoor(map, room)){ // in case the door can't be created
                possible_doors[TOP]=0;
                new_doors[i]=0;
            }
            break;
        case 'b':
            if(!createBottomDoor(map, room)){ // in case the door can't be created
                possible_doors[BOTTOM]=0;
                new_doors[i]=0;
            }
            break;
        default:
            break;
        }
    }
}

ROOM *Spawn(MAP *map){ // create the spawn of the map
    ROOM *spawn=malloc(sizeof(ROOM)); // memory allocation for the spawn
    if(spawn==NULL){
        perror("Memory allocation error for the spawn\n");
        exit(5);
    }
    spawn->data = (char **)malloc(spawn->height * sizeof(char *));
    if (spawn->data == NULL){
        perror("Memory allocation error for spawn data");
        exit(EXIT_FAILURE);
    for (int i = 0; i < spawn->height; i++) {
        spawn->data[i] = (char *)malloc(spawn->width * sizeof(char));
        if (spawn->data[i] == NULL) {
            perror("Memory allocation error for spawn data");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < spawn->width; j++) {
            spawn->data[i][j] = ' ';
        }
    }
    map->nb_rooms=1;
    spawn->room_ID=0;
    spawn->co_room.x=SPAWN_X;
    spawn->co_room.y=SPAWN_Y;
    spawn->width=MAX_SIZE_ROOM_WIDTH*2-1;
    spawn->height=MAX_SIZE_ROOM_HEIGHT*2-1;
    spawn->doors=MAX_DOOR;
    spawn->explored=1;
    for(int i=0; i<MAX_DOOR; i++){
        spawn->door[i].closed=1;
        switch(i){ // create the 4 doors for the spawn
        case 0: // left
            spawn->door[i].co_door.x=spawn->co_room.x;
            spawn->door[i].co_door.y=spawn->co_room.y+MAX_SIZE_ROOM_HEIGHT;
            spawn->door[i].location='l';
            createRoom(map, spawn, spawn->door[i].location);
            break;
        case 1: // right
            spawn->door[i].co_door.x=spawn->co_room.x+spawn->width;
            spawn->door[i].co_door.y=spawn->co_room.y+MAX_SIZE_ROOM_HEIGHT;
            spawn->door[i].location='r';
            createRoom(map, spawn, spawn->door[i].location);
            break;
        case 2: // top
            spawn->door[i].co_door.x=spawn->co_room.x+MAX_SIZE_ROOM_WIDTH;
            spawn->door[i].co_door.y=spawn->co_room.y;
            spawn->door[i].location='t';
            createRoom(map, spawn, spawn->door[i].location);
            break;
        case 3: // bottom
            spawn->door[i].co_door.x=spawn->co_room.x+MAX_SIZE_ROOM_WIDTH;
            spawn->door[i].co_door.y=spawn->co_room.y+spawn->height;
            spawn->door[i].location='b';
            createRoom(map, spawn, spawn->door[i].location);
            break;
        default:
            break;
        }
    }
    return spawn;
}


// void Display_room(PLAYER *player, MAP *map, int room_ID, char location) {
//     int ch;

//     while (1) {
//         display_visible_rooms(player, map);

//         ch = getch();
//         if (ch == 27) { // Escape key
//             timeout(-1);
//             clear();
//             refresh();
//             free(map->room);
//             free(map);
//             break;
//         }
//         move_player(player, map, ch);

//         napms(50);
//     }

//     endwin();
// }

// void display_visible_rooms(PLAYER *player, MAP *map) {
//     clear();
//     player->current_room=0;
//     ROOM *current_room = &map->room[player->current_room];

//     current_room->co_room.x=5;
//     current_room->co_room.y=6;
    
//     int term_height, term_width;
//     getmaxyx(stdscr, term_height, term_width);

//     int start_y = (term_height - DISPLAY_HEIGHT) / 2;
//     int start_x = (term_width - DISPLAY_WIDTH) / 2;

//     int offset_x = player->x - DISPLAY_WIDTH / 2;
//     int offset_y = player->y - DISPLAY_HEIGHT / 2;

//     for (int i = 0; i < DISPLAY_HEIGHT; i++) {
//         for (int j = 0; j < DISPLAY_WIDTH; j++) {
//             int global_x = current_room->co_room.x * MAX_SIZE_ROOM_WIDTH + offset_x + j;
//             int global_y = current_room->co_room.y * MAX_SIZE_ROOM_HEIGHT + offset_y + i;
//             char c = ' ';
//             for (int k = 0; k < map->nb_rooms; k++) {
//                 ROOM *room = &map->room[k];
//                 int local_x = global_x - room->co_room.x * MAX_SIZE_ROOM_WIDTH;
//                 int local_y = global_y - room->co_room.y * MAX_SIZE_ROOM_HEIGHT;
//                 if (local_x >= 0 && local_x < room->width && local_y >= 0 && local_y < room->height) {
//                     if (local_x == player->x && local_y == player->y && k == player->current_room) {
//                         c = 'P';
//                     } else {
//                         c = room->data[local_y][local_x];
//                     }
//                 }
//             }
//             mvprintw(start_y + i, start_x + j, "%c", c);
//         }
//     }
//     refresh();
// }




void Display_room(PLAYER *player, MAP *map, int room_ID, char location){ // room to display; location: door where you came from
    int width = map->room[room_ID].width;
    int height = map->room[room_ID].height;
    map->room[room_ID].data[height][width];
    int ch;

    // init the room
    for (int co_y=0; co_y < height; co_y++){
        for (int co_x=0; co_x < width; co_x++) {
            if (co_y == 0 || co_y == height-1 || co_x == 0 || co_x == width-1){
                map->room[room_ID].data[co_y][co_x] = '#'; // room borders
            }
            else {
                map->room[room_ID].data[co_y][co_x] = ' '; // blank space in the room
            }
        }
    }

    if(room_ID==0){
        player->y = map->room[room_ID].co_room.y+map->room[room_ID].height / 2; // player spawn at the middle of the spawn
        player->x = map->room[room_ID].co_room.x+map->room[room_ID].width / 2;
    }
    else{
        switch(location){ // create new door
        case 'l': // at the right
            map->room[room_ID].data[map->room[room_ID].door[RIGHT].co_door.y][map->room[room_ID].door[RIGHT].co_door.x]='d';
            map->room[room_ID].door[RIGHT].closed=0;
            createDoors(map, &map->room[room_ID], 'r');
            break;
        case 'r': // at the left
            map->room[room_ID].data[map->room[room_ID].door[LEFT].co_door.y][map->room[room_ID].door[LEFT].co_door.x]='d';
            map->room[room_ID].door[LEFT].closed=0;
            createDoors(map, &map->room[room_ID], 'l');
            break;
        case 't': // at the bottom
            map->room[room_ID].data[map->room[room_ID].door[BOTTOM].co_door.y][map->room[room_ID].door[BOTTOM].co_door.x]='d';
            map->room[room_ID].door[BOTTOM].closed=0;
            createDoors(map, &map->room[room_ID], 'b');
            break;
        case 'b': // at the top
            map->room[room_ID].data[map->room[room_ID].door[TOP].co_door.y][map->room[room_ID].door[TOP].co_door.x]='d';
            map->room[room_ID].door[TOP].closed=0;
            createDoors(map, &map->room[room_ID], 't');
            break;
        default:
            break;
        }
    }
    
    while (1) { // Boucle de jeu
        display_room_view(player, map, width, height, room_ID); // vision 11x11 (dans gen.h modifiable)
        ch = getch(); //prend un charactère
        move_player(player, map, ch); // déplace le joueur avec la charactère
        if (ch == 27) { // escape pour quitter
            clear(); //clear le terminal sinon ça se superpose avec le menu
            refresh(); //rafraîchit le terminal sinon ça change pas
            free(map->room); //libère la mémoire des rooms car plus besoin
            free(map);  //libère la mémoire de la carte
            FICHIER file = create_file(); //recréer un fichier (j'avais la flemme de changer)
            print_menu(file, 0); //affiche menu
            break;
        }
    }

    // Terminer ncurses
    endwin();
}

void display_room_view(PLAYER *player, MAP *map, int width, int height, int room_ID) { //fonction pour la vision

    clear();
    for (int i = 0; i < DISPLAY_HEIGHT; i++) { //affiche la longueur
        for (int j = 0; j < DISPLAY_WIDTH; j++) { //affiche la largeur
            int y = player->y - DISPLAY_HEIGHT / 2 + i; //les coordonnées y pour que ça soit centré
            int x = player->x - DISPLAY_WIDTH / 2 + j; //les coordonnées x pour que ça soit centré
            if (y >= 0 && y < height && x >= 0 && x < width) { // boucle pour afficher le tout
                if (y == player->y && x == player->x) {
                    printw("P"); // Afficher le joueur
                }
                else {
                    printw("%c", map->room[room_ID].data[y][x]); // Afficher les bordures et les espaces vides
                }
            } else {
                printw(" "); // Afficher un espace pour les cases hors de la salle
            }
        }
        printw("\n");
    }
    refresh(); // Rafraîchir l'affichage
}