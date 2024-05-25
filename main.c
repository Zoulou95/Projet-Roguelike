#include "struct.h"
#include "gen.h"
#include "menu.h"


int main() {
    FICHIER file = create_file();
    int choice = 0;
    print_menu(file, choice);
    void game_loop(PLAYER *player, MAP *map, int room_ID);

    exit(EXIT_SUCCESS);
}
