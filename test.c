#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void createDoor(int nb_doors, char existing_door) {
    // Les murs disponibles sont N (Nord), E (Est), S (Sud), W (Ouest)
    char walls[] = {'N', 'E', 'S', 'W'};
    int num_walls = 4;
    char doors[4] = {0}; // Tableau pour stocker les portes créées

    // Enlever le mur qui a déjà une porte
    for (int i = 0; i < num_walls; i++) {
        if (walls[i] == existing_door) {
            walls[i] = 0; // Marque le mur avec une porte existante comme indisponible
            break;
        }
    }

    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));

    // Créer nb_doors nouvelles portes aléatoires
    for (int i = 0; i < nb_doors; i++) {
        int wall_index;
        do {
            wall_index = rand() % num_walls; // Choisir un mur aléatoire
        } while (walls[wall_index] == 0); // Assurez-vous que le mur est disponible

        doors[i] = walls[wall_index]; // Ajouter la nouvelle porte
        walls[wall_index] = 0; // Marquer ce mur comme utilisé
    }

    // Afficher les portes créées
    printf("Portes créées : ");
    for (int i = 0; i < nb_doors; i++) {
        if (doors[i] != 0) {
            printf("%c ", doors[i]);
        }
    }
    printf("\n");
}

int main() {
    // Exemples d'utilisation de la fonction
    createDoor(1, 'N');
    createDoor(2, 'E');
    createDoor(3, 'S');
    return 0;
}
