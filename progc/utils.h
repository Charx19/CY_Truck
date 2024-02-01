#ifndef UTILS_H
#define UTILS_H

#define NAME_SIZE 64
#define LINE_SIZE 256

/*
* Get the contents of a line to array, with a ";" separator
*
* l = The line we are reading
*
* returns : The line contents
*/
char** lineToArray(char* l);

/*
* Get the maximum between two integers
*
* a = a int value
* b = a other int value
*
* returns : the maximum of a and b
*/ 
int max2i(int a, int b);

/*
* Get the minimum between two integers
*
* a = a int value
* b = a other int value
*
* returns : the minimum of a and b
*/ 
int min2i(int a, int b);

/*
* Get the maximum between two floats
*
* a = a float value
* b = a other float value
*
* returns : the maximum of a and b
*/ 
float max2f(float a, float b);

/*
* Get the minimum between two floats
*
* a = a float value
* b = a other float value
*
* returns : the minimum of a and b
*/ 
float min2f(float a, float b);

/*
* Open safely a file, if its fails, it send a error instead
*
* filePath : The path of the file we want to open
* mode : The mode we want to use for this file
*
* returns : The file we opened
*/
FILE* fopenSafe(char* filePath, char* mode);

#endif