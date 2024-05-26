#include "struct.h"
#include "gen.h"
#include "menu.h"


int main() {
    FICHIER file = create_file();
    print_menu(file);
    void game_loop(PLAYER *player, MAP *map, int room_ID);

    exit(EXIT_SUCCESS);
}
