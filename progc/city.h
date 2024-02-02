#ifndef CITY_H
#define CITY_H

#include "avl.h"
#include "utils.h"

typedef struct {
	char name[NAME_SIZE]; // Name of the city
	int totalRoutes; // The total routes count tied to that city
	int firstTown; // first town count
	AVL* idRoutes; // AVL to check if a route ID is here or not
} City;

typedef struct {
	int id;
} IdRoute;

IdRoute* constructIdRoute(int idRoute);

/*
* Construct a struct City
*
* townName : The town name
* driverName : The driver name
*
* returns : A struct City
*/ 
City* constructCity(char* townName, int idRoute);

/*
* Sort a array of City by their name alphabetically
*
* citiesSortedAlphabetically : Array where we put the cities that we are going to sort alphabetically later
* arraySize : The size of the array
*/ 
void bubbleSort(City* citiesSortedAlphabetically[], int arraySize);

/*
* Insert a AVL node sorted by route ID
*
* idRoutes : AVL containing the route IDs
* city : The city attached to that route ID, and count it if its not in the AVL
* idRouteToCompare : The route ID to compare, to see if it's in the AVL
*
* returns : AVL containing the routes sorted by route IDs
*/ 
AVL* insertIdRouteAVL(AVL* idRoutes, City* city, void* idRouteToCompare);

/*
* Insert a AVL node sorted by town names
*
* cities : AVL containing the routes sorted by town names
* townName : The town name used for the struct City construction
* idRoute : The route ID used for the struct City construction
* isStart : If the town is the starting point
*
* returns : AVL containing the routes sorted by route IDs
*/ 
AVL* insertCityAVL(AVL* cities, char* townName, int idRoute, int isStart);

/*
* Insert a AVL node sorted by town total routes
*
* citiesTotalRoutesSorted : AVL containing the routes sorted by town total routes
* totalRoutes : The town total routes to compare
* element : The existing route to re-use for the new AVL
*
* returns : AVL containing the routes sorted by town total routes
*/ 
AVL* insertCityTotalRoutesSortedAVL(AVL* citiesTotalRoutesSorted, int totalRoutes, City* element);

/*
* Construct a AVL containing the cities with the total routes as key, by using the cities AVL
*
* cities : AVL containing the cities with the townName in hash as key
* citiesTotalRoutesSorted : AVL containing the cities with the total routes as key
*
* returns : AVL containing the cities with the total routes as key
*/ 
AVL* constructCitiesTotalRoutesSorted(AVL* cities, AVL* citiesTotalRoutesSorted);

/*
* Get the top cities sorted by total routes
*
* citiesTotalRoutesSorted : AVL containing the cities with the townName in hash as key
* citiesSortedAlphabetically : Array where we put the cities that we are going to sort alphabetically later
* counter : The number of lines we want to write before stopping
*/ 
void getTopCities(AVL* citiesTotalRoutesSorted, City* citiesSortedAlphabetically[], int* counter);

/*
* Main function to sort the cities data
*
* inputFile = File where we read the datas
* outputFile = File where we write the sorted datas
*/
void sortCities(FILE* inputFile, FILE* outputFile);

#endif