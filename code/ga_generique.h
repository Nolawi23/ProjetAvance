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
    double* fitness;            // Tableau des longueurs (fitness)
    int taille;                 // Taille de la population
} Population;

// ============================================================================
//                      FONCTIONS PRINCIPALES
// ============================================================================

/**
 * Exécute l'algorithme génétique et affiche le résultat
 * 
 * @param instance L'instance TSP à résoudre
 * @param dist_func La fonction de distance appropriée
 * @param nb_individus Taille de la population
 * @param nb_generations Nombre de générations
 * @param taux_mutation Probabilité de mutation (ex: 0.01 pour 1%)
 */
void executer_ga(const InstanceTSP* instance, FonctionDistance dist_func,
                 int nb_individus, int nb_generations, double taux_mutation);

/**
 * Algorithme génétique générique
 * 
 * @param instance L'instance TSP à résoudre
 * @param dist_func La fonction de distance appropriée
 * @param params Les paramètres de l'algorithme
 * @return La meilleure tournée trouvée (à libérer par l'appelant)
 */
Tournee* algorithme_genetique(const InstanceTSP* instance, 
                              FonctionDistance dist_func,
                              const ParamsGA* params);

// ============================================================================
//                      GESTION DE LA POPULATION
// ============================================================================

/**
 * Crée une population vide
 * 
 * @param taille Nombre d'individus
 * @param dimension Nombre de villes
 * @return Pointeur vers la population créée
 */
Population* creer_population(int taille, int dimension);

/**
 * Libère la mémoire d'une population
 * 
 * @param pop La population à libérer
 */
void liberer_population(Population* pop);

/**
 * Initialise la population avec des tournées aléatoires (marche aléatoire)
 * 
 * @param pop La population à initialiser
 * @param instance L'instance TSP
 * @param dist_func La fonction de distance
 */
void initialiser_population(Population* pop, const InstanceTSP* instance,
                            FonctionDistance dist_func);

/**
 * Calcule la fitness de tous les individus
 * 
 * @param pop La population
 * @param instance L'instance TSP
 * @param dist_func La fonction de distance
 */
void evaluer_population(Population* pop, const InstanceTSP* instance, 
                        FonctionDistance dist_func);

// ============================================================================
//                      OPÉRATEURS GÉNÉTIQUES
// ============================================================================

/**
 * Sélectionne deux parents au hasard
 * 
 * @param pop La population
 * @param idx1 Pointeur pour stocker l'indice du premier parent
 * @param idx2 Pointeur pour stocker l'indice du second parent
 */
void selectionner_deux_parents(const Population* pop, int* idx1, int* idx2);

/**
 * Croisement ordonné (Ordered Crossover - OX)
 * 
 * @param parent1 Premier parent
 * @param parent2 Second parent
 * @return Nouvelle tournée fille (à libérer par l'appelant)
 */
Tournee* croisement_ordonne(const Tournee* parent1, const Tournee* parent2);

/**
 * Mutation par échange (Swap Mutation)
 * 
 * @param tournee La tournée à muter
 * @param taux_mutation Probabilité de mutation pour chaque position
 */
void mutation_echange(Tournee* tournee, double taux_mutation);

// ============================================================================
//                      UTILITAIRES
// ============================================================================

/**
 * Trouve l'indice du meilleur individu
 * 
 * @param pop La population
 * @return L'indice du meilleur individu
 */
int trouver_meilleur(const Population* pop);

/**
 * Trouve l'indice du pire individu
 * 
 * @param pop La population
 * @return L'indice du pire individu
 */
int trouver_pire(const Population* pop);

#endif // GA_GENERIQUE_H