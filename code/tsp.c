#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Pour getopt
#include <signal.h>
#include <time.h>

#include "tsp.h"
#include "parser.h"
#include "distance.h"
#include "matrice.h"
#include "tsp_utils.h"
#include "force_brute.h"
#include "heuristiques.h"


/*  Exécute la logique de test de la Partie 1 (force brute).
    Teste avec et sans matrice pré-calculée. */

void test_sans_avec_matrice(InstanceTSP* instance, FonctionDistance dist_func) {
    clock_t debut, fin;
    double temps_cpu, temps_matrice;
    Tournee* meilleure, * pire;

    long long nb_perms = (instance->dimension <= 1) ? 1 : factorial(instance->dimension - 1);
    printf("Instance : %s (%d villes)\n", instance->nom, instance->dimension);
    printf("Nombre de permutations à tester : (%d-1)! = %lld\n", instance->dimension, nb_perms);

    if (instance->dimension > 12) {
        printf("ATTENTION : Cela peut prendre TRÈS longtemps. Continuez ? (o/n) : ");
        char rep[10];
        scanf("%9s", rep);
        if (rep[0] != 'o' && rep[0] != 'O') {
            printf("Annulation.\n");
            return;
        }
    }

    printf("\n========== 1. TEST SANS MATRICE PRÉCALCULÉE ==========\n");
    instance->matrice_existe = false;

    debut = clock();
    signal(SIGINT, gestionnaire_interruption); // Armer le signal
    int res_sans = resoudre_force_brute(instance, dist_func, &meilleure, &pire);
    signal(SIGINT, SIG_DFL); // Désarmer le signal (comportement par défaut)
    fin = clock();
    temps_cpu = ((double)(fin - debut)) / CLOCKS_PER_SEC;

    if (res_sans) {
        printf("\n--- Résultats SANS matrice ---\n");
        afficher_banniere_normalisee();
        afficher_tournee_normalisee(instance, meilleure, "bf-sans-mat", temps_cpu);
        printf("Pire tournée : %.2f\n", pire->distanceTotale);
    }
    else {
        printf("\nCalcul SANS matrice interrompu.\n");
    }
    liberer_tournee(meilleure);
    liberer_tournee(pire);

    printf("\n========== 2. TEST AVEC MATRICE PRÉCALCULÉE ==========\n");

    clock_t debut_matrice = clock();
    calculer_matrice_distances(instance, dist_func);
    clock_t fin_matrice = clock();
    temps_matrice = ((double)(fin_matrice - debut_matrice)) / CLOCKS_PER_SEC;
    printf("Temps création matrice : %.4f secondes\n", temps_matrice);

    debut = clock();
    signal(SIGINT, gestionnaire_interruption); 
    int res_avec = resoudre_force_brute(instance, dist_func, &meilleure, &pire);
    signal(SIGINT, SIG_DFL);
    fin = clock();
    double temps_calcul_avec = ((double)(fin - debut)) / CLOCKS_PER_SEC;

    if (res_avec) {
        printf("\n--- Résultats AVEC matrice ---\n");
        afficher_banniere_normalisee();
        afficher_tournee_normalisee(instance, meilleure, "bf-avec-mat", temps_calcul_avec);
        printf("Pire tournée : %.2f\n", pire->distanceTotale);
    }
    else {
        printf("\nCalcul AVEC matrice interrompu.\n");
    }
    liberer_tournee(meilleure);
    liberer_tournee(pire);

    if (res_sans && res_avec) {
        printf("\n========== COMPARAISON (Partie 1) ==========\n");
        printf("Temps SANS matrice : %.4f sec\n", temps_cpu);
        printf("Temps AVEC matrice : %.4f (calcul) + %.4f (création) = %.4f sec\n",
            temps_calcul_avec, temps_matrice, temps_calcul_avec + temps_matrice);
        if (temps_cpu < (temps_calcul_avec + temps_matrice)) {
            printf("Le calcul à la volée (SANS matrice) était plus rapide.\n");
        }
        else {
            printf("Le calcul AVEC matrice précalculée était plus rapide.\n");
        }
    }
}

void executer_bf(InstanceTSP* instance, FonctionDistance dist_func) {
    clock_t debut, fin;
    Tournee* meilleure, * pire;

    debut = clock();
    calculer_matrice_distances(instance, dist_func);

    signal(SIGINT, gestionnaire_interruption);
    int res_sans = resoudre_force_brute(instance, dist_func, &meilleure, &pire);
    signal(SIGINT, SIG_DFL);
    fin = clock();
    double temps_calcul = ((double)(fin - debut)) / CLOCKS_PER_SEC;
    if (res_sans) {
        afficher_tournee_normalisee(instance,meilleure,"bf",temps_calcul);
        liberer_tournee(meilleure);
        liberer_tournee(pire);
    }

}

void executer_nn(const InstanceTSP* instance, FonctionDistance dist_func, bool avecOpt) {
    clock_t debut, fin;
    debut = clock();
    Tournee* tournee = plus_proche_voisin(instance, dist_func);
    fin = clock();
    double temps_calcul = ((double)(fin - debut)) / CLOCKS_PER_SEC;
    if (tournee && !avecOpt){
        afficher_tournee_normalisee(instance,tournee,"nn",temps_calcul);
        liberer_tournee(tournee);
    }    
    else if (tournee && avecOpt){
        debut = clock();
        bool ameliore = deux_opt(tournee, instance, dist_func);
        fin = clock();
        temps_calcul += ((double)(fin - debut)) / CLOCKS_PER_SEC;
        afficher_tournee_normalisee(instance,tournee,"2optnn",temps_calcul);
        liberer_tournee(tournee);
    }
}


void executer_rw(const InstanceTSP* instance, FonctionDistance dist_func, bool avecOpt) {
    clock_t debut, fin;
    debut = clock();
    Tournee* tournee = marche_aleatoire(instance);
    fin = clock();
    double temps_calcul = ((double)(fin - debut)) / CLOCKS_PER_SEC;
    if (tournee && !avecOpt){
        afficher_tournee_normalisee(instance,tournee,"rw",temps_calcul);
        liberer_tournee(tournee);
    }    
    else if (tournee && avecOpt){
        debut = clock();
        bool ameliore = deux_opt(tournee, instance, dist_func);
        fin = clock();
        temps_calcul += ((double)(fin - debut)) / CLOCKS_PER_SEC;
        afficher_tournee_normalisee(instance,tournee,"2optrw",temps_calcul);
        liberer_tournee(tournee);
    }
}

int main(int argc, char* argv[]) {
    char* nomFichier = NULL;
    char* methode = NULL;
    bool mode_canonique = false;
    int opt;

    while ((opt = getopt(argc, argv, "hf:m:c")) != -1) {
        switch (opt) {
        case 'h':
            printf("Usage: %s -f <fichier.tsp> [-m <methode> | -c]\n", argv[0]);
            printf("Options:\n");
            printf("  -f <fichier>  : Fichier TSP à charger\n");
            printf("  -c            : Calculer la tournée canonique\n");
            printf("  -m <methode>  : Méthode de résolution (bf, nn, rw, ...)\n");
            return 0;
        case 'f':
            nomFichier = optarg;
            break;
        case 'c':
            mode_canonique = true;
            break;
        case 'm':
            methode = optarg;
            break;
        default:
            fprintf(stderr, "Usage: %s -f <fichier.tsp> [-m <methode> | -c]\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (!nomFichier) {
        fprintf(stderr, "Erreur: L'option -f est obligatoire.\n");
        fprintf(stderr, "Usage: %s -f <fichier.tsp> [-m <methode> | -c]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Lecture
    InstanceTSP* instance = lire_fichier_tsplib(nomFichier);
    if (!instance) {
        return EXIT_FAILURE;
    }

    // Obtenir fonction distance 
    FonctionDistance dist_func = obtenir_fonction_distance(instance->type_distance);
    if (!dist_func) {
        liberer_instance(instance);
        return EXIT_FAILURE;
    }

    // Mode canonique
    if (mode_canonique) {
        // Construire la tournée canonique [1,2,...,n]
        Tournee* tour = creer_tournee(instance->dimension);
        if (!tour) {
            liberer_instance(instance);
            return EXIT_FAILURE;
        }
        for (int i = 0; i < tour->taille; i++) {
            tour->chemin[i] = i + 1;
        }

        clock_t debut = clock();
        calculer_longueur_tournee(tour, instance, dist_func);
        clock_t fin = clock();
        double temps_calcul = (double)(fin - debut) / CLOCKS_PER_SEC;
        afficher_tournee_normalisee(instance,tour,"c",temps_calcul);
        liberer_tournee(tour);
    }
    if (methode != NULL) {
        // Sélectionner la méthode
        if (strcmp(methode, "bf") == 0) {
            executer_bf(instance, dist_func);
        }
        else if (strcmp(methode, "nn") == 0) {
            executer_nn(instance, dist_func, false);
        }
        else if (strcmp(methode, "2optnn") == 0) {
            executer_nn(instance, dist_func, true);
        }
        else if (strcmp(methode, "rw") == 0) {
            executer_rw(instance, dist_func, false);
        }
        else if (strcmp(methode, "2optrw") == 0) {
            executer_rw(instance, dist_func, true);
        }
        else {
            fprintf(stderr, "Erreur: Méthode '%s' non reconnue.\n", methode);
            liberer_instance(instance);
            return EXIT_FAILURE;
        }
    }

    liberer_instance(instance);
    return EXIT_SUCCESS;
}