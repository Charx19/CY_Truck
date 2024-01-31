#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl.h"
#include "utils.h"

AVL* createAVL(float key, void* element) {
    AVL* avl = (AVL*)malloc(sizeof(AVL));
    avl->key = key;
    avl->element = element;
    avl->left = NULL;
    avl->right = NULL;
    avl->height = 1;
    return avl;
}

int getHeight(AVL* node) {
	if (node == NULL)
		return 0;
	return node->height;
}

int getBalance(AVL* node) {
	if (node == NULL)
		return 0;
	return getHeight(node->left) - getHeight(node->right);
}

AVL* insertAVL(AVL* node, float key, void* element) 
{ 
    if (node == NULL) 
        return(createAVL(key, element)); 
  
    if (key < node->key) 
        node->left  = insertAVL(node->left, key, element); 
    else if (key > node->key) 
        node->right = insertAVL(node->right, key, element); 
    else // Equal keys are not allowed in BST 
        return node; 
  
    /* 2. Update height of this ancestor node */
    node->height = 1 + max2i(getHeight(node->left), 
                        getHeight(node->right)); 
  
    /* 3. Get the balance factor of this ancestor 
          node to check whether this node became 
          unbalanced */
    int balance = getBalance(node); 
  
    // If this node becomes unbalanced, then 
    // there are 4 cases 
  
    // Left Left Case 
    if (balance > 1 && key < node->left->key) 
        return rotateRight(node); 
  
    // Right Right Case 
    if (balance < -1 && key > node->right->key) 
        return rotateLeft(node); 
  
    // Left Right Case 
    if (balance > 1 && key > node->left->key) 
    { 
        node->left =  rotateLeft(node->left); 
        return rotateRight(node); 
    } 
  
    // Right Left Case 
    if (balance < -1 && key < node->right->key) 
    { 
        node->right = rotateRight(node->right); 
        return rotateLeft(node); 
    } 
  
    /* return the (unchanged) node pointer */
    return node; 
} 

AVL* searchKeyAVL(AVL* node, float key) {
    if (!node) 
        return NULL;
    
    if (key < node->key)
        return searchKeyAVL(node->left, key);
    else if (key > node->key)
        return searchKeyAVL(node->right, key);
    
    return node;
}

AVL* rotateLeft(AVL* x) {
    AVL* y = x->right;
    AVL* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + max2i(getHeight(x->left), getHeight(x->right));
    y->height = 1 + max2i(getHeight(y->left), getHeight(y->right));

    return y;
}

AVL* rotateRight(AVL* y) {
    AVL* x = y->left;
    AVL* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + max2i(getHeight(y->left), getHeight(y->right));
    x->height = 1 + max2i(getHeight(x->left), getHeight(x->right));

    return x;
}

AVL* rotateDoubleLeft(AVL *avl) {
    avl->right = rotateRight(avl->right);
    return rotateLeft(avl);
}

AVL* rotateDoubleRight(AVL *avl) {
    avl->left = rotateLeft(avl->left);
    return rotateRight(avl);
}

void preOrder(AVL*root) 
{ 
    if(root != NULL) 
    { 
        printf("%f ", root->key); 
        preOrder(root->left); 
        preOrder(root->right); 
    } 
} 

void destroyAVL(AVL* root) {
    if (root != NULL) {
        destroyAVL(root->left);
        destroyAVL(root->right);
        free(root);
    }
}