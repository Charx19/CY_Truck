#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "route.h"
#include "avl.h"
#include "utils.h"

// Fonction pour créer un nouveau nœud de Route
Route* createRoute(int id, float distance) {
	Route* newRoute = (Route*)malloc(sizeof(Route));
	
	newRoute->id = id;
	newRoute->min = distance;
	newRoute->max = distance;
	newRoute->sum = distance;
	newRoute->n = 1;
	newRoute->average = 0;

	return newRoute;
}

AVL* constructRoutesMinxMaxSortedAVL(AVL* routes, AVL* routesMinMaxSorted) 
{ 
    if(routes != NULL) 
    { 
		routesMinMaxSorted = constructRoutesMinxMaxSortedAVL(routes->left, routesMinMaxSorted); 
		
		Route* route = (Route*)routes->element;
		AVL* element = NULL;
		element = searchKeyAVL(routesMinMaxSorted, route->max - route->min);
		if (element == NULL) {
			routesMinMaxSorted = insertAVL(routesMinMaxSorted, route->max - route->min, route);
		}
        
		routesMinMaxSorted = constructRoutesMinxMaxSortedAVL(routes->right, routesMinMaxSorted); 
    }
	return routesMinMaxSorted;
} 

void writeRouteDatas(AVL* routes, FILE* file, int* counter, int limit) 
{ 
    if(routes != NULL && *counter > 0) 
    { 
		Route* route = (Route*)routes->element;

		writeRouteDatas(routes->right, file, counter, limit);
		if (*counter == 0) {
			return;
		}
		fprintf(file, "%d;%d;%f;%f;%f;%f\n", limit - *counter, route->id, route->min, route->sum / route->n, route->max, route->max - route->min);
		(*counter)--;
        writeRouteDatas(routes->left, file, counter, limit);
		if (*counter == 0) {
			return;
		}
    } 
} 

// Fonction principale pour analyser les trajets à partir d'un fichier
void sortS(FILE* inputFile, FILE* outputFile) {
	// Racine de l'arbre AVL
	AVL* routes = NULL;

	int idRoute, idStep;
	float distance;
	char townA[TAILLE], townB[TAILLE], driverName[TAILLE];
	char ligne[TAILLE];
	char** ligneTab = NULL;
	
    fgets(ligne, sizeof(ligne), inputFile);
	while(fgets(ligne, sizeof(ligne), inputFile) != NULL) // on lit chaque ligne
	{
		// et on les sépares par leur ';' avant de stocker les informations importantes
		ligneTab = ligneToTab(ligne);
		if (sscanf(ligneTab[0], "%d", &idRoute) != 1) {
			printf("Erreur de conversion. La chaîne n'est pas un nombre valide1.\n");
			exit(1);
		}
		if (sscanf(ligneTab[1], "%d", &idStep) != 1) {
			printf("Erreur de conversion. La chaîne n'est pas un nombre valide2.\n");
			exit(1);
		}
		strcpy(townA, ligneTab[2]);
		strcpy(townB, ligneTab[3]);
		if (sscanf(ligneTab[4], "%f", &distance) != 1) {
			printf("Erreur de conversion. La chaîne n'est pas un nombre valide3.\n");
			exit(1);
		}
		strcpy(driverName, ligneTab[5]);

		// Construct a AVL from the idRoute as key
		AVL* element = NULL;
		element = searchKeyAVL(routes, idRoute);
		if (element == NULL) {
			routes = insertAVL(routes, idRoute, createRoute(idRoute, distance));
		} else {
			Route* route = (Route*)element->element;
			route->max = max2f(distance, route->max);
			route->min = min2f(distance, route->min);
			route->sum += distance;
			route->n++;
		}
	}

	fclose(inputFile);

	// Construct a AVL from the minmax as keys
	AVL* routesMinMaxSorted = NULL;

	routesMinMaxSorted = constructRoutesMinxMaxSortedAVL(routes, routesMinMaxSorted);

	// Write routes data sorted from minmax in "data_s.dat"
    int counter = 50;
	int limit = counter + 1;
	writeRouteDatas(routesMinMaxSorted, outputFile, &counter, limit);

	fclose(outputFile);

	// Free memory from AVL trees
	destroyAVL(routes);
	destroyAVL(routesMinMaxSorted);
}