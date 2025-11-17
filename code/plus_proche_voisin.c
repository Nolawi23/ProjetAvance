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


Tournee* plus_proche_voisin(const InstanceTSP* instance, FonctionDistance dist_func) {
    int n = instance->dimension;
    Tournee* tournee = creer_tournee(n);
    if (!tournee) return NULL;
    
    // Tableau pour marquer les villes visitées
    bool* visite = (bool*)calloc(n, sizeof(bool));
    if (!visite) {
        liberer_tournee(tournee);
        return NULL;
    }
    
    // Commencer par la ville 1
    int ville_courante = 0;
    tournee->chemin[0] = 1;  
    visite[0] = true;
    
    // Pour chaque position dans la tournée
    for (int pos = 1; pos < n; pos++) {
        double distance_min = DBL_MAX;
        int ville_proche = -1;
        
        // Chercher la ville non visitée la plus proche
        for (int v = 0; v < n; v++) {
            if (!visite[v]) {
                double dist;
                
                if (instance->matrice_existe) {
                    dist = obtenir_distance_matrice(instance, ville_courante, v);
                } else {
                    dist = dist_func(&instance->villes[ville_courante], &instance->villes[v]);
                }
                
                if (dist < distance_min) {
                    distance_min = dist;
                    ville_proche = v;
                }
            }
        }
        
        // Ajouter la ville la plus proche à la tournée
        if (ville_proche != -1) {
            tournee->chemin[pos] = ville_proche + 1;
            visite[ville_proche] = true;
            ville_courante = ville_proche;
        }
    }
    
    calculer_longueur_tournee(tournee, instance, dist_func);
    
    free(visite);
    return tournee;
}