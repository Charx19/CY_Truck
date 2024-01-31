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

void sortRoutes(FILE* inputFile, FILE* outputFile) {
	AVL* routes = NULL;

	int idRoute, idStep;
	float distance;
	char townA[NAME_SIZE], townB[NAME_SIZE], driverName[NAME_SIZE];
	
	char line[LINE_SIZE];
	char** lineArray = NULL;

	printf("Making routes AVL... 1/3\n");
	
    fgets(line, sizeof(line), inputFile); // Skip header
	while(fgets(line, sizeof(line), inputFile) != NULL) // Read each line of input file
	{
		// et on les sépares par leur ';' avant de stocker les informations importantes
		lineArray = lineToArray(line);
		if (sscanf(lineArray[0], "%d", &idRoute) != 1) {
			printf("Erreur de conversion. La chaîne n'est pas un nombre valide1.\n");
			exit(1);
		}
		if (sscanf(lineArray[1], "%d", &idStep) != 1) {
			printf("Erreur de conversion. La chaîne n'est pas un nombre valide2.\n");
			exit(1);
		}
		strcpy(townA, lineArray[2]);
		strcpy(townB, lineArray[3]);
		if (sscanf(lineArray[4], "%f", &distance) != 1) {
			printf("Erreur de conversion. La chaîne n'est pas un nombre valide3.\n");
			exit(1);
		}
		strcpy(driverName, lineArray[5]);

		// Construct a AVL with the route ID as a key
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

	printf("Sorting routes AVL from minmax... 2/3\n");

	// Construct a AVL from the minmax as keys
	AVL* routesMinMaxSorted = NULL;

	routesMinMaxSorted = constructRoutesMinxMaxSortedAVL(routes, routesMinMaxSorted);

	printf("Writing sorted data... 3/3\n");

	// Write routes data sorted from minmax in the output file
    int counter = 50;
	writeRouteDatas(routesMinMaxSorted, outputFile, &counter, counter + 1);

	fclose(outputFile);

	// Free memory from AVL trees
	destroyAVL(routes);
	destroyAVL(routesMinMaxSorted);
}