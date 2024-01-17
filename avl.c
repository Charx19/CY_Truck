#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Ville {
    char nom[50];
    int trajets_total;
    int trajets_depart;
    struct Ville *gauche;
    struct Ville *droite;
    int hauteur;
} Ville;

typedef struct {
    int routeID;
    int stepID;
    char townA[50];
    char townB[50];
    int distance;
    char driverName[50];
} DonneesCSV;

typedef struct {
    char nom[50];
    int trajets_total;
} VilleInfo;

int max(int a, int b) {
    return (a > b) ? a : b;
}


int hauteur(Ville* n) {
    if (n == NULL)
        return 0;
    return n->hauteur;
}


int facteurEquilibre(Ville* n) {
    if (n == NULL)
        return 0;
    return hauteur(n->gauche) - hauteur(n->droite);
}


Ville* rotationDroite(Ville* y) {
    Ville* x = y->gauche;
    Ville* T2 = x->droite;

    x->droite = y;
    y->gauche = T2;

    y->hauteur = 1 + max(hauteur(y->gauche), hauteur(y->droite));
    x->hauteur = 1 + max(hauteur(x->gauche), hauteur(x->droite));

    return x;
}


Ville* rotationGauche(Ville* x) {
    Ville* y = x->droite;
    Ville* T2 = y->gauche;

    y->gauche = x;
    x->droite = T2;

    x->hauteur = 1 + max(hauteur(x->gauche), hauteur(x->droite));
    y->hauteur = 1 + max(hauteur(y->gauche), hauteur(y->droite));

    return y;
}



Ville* nouvelleVille(char nom[]) {
    Ville* ville = (Ville*)malloc(sizeof(Ville));
    strcpy(ville->nom, nom);
    ville->trajets_total = 0;
    ville->trajets_depart = 0;
    ville->gauche = NULL;
    ville->droite = NULL;
    ville->hauteur = 1;
    return ville;
}



Ville* inserer(Ville* racine, char nom[], int trajets_total, int trajets_depart) {
    
    if (racine == NULL)
        return nouvelleVille(nom);

    if (strcmp(nom, racine->nom) < 0)
        racine->gauche = inserer(racine->gauche, nom, trajets_total, trajets_depart);
    else if (strcmp(nom, racine->nom) > 0)
        racine->droite = inserer(racine->droite, nom, trajets_total, trajets_depart);
    else 

    racine->hauteur = 1 + max(hauteur(racine->gauche), hauteur(racine->droite));

    int equilibre = facteurEquilibre(racine);

    if (equilibre > 1 && strcmp(nom, racine->gauche->nom) < 0)
        return rotationDroite(racine);

    if (equilibre < -1 && strcmp(nom, racine->droite->nom) > 0)
        return rotationGauche(racine);

    if (equilibre > 1 && strcmp(nom, racine->gauche->nom) > 0) {
        racine->gauche = rotationGauche(racine->gauche);
        return rotationDroite(racine);
    }

    if (equilibre < -1 && strcmp(nom, racine->droite->nom) < 0) {
        racine->droite = rotationDroite(racine->droite);
        return rotationGauche(racine);
    }

    return racine;
}


void libererMemoire(Ville* racine) {
    if (racine != NULL) {
        libererMemoire(racine->gauche);
        libererMemoire(racine->droite);
        free(racine);
    }
}


void collecteInfos(Ville* racine, VilleInfo* infos, int* index) {
    if (racine != NULL) {
        collecteInfos(racine->gauche, infos, index);

        strcpy(infos[*index].nom, racine->nom);
        infos[*index].trajets_total = racine->trajets_total;
        (*index)++;

        collecteInfos(racine->droite, infos, index);
    }
}

int comparaisonVilles(const void* a, const void* b) {
    return ((VilleInfo*)b)->trajets_total - ((VilleInfo*)a)->trajets_total;
}


void afficherTop10Parcours(Ville* racine, int* count) {
    if (racine != NULL && (*count) < 10) {
        afficherTop10Parcours(racine->droite, count);

        printf("%d. Ville: %s, Trajets Total: %d\n", (*count) + 1, racine->nom, racine->trajets_total);
        (*count)++;

        afficherTop10Parcours(racine->gauche, count);
    }
}
