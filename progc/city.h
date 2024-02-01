#ifndef CITY_H
#define CITY_H

#include "avl.h"
#include "utils.h"

typedef struct {
	char name[NAME_SIZE]; // Name of the city
	int totalRoutes; // The total routes count tied to that city
	int firstTown; // drivers count
	AVL* drivers; // AVL for the drivers so its doesn't count twice
} City;

/*
* Construct a struct City
*
* townName : The town name
* driverName : The driver name
*
* returns : A struct City
*/ 
City* constructCity(char* townName, char* driverName);

/*
* Sort a array of City by their name alphabetically
*
* citiesSortedAlphabetically : Array where we put the cities that we are going to sort alphabetically later
* arraySize : The size of the array
*/ 
void bubbleSort(City* citiesSortedAlphabetically[], int arraySize);

/*
* Construct a AVL containing the cities with the total routes as key
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