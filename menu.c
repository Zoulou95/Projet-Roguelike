#include "menu.h"
#include "gen.h"
#include "struct.h"

// Function to create a file and write the menu to it
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
        fprintf(file.fp, "%s\n", file.menu[i]); 
    //Write each menu item to the file
    }
    return file;
}
// Function to manage menu selection
void choice_menu(int choice){
    if(choice == 0){// If the user chooses "Start Game"
        clear();// Clean the screen
        create_map(); // create the map
    }

    else if(choice == 1){// If the user chooses "Quit Game"

        endwin();
        exit(EXIT_SUCCESS);//quit the program
    }
}

// Function to display the menu
void print_menu(FICHIER file){ 
    initscr(); // init ncurses
    noecho(); // hide user input
    cbreak(); // read input key without pressing entre to continue
    curs_set(0); // set cursor opacity to 0

    char ch;
    int arrPos = 0;
    fclose(file.fp);// Close File
    refresh();// Refresh the screen
    while(1){ // menu loop
        for(int i = 0; i < maxMenu; i++){
            if(arrPos == i){
                move(i, 0); // arrow menu postion
                attron(A_REVERSE); // start reverse backround and foreground color
                printw("-->%s\n", file.menu[i]); // display menu arrow
                attroff(A_REVERSE); // end reverse backround and foreground color
                if(ch == 10){ //If the user presses Enter
                    choice_menu(i);
                }
            }
            else {
                move(i, 0);
                printw("   %s\n", file.menu[i]);// Move the cursor to the menu position
                printw("Press Z to go top or Press S to go down.");
                refresh();
            }
        }
        ch = getch(); // Get User key
        switch (ch){
        case 'z': // up
            arrPos--;
            if(arrPos == -1) // Prevent the position from going beyond the first element
                arrPos = 0;
            break;
        case 's': // down
            arrPos++; 
            if(arrPos == 2) // Prevent the position from going beyond the last element
                arrPos = 1;
            break;
        default:
            break;
        }
    }
}

// Function to run the main game loop
void game_loop(PLAYER *player, MAP *map, int room_ID) {
    int ch;
    int height = map->room[room_ID].height;

    while (1) { // Game loop
        display_room_view(player, map); // Display room view
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
            print_menu(file); // Display menu
            break;
        }
    }

    // Terminate ncurses
    endwin();
}