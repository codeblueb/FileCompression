#ifndef FILECOMPRESSION_BINARYTREE_H
#define FILECOMPRESSION_BINARYTREE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structure for node of Binary Tree
typedef struct BinaryTreeNode {
    // Pointing towards characters
    char *character_array;
    // left_child and right_child child pointers
    struct BTNode *left_child, *right_child;
} BTNode;

//Function used for making new binary tree ndde
BTNode *makeBinaryTreeNode(char *character_array, BTNode *left_children, BTNode* right_children);

//function for clearing binary tree
void clearBinaryTree(BTNode *root_node);

//function for displaying binary tree
void displayBinaryTree(BTNode *root_node);
#endif //FILECOMPRESSION_BINARYTREE_H
