#ifndef FILECOMPRESSION_AUGMENTEDTREE_H
#define FILECOMPRESSION_AUGMENTEDTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CustomHeap.h"

typedef struct AugmentTreeNode {
    int counter;
    char *char_array;
    struct AugmentTreeNode *left_child, *right_child;
} AugmentTreeNode;

typedef struct AugmentTree {
    AugmentTreeNode *root;
    int size;
} AugmentTree;

AugmentTree *makeAugmentTree();

void addToAugmentTree(AugmentTree *root, char *char_array);


void clearAugmentTree(AugmentTree *augment_tree);

CustomMinHeap* changeAugmentToHeap(AugmentTree *augment_tree);


#endif //FILECOMPRESSION_AUGMENTEDTREE_H
