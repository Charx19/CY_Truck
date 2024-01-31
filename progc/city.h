#ifndef CITY_H
#define CITY_H

#include "avl.h"
#include "utils.h"

typedef struct _Ville {
	char name[NAME_SIZE];
	int totalRoutes; 
	int firstTown; // drivers count
	AVL* drivers;
} City;

City* constructCity(char* townName, char* driverName);

AVL* constructCitiesSorted(AVL* cities, AVL* citiesSorted);

void getTopCities(AVL* cities, FILE* outputFile, City* citiesSortedAlphabetacally[], int* counter);

void sortCities(FILE* inputFile, FILE* outputFile);

#endif