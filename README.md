# Projet-Roguelike

To run the program, in the bash, run the following command:
make run

Then, you will see the menu, you can navigate with Z and S (for UP and DOWN respectivly).
You will need to press Enter to continue in the following category:
- "Start Game" to start the game
- "Quit Game" to quit the game and end the program

In the game, you can at any moment return to the menu by pressing Escape.

To move in the game you will need to press Z, Q, S, D (for UP, LEFT, DOWN, RIGHT respectivly) to move through the dungeon.

The program have header files (ending in .h) and C files.
main.c for the main function, menu.c is for the menu, player.c is for the player movement through the dungeon and gen.c is the procedural creation of the dungeon.
The Makefile is the program runner.
"menu.txt" is a temporary file (used to test the use of files).