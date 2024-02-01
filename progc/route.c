#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "route.h"
#include "avl.h"
#include "utils.h"

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

AVL* insertRouteAVL(AVL* routes, int idRoute, float distance) 
{ 
    if (routes == NULL) {
		return(createAVL(createRoute(idRoute, distance)));
	}
        
	Route* route = (Route*)routes->element;
  
    if (idRoute < route->id) {
		routes->left  = insertRouteAVL(routes->left, idRoute, distance); 
	}
        
    else if (idRoute > route->id) {
		routes->right = insertRouteAVL(routes->right, idRoute, distance); 
	}
        
    else {
		route->max = max2f(distance, route->max);
		route->min = min2f(distance, route->min);
		route->sum += distance;
		route->n++;
		return routes; 
	}
	
    routes->height = 1 + max2i(getHeight(routes->left), getHeight(routes->right)); 
  
    int balance = getBalance(routes); 

    // Left Left Case 
    if (balance > 1) {
		Route* routeLeft = (Route*)routes->left->element;
		if (idRoute < routeLeft->id) {
			return rotateRight(routes);
		}
	}
         
    // Right Right Case 
    if (balance < -1) {
		Route* routeRight = (Route*)routes->right->element;
		if (idRoute > routeRight->id) {
			return rotateLeft(routes); 
		}
		
	}
        
    // Left Right Case 
    if (balance > 1) { 
		Route* routeLeft = (Route*)routes->left->element;
		if (idRoute > routeLeft->id){
			return rotateDoubleRight(routes);
		}
    } 
  
    // Right Left Case 
    if (balance < -1) { 
		Route* routeRight = (Route*)routes->right->element;
		if (idRoute < routeRight->id){
			return rotateDoubleLeft(routes);
		}
    } 
  
    return routes; 
} 

AVL* insertRouteMinMaxAVL(AVL* routesMinMaxSorted, float minMax, Route* element)
{
	if (routesMinMaxSorted == NULL) {
		return(createAVL(element));
	}
        
	Route* route = (Route*)routesMinMaxSorted->element;
  
    if (minMax < (route->max - route->min)) {
		routesMinMaxSorted->left  = insertRouteMinMaxAVL(routesMinMaxSorted->left, minMax, element); 
	}
        
    else if (minMax > (route->max - route->min)) {
		routesMinMaxSorted->right = insertRouteMinMaxAVL(routesMinMaxSorted->right, minMax, element); 
	}
        
    else {
		return routesMinMaxSorted; 
	}
	
    routesMinMaxSorted->height = 1 + max2i(getHeight(routesMinMaxSorted->left), getHeight(routesMinMaxSorted->right)); 
  
    int balance = getBalance(routesMinMaxSorted); 

    // Left Left Case 
    if (balance > 1) {
		Route* routeLeft = (Route*)routesMinMaxSorted->left->element;
		if (minMax < (routeLeft->max - routeLeft->min)) {
			return rotateRight(routesMinMaxSorted);
		}
	}
         
    // Right Right Case 
    if (balance < -1) {
		Route* routeRight = (Route*)routesMinMaxSorted->right->element;
		if (minMax > (routeRight->max - routeRight->min)) {
			return rotateLeft(routesMinMaxSorted); 
		}
		
	}
        
    // Left Right Case 
    if (balance > 1) { 
		Route* routeLeft = (Route*)routesMinMaxSorted->left->element;
		if (minMax > (routeLeft->max - routeLeft->min)){
			return rotateDoubleRight(routesMinMaxSorted);
		}
    } 
  
    // Right Left Case 
    if (balance < -1) { 
		Route* routeRight = (Route*)routesMinMaxSorted->right->element;
		if (minMax < (routeRight->max - routeRight->min)){
			return rotateDoubleLeft(routesMinMaxSorted);
		}
    } 
  
    return routesMinMaxSorted; 
}

AVL* constructRoutesMinxMaxSortedAVL(AVL* routes, AVL* routesMinMaxSorted) 
{ 
    if(routes != NULL) 
    { 
		routesMinMaxSorted = constructRoutesMinxMaxSortedAVL(routes->left, routesMinMaxSorted); 
		
		Route* route = (Route*)routes->element;
		// Insert a AVL node sorted by minMax
		routesMinMaxSorted = insertRouteMinMaxAVL(routesMinMaxSorted, route->max - route->min, route);
        
		routesMinMaxSorted = constructRoutesMinxMaxSortedAVL(routes->right, routesMinMaxSorted); 
    }
	return routesMinMaxSorted;
} 

void writeRouteDatas(AVL* routesMinMaxSorted, FILE* file, int* counter, int limit) 
{ 
    if(routesMinMaxSorted != NULL && *counter > 0) 
    { 
		Route* route = (Route*)routesMinMaxSorted->element;

		writeRouteDatas(routesMinMaxSorted->right, file, counter, limit);
		if (*counter == 0) {
			return;
		}
		
		fprintf(file, "%d;%d;%f;%f;%f;%f\n", limit - *counter, route->id, route->min, route->sum / route->n, route->max, route->max - route->min);
		(*counter)--;
        
		writeRouteDatas(routesMinMaxSorted->left, file, counter, limit);
		if (*counter == 0) {
			return;
		}
    } 
} 

void sortRoutes(FILE* inputFile, FILE* outputFile) {
	AVL* routes = NULL;

	// Get file contents
	int idRoute, idStep;
	float distance;
	char townA[NAME_SIZE], townB[NAME_SIZE], driverName[NAME_SIZE];
	
	char line[LINE_SIZE];
	char** lineArray = NULL;

	printf("Making routes AVL... 1/3\n");
	
    fgets(line, sizeof(line), inputFile); // Skip header
	while(fgets(line, sizeof(line), inputFile) != NULL) // Read each line of input file
	{
		// Separating by ';' to get informations
		lineArray = lineToArray(line);
		if (sscanf(lineArray[0], "%d", &idRoute) != 1) {
			printf("Conversion Error, string is not a valid number (Column 1)\n");
			exit(1);
		}
		if (sscanf(lineArray[1], "%d", &idStep) != 1) {
			printf("Conversion Error, string is not a valid number (Column 2)\n");
			exit(1);
		}
		strcpy(townA, lineArray[2]);
		strcpy(townB, lineArray[3]);
		if (sscanf(lineArray[4], "%f", &distance) != 1) {
			printf("Conversion Error, string is not a valid number (Column 4)\n");
			exit(1);
		}
		strcpy(driverName, lineArray[5]);

		routes = insertRouteAVL(routes, idRoute, distance); 
	}

	fclose(inputFile);
	//

	printf("Sorting routes AVL from minMax... 2/3\n");

	// Construct a AVL from the minMax as keys (minMax = max - min)
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