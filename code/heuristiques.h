#ifndef HEURISTIQUES_H
#define HEURISTIQUES_H

#include <stdbool.h>
#include "tsp.h"

//          ALGORITHMES HEURISTIQUES

Tournee* plus_proche_voisin(const InstanceTSP* instance, FonctionDistance dist_func);

Tournee* marche_aleatoire(const InstanceTSP* instance, FonctionDistance dist_func);

bool deux_opt(Tournee* tournee, const InstanceTSP* instance, FonctionDistance dist_func);

static void inverser_segment(int* chemin, int i, int j);

static double calculer_gain_2opt(const InstanceTSP* instance, 
                                 const Tournee* tournee,
                                 FonctionDistance dist_func,
                                 int i, int j);

#endif