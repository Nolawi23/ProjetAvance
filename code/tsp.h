#ifndef TSP_H
#define TSP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define NOM_MAX 256
#define COMMENTAIRE_MAX 512

typedef enum {
    DIST_EUCL_2D,    // Distance euclidienne
    DIST_GEO,        // Distance géographique
    DIST_ATT         // Distance ATT pseudo-euclidienne
} TypeDistance;


// Structure d'une ville
typedef struct {
    int numero;    // Numéro de la ville (1 à N selon TSPLIB)
    double x;      
    double y;      
} Ville;


// Structure pour une instance TSP avec les données du problème */
typedef struct {
    char nom[NOM_MAX];
    char commentaire[COMMENTAIRE_MAX];
    int dimension;
    TypeDistance type_distance;     // Type de calcul de distance (euclidienne, géographique ou ATT)
    
    
    Ville* villes;                // Tableau des villes
    double** matrice_distances;   // Matrice triangulaire inférieure
    bool matrice_existe;        // true si matrice existe/remplit, false sinon
} InstanceTSP;

// Structure pour une tournée (solution)
typedef struct {
    int* chemin;          // Ordre de visite [1,2,3,...,N]
    int taille;           // Nb villes
    double distanceTotale;
} Tournee;



// Type pour les fonctions de calcul de distance
typedef double (*FonctionDistance)(const Ville* v1, const Ville* v2);

#endif