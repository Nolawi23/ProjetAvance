#ifndef HEURISTIQUES_H
#define HEURISTIQUES_H

#include <stdbool.h>
#include "tsp.h"

//          ALGORITHMES HEURISTIQUES

Tournee* plus_proche_voisin(const InstanceTSP* instance, FonctionDistance dist_func);

Tournee* marche_aleatoire(const InstanceTSP* instance, FonctionDistance dist_func);

void deux_opt(Tournee* tournee, const InstanceTSP* instance, FonctionDistance dist_func);

#endif