#ifndef ROUTE_H
#define ROUTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl.h"

typedef struct _Route {
	int id;
	float max;
	float min;
	float sum;
	int n;
	float average;
} Route;

Route* createRoute(int id, float distance);

// Construct a AVL with minmax as key
AVL* constructRoutesMinxMaxSortedAVL(AVL* root, AVL* routes);

void writeRouteDatas(AVL* routes, FILE* file, int* counter, int limit);

// Fonction principale pour analyser les trajets à partir d'un fichier
void sortRoutes(FILE* inputFile, FILE* outputFile);

#endif