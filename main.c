#include "struct.h"
#include "gen.h"
#include "menu.h"


int main() {
    FICHIER file = create_file();
    int choice = 0;
    PLAYER *player = (PLAYER *)malloc(sizeof(PLAYER));  // Initialize player
    if (player == NULL) {
        perror("Memory allocation error for player\n");
        exit(4);
    }
    MAP *map = (MAP *)malloc(sizeof(MAP)); // memory allocation for the map
    if (map == NULL) { // security
        perror("Memory allocation error for the map\n");
        exit(2);
    }
    print_menu(file, choice, player, map);


    exit(EXIT_SUCCESS);
}