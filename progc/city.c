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

void bubbleSort(City* arr[], int limit) {
    int i, j;
    City* temp;

    for (i = 0; i < limit - 1; i++) {
        for (j = 0; j < limit - i - 1; j++) {
            if (strcmp(arr[j]->name, arr[j + 1]->name) > 0) {
                // swap the elements if they are in the wrong order
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

AVL* constructCitiesSorted(AVL* cities, AVL* citiesSorted) 
{ 
    if(cities != NULL) 
    { 
		citiesSorted = constructCitiesSorted(cities->left, citiesSorted);

		City* city = (City*)cities->element;
		AVL* element = NULL;
		element = searchKeyAVL(citiesSorted, city->totalRoutes);
		if (element == NULL) {
			citiesSorted = insertAVL(citiesSorted, city->totalRoutes, city);
		}

        citiesSorted = constructCitiesSorted(cities->right, citiesSorted); 
    } 
	return citiesSorted;
} 

void getTopCities(AVL* cities, FILE* outputFile, City* citiesSortedAlphabetically[], int* counter) 
{ 
    if(cities != NULL && *counter > 0) 
    { 
		getTopCities(cities->right, outputFile, citiesSortedAlphabetically, counter); 
		if (*counter == 0) {
			return;
		}
		
		City* city = (City*)cities->element;
		(*counter)--;
		citiesSortedAlphabetically[*counter] = city;
        
		getTopCities(cities->left, outputFile, citiesSortedAlphabetically, counter); 
		if (*counter == 0) {
			return;
		}
    } 
} 

void sortCities(FILE* inputFile, FILE* outputFile) {
	// Racine de l'arbre AVL
	AVL* cities = NULL;

	int idRoute, idStep, distance;
	char townA[NAME_SIZE], townB[NAME_SIZE], driverName[NAME_SIZE];
	
	char line[LINE_SIZE];
	char** lineArray = NULL;

	printf("Making cities AVL... 1/4\n");
	
    fgets(line, sizeof(line), inputFile); // Skip header
	while(fgets(line, sizeof(line), inputFile) != NULL) // Read each line of input file
	{
		// et on les sépares par leur ';' avant de stocker les informations importantes
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

    citiesRoutesSorted = constructCitiesSorted(cities, citiesRoutesSorted);

	printf("Get top 10 cities with most routes... 3/4\n");

	// Array for the top ten cities with the most routes
	City* citiesSortedAlphabetically[10] = {NULL};

	int counter = 10;
	
	// Get the top 10 city with the most routes
	getTopCities(citiesRoutesSorted, outputFile, citiesSortedAlphabetically, &counter);

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
