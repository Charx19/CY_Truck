#ifndef UTILS_H
#define UTILS_H

#define NAME_SIZE 64
#define LINE_SIZE 256

// Get the contents of a line to array, with a ";" separator
char** lineToArray(char* l);

// Get the maximum between two integers
int max2i(int a, int b);

// Get the minimum between two integers
int min2i(int a, int b);

// Get the maximum between two floats
float max2f(float a, float b);

// Get the minimum between two floats
float min2f(float a, float b);

// https://stackoverflow.com/questions/20462826/hash-function-for-strings-in-c
// Transform a string into a hash
int hash(char *str);

FILE* fopenSafe(char* filePath, char* mode);

#endif