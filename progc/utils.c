#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

char** lineToArray(char* l)
{
	char c;
	char* s = &c;
	char** res = &s;
	int count = 0;
	char* tmp = l;
	char* last_comma = 0;
	char delim[3];
	delim[0] = ';';
	delim[1] = 0;
	
	// compte le nombre d'elements a extraire
	while (*tmp)
	{
		if (';' == *tmp)
			{
				count++;
				last_comma = tmp;
			}
			tmp++;
	}

	// ajoute de l'espace pour le token
	if(last_comma < (l + strlen(l) - 1))
	{
		count ++;
	}

	count++;
	res = malloc(sizeof(char*) * count);
	if(res == NULL)
	{
			printf("Error");
	}

	int idx  = 0;
	char* token = strtok(l, delim);

	while (token)
	{
		*(res + idx) = strdup(token);
		token = strtok(0, delim);
		idx++;
	}
	*(res + idx) = 0;

	return(res);
}

int max2i(int a, int b) {
	return (a > b) ? a : b;
}

int min2i(int a, int b) {
	return (a < b) ? a : b;
}

float max2f(float a, float b){
	return (a > b) ? a : b;
}

float min2f(float a, float b){
	return (a < b) ? a : b;
}

int hash(char *str)
{
    int hash = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        hash = 31 * hash + str[i];
    }
    return hash;
}

FILE* fopenSafe(char* filePath, char* mode) {
	FILE* file = fopen(filePath, mode);
	if (file == NULL){
		printf("Error opening %s\n", filePath);
		exit(2);
	}
	return file;
}