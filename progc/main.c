#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "city.h"
#include "route.h"
#include "utils.h"


int main(int argc, char **argv)
{
    // File to get the data
    FILE* inputFile = fopenSafe(argv[1], "r");

    // File to write the sorted data
    FILE* outputFile = fopenSafe(argv[2], "w");
    
    // Sort cities
    if(strcmp(argv[3], "-t") == 0) {
        sortCities(inputFile, outputFile);
    } 
    // Sort routes
    else if (strcmp(argv[3], "-s") == 0) {
        sortRoutes(inputFile, outputFile);
    }

    return 0;
}