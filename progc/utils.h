#ifndef UTILS_H
#define UTILS_H

#define TAILLE 256

char** ligneToTab(char* l);

// Fonction pour trouver le maximum entre deux valeurs
int max2i(int a, int b);

int min2i(int a, int b);

float max2f(float a, float b);

float min2f(float a, float b);

int hash(char *str);

FILE* fopenSafe(char* filePath, char* mode);

#endif