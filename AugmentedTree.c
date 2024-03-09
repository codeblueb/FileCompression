#include "AugmentedTree.h"

//Make A Augment Tree in this function
AugmentTree *makeAugmentTree() {
    AugmentTree *tree = (AugmentTree *) malloc(sizeof(AugmentTree));
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

//Fucntion to make augmented tree node
AugmentTreeNode *makeAugmentTreeNode(char *char_array) {
    AugmentTreeNode *augment_node = (AugmentTreeNode *) malloc(sizeof(AugmentTreeNode));

    augment_node->char_array = strdup(char_array);
    augment_node->counter = 1;
    augment_node->left_child = NULL;
    augment_node->right_child = NULL;
    return augment_node;
}

//Add node to tree (Existing)
AugmentTreeNode *add_augment_node(AugmentTreeNode *root, char *char_array, int *treeCount) {
    if(root == NULL) {
        root = makeAugmentTreeNode(char_array);
        *treeCount += 1;
    } else {
        if(strcmp(root->char_array, char_array) == 0) {
            root->counter += 1;
        } else if(strcmp(root->char_array, char_array) < 0) {
            root->right_child = add_augment_node(root->right_child, char_array, treeCount);
        } else {
            root->left_child = add_augment_node(root->left_child, char_array, treeCount);
        }
    }
    return root;
}

void addToAugmentTree(AugmentTree *root, char *char_array) {
    root->root = add_augment_node(root->root, char_array, &(root->size));
}

void insertMinHeapInOrder(CustomMinHeap* min_heap, AugmentTreeNode *node) {
    if(node != NULL) {
        // process left_child subtree
        insertMinHeapInOrder(min_heap, node->left_child);

        // process current node
        CustomHeapNode *tmp = (CustomHeapNode *) malloc(sizeof(CustomHeapNode));
        tmp->cost = node->counter;
        tmp->data = makeBinaryTreeNode(node->char_array, NULL, NULL);
        addToCustomHeap(min_heap, tmp);

        // process right_child subtree
        insertMinHeapInOrder(min_heap, node->right_child);
    }
}

CustomMinHeap* changeAugmentToHeap(AugmentTree *augment_tree) {
    CustomMinHeap* heap = createCustomHeap(augment_tree->size);

    insertMinHeapInOrder(heap, augment_tree->root);

    return heap;
}

void printAugmentTreeNode(AugmentTreeNode *root_node) {
    if(root_node != NULL) {
        printAugmentTreeNode(root_node->left_child);
        printf("%s: %d\n", root_node->char_array, root_node->counter);
        printAugmentTreeNode(root_node->right_child);
    }
}

void printAugmentTree(AugmentTree *augment_tree) {
    printAugmentTreeNode(augment_tree->root);
}


void clearAugmentTreeNode(AugmentTreeNode *root_node) {
    if(root_node != NULL) {
        clearAugmentTreeNode(root_node->left_child);
        clearAugmentTreeNode(root_node->right_child);
        free(root_node->char_array);
        free(root_node);
    }
}
void clearAugmentTree(AugmentTree *augment_tree) {
    clearAugmentTreeNode(augment_tree->root);
    free(augment_tree);
}