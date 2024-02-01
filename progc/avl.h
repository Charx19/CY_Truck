#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include <stdlib.h>

// https://www.geeksforgeeks.org/insertion-in-an-avl-tree/

typedef struct _AVL {
    void* element;
    struct _AVL *left;
    struct _AVL *right;
    int height;
} AVL;

/*
* Construct a struct AVL
*
* element : The element we put in the AVL
*
* returns : A struct AVL
*/ 
AVL* createAVL(void* element);

/*
* Get the height of the AVL
*
* node = The AVL we want to get the height of
*
* returns : The height of the AVL
*/
int getHeight(AVL* node);

/*
* Get the balance factor of the AVL
*
* node = The AVL we want to get the balance factor of
*
* returns : The balance factor of the AVL
*/
int getBalance(AVL* node);

AVL* rotateLeft(AVL* x);

AVL* rotateRight(AVL* y);

AVL* rotateDoubleLeft(AVL *avl);

AVL* rotateDoubleRight(AVL *avl);

void destroyAVL(AVL* root);

#endif
