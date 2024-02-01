#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl.h"
#include "utils.h"

AVL* createAVL(void* element) {
    AVL* avl = (AVL*)malloc(sizeof(AVL));
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

void destroyAVL(AVL* root) {
    if (root != NULL) {
        destroyAVL(root->left);
        destroyAVL(root->right);
        free(root);
    }
}