#include "struct.h"
#include "gen.h"


int main(void){
    initscr(); // initialise ncurses
    noecho(); // hide user input
    cbreak(); // read keyboard key without pressing space
    curs_set(0); // cursor opacity in hidden
    srand(time(NULL)); // add time in seconds in the random function
    //unsigned int SEED=rand(); // init the SEED
    // SEED = [SEED entered by the player] // action done if the player start the game with a predetermined seed
    int SEED=5;
    srand(SEED); // overwrite time(NULL) to make the rand() predetermined
    MAP *map=create_map();


    getch(); // read user key input
    // delwin(win); // free the memory
    endwin(); // restore terminal from ncurses modifications
    return 0;
}