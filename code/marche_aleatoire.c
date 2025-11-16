/**
 * plus_proche_voisin.c
 * Partie 2 : Algorithmes utilisant des heuristiques
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include <math.h>

#include "heuristiques.h"
#include "tsp_utils.h"
#include "distance.h"
#include "matrice.h"


Tournee* marche_aleatoire(const InstanceTSP* instance, FonctionDistance dist_func) {
    int n = instance->dimension;
    Tournee* tournee = creer_tournee(n);
    if (!tournee) return NULL;
    
    // Créer un tableau avec toutes les villes
    int* villes_disponibles = (int*)malloc(n * sizeof(int));
    if (!villes_disponibles) {
        liberer_tournee(tournee);
        return NULL;
    }
    
    // Initialiser avec les numéros de villes (1 à n)
    for (int i = 0; i < n; i++) {
        villes_disponibles[i] = i + 1;
    }
    
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = villes_disponibles[i];
        villes_disponibles[i] = villes_disponibles[j];
        villes_disponibles[j] = temp;
    }
    
    memcpy(tournee->chemin, villes_disponibles, n * sizeof(int));
    
    calculer_longueur_tournee(tournee, instance, dist_func);
    
    free(villes_disponibles);
    return tournee;
}