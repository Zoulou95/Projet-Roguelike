#include"gen.h"

int isSpaceAvailable(MAP *map, ROOM *new_room){
    // Check for collisions with existing rooms
    for (int i = 0; i<map->nb_rooms; i++){ // compare new_room coordinate with an existing one
        ROOM existingRoom = map->room[i];
        if (!(new_room->co_room.x + new_room->width <= existingRoom.co_room.x ||
              new_room->co_room.x >= existingRoom.co_room.x + existingRoom.width || 
              new_room->co_room.y + new_room->height <= existingRoom.co_room.y || 
              new_room->co_room.y >= existingRoom.co_room.y + existingRoom.height)){
            free(new_room); // void the new room (MODIF) marche peut-être pas (ne vide pas)
            map->nb_rooms--; // total numbers of rooms -1
            return 0; // false
        }
    }
    return 1; // true
}

ROOM *createRoom(MAP *map, ROOM *prev_room, char location){ // create a room
    ROOM *new_room=(ROOM *)malloc(sizeof(ROOM)); // memory allocation for the new room
    if (new_room==NULL) {
        perror("Memory allocation error for the new room\n");
        exit(EXIT_FAILURE);
    }
    char dir; // direction to create the new room
    new_room->doors=1;
    switch (location){ // create the door of entry of the new room
    case 'l': // new door location at the right
        new_room->door[RIGHT].co_door.x=prev_room->door[LEFT].co_door.x-1;
        new_room->door[RIGHT].co_door.y=prev_room->door[LEFT].co_door.y;
        new_room->door[RIGHT].door_ID=RIGHT+1;
        new_room->door[RIGHT].location='r';
        dir='l';
        break;
    case 'r': // new door location at the left
        new_room->door[LEFT].co_door.x=prev_room->door[RIGHT].co_door.x+1;
        new_room->door[LEFT].co_door.y=prev_room->door[RIGHT].co_door.y;
        new_room->door[LEFT].door_ID=LEFT+1;
        new_room->door[LEFT].location='l';
        dir='r';
        break;
    case 't': // new door location at the bottom
        new_room->door[BOTTOM].co_door.x=prev_room->door[TOP].co_door.x;
        new_room->door[BOTTOM].co_door.y=prev_room->door[TOP].co_door.y-1;
        new_room->door[BOTTOM].door_ID=BOTTOM+1;
        new_room->door[BOTTOM].location='b';
        dir='t';
        break;
    case 'b': // new door location at the top
        new_room->door[TOP].co_door.x=prev_room->door[BOTTOM].co_door.x;
        new_room->door[TOP].co_door.y=prev_room->door[BOTTOM].co_door.y+1;
        new_room->door[TOP].door_ID=TOP+1;
        new_room->door[TOP].location='t';
        dir='b';
        break;      
    default:
        break;
    }
    map->nb_rooms++;
    new_room->room_ID=prev_room->room_ID+1;
    new_room->width=(rand()%(MAX_SIZE_ROOM_WIDHT-MIN_SIZE_ROOM_WIDHT+1))+MIN_SIZE_ROOM_WIDHT;
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
    map->room[map->nb_rooms-1]=*new_room; // AJOUTER NOUVELLE ROOM DANS MAP OU LE FAIRE DANS LA FONCTION createLeftDoor (MODIF)
    return &new_room; // modif ça ?
}

int numberOfDoors(MAP *map){ // probability to create a new door
    switch (map->max_room-map->nb_rooms){
    case 1:
        return rand()%2;
        break;
    case 2:
        return rand()%3;
    default:
        if(map->nb_rooms<=(map->max_room)/4){
            return 2+rand()%2;
        }
        else if((map->nb_rooms<=(map->max_room)/2)&&(map->nb_rooms>(map->max_room)/4)){
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
    room->doors++;
    ROOM *new_room;
    for(int i=0; i<CHECK; i++){
        new_room=createRoom(&map, &room, 'l');
        if(isSpaceAvailable(&map, new_room)){
            return 0; // (MODIF) tester si ça pointe bien vers la new room dans la map
        }
    }
    room->door[LEFT].co_door.x=NULL; // (MODIF) marche peut etre pas
    room->door[LEFT].co_door.y=NULL;
    room->doors--;
    return 1; // failure
}

void createDoors(MAP *map, ROOM *room, char existing_door){
    int nb_doors=numberOfDoors(map);
    char possible_doors[]={'l', 'r', 't', 'b'};
    char new_doors[4]={0};
    for (int i=0; i<MAX_DOOR; i++){ // delete the existing door from the possible doors
        if (possible_doors[i]==existing_door){
            possible_doors[i]=0;
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
    //
    // funtion to verify if there is enough space for a room to be created
    //
    for(int i=0; i<MAX_DOOR; i++){ // create the new doors
        switch(new_doors[i]){
        case 'l':
            if(createLeftDoor(&map, &room->door[i])){
                possible_doors[LEFT]=0;
                new_doors[i]=0;
                // (MODIF) virer la porte ici ou dans la fonction
            }
            break;
        case 'r':
            room->door[i].co_door.x=room->co_room.x+room->width-1;
            room->door[i].co_door.y=room->co_room.y+1+rand()%(room->height-2);
            break;
        case 't':
            room->door[i].co_door.x=room->co_room.x+1+rand()%(room->width);
            room->door[i].co_door.y=room->co_room.y;
            break;
        case 'b':
            room->door[i].co_door.x=room->co_room.x+1+rand()%(room->width);
            room->door[i].co_door.y=room->co_room.y+room->height-1;
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
        exit(EXIT_FAILURE);
    }
    map->nb_rooms++;
    spawn->room_ID=0;
    spawn->co_room.x=0;
    spawn->co_room.y=0;
    spawn->width=MAX_SIZE_ROOM_WIDHT;
    spawn->height=MAX_SIZE_ROOM_HEIGHT;
    spawn->doors=4;
    for(int i=0; i<MAX_DOOR; i++){
        spawn->door[i].door_ID=i+1;
        switch(i){ // create the 4 doors for the spawn
        case 0: // left
            spawn->door[i].co_door.x=0;
            spawn->door[i].co_door.y=1+rand()%(spawn->height-2);
            spawn->door[i].location='l';
            break;
        case 1: // right
            spawn->door[i].co_door.x=spawn->width;
            spawn->door[i].co_door.y=1+rand()%(spawn->height-2);
            spawn->door[i].location='r';
            break;
        case 2: // top
            spawn->door[i].co_door.x=1+rand()%(spawn->width-2);
            spawn->door[i].co_door.y=0;
            spawn->door[i].location='t';
            break;
        case 3: // bottom
            spawn->door[i].co_door.x=1+rand()%(spawn->width-2);
            spawn->door[i].co_door.y=spawn->height;
            spawn->door[i].location='b';
            break;
        default:
            break;
        }
    }
    return spawn;
}

int main(void){
    srand(time(NULL));
    unsigned int SEED=rand();
    srand(SEED);
    int *nb_rooms=0;
    return 0;
}