#include "BinaryTree.h"

//Function for making new node of binary tree
BTNode *makeBinaryTreeNode(char *character_array, BTNode *left_children, BTNode* right_children) {
    BTNode *new_node = (BTNode *) malloc(sizeof(BTNode));
    new_node->character_array = strdup(character_array);
    new_node->left_child = left_children;
    new_node->right_child = right_children;
    return new_node;
}

//Function for clearing binary tree
void clearBinaryTree(BTNode *root_node) {
    if(root_node != NULL) {
        clearBinaryTree(root_node->left_child);
        clearBinaryTree(root_node->right_child);
        free(root_node->character_array);
        free(root_node);
    }
}

//Function for displaying binary tree
void displayBinaryTree(BTNode *root_node) {
    if(root_node != NULL) {
        displayBinaryTree(root_node->left_child);
        printf("%s ", root_node->character_array ? root_node->character_array : " ");
        displayBinaryTree(root_node->right_child);
    }
}