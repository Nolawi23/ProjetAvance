/*
 * Algorithme Génétique Générique pour le TSP
 * Implémentation suivant les spécifications du projet
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "tsp.h"
#include "heuristiques.h"
#include "tsp_utils.h"
#include "distance.h"
#include "matrice.h"
#include "ga_generique.h"

Population* creer_population(int taille, int dimension) {
    Population* pop = (Population*)malloc(sizeof(Population));
    if (!pop) return NULL;
    
    pop->taille = taille;
    pop->individus = (Tournee**)malloc(taille * sizeof(Tournee*));
    pop->distances = (double*)malloc(taille * sizeof(double));
    
    if (!pop->individus || !pop->distances) {
        free(pop->individus);
        free(pop->distances);
        free(pop);
        return NULL;
    }
    
    // Créer chaque individu
    for (int i = 0; i < taille; i++) {
        pop->individus[i] = creer_tournee(dimension);
        if (!pop->individus[i]) {
            for (int j = 0; j < i; j++) {
                liberer_tournee(pop->individus[j]);
            }
            free(pop->individus);
            free(pop->distances);
            free(pop);
            return NULL;
        }
        pop->distances[i] = 0.0;
    }
    
    return pop;
}

void liberer_population(Population* pop) {
    if (!pop) return;
    
    if (pop->individus) {
        for (int i = 0; i < pop->taille; i++) {
            liberer_tournee(pop->individus[i]);
        }
        free(pop->individus);
    }
    
    free(pop->distances);
    free(pop);
}

// Initialiser la population avec des tournées aléatoires (marche aléatoire)
void initialiser_population(Population* pop, const InstanceTSP* instance, 
                            FonctionDistance dist_func) {
    for (int i = 0; i < pop->taille; i++) {
        // Utiliser marche_aleatoire pour créer chaque individu
        Tournee* tournee_aleatoire = marche_aleatoire(instance, dist_func);
        if (tournee_aleatoire) {
            copier_tournee(pop->individus[i], tournee_aleatoire);
            liberer_tournee(tournee_aleatoire);
        }
    }
}

// Calculer la distances de toute la population
void evaluer_population(Population* pop, const InstanceTSP* instance, 
                        FonctionDistance dist_func) {
    for (int i = 0; i < pop->taille; i++) {
        pop->distances[i] = calculer_longueur_tournee(pop->individus[i], 
                                                     instance, dist_func);
    }
}

// ============================================================================
//                      SÉLECTION
// ============================================================================

// Sélection aléatoire simple de deux individus
void selectionner_deux_parents(const Population* pop, int* idx1, int* idx2) {
    *idx1 = rand() % pop->taille;
    do {
        *idx2 = rand() % pop->taille;
    } while (*idx2 == *idx1);
}

Tournee* croisement_ordonne(const Tournee* parent1, const Tournee* parent2) {
    int n = parent1->taille;
    Tournee* enfant = creer_tournee(n);
    
    // Initialiser avec -1 (vide)
    for (int i = 0; i < n; i++) {
        enfant->chemin[i] = -1;
    }
    
    // Choisir deux points de croisement aléatoires
    int point1 = rand() % n;
    int point2 = rand() % n;
    
    if (point1 > point2) {
        int temp = point1;
        point1 = point2;
        point2 = temp;
    }
    
    // Copier le segment [point1, point2] depuis parent1
    for (int i = point1; i <= point2; i++) {
        enfant->chemin[i] = parent1->chemin[i];
    }
    
    // Remplir le reste avec parent2 dans l'ordre
    int pos_enfant = (point2 + 1) % n;
    int pos_parent2 = (point2 + 1) % n;
    
    while (pos_enfant != point1) {
        int ville = parent2->chemin[pos_parent2];
        
        // Vérifier si la ville est déjà présente
        bool present = false;
        for (int i = point1; i <= point2; i++) {
            if (enfant->chemin[i] == ville) {
                present = true;
                break;
            }
        }
        
        if (!present) {
            enfant->chemin[pos_enfant] = ville;
            pos_enfant = (pos_enfant + 1) % n;
        }
        
        pos_parent2 = (pos_parent2 + 1) % n;
    }
    
    return enfant;
}

void mutation_echange(Tournee* tournee, double taux_mutation) {
    int n = tournee->taille;
    
    // Pour chaque position, décider si on mute
    for (int i = 0; i < n; i++) {
        double r = (double)rand() / RAND_MAX;
        if (r < taux_mutation) {
            // Échanger avec une position aléatoire
            int j = rand() % n;
            int temp = tournee->chemin[i];
            tournee->chemin[i] = tournee->chemin[j];
            tournee->chemin[j] = temp;
        }
    }
}

int trouver_meilleur(const Population* pop) {
    int meilleur = 0;
    double meilleure_distances = pop->distances[0];
    
    for (int i = 1; i < pop->taille; i++) {
        if (pop->distances[i] < meilleure_distances) {
            meilleur = i;
            meilleure_distances = pop->distances[i];
        }
    }
    
    return meilleur;
}

int trouver_pire(const Population* pop) {
    int pire = 0;
    double pire_distances = pop->distances[0];
    
    for (int i = 1; i < pop->taille; i++) {
        if (pop->distances[i] > pire_distances) {
            pire = i;
            pire_distances = pop->distances[i];
        }
    }
    
    return pire;
}

Tournee* algorithme_genetique(const InstanceTSP* instance, 
                              FonctionDistance dist_func,
                              const ParamsGA* params) {
    
    int dimension = instance->dimension;
    
    // Créer la population initiale
    Population* population = creer_population(params->taille_population, dimension);
    if (!population) {
        fprintf(stderr, "Erreur : impossible de créer la population\n");
        return NULL;
    }
    
    // Initialiser avec des tournées aléatoires (utilise marche_aleatoire)
    initialiser_population(population, instance, dist_func);
    
    // Évaluer la population initiale
    evaluer_population(population, instance, dist_func);
    
    // Garder trace du meilleur global
    int idx_meilleur = trouver_meilleur(population);
    Tournee* meilleur_global = creer_tournee(dimension);
    copier_tournee(meilleur_global, population->individus[idx_meilleur]);
    double meilleur_distances = population->distances[idx_meilleur];
    
    // Boucle principale : générations
    for (int gen = 0; gen < params->nombre_generations; gen++) {
        
        // Boucle de croisements
        for (int crois = 0; crois < params->nb_croisements; crois++) {
            
            // Sélectionner deux parents au hasard
            int idx_parent1, idx_parent2;
            selectionner_deux_parents(population, &idx_parent1, &idx_parent2);
            
            // Croisement ordonné
            Tournee* fille = croisement_ordonne(
                population->individus[idx_parent1],
                population->individus[idx_parent2]
            );
            
            // Mutation avec probabilité taux_mutation
            double r = (double)rand() / RAND_MAX;
            if (r < params->taux_mutation) {
                mutation_echange(fille, params->taux_mutation);
            }
            
            // Évaluer la fille
            double distances_fille = calculer_longueur_tournee(fille, instance, dist_func);
            
            // Remplacer le pire individu de la population
            int idx_pire = trouver_pire(population);
            
            // Remplacer seulement si la fille est meilleure que le pire
            if (distances_fille < population->distances[idx_pire]) {
                liberer_tournee(population->individus[idx_pire]);
                population->individus[idx_pire] = fille;
                population->distances[idx_pire] = distances_fille;
            } else {
                liberer_tournee(fille);
            }
        }
        
        // Mettre à jour le meilleur global
        idx_meilleur = trouver_meilleur(population);
        if (population->distances[idx_meilleur] < meilleur_distances) {
            copier_tournee(meilleur_global, population->individus[idx_meilleur]);
            meilleur_distances = population->distances[idx_meilleur];
        }
        
        // Élitisme : s'assurer que le meilleur global est dans la population
        int idx_pire = trouver_pire(population);
        if (population->distances[idx_pire] > meilleur_distances) {
            copier_tournee(population->individus[idx_pire], meilleur_global);
            population->distances[idx_pire] = meilleur_distances;
        }
    }
    
    // Nettoyer
    liberer_population(population);
    
    return meilleur_global;
}

void executer_ga(const InstanceTSP* instance, FonctionDistance dist_func,
                 int nb_individus, int nb_generations, double taux_mutation) {
    
    ParamsGA params;
    params.taille_population = nb_individus;
    params.nombre_generations = nb_generations;
    params.taux_mutation = taux_mutation;
    params.nb_croisements = nb_individus / 2;
    
    // Mesurer le temps
    clock_t debut = clock();
    
    // Exécuter l'algorithme génétique
    Tournee* solution = algorithme_genetique(instance, dist_func, &params);
    
    clock_t fin = clock();
    double temps_cpu = ((double)(fin - debut)) / CLOCKS_PER_SEC;
    
    if (!solution) {
        fprintf(stderr, "Erreur : échec de l'algorithme génétique\n");
        return;
    }
    
    // Afficher le résultat normalisé
    afficher_tournee_normalisee(instance, solution, "ga", temps_cpu);
    
    // Libérer
    liberer_tournee(solution);
}