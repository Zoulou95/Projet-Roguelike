#include"gen.h"
#include"menu.h"
#include"struct.h"

void give_seed(){
    echo();
    system("clear");
        int seed; 
    do{
        char buffer[MAX_CHAR];
        printw("Type your number ");
        getstr(buffer);
        seed = strtol(buffer, NULL, 10);
        clear();
        if(seed == 0){
            printw("Seed can only contain numbers (except 0)\n");
        }
    }while(seed == 0);

    srand(seed);
}

int getMaxRooms(){
    int minthRooms = MIN_ROOM;
    int maxthRooms = MAX_DOOR;
    return rand() % (maxthRooms - minthRooms + 1) + minthRooms; // number of rooms
}
MAP *create_map(){
    give_seed();
    MAP *map = (MAP *)malloc(sizeof(MAP)); // memory allocation for the map
    if (map == NULL) { // security
        perror("Memory allocation error for the map\n");
        exit(2);
    }
    map->max_room = getMaxRooms(); // max_room
    map->room = (ROOM *)malloc(sizeof(ROOM) * map->max_room); // memory allocation for the table of rooms
    if (map->room == NULL) { // security
        perror("Memory allocation error for the map rooms\n");
        free(map);
        exit(3);
    }
    PLAYER *player = (PLAYER *)malloc(sizeof(PLAYER));  // Initialize player
    if (player == NULL) {
        perror("Memory allocation error for player\n");
        free(map->room);
        free(map);
        exit(4);
    }
    player->current_room=0;
    Spawn(map);
    player->y = map->room[0].co_room.y+map->room[0].height / 2; // player spawn at the middle of the spawn
    player->x = map->room[0].co_room.x+map->room[0].width / 2;
    Display_room(player, map, 0); // (MODIF)
    
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
            free(new_room->data); // purge the new_room since it failed
            map->nb_rooms--; // total numbers of rooms -1
            return 0; // false
        }
    }
    return 1; // true
}

ROOM *createRoom(MAP *map, ROOM *prev_room, char location){ // create a room
    ROOM *new_room=&map->room[map->nb_rooms]; // memory allocation for the new room
    if (new_room==NULL) { // security
        perror("Memory allocation error for the new room\n");
        exit(5);
    }
    for(int i=0; i<MAX_DOOR; i++){ // init doors ID to -1
        new_room->door[i].closed=-1;
        new_room->door[i].track=-1;
    }
    new_room->explored=0;
    new_room->doors=1;
    map->nb_rooms++;
    new_room->room_ID=map->nb_rooms-1;
    new_room->width=(rand()%(MAX_SIZE_ROOM_WIDTH-MIN_SIZE_ROOM_WIDTH+1))+MIN_SIZE_ROOM_WIDTH;
    new_room->height=(rand()%(MAX_SIZE_ROOM_HEIGHT-MIN_SIZE_ROOM_HEIGHT+1))+MIN_SIZE_ROOM_HEIGHT;
    switch (location){ // create the position of the room depending of previous room and door location
    case 'l': // new room location at the left and first door initialized
        new_room->door[RIGHT].gap_y=1+rand()%(new_room->height-2);
        new_room->door[RIGHT].gap_x=0;
        new_room->door[RIGHT].closed=0;
        new_room->door[RIGHT].location='r';
        new_room->co_room.x=prev_room->co_room.x-new_room->width;
        new_room->co_room.y=prev_room->co_room.y+prev_room->door[LEFT].gap_y-new_room->door[RIGHT].gap_y;
        prev_room->door[LEFT].track=new_room->room_ID;
        new_room->door[RIGHT].track=prev_room->room_ID;
        initRoom(map, new_room->room_ID, new_room->height, new_room->width, new_room->door[RIGHT].location);
        break;
    case 'r': // new room location at the right and first door initialized
        new_room->door[LEFT].gap_y=1+rand()%(new_room->height-2);
        new_room->door[LEFT].gap_x=0;
        new_room->door[LEFT].closed=0;
        new_room->door[LEFT].location='l';
        new_room->co_room.x=prev_room->co_room.x+prev_room->width;
        new_room->co_room.y=prev_room->co_room.y+prev_room->door[RIGHT].gap_y-new_room->door[LEFT].gap_y;
        prev_room->door[RIGHT].track=new_room->room_ID;
        new_room->door[LEFT].track=prev_room->room_ID;
        initRoom(map, new_room->room_ID, new_room->height, new_room->width, new_room->door[LEFT].location);
        break;
    case 't': // new room location at the top and first door initialized
        new_room->door[BOTTOM].gap_x=1+rand()%(new_room->width-2);
        new_room->door[BOTTOM].gap_y=0;
        new_room->door[BOTTOM].closed=0;
        new_room->door[BOTTOM].location='b';
        new_room->co_room.x=prev_room->co_room.x+prev_room->door[TOP].gap_x-new_room->door[BOTTOM].gap_x;
        new_room->co_room.y=prev_room->co_room.y-new_room->height;
        prev_room->door[TOP].track=new_room->room_ID;
        new_room->door[BOTTOM].track=prev_room->room_ID;
        initRoom(map, new_room->room_ID, new_room->height, new_room->width, new_room->door[BOTTOM].location);
        break;
    case 'b': // new room location at the bottom and first door initialized
        new_room->door[TOP].gap_x=1+rand()%(new_room->width-2);
        new_room->door[TOP].gap_y=0;
        new_room->door[TOP].closed=0;
        new_room->door[TOP].location='t';
        new_room->co_room.x=prev_room->co_room.x+prev_room->door[BOTTOM].gap_x-new_room->door[TOP].gap_x;
        new_room->co_room.y=prev_room->co_room.y+new_room->height;
        prev_room->door[BOTTOM].track=new_room->room_ID;
        new_room->door[TOP].track=prev_room->room_ID;
        initRoom(map, new_room->room_ID, new_room->height, new_room->width, new_room->door[TOP].location);
        break;
    default:
        break;
    }
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
    room->door[LEFT].gap_y=1+rand()%(room->height-2);
    room->door[LEFT].gap_x=0;
    ROOM *new_room;
    for(int i=0; i<CHECK; i++){
        new_room=createRoom(map, room, 'l');
        if(isSpaceAvailable(map, new_room)){
            room->doors++;
            room->door[LEFT].closed=1;
            room->door[LEFT].location='l';
            room->data[room->door[LEFT].gap_y][0]='d';
            return 1; // success
        }
    }
    return 0; // failure
}
int createRightDoor(MAP *map, ROOM *room){
    room->door[RIGHT].gap_y=1+rand()%(room->height-2);
    room->door[RIGHT].gap_x=0;
    ROOM *new_room;
    for(int i=0; i<CHECK; i++){
        new_room=createRoom(map, room, 'r');
        if(isSpaceAvailable(map, new_room)){
            room->doors++;
            room->door[RIGHT].closed=1;
            room->door[RIGHT].location='r';
            room->data[room->door[RIGHT].gap_y][room->width-1]='d';
            return 1; // success
        }
    }
    return 0; // failure
}
int createTopDoor(MAP *map, ROOM *room){
    room->door[TOP].gap_x=1+rand()%(room->width-2);
    room->door[TOP].gap_y=0;
    ROOM *new_room;
    for(int i=0; i<CHECK; i++){
        new_room=createRoom(map, room, 't');
        if(isSpaceAvailable(map, new_room)){
            room->doors++;
            room->door[TOP].closed=1;
            room->door[TOP].location='t';
            room->data[0][room->door[TOP].gap_x]='d';
            return 1; // success
        }
    }
    return 0; // failure
}
int createBottomDoor(MAP *map, ROOM *room){
    room->door[BOTTOM].gap_x=1+rand()%(room->width-2);
    room->door[BOTTOM].gap_y=0;
    ROOM *new_room;
    for(int i=0; i<CHECK; i++){
        new_room=createRoom(map, room, 'b');
        if(isSpaceAvailable(map, new_room)){
            room->doors++;
            room->door[BOTTOM].closed=1;
            room->door[BOTTOM].location='b';
            room->data[room->height-1][room->door[BOTTOM].gap_x]='d';
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
            break;
        }
    }
    for (int i=0; i<nb_doors; i++){
        int wall_index;
        do{
            wall_index=rand()%MAX_DOOR; // choose a random wall for the new door
        }while(possible_doors[wall_index]==0); // excluding the wall with an existing door

        new_doors[i]=possible_doors[wall_index]; // add the new door
        possible_doors[wall_index]=0; // delete the new door from the possible doors
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

void Spawn(MAP *map){ // create the spawn of the map
    ROOM *spawn = &map->room[0];
    spawn->width=MAX_SIZE_ROOM_WIDTH*2-1;
    spawn->height=MAX_SIZE_ROOM_HEIGHT*2-1;
    map->nb_rooms=1;
    spawn->room_ID=0;
    spawn->co_room.x=SPAWN_X;
    spawn->co_room.y=SPAWN_Y;
    spawn->doors=MAX_DOOR;
    spawn->explored=1;
    initRoom(map, spawn->room_ID, spawn->height, spawn->width, 0);
    for(int i=0; i<MAX_DOOR; i++){
        spawn->door[i].closed=1;
        switch(i){ // create the 4 doors for the spawn
        case 0: // left
            spawn->door[i].location='l';
            spawn->door[i].gap_y=spawn->height/2;
            spawn->door[i].gap_x=0;
            createRoom(map, spawn, spawn->door[i].location);
            break;
        case 1: // right
            spawn->door[i].location='r';
            spawn->door[i].gap_y=spawn->height/2;
            spawn->door[i].gap_x=0;
            createRoom(map, spawn, spawn->door[i].location);
            break;
        case 2: // top
            spawn->door[i].location='t';
            spawn->door[i].gap_x=spawn->width/2;
            spawn->door[i].gap_y=0;
            createRoom(map, spawn, spawn->door[i].location);
            break;
        case 3: // bottom
            spawn->door[i].location='b';
            spawn->door[i].gap_x=spawn->width/2;
            spawn->door[i].gap_y=0;
            createRoom(map, spawn, spawn->door[i].location);
            break;
        default:
            break;
        }
    }
}


void initRoom(MAP *map, int room_ID, int height, int width, char location){
    map->room[room_ID].data = (char **)malloc(height * sizeof(char *));
    if (map->room[room_ID].data == NULL){
        perror("Memory allocation error for spawn data");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < height; i++) {
        map->room[room_ID].data[i] = (char *)malloc(width * sizeof(char));
        if (map->room[room_ID].data[i] == NULL) {
            perror("Memory allocation error for row data");
            // Free previously allocated memory before exiting
            for (int k = 0; k < i; k++) {
                free(map->room[room_ID].data[k]);
            }
            free(map->room[room_ID].data);
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < width; j++) {
            map->room[room_ID].data[i][j] = ' ';
        }
    }
    if(room_ID==0){ // init spawn
        for (int y=0; y < height; y++){
            for (int x=0; x < width; x++) {
                if((y==height/2 && x==0) || // create the left door
                (y==height/2 && x==width-1) || // create the right door
                (y==0 && x==width/2) || //  create the top door
                (y==height-1 && x==width/2)){ // create the bottom door
                    map->room[room_ID].data[y][x] = 'd';
                }
                else if (y == 0 || y == height-1 || x == 0 || x == width-1){
                    map->room[room_ID].data[y][x] = '#';// room borders
                }
                else {
                    map->room[room_ID].data[y][x] = ' '; // blank space in the room
                }
            }
        }
    }
    else if (room_ID>0){ // init when no overlay failure
        for (int y=0; y < height; y++){
            for (int x=0; x < width; x++) { //
                if (y == 0 || y == height-1 || x == 0 || x == width-1){
                    map->room[room_ID].data[y][x] = '#';// room borders
                }
                else {
                    map->room[room_ID].data[y][x] = ' '; // blank space in the room
                }
            }
        }
        switch(location){
        case 'l':
            map->room[room_ID].data[map->room[room_ID].door[LEFT].gap_y][0] = 'd';
            break;
        case 'r':
            map->room[room_ID].data[map->room[room_ID].door[RIGHT].gap_y][width-1] = 'd';
            break;
        case 't':
            map->room[room_ID].data[0][map->room[room_ID].door[TOP].gap_x] = 'd';
            break;
        case 'b':
            map->room[room_ID].data[height-1][map->room[room_ID].door[BOTTOM].gap_x] = 'd';
            break;
        default:
            break;
        }
    }
    else{
        perror("Room_ID invalid");
        exit(EXIT_FAILURE);
    }
}

void Display_room(PLAYER *player, MAP *map, int room_ID){
    int ch;
    if (!map->room[room_ID].data) {
        printw("disp error\n");
        refresh();
        exit(-5);
    }
    while (1) { // Boucle de jeu
        display_room_view(player, map, room_ID); // vision 11x11 (dans gen.h modifiable)
        ch = getch(); //prend un charactère
        if (ch == 27) { // escape pour quitter
            clear(); //clear le terminal sinon ça se superpose avec le menu
            refresh(); //rafraîchit le terminal sinon ça change pas
            free(map->room); //libère la mémoire des rooms car plus besoin
            free(map);  //libère la mémoire de la carte
            FICHIER file = create_file(); //recréer un fichier (j'avais la flemme de changer)
            print_menu(file); //affiche menu
            break;
        }
        move_player(player, map, ch); // déplace le joueur avec la charactère
    }

    // Terminer ncurses
    endwin();
}


void display_room_view(PLAYER *player, MAP *map, int room_ID) {
    // Vérifier si le joueur, la carte ou les données de la pièce sont nuls
    if (!map->room[room_ID].data) {
        printw("Error: Invalid map data.\n");
        refresh();
        return;
    }
    clear();
    ROOM *current_room = &map->room[player->current_room];

    int term_height, term_width;
    getmaxyx(stdscr, term_height, term_width);

    int start_y = (term_height - DISPLAY_HEIGHT) / 2;
    int start_x = (term_width - DISPLAY_WIDTH) / 2;
    

    int offset_x = player->x - DISPLAY_WIDTH / 2;
    int offset_y = player->y - DISPLAY_HEIGHT / 2;

    for (int i = 0; i < DISPLAY_HEIGHT; i++) {
        for (int j = 0; j < DISPLAY_WIDTH; j++) {
            int global_x = current_room->co_room.x * current_room->width + offset_x + j;
            int global_y = current_room->co_room.y * current_room->height + offset_y + i;

            char c = ' ';
            for (int k = 0; k < map->nb_rooms; k++) {
                ROOM *room = &map->room[k];
                int local_x = global_x - room->co_room.x * room->width;
                int local_y = global_y - room->co_room.y * room->height;
                if (local_x >= 0 && local_x < room->width && local_y >= 0 && local_y < room->height) {
                    if (local_x == player->x && local_y == player->y && k == player->current_room) {
                        c = 'P';
                    } else {
                        c = room->data[local_y][local_x];
                    }
                }
            }
            mvprintw(start_y + i, start_x + j, "%c", c);
            mvprintw(1,1,"%d %d",player->x, player->y);
        }
    }
    refresh();
}


// void display_room_view(PLAYER *player, MAP *map, int room_ID) {
//     // Vérifier si le joueur, la carte ou les données de la pièce sont nuls
//     if (!map->room[room_ID].data) {
//         printw("Error: Invalid map data.\n");
//         refresh();
//         return;
//     }
//     clear();
//     ROOM *current_room = &map->room[room_ID];
//     int width = current_room->width;
//     int height = current_room->height;

//     int offset_x = player->x - DISPLAY_WIDTH / 2;
//     int offset_y = player->y - DISPLAY_HEIGHT / 2;

//     for (int i = 0; i < DISPLAY_HEIGHT; i++) {
//         for (int j = 0; j < DISPLAY_WIDTH; j++) {
//             int y = offset_y + i;
//             int x = offset_x + j;
//             if (y >= 0 && y < height && x >= 0 && x < width) {
//                 if (y == player->y && x == player->x) {
//                     printw("P"); // Afficher le joueur
//                 } else {
//                     // Vérifiez que vous accédez bien à une zone valide de la carte
//                     if (x >= 0 && x < width && y >= 0 && y < height) {
//                         printw("%c", current_room->data[y][x]); // Afficher les bordures et les espaces vides
//                     } else {
//                         printw(" ");
//                     }
//                 }
//             } else {
//                 printw(" "); // Afficher un espace pour les cases hors de la salle
//             }
//         }
//         printw("\n");
//     }
//     refresh(); // Rafraîchir l'affichage
// }