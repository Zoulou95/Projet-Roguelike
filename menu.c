#include "menu.h"
#include "gen.h"
#include "struct.h"

FICHIER create_file() {
    FICHIER file;
    file.fp = fopen("menu.txt", "w"); // open a file in writing mode
    if (file.fp == NULL) {
        perror("Erreur lors de l'ouverture du fichier"); 
        exit(-1);
    }

    snprintf(file.menu[0], sizeof(file.menu[0]), "Start Game"); // copy and paste in file.menu
    snprintf(file.menu[1], sizeof(file.menu[1]), "Quit Game");

    for (int i = 0; i < maxMenu; ++i) {
        fprintf(file.fp, "%s\n", file.menu[i]); // write a file
    }
    return file;
}

void choice_menu(int choice){
    if(choice == 0){
        clear();
        create_map(); // create the map
    }

    else if(choice == 1){

        endwin();
        exit(EXIT_SUCCESS);
    }
}

void print_menu(FICHIER file){ 
    initscr(); // init ncurses
    noecho(); // hide user input
    cbreak(); // read input key without pressing entre to continue
    curs_set(0); // set cursor opacity to 0
    keypad(stdscr, 1);

    char ch;
    int arrPos = 0;
    fclose(file.fp);
    refresh();
    while(1){ // menu loop
        for(int i = 0; i < maxMenu; i++){
            if(arrPos == i){
                move(i, 0); // arrow menu postion
                attron(A_REVERSE); // start reverse backround and foreground color
                printw("-->%s\n", file.menu[i]); // display menu arrow
                attroff(A_REVERSE); // end reverse backround and foreground color
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
            if(arrPos == 2) // bloquer la position pour ne pas aller en dessous du champs
                arrPos = 1;
            break;
        default:
            break;
        }
    }
}