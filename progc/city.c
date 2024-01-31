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

void bubbleSort(City* arr[], int n) {
    int i, j;
    City* temp;

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (strcmp(arr[j]->name, arr[j + 1]->name) > 0) {
                // Swap the elements if they are in the wrong order
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

void getTopCities(AVL* cities, FILE* outputFile, City* citiesSortedAlphabetacally[], int* counter) 
{ 
    if(cities != NULL && *counter > 0) 
    { 
		getTopCities(cities->right, outputFile, citiesSortedAlphabetacally, counter); 
		if (*counter == 0) {
			return;
		}
		
		City* city = (City*)cities->element;
		(*counter)--;
		citiesSortedAlphabetacally[*counter] = city;
        
		getTopCities(cities->left, outputFile, citiesSortedAlphabetacally, counter); 
		if (*counter == 0) {
			return;
		}
    } 
} 

void sortT(FILE* inputFile, FILE* outputFile) {
	// Racine de l'arbre AVL
	AVL* cities = NULL;

	int idRoute, idEtape, distance;
	char villeA[TAILLE], villeB[TAILLE], driverName[TAILLE];
	char ligne[TAILLE];
	char** ligneTab = NULL;
	int h = 1;
	
    fgets(ligne, sizeof(ligne), inputFile);
	while(fgets(ligne, sizeof(ligne), inputFile) != NULL) // on lit chaque ligne
	{
		// et on les sépares par leur ';' avant de stocker les informations importantes
		ligneTab = ligneToTab(ligne);
		if (sscanf(ligneTab[0], "%d", &idRoute) != 1) {
			printf("Erreur de conversion. La chaîne n'est pas un nombre valide, columne 1.\n");
			exit(1);
		}
		if (sscanf(ligneTab[1], "%d", &idEtape) != 1) {
			printf("Erreur de conversion. La chaîne n'est pas un nombre valide, columne 2.\n");
			exit(1);
		}
		strcpy(villeA, ligneTab[2]);
		strcpy(villeB, ligneTab[3]);
		if (sscanf(ligneTab[4], "%d", &distance) != 1) {
			printf("Erreur de conversion. La chaîne n'est pas un nombre valide3.\n");
			exit(1);
		}
		strcpy(driverName, ligneTab[5]);

		AVL* element = searchKeyAVL(cities, hash(villeA));
        if (element == NULL){
            cities = insertAVL(cities, hash(villeA), constructCity(villeA, driverName));
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
        
        element = searchKeyAVL(cities, hash(villeB));
        if (element == NULL){
            cities = insertAVL(cities, hash(villeB), constructCity(villeB, driverName));
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

	AVL* citiesSorted = NULL;

    citiesSorted = constructCitiesSorted(cities, citiesSorted);

	City* citiesSortedAlphabetacally[10] = {NULL};

	int counter = 10;
	
	getTopCities(citiesSorted, outputFile, citiesSortedAlphabetacally, &counter);
	
	bubbleSort(citiesSortedAlphabetacally, 10);
	
	for (int i = 0; i < 10; i++){
		if (citiesSortedAlphabetacally[i] != NULL){
			City* city = citiesSortedAlphabetacally[i];
			fprintf(outputFile, "%s;%d;%d\n", city->name, city->totalRoutes, city->firstTown);
		}
	}

	fclose(outputFile);

	// Libérer la mémoire utilisée par l'arbre AVL
	destroyAVL(cities);
}
