#ifndef MATRICE_H
#define MATRICE_H

#include "tsp.h"

// Prend une fonction de distance et calcule la matrice des distances
void calculer_matrice_distances(InstanceTSP* instance, FonctionDistance fonc_dist);

// Libère la mémoire allouée pour la matrice des distances
void liberer_matrice_distances(InstanceTSP* instance);

// Obtient la distance entre les villes i et j à partir de la matrice
double obtenir_distance_matrice(const InstanceTSP* instance, int i, int j);

#endif