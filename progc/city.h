#ifndef CITY_H
#define CITY_H

#include "avl.h"
#include "utils.h"

#define GRANDETAILLE 10000

// Structure pour représenter une ville
typedef struct _Ville {
	char name[TAILLE];
	int totalRoutes; 
	int firstTown; // drivers count
	AVL* drivers;
} City;

City* constructCity(char* townName, char* driverName);

AVL* constructCitiesSorted(AVL* cities, AVL* citiesSorted);

void getTopCities(AVL* cities, FILE* outputFile, City* citiesSortedAlphabetacally[], int* counter);

// Fonction principale pour analyser les trajets à partir d'un fichier
void sortT(FILE* inputFile, FILE* outputFile);

#endif