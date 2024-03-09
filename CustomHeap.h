#ifndef FILECOMPRESSION_CUSTOMHEAP_H
#define FILECOMPRESSION_CUSTOMHEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BinaryTree.h"

typedef struct HeapNode {
    int cost;
    BTNode *data;
} CustomHeapNode;

typedef struct MinHeap {
    CustomHeapNode **nodes;
    int heapSize;
    int heapCapacity;
} CustomMinHeap;

CustomMinHeap* createCustomHeap(int numEntries);

void addToCustomHeap(CustomMinHeap* heap, CustomHeapNode *heapNode);

CustomHeapNode* removeFromCustomHeap(CustomMinHeap* heap);

BTNode* changeCustomHeapToHuffManTree(CustomMinHeap *heap);

void displayCustomHeap(CustomMinHeap* heap);

void clearCustomHeap(CustomMinHeap* heap);

#endif //FILECOMPRESSION_CUSTOMHEAP_H
