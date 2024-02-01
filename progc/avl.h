#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include <stdlib.h>

// https://www.geeksforgeeks.org/insertion-in-an-avl-tree/

typedef struct _AVL {
    float key;
    void* element;
    struct _AVL *left;
    struct _AVL *right;
    int height;
} AVL;

AVL* createAVL(float key, void* element);

int getHeight(AVL* node);

int getBalance(AVL* node);

AVL* insertAVL(AVL* node, float key, void* element);

AVL* searchKeyAVL(AVL* node, float key);

AVL* rotateLeft(AVL* x);

AVL* rotateRight(AVL* y);

AVL* rotateDoubleLeft(AVL *avl);

AVL* rotateDoubleRight(AVL *avl);

void destroyAVL(AVL* root);

#endif
