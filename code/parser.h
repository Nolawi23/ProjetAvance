#ifndef PARSER_H
#define PARSER_H

#include "tsp.h"

// Lit un fichier TSPLIB et retourne une instance TSP remplie
InstanceTSP* lire_fichier_tsplib(const char* nomFichier);

// Libère la mémoire associée à une instance TSP
void liberer_instance(InstanceTSP* instance);

#endif