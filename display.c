void Display_room(PLAYER *player, MAP *map, int room_index) {
    int width = map->room[room_index].width; // nom plus court pour width
    int height = map->room[room_index].height; // nom plus court pour height
    char room[height][width]; // tableau de la room pour mettre les char (double tableau = matrice)
    int ch;

    // Initialisation de la salle avec des bordures et des espaces vides
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                room[i][j] = '#'; // bordures de la salle
            } else {
                room[i][j] = ' '; // espaces vides à l'intérieur de la salle
            }
        }
    }
 
    player->y = height / 2; // mettre le joueur au milieu de la salle (ici le spawn)
    player->x = width / 2;

    
    while (1) { // Boucle de jeu
        display_room_view(player, width, height, room); // vision 11x11 (dans gen.h modifiable)
        ch = getch(); //prend un charactère
        move_player(player, width, height, room, ch); // déplace le joueur avec la charactère
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

void display_room_view(PLAYER *player, int width, int height, char room[][width]) { //fonction pour la vision

    clear();
    for (int i = 0; i < DISPLAY_HEIGHT; i++) { //affiche la longueur
        for (int j = 0; j < DISPLAY_WIDTH; j++) { //affiche la largeur
            int y = player->y - DISPLAY_HEIGHT / 2 + i; //les coordonnées y pour que ça soit centré
            int x = player->x - DISPLAY_WIDTH / 2 + j; //les coordonnées x pour que ça soit centré
            if (y >= 0 && y < height && x >= 0 && x < width) { // boucle pour afficher le tout
                if (y == player->y && x == player->x) {
                    printw("P"); // Afficher le joueur
                } else {
                    printw("%c", room[y][x]); // Afficher les bordures et les espaces vides
                }
            } else {
                printw(" "); // Afficher un espace pour les cases hors de la salle
            }
        }
        printw("\n");
    }
    refresh(); // Rafraîchir l'affichage
}
