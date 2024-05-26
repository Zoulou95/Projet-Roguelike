#include "menu.h"
#include "gen.h"
#include "struct.h"

// Function to create a file and write the menu to it
FICHIER create_file() {
    FICHIER file;
    file.fp = fopen("menu.txt", "w"); //open a file in writing mode
    if (file.fp == NULL) {
        perror("Erreur lors de l'ouverture du fichier"); 
        exit(-1);
    }
// copy and paste in file.menu
    snprintf(file.menu[0], sizeof(file.menu[0]), "Start Game");
    snprintf(file.menu[1], sizeof(file.menu[1]), "Quit Game");

    //Write each menu item to the file
    for (int i = 0; i < maxMenu; ++i) {
        fprintf(file.fp, "%s\n", file.menu[i]);
    }
    return file;
}

// Function to manage menu selection
void choice_menu(int choice){
    if (choice == 0) { // If the user chooses "Start Game"
        clear(); // Clean the screen
        create_map(); // Create the game map
    } else if (choice == 1) { // If the user chooses "Quit Game"
        endwin(); 
        exit(EXIT_SUCCESS); //quit the program
    }
}

// Function to display the menu
void print_menu(FICHIER file) { 
    initscr(); //init ncurses
    noecho(); // hide user input
    cbreak(); // read input key without pressing entre to continue
    curs_set(0); // set cursor opacity to 0

    char ch;
    int arrPos = 0;
    fclose(file.fp); // Close File
    refresh(); // Refresh the screen

    while (1) { 
        for (int i = 0; i < maxMenu; i++) {
            if (arrPos == i) { // Whether the cursor position matches the menu option
                move(i, 0); // arrow menu postion
                attron(A_REVERSE); //start reverse backround and foreground color
                printw("-->%s\n", file.menu[i]); //display menu arrow
                attroff(A_REVERSE); // end reverse backround and foreground color
                if (ch == 10) { // If the user presses Enter
                    choice_menu(i); // Run Menu Choice
                }
            } else {
                move(i, 0); // Move the cursor to the menu position
                printw("   %s\n", file.menu[i]); // Show the menu without the arrow
            }
        }
        printw("Press Z to go up or Press S to go down."); // User Instructions
        refresh(); 
        ch = getch(); // Get User Touch
        switch (ch) { 
        case 'z': 
            arrPos--; 
            if (arrPos == -1) // Prevent position from going below zero
                arrPos = 0;
            break;
        case 's': 
            arrPos++; 
            if (arrPos == 2) // Prevent the position from going beyond the last element
                arrPos = 1;
            break;
        default:
            break;
        }
    }
}

// Function to run the main loop of the game
void game_loop(PLAYER *player, MAP *map, int room_ID) {
    int ch;
    int height = map->room[room_ID].height;

    while (1) { 
        display_room_view(player, map); // View the room view
        ch = getch(); 
        move_player(player, map, ch); 
        if (ch == 27) { 
            clear(); // Clean the terminal
            refresh(); 
            for (int i = 0; i < height; i++) {
                free(map->room[room_ID].data[i]); // Freeing Memory
            }
            free(map->room[room_ID].data); // Free up room data memory
            free(map->room); // Free the memory of the rooms
            free(map); // Free up the card's memory
            FICHIER file = create_file(); // Recreate a file
            print_menu(file); // View Menu
            break;
        }
