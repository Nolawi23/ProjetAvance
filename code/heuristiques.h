#ifndef HEURISTIQUES_H
#define HEURISTIQUES_H

#include <stdbool.h>
#include "tsp.h"

/* ============================================== */
/*         ALGORITHMES HEURISTIQUES              */
/* ============================================== */

/**
 * Algorithme du plus proche voisin (Nearest Neighbor)
 * Commence par la ville 1 et visite toujours la ville non visitée la plus proche
 * Complexité : O(n²)
 */
Tournee* plus_proche_voisin(const InstanceTSP* instance, FonctionDistance dist_func);

/**
 * Algorithme de marche aléatoire (Random Walk)
 * Construit une tournée en choisissant les villes aléatoirement
 * Complexité : O(n)
 */
Tournee* marche_aleatoire(const InstanceTSP* instance, FonctionDistance dist_func);

/**
 * Algorithme 2-opt
 * Améliore une tournée existante en échangeant des arêtes
 * Modifie la tournée en place
 * @return true si des améliorations ont été faites
 */
bool deux_opt(Tournee* tournee, const InstanceTSP* instance, FonctionDistance dist_func);

/**
 * Plus proche voisin suivi de 2-opt
 */
Tournee* nn_avec_2opt(const InstanceTSP* instance, FonctionDistance dist_func);

/**
 * Marche aléatoire suivie de 2-opt
 */
Tournee* rw_avec_2opt(const InstanceTSP* instance, FonctionDistance dist_func);

/**
 * Fonction principale pour exécuter une heuristique
 * @param methode "nn", "rw", "2optnn", ou "2optrw"
 */
Tournee* executer_heuristique(const InstanceTSP* instance, 
                              FonctionDistance dist_func,
                              const char* methode);

#endif /* HEURISTIQUES_H */