/*
 * Algorithme Génétique Générique pour le TSP
 * Version complète + vrai DPX conforme au sujet
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "ga_generique.h"
#include "tsp.h"
#include "heuristiques.h"
#include "tsp_utils.h"
#include "distance.h"
#include "matrice.h"

/* =======================================================================
 *                   GESTION DE LA POPULATION
 * ======================================================================= */

Population* creer_population(int taille, int dimension) {
    Population* pop = malloc(sizeof(Population));
    if (!pop) return NULL;

    pop->taille = taille;
    pop->individus = malloc(taille * sizeof(Tournee*));
    pop->distances = malloc(taille * sizeof(double));

    for (int i = 0; i < taille; i++) {
        pop->individus[i] = creer_tournee(dimension);
        pop->distances[i] = 0;
    }
    return pop;
}

void liberer_population(Population* pop) {
    if (!pop) return;

    for (int i = 0; i < pop->taille; i++)
        liberer_tournee(pop->individus[i]);

    free(pop->individus);
    free(pop->distances);
    free(pop);
}

void initialiser_population(Population* pop, const InstanceTSP* instance,
                            FonctionDistance dist_func) {

    for (int i = 0; i < pop->taille; i++) {
        Tournee* t = marche_aleatoire(instance, dist_func);
        copier_tournee(pop->individus[i], t);
        liberer_tournee(t);
    }
}

void evaluer_population(Population* pop, const InstanceTSP* instance,
                        FonctionDistance dist_func) {

    for (int i = 0; i < pop->taille; i++)
        pop->distances[i] = calculer_longueur_tournee(pop->individus[i], instance, dist_func);
}

/* =======================================================================
 *                            SELECTION
 * ======================================================================= */

void selectionner_deux_parents(const Population* pop, int* idx1, int* idx2) {
    *idx1 = rand() % pop->taille;
    do { *idx2 = rand() % pop->taille; } while (*idx2 == *idx1);
}

int trouver_meilleur(const Population* pop) {
    int best = 0;
    for (int i = 1; i < pop->taille; i++)
        if (pop->distances[i] < pop->distances[best])
            best = i;
    return best;
}

int trouver_pire(const Population* pop) {
    int worst = 0;
    for (int i = 1; i < pop->taille; i++)
        if (pop->distances[i] > pop->distances[worst])
            worst = i;
    return worst;
}

/* =======================================================================
 *                             MUTATION
 * ======================================================================= */

void mutation_echange(Tournee* t, double taux) {
    int n = t->taille;
    for (int i = 0; i < n; i++) {
        if (((double)rand() / RAND_MAX) < taux) {
            int j = rand() % n;
            int tmp = t->chemin[i];
            t->chemin[i] = t->chemin[j];
            t->chemin[j] = tmp;
        }
    }
}

/* =======================================================================
 *                    OX (Ordonned Crossover)
 * ======================================================================= */

Tournee* croisement_ordonne(const Tournee* p1, const Tournee* p2) {
    int n = p1->taille;
    Tournee* child = creer_tournee(n);

    for (int i = 0; i < n; i++)
        child->chemin[i] = -1;

    int a = rand() % n;
    int b = rand() % n;
    if (a > b) { int tmp = a; a = b; b = tmp; }

    for (int i = a; i <= b; i++)
        child->chemin[i] = p1->chemin[i];

    int pos = (b + 1) % n;
    int src = (b + 1) % n;
    while (pos != a) {
        int v = p2->chemin[src];
        int ok = 1;

        for (int k = a; k <= b; k++)
            if (child->chemin[k] == v) ok = 0;

        if (ok) {
            child->chemin[pos] = v;
            pos = (pos + 1) % n;
        }
        src = (src + 1) % n;
    }

    return child;
}

/* =======================================================================
 *                       DPX — VERSION DU SUJET
 * ======================================================================= */

Tournee* croisement_dpx(const Tournee* p1, const Tournee* p2,
                        const InstanceTSP* instance, FonctionDistance dist_func) {

    int n = p1->taille;
    Tournee* fille = creer_tournee(n);

    /* 1) Copier parent1 */
    for (int i = 0; i < n; i++)
        fille->chemin[i] = p1->chemin[i];

    /* 2) Détection des arêtes communes */
    bool** commune = malloc((n+1) * sizeof(bool*));
    for (int i = 0; i <= n; i++)
        commune[i] = calloc(n+1, sizeof(bool));

    for (int i = 0; i < n; i++) {
        int a1 = p1->chemin[i];
        int b1 = p1->chemin[(i+1) % n];

        int a2 = p2->chemin[i];
        int b2 = p2->chemin[(i+1) % n];

        if (a1 == a2 && b1 == b2)
            commune[a1][b1] = commune[b1][a1] = true;
    }

    /* 3) Construire next[u] avec destruction des arêtes non communes */
    int* next = malloc((n+1) * sizeof(int));
    for (int i = 0; i < n; i++)
        next[fille->chemin[i]] = fille->chemin[(i+1)%n];

    for (int i = 0; i < n; i++) {
        int u = p1->chemin[i];
        int v = p1->chemin[(i+1)%n];
        if (!commune[u][v])
            next[u] = -1;
    }

    /* 4) Extraction des fragments */
    int** frag = malloc(n * sizeof(int*));
    int* frag_taille = calloc(n, sizeof(int));
    bool* vis = calloc(n+1, sizeof(bool));
    int nb_frag = 0;

    for (int i = 0; i < n; i++) {
        int start = p1->chemin[i];
        if (!vis[start]) {

            frag[nb_frag] = malloc(n * sizeof(int));
            int k = 0, cur = start;

            while (cur != -1 && !vis[cur]) {
                vis[cur] = true;
                frag[nb_frag][k++] = cur;
                cur = next[cur];
            }

            frag_taille[nb_frag] = k;
            nb_frag++;
        }
    }

    /* 5) Reconstruction en reliant les fragments par PPV */
    int* sol = malloc(n * sizeof(int));
    bool* used = calloc(n+1, sizeof(bool));
    int idx = 0;

    /* Premier fragment inchangé */
    for (int i = 0; i < frag_taille[0]; i++) {
        sol[idx++] = frag[0][i];
        used[frag[0][i]] = true;
    }

    int last = sol[idx-1];

    while (idx < n) {

        int best_f = -1;
        double best_d = 1e18;
        bool reverse = false;

        for (int f = 1; f < nb_frag; f++) {
            int u = frag[f][0];
            int v = frag[f][frag_taille[f]-1];

            if (!used[u]) {
                double d = dist_func(&instance->villes[last], &instance->villes[u]);
                if (d < best_d) {
                    best_d = d;
                    best_f = f;
                    reverse = false;
                }
            }

            if (!used[v]) {
                double d = dist_func(&instance->villes[last], &instance->villes[v]);
                if (d < best_d) {
                    best_d = d;
                    best_f = f;
                    reverse = true;
                }
            }
        }

        /* Ajouter le fragment entier */
        if (reverse) {
            for (int k = frag_taille[best_f]-1; k >= 0; k--) {
                int v = frag[best_f][k];
                if (!used[v]) {
                    sol[idx++] = v;
                    used[v] = true;
                }
            }
        } else {
            for (int k = 0; k < frag_taille[best_f]; k++) {
                int v = frag[best_f][k];
                if (!used[v]) {
                    sol[idx++] = v;
                    used[v] = true;
                }
            }
        }

        last = sol[idx-1];
    }

    /* Copier la solution */
    for (int i = 0; i < n; i++)
        fille->chemin[i] = sol[i];

    calculer_longueur_tournee(fille, instance, dist_func);
    return fille;
}

/* =======================================================================
 *                       ALGORITHME GENETIQUE
 * ======================================================================= */

Tournee* algorithme_genetique(const InstanceTSP* instance,
                              FonctionDistance dist_func,
                              const ParamsGA* params) {

    int n = instance->dimension;

    Population* pop = creer_population(params->taille_population, n);
    initialiser_population(pop, instance, dist_func);
    evaluer_population(pop, instance, dist_func);

    int best_idx = trouver_meilleur(pop);
    Tournee* best = creer_tournee(n);
    copier_tournee(best, pop->individus[best_idx]);
    double best_dist = pop->distances[best_idx];

    for (int g = 0; g < params->nombre_generations; g++) {

        for (int c = 0; c < params->nb_croisements; c++) {

            int i1, i2;
            selectionner_deux_parents(pop, &i1, &i2);

            Tournee* child;

            /* 50% OX / 50% DPX */
            if (rand() % 2 == 0)
                child = croisement_ordonne(pop->individus[i1], pop->individus[i2]);
            else
                child = croisement_dpx(pop->individus[i1], pop->individus[i2], instance, dist_func);

            if (((double)rand() / RAND_MAX) < params->taux_mutation)
                mutation_echange(child, params->taux_mutation);

            double d = calculer_longueur_tournee(child, instance, dist_func);

            int worst = trouver_pire(pop);

            if (d < pop->distances[worst]) {
                liberer_tournee(pop->individus[worst]);
                pop->individus[worst] = child;
                pop->distances[worst] = d;
            } else {
                liberer_tournee(child);
            }
        }

        int b = trouver_meilleur(pop);
        if (pop->distances[b] < best_dist) {
            copier_tournee(best, pop->individus[b]);
            best_dist = pop->distances[b];
        }

        int w = trouver_pire(pop);
        if (pop->distances[w] > best_dist) {
            copier_tournee(pop->individus[w], best);
            pop->distances[w] = best_dist;
        }
    }

    liberer_population(pop);
    return best;
}

/* =======================================================================
 *                             EXECUTION
 * ======================================================================= */

void executer_ga(const InstanceTSP* instance, FonctionDistance dist_func,
                 int nb_individus, int nb_generations, double taux_mutation) {

    ParamsGA params;
    params.taille_population = nb_individus;
    params.nombre_generations = nb_generations;
    params.taux_mutation = taux_mutation;
    params.nb_croisements = nb_individus / 2;

    clock_t start = clock();
    Tournee* solution = algorithme_genetique(instance, dist_func, &params);
    clock_t end = clock();

    double cpu = (double)(end - start) / CLOCKS_PER_SEC;

    afficher_tournee_normalisee(instance, solution, "ga", cpu);
    liberer_tournee(solution);
}
