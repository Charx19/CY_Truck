#ifndef ROUTE_H
#define ROUTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl.h"

typedef struct {
	int id;
	float max;
	float min;
	float sum;
	int n;
	float average;
} Route;

/*
* Construct a struct Route
*
* id : The route ID
* distance : The distance of that route
*
* returns : A struct Route
*/ 
Route* createRoute(int id, float distance);

/*
* Insert a AVL node sorted by route ID
*
* routes : AVL containing the routes sorted by route IDs
* idRoute : The route ID used for the struct Route construction
* distance : The distance of the route used for the struct Route construction
*
* returns : AVL containing the routes sorted by route IDs
*/ 
AVL* insertRouteAVL(AVL* routes, int idRoute, float distance);

/*
* Insert a AVL node sorted by minMax
*
* routesMinMaxSorted : AVL containing the routes sorted by minMaxs
* minMax : The minMax to compare
* element : The existing route to re-use for the new AVL
*
* returns : AVL containing the routes sorted by minMaxs
*/ 
AVL* insertRouteMinMaxAVL(AVL* routesMinMaxSorted, float minMax, Route* element);

/*
* Construct a AVL with minMax as key, by using the routes AVL
*
* routes : AVL containing the routes with the route ID as keys
* routesMinMaxSorted : AVL containing the routes with minMax as key
*
* returns : AVL containing the routes with minMax as key
*/ 
AVL* constructRoutesMinxMaxSortedAVL(AVL* routes, AVL* routesMinMaxSorted);

/*
* Read the AVL in reverse infix order to get the datas in descending order
*
* routesMinMaxSorted : AVL containing the routes with minxMax as keys
* file : The output file to write the datas in
* counter : The number of line we want to write in the output file
* limit : Used for the printf, to get the index, it's counter + 1
*/
void writeRouteDatas(AVL* routesMinMaxSorted, FILE* file, int* counter, int limit);

/*
* Main function to sort the routes data
*
* inputFile = File where we read the datas
* outputFile = File where we write the sorted datas
*/
void sortRoutes(FILE* inputFile, FILE* outputFile);

#endif