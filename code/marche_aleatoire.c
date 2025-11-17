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

    int* est_visitee = (int*)calloc(n, sizeof(int));
    if (!est_visitee) {
        liberer_tournee(tournee);
        return NULL;
    }

    int ville_courante = 1;

    tournee->chemin[0] = ville_courante;
    est_visitee[ville_courante - 1] = 1;

    int villes_restantes = n - 1;

    for (int i = 1; i < n; i++) {
        int* disponibles = (int*)malloc(villes_restantes * sizeof(int));
        if (!disponibles) {
            liberer_tournee(tournee);
            free(est_visitee);
            return NULL;
        }

        int k = 0;
        for (int j = 1; j <= n; j++) {
            if (!est_visitee[j - 1]) {
                disponibles[k++] = j;
            }
        }

        int index_aleatoire = rand() % villes_restantes;
        int prochaine_ville = disponibles[index_aleatoire];

        tournee->chemin[i] = prochaine_ville;
        est_visitee[prochaine_ville - 1] = 1;
        ville_courante = prochaine_ville;
        villes_restantes--;

        free(disponibles);
    }
    calculer_longueur_tournee(tournee, instance, dist_func);

    free(est_visitee);
    return tournee;
}