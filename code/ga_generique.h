/*
 * Algorithme Génétique Générique pour le TSP
 * Fichier d'en-tête
 */

#ifndef GA_GENERIQUE_H
#define GA_GENERIQUE_H

#include "tsp.h"

// Paramètres de l'algorithme génétique
typedef struct {
    int taille_population;      // Nombre d'individus dans la population
    int nombre_generations;     // Nombre de générations à exécuter
    double taux_mutation;       // Probabilité de mutation (0.0 à 1.0)
    int nb_croisements;         // Nombre de croisements par génération
} ParamsGA;

// Population d'individus (tournées)
typedef struct {
    Tournee** individus;        // Tableau de tournées
    double* distances;            // Tableau des longueurs (fitness)
    int taille;                 // Taille de la population
} Population;

//     FONCTIONS PRINCIPALES

void executer_ga(const InstanceTSP* instance, FonctionDistance dist_func,
                 int nb_individus, int nb_generations, double taux_mutation,
                Tournee* foncCroisement(const Tournee *parent1, const Tournee *parent2));

Tournee* algorithme_genetique(const InstanceTSP* instance, 
                              FonctionDistance dist_func,
                              const ParamsGA* params,
                              Tournee* foncCroisement(const Tournee *parent1, const Tournee *parent2)
);

//GESTION DE LA POPULATION

Population* creer_population(int taille, int dimension);

void liberer_population(Population* pop);

void initialiser_population(Population* pop, const InstanceTSP* instance,
                            FonctionDistance dist_func);

void evaluer_population(Population* pop, const InstanceTSP* instance, 
                        FonctionDistance dist_func);

//    OPÉRATEURS GÉNÉTIQUES

void selectionner_deux_parents(const Population* pop, int* idx1, int* idx2);

Tournee* croisement_ordonne(const Tournee* parent1, const Tournee* parent2);

Tournee* croisement_dpx(const Tournee *parent1, const Tournee *parent2);


void mutation_echange(Tournee* tournee, double taux_mutation);


//   UTILITAIRES

int trouver_meilleur(const Population* pop);

int trouver_pire(const Population* pop);

#endif