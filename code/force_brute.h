#ifndef FORCE_BRUTE_H
#define FORCE_BRUTE_H

#include "tsp.h"
#include "tsp_utils.h"

// Résout le problème du TSP par la méthode de force brute.
int resoudre_force_brute(InstanceTSP* instance, 
                         FonctionDistance dist_func,
                         Tournee** meilleure, 
                         Tournee** pire);



// Gestionnaire de signal pour SIGINT (Ctrl+C).
void gestionnaire_interruption(int sig);

#endif