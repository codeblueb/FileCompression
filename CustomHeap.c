#include "CustomHeap.h"

CustomMinHeap* createCustomHeap(int numEntries) {
    CustomMinHeap* heap = (CustomMinHeap *) malloc(sizeof(CustomMinHeap));
    heap->nodes = (CustomHeapNode **) malloc(numEntries * sizeof(CustomHeapNode *));
    heap->heapCapacity = numEntries;
    heap->heapSize = 0;
    return heap;
}

void incCustomnHeapCapacity(CustomMinHeap* heap) {
    int s = heap->heapSize;
    int i;

    CustomHeapNode **new_heap = malloc((s + 10) * sizeof(CustomHeapNode *));
    for(i=0; i<s; i++) {
        new_heap[i] = heap->nodes[i];
    }

    // free old array
    free(heap->nodes);

    // assign the new array on old reference
    heap->nodes = new_heap;
    heap->heapCapacity = s + 10;
}

// swap 2 indices in the heap array
void swap(CustomMinHeap *heap, int i, int j) {
    CustomHeapNode *temp = heap->nodes[i];
    heap->nodes[i] = heap->nodes[j];
    heap->nodes[j] = temp;
}


int parent(int i) { return (i-1)/2; }
int left(int i) { return (2*i + 1); }
int right(int i) { return (2*i + 2); }


void addToCustomHeap(CustomMinHeap* heap, CustomHeapNode *heapNode) {
    if(heap->heapSize == heap->heapCapacity) {
        incCustomnHeapCapacity(heap);
    }

    int i = heap->heapSize;
    heap->nodes[i] = heapNode;
    heap->heapSize += 1;

    while (i != 0 && heap->nodes[parent(i)]->cost > heap->nodes[i]->cost) {
        swap(heap, i, parent(i));
        i = parent(i);
    }
}


void traversDownCustomHeap(CustomMinHeap* heap, int i) {

    CustomHeapNode *current = heap->nodes[i];
    CustomHeapNode *left_node = NULL;
    CustomHeapNode *right_node = NULL;

    CustomHeapNode *smallest = current;

    if(left(i) < heap->heapSize) {
        left_node = heap->nodes[left(i)];
    }
    if(right(i) < heap->heapSize) {
        right_node = heap->nodes[right(i)];
    }

    if ((left_node != NULL) && (left_node->cost < smallest->cost)) {
        smallest = left_node;
    }
    if ((right_node != NULL) && (right_node->cost < smallest->cost)) {
        smallest = right_node;
    }

    if (smallest != current) {
        int swapIndex = (smallest == left_node) ? left(i) : right(i);
        swap(heap, i, swapIndex);
        traversDownCustomHeap(heap, swapIndex);
    }
}
//This function removes node from custom heap
CustomHeapNode* removeFromCustomHeap(CustomMinHeap* heap) {
    CustomHeapNode *pop_node = NULL;

    if (heap->heapSize <= 0) {
        return pop_node;
    }
    pop_node = heap->nodes[0];
    heap->heapSize--;
    heap->nodes[0] = heap->nodes[heap->heapSize];
    heap->nodes[heap->heapSize] = NULL;
    if(heap->heapSize != 0) {
        traversDownCustomHeap(heap, 0);
    }
    return pop_node;
}

BTNode* changeCustomHeapToHuffManTree(CustomMinHeap *heap) {

    if(heap->heapSize == 1) {
        BTNode *tmpRoot = makeBinaryTreeNode("", heap->nodes[0]->data, NULL);
        heap->nodes[0]->data = tmpRoot;
    }
    while(heap->heapSize > 1) {
        CustomHeapNode *temp1 = removeFromCustomHeap(heap);
        CustomHeapNode *temp2 = removeFromCustomHeap(heap);

        CustomHeapNode *consolidatedNode = (CustomHeapNode *) malloc(sizeof(CustomHeapNode));
        consolidatedNode->cost = temp1->cost + temp2->cost;
        consolidatedNode->data = makeBinaryTreeNode("", temp1->data, temp2->data);
        free(temp1);
        free(temp2);
        addToCustomHeap(heap, consolidatedNode);
    }
    if(heap->heapSize > 0) {
        return heap->nodes[0]->data;
    } else {
        return NULL;
    }
}


void displayCustomHeap(CustomMinHeap* heap) {
    int i = 0;
    while(i < heap->heapSize) {
        printf("Heap node %d cost: %d\n", i, heap->nodes[i]->cost);
        displayBinaryTree(heap->nodes[i]->data);
        printf("\n");
        i++;
    }
    printf("HeapSize: %d\n", heap->heapSize);
}

void clearCustomHeap(CustomMinHeap* heap) {
    int i = 0;
    while(i < heap->heapSize) {
        if(heap->nodes[i] != NULL) {
            free(heap->nodes[i]);
        }
        i++;
    }
    free(heap->nodes);
    free(heap);
}