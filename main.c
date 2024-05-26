#include "struct.h"
#include "gen.h"
#include "menu.h"


int main() {
    FICHIER file = create_file();
    print_menu(file);
    exit(EXIT_SUCCESS);
}
