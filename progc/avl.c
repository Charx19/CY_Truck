#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AVL {
    char name[256];
    int totalRoutes;
    int startRoutes;
    struct AVL *left;
    struct AVL *right;
    int height;
} AVL;

typedef struct {
    int routeID;
    int stepID;
    char townA[256];
    char townB[256];
    int distance;
    char driverName[256];
} DonneesCSV;

typedef struct {
    char name[256];
    int totalRoutes;
} VilleInfo;

AVL* createAVL(char name[]) {
    AVL* avl = (AVL*)malloc(sizeof(AVL));
    strcpy(avl->name, name);
    avl->totalRoutes = 0;
    avl->startRoutes = 0;
    avl->left = NULL;
    avl->right = NULL;
    avl->height = 1;
    return avl;
}

int heightAVL(AVL* n) {
    if (n == NULL)
        return 0;
    return n->height;
}

int facteurEquilibre(AVL* n) {
    if (n == NULL)
        return 0;
    return heightAVL(n->left) - heightAVL(n->right);
}

AVL* rotateLeft(AVL* x) {
    AVL* y = x->right;
    AVL* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + max(heightAVL(x->left), heightAVL(x->right));
    y->height = 1 + max(heightAVL(y->left), heightAVL(y->right));

    return y;
}

AVL* rotateRight(AVL* y) {
    AVL* x = y->left;
    AVL* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + max(heightAVL(y->left), heightAVL(y->right));
    x->height = 1 + max(heightAVL(x->left), heightAVL(x->right));

    return x;
}

AVL* insertAVL(AVL* root, char name[], int totalRoutes, int startRoutes) {
    
    if (root == NULL)
        return createAVL(name);

    if (strcmp(name, root->name) < 0)
        root->left = insertAVL(root->left, name, totalRoutes, startRoutes);
    else if (strcmp(name, root->name) > 0)
        root->right = insertAVL(root->right, name, totalRoutes, startRoutes);
    else 

    root->height = 1 + max(heightAVL(root->left), heightAVL(root->right));

    int equilibre = facteurEquilibre(root);

    if (equilibre > 1 && strcmp(name, root->left->name) < 0)
        return rotateRight(root);

    if (equilibre < -1 && strcmp(name, root->right->name) > 0)
        return rotateLeft(root);

    if (equilibre > 1 && strcmp(name, root->left->name) > 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    if (equilibre < -1 && strcmp(name, root->right->name) < 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

int min (int a, int b ){
    return (a > b) ? b : a;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

void destroyAVL(AVL* root) {
    if (root != NULL) {
        destroyAVL(root->left);
        destroyAVL(root->right);
        free(root);
    }
}

void collecteInfos(AVL* root, VilleInfo* infos, int* index) {
    if (root != NULL) {
        collecteInfos(root->left, infos, index);

        strcpy(infos[*index].name, root->name);
        infos[*index].totalRoutes = root->totalRoutes;
        (*index)++;

        collecteInfos(root->right, infos, index);
    }
}

int comparaisonVilles(const void* a, const void* b) {
    return ((VilleInfo*)b)->totalRoutes - ((VilleInfo*)a)->totalRoutes;
}

void afficherTop10Parcours(AVL* root, int* count) {
    if (root != NULL && (*count) < 10) {
        afficherTop10Parcours(root->right, count);

        printf("%d. Ville: %s, Trajets Total: %d\n", (*count) + 1, root->name, root->totalRoutes);
        (*count)++;

        afficherTop10Parcours(root->left, count);
    }
}

void readCSV(FILE *fichier) {   
    printf("teste de debugage\n");

    AVL *root = NULL;
    DonneesCSV donnees;
    while (!feof(fichier)) {
        fscanf(fichier, "%d, %d, %s, %s, %d, %s",
            &donnees.routeID, &donnees.stepID, donnees.townA, donnees.townB, &donnees.distance, donnees.driverName);

        root = insertAVL(root, donnees.townA, donnees.routeID, 1);
        root = insertAVL(root, donnees.townB, donnees.routeID, 0); 
    }

    if (root != NULL) {
        VilleInfo infos[100];
        int index = 0;
        collecteInfos(root, infos, &index);
        qsort(infos, index, sizeof(VilleInfo), comparaisonVilles);
        afficherTop10(infos, index);
        
        printf("teste de debugage\n");
        destroyAVL(root);
    }    
}
