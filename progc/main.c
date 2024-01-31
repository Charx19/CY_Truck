#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "city.h"
#include "route.h"
#include "utils.h"


int main(int argc, char **argv)
{
    // Traitement des données du fichier
    FILE* inputFile = fopenSafe(argv[1], "r"); // "../data/data.csv"

    FILE* outputFile = fopenSafe(argv[2], "w");
    
    if(strcmp(argv[3], "-t") == 0){
        sortT(inputFile, outputFile);
    } else if (strcmp(argv[3], "-s") == 0){
        sortS(inputFile, outputFile);
    }

    return 0;
}