#include "distance.h"

// EUCL_2D : Arrondi à l'entier le plus proche
double dist_eucl_2d(const Ville* v1, const Ville* v2) {
    double xd = v1->x - v2->x;
    double yd = v1->y - v2->y;
    return round(sqrt(xd*xd + yd*yd));
}

// GEO : Calcul standard TSPLIB
double dist_geo(const Ville* v1, const Ville* v2) {
    double lat1 = v1->x * M_PI / 180.0;
    double lon1 = v1->y * M_PI / 180.0;
    double lat2 = v2->x * M_PI / 180.0;
    double lon2 = v2->y * M_PI / 180.0;
    
    double RRR = 6378.388;
    double q1 = cos(lon1 - lon2);
    double q2 = cos(lat1 - lat2);
    double q3 = cos(lat1 + lat2);
    
    return (int)(RRR * acos(0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3)) + 1.0);
}

// ATT : Calcul standard TSPLIB
double dist_att(const Ville* v1, const Ville* v2) {
    double xd = v1->x - v2->x;
    double yd = v1->y - v2->y;
    double rij = sqrt((xd*xd + yd*yd) / 10.0);
    int tij = (int)rij; // Tronquer
    
    return (tij < rij) ? (double)(tij + 1) : (double)tij;
}


FonctionDistance obtenir_fonction_distance(TypeDistance type) {
    switch (type) {
        case DIST_EUCL_2D:
            return dist_eucl_2d;
        case DIST_GEO:
            return dist_geo;
        case DIST_ATT:
            return dist_att;
        default:
            fprintf(stderr, "Erreur: Type de distance inconnu.\n");
            return NULL;
    }
}