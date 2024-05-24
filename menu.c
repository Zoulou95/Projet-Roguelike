#include "menu.h"
#include "gen.h"
#include "struct.h"

FICHIER create_file() {
    FICHIER file;
    file.fp = fopen("menu.txt", "w"); // Ouvrir un fichier en mode écriture
    if (file.fp == NULL) {
        perror("Erreur lors de l'ouverture du fichier"); 
        exit(-1);
    }

    snprintf(file.menu[0], sizeof(file.menu[0]), "Start Game"); //ça copie colle dans file.menu
    snprintf(file.menu[1], sizeof(file.menu[1]), "Continue");
    snprintf(file.menu[2], sizeof(file.menu[2]), "Quit Game");

    for (int i = 0; i < maxMenu; ++i) {
        fprintf(file.fp, "%s\n", file.menu[i]); // écrire dans le fichier
    }
    return file;
}

void choice_menu(int choice){
    if(choice == 0){
        clear();
        // Créer une carte
        create_map();
    }

    else if(choice == 2){

        endwin();
        exit(EXIT_SUCCESS);
    }
}

void print_menu(FICHIER file, int choice){ 
    initscr(); // initialiser ncurses
    noecho(); // ne pas montrer l'input de l'utilisateur car sans ça sur la fenêtre il y aurait des 'z' et 's' partout
    cbreak(); // lire les touches immédiatement sans appuyer sur espace
    curs_set(0); // mettre le curseur en invisible (opacité de 0 à 1)

    char ch;
    int arrPos = 0;
    fclose(file.fp);
    refresh();
    while(1){ // boucle infini car 1 = vrai
        for(int i = 0; i < maxMenu; i++){
            if(arrPos == i){ // si la position vaut 1 alors il va attendre jusqu'à que i + 1 = 1 soit i = 0 donc menu[0]
                move(i, 0); //déplace le curseur de i+1 (y) et 1 (x)
                attron(A_REVERSE); // début de l'attribut qui permet de renverser la couleur du texte et du background
                printw("-->%s\n", file.menu[i]); // écrire l'affichage sur la fenêtre avec la flèche
                attroff(A_REVERSE); // fin de l'attribut qui permet de renverser la couleur du texte et du background
                refresh(); //rafraichir la fenêtre pour afficher la fenêtre actuelle sinon ça ne s'affiche pas
                if(ch == 10){
                    choice_menu(i);
                }
            }
            else {
                move(i, 0);
                printw("   %s\n", file.menu[i]);// même chose qu'au dessus mais sans la flèche
                printw("Press Z to go top or Press S to go down.");
                refresh();
            }
        }
        ch = getch(); // avoir la touche de l'utilisateur
        switch (ch){ // switch case pour le charactère
        case 'z': // le charactère 'z' pour aller en haut
            arrPos--; // enlever de 1 la position car si la flèche est en 2nd position après elle sera en 1er position
            if(arrPos == -1) // bloquer la position pour ne pas aller au dessus du champs
                arrPos = 0;
            break;
        case 's': // le charactère 's' pour aller en haut
            arrPos++; // ajouter de 1 la position car si la flèche est en 1er position après elle sera en 2nd position
            if(arrPos == 3) // bloquer la position pour ne pas aller en dessous du champs
                arrPos = 2;
            break;
        default:
            break;
        }
    }
}

// Function to run the main game loop
void game_loop(PLAYER *player, MAP *map, int room_ID) {
    int ch;
    int width = map->room[room_ID].width;
    int height = map->room[room_ID].height;

    while (1) { // Game loop
        display_room_view(player, map, width, height, room_ID); // Display room view
        ch = getch(); // Take a character input
        move_player(player, map, ch); // Move the player according to the input character
        if (ch == 27) { // Escape to quit
            clear(); // Clear the terminal
            refresh(); // Refresh the terminal
            for (int i = 0; i < height; i++) {
                free(map->room[room_ID].data[i]); // Free memory of room data rows
            }
            free(map->room[room_ID].data); // Free memory of room data
            free(map->room); // Free memory of rooms
            free(map);  // Free memory of the map
            FICHIER file = create_file(); // Recreate a file
            print_menu(file, 0); // Display menu
            break;
        }
    }

    // Terminate ncurses
    endwin();
}