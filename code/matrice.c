#include "matrice.h"

void calculer_matrice_distances(InstanceTSP* instance, FonctionDistance fonc_dist) {
    // Si une matrice existe déjà, la libérer d'abord
    if (instance->matrice_existe) {
        liberer_matrice_distances(instance);
    }

    int n = instance->dimension;
    instance->matrice_distances = (double**)malloc(n * sizeof(double*));

    // Matrice triangulaire inférieure "jagged"
    for (int i = 0; i < n; i++) {
        // La ligne 'i' a 'i' colonnes (pour 0 à i-1)
        if (i == 0) {
            instance->matrice_distances[i] = NULL;
        } else {
            instance->matrice_distances[i] = (double*)malloc(i * sizeof(double));
            // Calculer les distances pour cette ligne
            for (int j = 0; j < i; j++) {
                instance->matrice_distances[i][j] = fonc_dist(&instance->villes[i], &instance->villes[j]);
            }
        }
    }
    instance->matrice_existe = true;
}

void liberer_matrice_distances(InstanceTSP* instance) {
    if (instance && instance->matrice_existe) {
        for (int i = 0; i < instance->dimension; i++) {
            if (instance->matrice_distances[i]) {
                free(instance->matrice_distances[i]);
            }
        }
        free(instance->matrice_distances);
        instance->matrice_distances = NULL;
        instance->matrice_existe = false;
    }
}

// i et j sont 0-indexés
double obtenir_distance_matrice(const InstanceTSP* instance, int i, int j) {
    if (i == j) return 0.0;
    
    // Assurer i > j pour lire la matrice triangulaire inférieure
    if (j > i) {
        int temp = i;
        i = j;
        j = temp;
    }
    
    return instance->matrice_distances[i][j];
}