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


//              2-OPTIMISATION                   

static void inverser_segment(int* chemin, int i, int j) {
    while (i < j) {
        int temp = chemin[i];
        chemin[i] = chemin[j];
        chemin[j] = temp;
        i++;
        j--;
    }
}

/**
 * Calcule le gain d'une opération 2-opt
 * Les arêtes (i, i+1) et (j, j+1) sont remplacées par (i, j) et (i+1, j+1)
 */
static double calculer_gain_2opt(const InstanceTSP* instance, 
                                 const Tournee* tournee,
                                 FonctionDistance dist_func,
                                 int i, int j) {
    int n = tournee->taille;
    
    // Indices des villes (conversion 1-based vers 0-based)
    int v1 = tournee->chemin[i] - 1;
    int v2 = tournee->chemin[(i + 1) % n] - 1;
    int v3 = tournee->chemin[j] - 1;
    int v4 = tournee->chemin[(j + 1) % n] - 1;
    
    double ancien_cout, nouveau_cout;
    
    if (instance->matrice_existe) {
        ancien_cout = obtenir_distance_matrice(instance, v1, v2) +
                     obtenir_distance_matrice(instance, v3, v4);
        
        nouveau_cout = obtenir_distance_matrice(instance, v1, v3) +
                      obtenir_distance_matrice(instance, v2, v4);
    } else {
        // Calcul direct avec la fonction de distance
        ancien_cout = dist_func(&instance->villes[v1], &instance->villes[v2]) +
                     dist_func(&instance->villes[v3], &instance->villes[v4]);
        
        nouveau_cout = dist_func(&instance->villes[v1], &instance->villes[v3]) +
                      dist_func(&instance->villes[v2], &instance->villes[v4]);
    }
    
    return ancien_cout - nouveau_cout;
}

/**
 * Algorithme 2-opt pour améliorer une tournée existante
 */
bool deux_opt(Tournee* tournee, const InstanceTSP* instance, FonctionDistance dist_func) {
    int n = tournee->taille;
    bool amelioration = false;
    bool continuer = true;
    int iterations = 0;
    const int MAX_ITERATIONS = 1000;
    
    while (continuer && iterations < MAX_ITERATIONS) {
        continuer = false;
        iterations++;
        
        for (int i = 0; i < n - 2; i++) {
            for (int j = i + 2; j < n; j++) {
                // Éviter le cas où on inverse toute la tournée (j+1 == n && i == 0)
                if (j == n - 1 && i == 0) continue;
                
                double gain = calculer_gain_2opt(instance, tournee, dist_func, i, j);
                
                if (gain > 0.001) {  // Petit epsilon pour éviter les erreurs d'arrondi
                    inverser_segment(tournee->chemin, i + 1, j);
                    
                    tournee->distanceTotale -= gain;
                    
                    amelioration = true;
                    continuer = true;
                }
            }
        }
    }
    return amelioration;
}