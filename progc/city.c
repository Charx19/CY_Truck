#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "city.h"
#include "avl.h"
#include "utils.h"

IdRoute* constructIdRoute(int id) {
	IdRoute* idRoute = (IdRoute*)malloc(sizeof(IdRoute));

	idRoute->id = id;

	return idRoute;
}

City* constructCity(char* townName, int idRoute) {
    City* newCity = (City*)malloc(sizeof(City));

	strcpy(newCity->name, townName);
	newCity->totalRoutes = 0;
	newCity->firstTown = 0;
	newCity->idRoutes = insertIdRouteAVL(newCity->idRoutes, newCity, &idRoute);

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

AVL* insertIdRouteAVL(AVL* idRoutes, City* city, void* idRouteToCompare) 
{ 
    if (idRoutes == NULL) {
		city->totalRoutes++;
		return(createAVL(constructIdRoute(*(int*)idRouteToCompare)));
	}
        
	IdRoute* idRoute = (IdRoute*)idRoutes->element;
  
    if (*(int*)idRouteToCompare < idRoute->id) {
		idRoutes->left  = insertIdRouteAVL(idRoutes->left, city, idRouteToCompare); 
	}
        
    else if (*(int*)idRouteToCompare > idRoute->id) {
		idRoutes->right = insertIdRouteAVL(idRoutes->right, city, idRouteToCompare); 
	}
        
    else {
		return idRoutes; 
	}
	
    idRoutes->height = 1 + max2i(getHeight(idRoutes->left), getHeight(idRoutes->right)); 
  
    int balance = getBalance(idRoutes); 

    // Left Left Case 
    if (balance > 1) {
		IdRoute* idRouteLeft = (IdRoute*)idRoutes->left->element;
		if (*(int*)idRouteToCompare < idRouteLeft->id) {
			return rotateRight(idRoutes);
		}
	}
         
    // Right Right Case 
    if (balance < -1) {
		IdRoute* idRouteRight = (IdRoute*)idRoutes->right->element;
		if (*(int*)idRouteToCompare > idRouteRight->id) {
			return rotateLeft(idRoutes); 
		}
		
	}
        
    // Left Right Case 
    if (balance > 1) { 
		IdRoute* idRouteLeft = (IdRoute*)idRoutes->left->element;
		if (*(int*)idRouteToCompare > idRouteLeft->id){
			return rotateDoubleRight(idRoutes);
		}
    } 
  
    // Right Left Case 
    if (balance < -1) { 
		IdRoute* idRouteRight = (IdRoute*)idRoutes->right->element;
		if (*(int*)idRouteToCompare < idRouteRight->id){
			return rotateDoubleLeft(idRoutes);
		}
    } 
  
    return idRoutes; 
}

AVL* insertCityAVL(AVL* cities, char* townName, int idRoute, int isStart) 
{ 
    if (cities == NULL) {
		
		return(createAVL(constructCity(townName, idRoute)));
	}
        
	City* city = (City*)cities->element;
  
    if (strcmp(townName, city->name) < 0) {
		cities->left  = insertCityAVL(cities->left, townName, idRoute, isStart); 
	}
        
    else if (strcmp(townName, city->name) > 0) {
		cities->right = insertCityAVL(cities->right, townName, idRoute, isStart); 
	}
        
    else {
		city->idRoutes = insertIdRouteAVL(city->idRoutes, city, &idRoute);

		if (isStart == 1){
			city->firstTown++;
		}
		return cities; 
	}
	
    cities->height = 1 + max2i(getHeight(cities->left), getHeight(cities->right)); 
  
    int balance = getBalance(cities); 

    // Left Left Case 
    if (balance > 1) {
		City* cityLeft = (City*)cities->left->element;
		if (strcmp(townName, cityLeft->name) < 0) {
			return rotateRight(cities);
		}
	}
         
    // Right Right Case 
    if (balance < -1) {
		City* cityRight = (City*)cities->right->element;
		if (strcmp(townName, cityRight->name) > 0) {
			return rotateLeft(cities); 
		}
		
	}
        
    // Left Right Case 
    if (balance > 1) { 
		City* cityLeft = (City*)cities->left->element;
		if (strcmp(townName, cityLeft->name) > 0){
			return rotateDoubleRight(cities);
		}
    } 
  
    // Right Left Case 
    if (balance < -1) { 
		City* cityRight = (City*)cities->right->element;
		if (strcmp(townName, cityRight->name) < 0){
			return rotateDoubleLeft(cities);
		}
    } 
  
    return cities; 
} 

AVL* insertCityTotalRoutesSortedAVL(AVL* citiesTotalRoutesSorted, int totalRoutes, City* element)
{
	if (citiesTotalRoutesSorted == NULL) {
		return(createAVL(element));
	}
        
	City* city = (City*)citiesTotalRoutesSorted->element;
  
    if (totalRoutes < city->totalRoutes) {
		citiesTotalRoutesSorted->left  = insertCityTotalRoutesSortedAVL(citiesTotalRoutesSorted->left, totalRoutes, element); 
	}
        
    else if (totalRoutes > city->totalRoutes) {
		citiesTotalRoutesSorted->right = insertCityTotalRoutesSortedAVL(citiesTotalRoutesSorted->right, totalRoutes, element); 
	}
        
    else {
		return citiesTotalRoutesSorted; 
	}
	
    citiesTotalRoutesSorted->height = 1 + max2i(getHeight(citiesTotalRoutesSorted->left), getHeight(citiesTotalRoutesSorted->right)); 
  
    int balance = getBalance(citiesTotalRoutesSorted); 

    // Left Left Case 
    if (balance > 1) {
		City* cityLeft = (City*)citiesTotalRoutesSorted->left->element;
		if (totalRoutes < cityLeft->totalRoutes) {
			return rotateRight(citiesTotalRoutesSorted);
		}
	}
         
    // Right Right Case 
    if (balance < -1) {
		City* cityRight = (City*)citiesTotalRoutesSorted->right->element;
		if (totalRoutes > cityRight->totalRoutes) {
			return rotateLeft(citiesTotalRoutesSorted); 
		}
		
	}
        
    // Left Right Case 
    if (balance > 1) { 
		City* cityLeft = (City*)citiesTotalRoutesSorted->left->element;
		if (totalRoutes > cityLeft->totalRoutes){
			return rotateDoubleRight(citiesTotalRoutesSorted);
		}
    } 
  
    // Right Left Case 
    if (balance < -1) { 
		City* cityRight = (City*)citiesTotalRoutesSorted->right->element;
		if (totalRoutes < cityRight->totalRoutes){
			return rotateDoubleLeft(citiesTotalRoutesSorted);
		}
    } 
  
    return citiesTotalRoutesSorted; 
}

AVL* constructCitiesTotalRoutesSorted(AVL* cities, AVL* citiesTotalRoutesSorted) 
{ 
    if(cities != NULL) 
    { 
		citiesTotalRoutesSorted = constructCitiesTotalRoutesSorted(cities->left, citiesTotalRoutesSorted);

		City* city = (City*)cities->element;
		citiesTotalRoutesSorted = insertCityTotalRoutesSortedAVL(citiesTotalRoutesSorted, city->totalRoutes, city);

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

	// Get file contents
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
			printf("Conversion Error, string is not a valid number (Column 1)\n");
			exit(1);
		}
		if (sscanf(lineArray[1], "%d", &idStep) != 1) {
			printf("Conversion Error, string is not a valid number (Column 2)\n");
			exit(1);
		}
		strcpy(townA, lineArray[2]);
		strcpy(townB, lineArray[3]);
		if (sscanf(lineArray[4], "%d", &distance) != 1) {
			printf("Conversion Error, string is not a valid number (Column 5)\n");
			exit(1);
		}
		strcpy(driverName, lineArray[5]);

		if(idStep == 1) {
			cities = insertCityAVL(cities, townA, idRoute, 1);
		}
		else {
			cities = insertCityAVL(cities, townA, idRoute, 0);
		}
		cities = insertCityAVL(cities, townB, idRoute, 0);
	}

	fclose(inputFile);
	//

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
