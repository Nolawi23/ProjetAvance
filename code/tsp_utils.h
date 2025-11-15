#ifndef TSP_UTILS_H
#define TSP_UTILS_H

#include "tsp.h"

// Gestion Tournée
Tournee* creer_tournee(int dimension);
void liberer_tournee(Tournee* tournee);
void copier_tournee(Tournee* dest, const Tournee* src);

// Calculs
double calculer_longueur_tournee( Tournee* tournee, 
                                 const InstanceTSP* instance, 
                                 FonctionDistance dist_func);
long long factorial(int n);

// Affichage
void afficher_tournee_normalisee(const InstanceTSP* instance, 
                                 const Tournee* tournee, 
                                 const char* methode, 
                                 double temps_cpu);

                                 
void afficher_banniere_normalisee(void);

#endif