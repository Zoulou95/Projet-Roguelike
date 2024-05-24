#include "struct.h"
#include "gen.h"
#include "menu.h"


int main() {
    FICHIER file = create_file();
    int choice = 0;
    print_menu(file, choice);

    exit(EXIT_SUCCESS);
}