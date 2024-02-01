#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "city.h"
#include "avl.h"
#include "utils.h"

City* constructCity(char* townName, char* driverName) {
    City* newCity = (City*)malloc(sizeof(City));

	strcpy(newCity->name, townName);
	newCity->totalRoutes = 1;
	newCity->firstTown = 1;
	newCity->drivers = insertAVL(newCity->drivers, hash(driverName), NULL);

    return newCity;
}

void bubbleSort(City* citiesSortedAlphabetically[], int arraySize) {
    int i, j;
    City* temp;

    for (i = 0; i < arraySize - 1; i++) {
        for (j = 0; j < arraySize - i - 1; j++) {
            if (strcmp(citiesSortedAlphabetically[j]->name, citiesSortedAlphabetically[j + 1]->name) > 0) {
                // swap the elements if they are in the wrong order
                temp = citiesSortedAlphabetically[j];
                citiesSortedAlphabetically[j] = citiesSortedAlphabetically[j + 1];
                citiesSortedAlphabetically[j + 1] = temp;
            }
        }
    }
}

AVL* constructCitiesTotalRoutesSorted(AVL* cities, AVL* citiesTotalRoutesSorted) 
{ 
    if(cities != NULL) 
    { 
		citiesTotalRoutesSorted = constructCitiesTotalRoutesSorted(cities->left, citiesTotalRoutesSorted);

		City* city = (City*)cities->element;
		AVL* element = NULL;
		element = searchKeyAVL(citiesTotalRoutesSorted, city->totalRoutes);
		if (element == NULL) {
			citiesTotalRoutesSorted = insertAVL(citiesTotalRoutesSorted, city->totalRoutes, city);
		}

        citiesTotalRoutesSorted = constructCitiesTotalRoutesSorted(cities->right, citiesTotalRoutesSorted); 
    } 
	return citiesTotalRoutesSorted;
} 

void getTopCities(AVL* citiesTotalRoutesSorted, City* citiesSortedAlphabetically[], int* counter) 
{ 
    if(citiesTotalRoutesSorted != NULL && *counter > 0) 
    { 
		getTopCities(citiesTotalRoutesSorted->right, citiesSortedAlphabetically, counter); 
		if (*counter == 0) {
			return;
		}
		
		City* city = (City*)citiesTotalRoutesSorted->element;
		(*counter)--;
		citiesSortedAlphabetically[*counter] = city;
        
		getTopCities(citiesTotalRoutesSorted->left, citiesSortedAlphabetically, counter); 
		if (*counter == 0) {
			return;
		}
    } 
} 

void sortCities(FILE* inputFile, FILE* outputFile) {
	AVL* cities = NULL;

	int idRoute, idStep, distance;
	char townA[NAME_SIZE], townB[NAME_SIZE], driverName[NAME_SIZE];
	
	char line[LINE_SIZE];
	char** lineArray = NULL;

	printf("Making cities AVL... 1/4\n");
	
    fgets(line, sizeof(line), inputFile); // Skip header
	while(fgets(line, sizeof(line), inputFile) != NULL) // Read each line of input file
	{
		// Separating by ';' to get informations
		lineArray = lineToArray(line);
		if (sscanf(lineArray[0], "%d", &idRoute) != 1) {
			printf("Erreur de conversion. La chaîne n'est pas un nombre valide, columne 1.\n");
			exit(1);
		}
		if (sscanf(lineArray[1], "%d", &idStep) != 1) {
			printf("Erreur de conversion. La chaîne n'est pas un nombre valide, columne 2.\n");
			exit(1);
		}
		strcpy(townA, lineArray[2]);
		strcpy(townB, lineArray[3]);
		if (sscanf(lineArray[4], "%d", &distance) != 1) {
			printf("Erreur de conversion. La chaîne n'est pas un nombre valide3.\n");
			exit(1);
		}
		strcpy(driverName, lineArray[5]);

		AVL* element = searchKeyAVL(cities, hash(townA));
        if (element == NULL){
            cities = insertAVL(cities, hash(townA), constructCity(townA, driverName));
        }
        else {
            City* city = (City*)element->element;
            city->totalRoutes++;
			AVL* driver = searchKeyAVL(city->drivers, hash(driverName));
			if (driver == NULL){
				city->drivers = insertAVL(city->drivers, hash(driverName), NULL);
				city->firstTown++;
			}
        }
        
        element = searchKeyAVL(cities, hash(townB));
        if (element == NULL){
            cities = insertAVL(cities, hash(townB), constructCity(townB, driverName));
        } else {
            City* city = (City*)element->element;
            city->totalRoutes++;
			AVL* driver = searchKeyAVL(city->drivers, hash(driverName));
			if (driver == NULL){
				city->drivers = insertAVL(city->drivers, hash(driverName), NULL);
				city->firstTown++;
			}
        }
	}

	fclose(inputFile);

	printf("Making cities AVL by routes... 2/4\n");

	// Construct a AVL from the routes count as keys
	AVL* citiesRoutesSorted = NULL;

    citiesRoutesSorted = constructCitiesTotalRoutesSorted(cities, citiesRoutesSorted);

	printf("Get top 10 cities with most routes... 3/4\n");

	// Array for the top ten cities with the most routes
	City* citiesSortedAlphabetically[10] = {NULL};

	int counter = 10;
	
	// Get the top 10 city with the most routes
	getTopCities(citiesRoutesSorted, citiesSortedAlphabetically, &counter);

	printf("Sort cities alphabetically... 4/4\n");
	
	// Sort the cities alphabetically
	bubbleSort(citiesSortedAlphabetically, 10);
	
	// Write cities to output file
	for (int i = 0; i < 10; i++){
		if (citiesSortedAlphabetically[i] != NULL){
			City* city = citiesSortedAlphabetically[i];
			fprintf(outputFile, "%s;%d;%d\n", city->name, city->totalRoutes, city->firstTown);
		}
	}

	fclose(outputFile);

	// Free memory from AVL trees
	destroyAVL(cities);
	destroyAVL(citiesRoutesSorted);
}
