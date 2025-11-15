#include "tsp_utils.h"
#include "matrice.h" // Pour obtenir_distance_matrice

// Gestion Tournée

Tournee* creer_tournee(int dimension) {
    Tournee* tournee = (Tournee*)malloc(sizeof(Tournee));
    if (!tournee) return NULL;
    
    tournee->chemin = (int*)malloc(dimension * sizeof(int));
    if (!tournee->chemin) {
        free(tournee);
        return NULL;
    }
    tournee->taille = dimension;
    tournee->distanceTotale = 0.0;
    return tournee;
}

void liberer_tournee(Tournee* tournee) {
    if (tournee) {
        if (tournee->chemin) {
            free(tournee->chemin);
        }
        free(tournee);
    }
}

void copier_tournee(Tournee* dest, const Tournee* src) {
    if (!dest || !src || dest->taille != src->taille) return;
    
    memcpy(dest->chemin, src->chemin, src->taille * sizeof(int));
    dest->distanceTotale = src->distanceTotale;
}


// Calculs

double calculer_longueur_tournee( Tournee* tournee, 
                                 const InstanceTSP* instance, 
                                 FonctionDistance dist_func) {
    double longueur = 0.0;
    
    for (int i = 0; i < tournee->taille; i++) {
        int ville1_idx = tournee->chemin[i] - 1;
        int ville2_idx = tournee->chemin[(i + 1) % tournee->taille] - 1;
        
        if (instance->matrice_existe) {
            // Utiliser la matrice précalculée
            longueur += obtenir_distance_matrice(instance, ville1_idx, ville2_idx);
        } else {
            // Calculer à la volée
            longueur += dist_func(&instance->villes[ville1_idx], &instance->villes[ville2_idx]);
        }
    }
    tournee->distanceTotale = longueur;
    return longueur;
}

long long factorial(int n) {
    if (n < 0) return -1;
    if (n == 0) return 1;
    long long res = 1;
    for (int i = 2; i <= n; i++) {
        res *= i;
    }
    return res;
}

// Affichage

void afficher_banniere_normalisee(void) {
    printf("Instance; Méthode; Temps CPU (sec); Longueur; Tour\n");
}

void afficher_tournee_normalisee(const InstanceTSP* instance, 
                                 const Tournee* tournee, 
                                 const char* methode, 
                                 double temps_cpu) {
                                    
    printf("Tour : %s %s %.6f %.0f [", instance->nom, methode, temps_cpu, tournee->distanceTotale);    
    for (int i = 0; i < tournee->taille; i++) {
        printf("%d", tournee->chemin[i]);
        if (i < tournee->taille - 1) printf(",");
    }
    printf("]\n");
}