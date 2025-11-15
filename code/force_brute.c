#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>

#include "force_brute.h"
#include "tsp_utils.h" // Pour creer/copier/liberer_tournee



// VARIABLES GLOBALES (STATIQUES)

static volatile sig_atomic_t interruption_recue = 0;
static Tournee* meilleure_globale = NULL;
static Tournee* pire_globale = NULL;
static Tournee* permutation_courante_globale = NULL;


// GESTION DES INTERRUPTIONS

void gestionnaire_interruption(int sig) {
    (void)sig;
    interruption_recue = 1;
}

static void afficher_etat_actuel() {
    printf("\n\n========== INTERRUPTION DÉTECTÉE ==========\n");
    if (meilleure_globale) {
        printf("Meilleure tournée trouvée jusqu'à présent :\n");
        printf("  Longueur : %.2f\n", meilleure_globale->distanceTotale);
        printf("  Chemin : [");
        for (int i = 0; i < meilleure_globale->taille; i++) {
            printf("%d", meilleure_globale->chemin[i]);
            if (i < meilleure_globale->taille - 1) printf(",");
        }
        printf("]\n\n");
    }
    
    if (pire_globale) {
        printf("Pire tournée trouvée :\n");
        printf("  Longueur : %.2f\n", pire_globale->distanceTotale);
    }
    
    if (permutation_courante_globale) {
        printf("\nPermutation en cours : [");
        for (int i = 0; i < permutation_courante_globale->taille; i++) {
            printf("%d", permutation_courante_globale->chemin[i]);
            if (i < permutation_courante_globale->taille - 1) printf(",");
        }
        printf("]\n");
    }
}

int demander_continuer() {
    char reponse[10];
    printf("\nVoulez-vous continuer les calculs ? (o/n) : ");
    fflush(stdout);
    
    int c;    
    if (scanf("%9s", reponse) == 1) {
        // Vider à nouveau au cas où l'utilisateur a tapé "oui"
        while ((c = getchar()) != '\n' && c != EOF);
        return (reponse[0] == 'o' || reponse[0] == 'O');
    }
    return 0;
}


// ALGORITHME DE PERMUTATION SUIVANTE

static bool permutation_suivante(int tableau[], size_t longueur) {
    if (longueur == 0) return false;
    
    size_t i = longueur - 1;
    while (i > 0 && tableau[i - 1] >= tableau[i])
        i--;
    if (i == 0) return false;
    
    size_t j = longueur - 1;
    while (tableau[j] <= tableau[i - 1])
        j--;
    
    int temp = tableau[i - 1];
    tableau[i - 1] = tableau[j];
    tableau[j] = temp;
    
    j = longueur - 1;
    while (i < j) {
        temp = tableau[i];
        tableau[i] = tableau[j];
        tableau[j] = temp;
        i++;
        j--;
    }
    return true;
}
                          

// ALGORITHME DE FORCE BRUTE

int resoudre_force_brute(InstanceTSP* instance, 
                         FonctionDistance dist_func,
                         Tournee** meilleure, 
                         Tournee** pire) {
    
                            
    int n = instance->dimension;
    interruption_recue = 0; // Réinitialiser au cas où
    
    permutation_courante_globale = creer_tournee(n);
    meilleure_globale = creer_tournee(n);
    pire_globale = creer_tournee(n);
    
    if (!permutation_courante_globale || !meilleure_globale || !pire_globale) return 0; // Échec

    // Première permutation : on fixe ville 1 au début [cite: 95]
    // Chemin initial : [1, 2, 3, 4, ..., n]
    permutation_courante_globale->chemin[0] = 1;
    for (int i = 1; i < n; i++) {
        permutation_courante_globale->chemin[i] = i + 1;
    }
    
    // Évaluer la première permutation
    calculer_longueur_tournee(permutation_courante_globale, instance, dist_func);
    copier_tournee(meilleure_globale, permutation_courante_globale);
    copier_tournee(pire_globale, permutation_courante_globale);
    
    
    // Tableau temporaire pour les permutations (sans la ville 1)
    int* sous_chemin = (int*)malloc((n - 1) * sizeof(int));
    for (int i = 0; i < n - 1; i++) {
        sous_chemin[i] = i + 2;  // [2, 3, 4, ..., n]
    }
        
    do {
        // Vérifier interruption
        if (interruption_recue) {
            afficher_etat_actuel();
            if (!demander_continuer()) {
                *meilleure = meilleure_globale;
                *pire = pire_globale;
                free(sous_chemin);
                liberer_tournee(permutation_courante_globale);
                return 0; // Interrompu
            }
            interruption_recue = 0;
            printf("Reprise des calculs...\n");
        }
        
        // Générer permutation suivante [cite: 107]
        if (!permutation_suivante(sous_chemin, n - 1)) {
            break; // Plus de permutations
        }
        
        // Reconstruire le chemin complet avec 1 au début
        for (int i = 0; i < n - 1; i++) {
            permutation_courante_globale->chemin[i + 1] = sous_chemin[i];
        }
        
        // Évaluer
        calculer_longueur_tournee(permutation_courante_globale, instance, dist_func);
        
        // Mise à jour meilleure
        if (permutation_courante_globale->distanceTotale < meilleure_globale->distanceTotale) {
            copier_tournee(meilleure_globale, permutation_courante_globale);
        }
        
        // Mise à jour pire
        if (permutation_courante_globale->distanceTotale > pire_globale->distanceTotale) {
            copier_tournee(pire_globale, permutation_courante_globale);
        }
        
    } while (true);
    
    *meilleure = meilleure_globale;
    *pire = pire_globale;
    
    free(sous_chemin);
    liberer_tournee(permutation_courante_globale);
    
    return 1;
}