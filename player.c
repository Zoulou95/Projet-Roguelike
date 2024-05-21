#include "player.h"
#include "gen.h"

void move_player(PLAYER *player, int width, int height, char room[][width], int ch) {
    switch(ch) {
        case 'z': // Déplacement vers le haut
            if (player->y > 1 && room[player->y - 1][player->x] != '#') { // != pour pas que le joueur soit sur la bordure
                player->y--;
            }
            break;
        case 's': // Déplacement vers le bas
            if (player->y < height - 2 && room[player->y + 1][player->x] != '#') {
                player->y++;
            }
            break;
        case 'q': // Déplacement vers la gauche
            if (player->x > 1 && room[player->y][player->x - 1] != '#') {
                player->x--;
            }
            break;
        case 'd': // Déplacement vers la droite
            if (player->x < width - 2 && room[player->y][player->x + 1] != '#') {
                player->x++;
            }
            break;
        default :
            break;
    }
}
