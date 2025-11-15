#include "parser.h"
#include "matrice.h" // Pour liberer_matrice_distances
#include <string.h>

#define TAILLE_TAMPON 512

// Fonction utilitaire pour supprimer les espaces + les retours à la ligne en fin de chaîne
static void supprimer_espaces_fin(char* chaine) {
    int longueur = strlen(chaine);
    while (longueur > 0 && (chaine[longueur - 1] == '\n' || chaine[longueur - 1] == '\r' || chaine[longueur - 1] == ' ')) {
        chaine[--longueur] = '\0';
    }
}

InstanceTSP* lire_fichier_tsplib(const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "r");
    if (!fichier) {
        perror("Erreur ouverture du fichier");
        return NULL;
    }

    InstanceTSP* instance = (InstanceTSP*)calloc(1, sizeof(InstanceTSP));
    instance->matrice_existe = false;

    char tampon[TAILLE_TAMPON];
    bool dans_section_coords = false;
    int indice_ville = 0;

    while (fgets(tampon, TAILLE_TAMPON, fichier)) {
        supprimer_espaces_fin(tampon);

        if (strstr(tampon, "NAME")) {
            sscanf(tampon, "NAME : %255s", instance->nom);
        } else if (strstr(tampon, "COMMENT")) {
            // Gérer les commentaires (si besoin)
        } else if (strstr(tampon, "TYPE : TSP")) {
            // Pour le projet en traite que le type TSP
        } else if (strstr(tampon, "DIMENSION")) {
            sscanf(tampon, "DIMENSION : %d", &instance->dimension);
        } else if (strstr(tampon, "EDGE_WEIGHT_TYPE")) {
            if (strstr(tampon, "EUC_2D")) instance->type_distance = DIST_EUCL_2D;
            else if (strstr(tampon, "GEO")) instance->type_distance = DIST_GEO;
            else if (strstr(tampon, "ATT")) instance->type_distance = DIST_ATT;
        } else if (strstr(tampon, "NODE_COORD_SECTION")) {
            instance->villes = (Ville*)malloc(instance->dimension * sizeof(Ville));
            dans_section_coords = true;
        } else if (strstr(tampon, "EOF")) {
            break;
        } else if (dans_section_coords) {
            int num;
            double x, y;
            if (sscanf(tampon, "%d %lf %lf", &num, &x, &y) == 3) {
                instance->villes[indice_ville].numero = num;
                instance->villes[indice_ville].x = x;
                instance->villes[indice_ville].y = y;
                indice_ville++;
            }
        }
    }

    fclose(fichier);    
    return instance;
}

void liberer_instance(InstanceTSP* instance) {
    if (instance) {
        if (instance->villes) {
            free(instance->villes);
        }
        if (instance->matrice_existe) {
            liberer_matrice_distances(instance);
        }
        free(instance);
    }
}