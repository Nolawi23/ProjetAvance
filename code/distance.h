#ifndef DISTANCES_H
#define DISTANCES_H

#include "tsp.h"

double dist_eucl_2d(const Ville* v1, const Ville* v2);

double dist_geo(const Ville* v1, const Ville* v2);

double dist_att(const Ville* v1, const Ville* v2);

// Retourne la fonction de distance correspondant au type donné
FonctionDistance obtenir_fonction_distance(TypeDistance type);

#endif