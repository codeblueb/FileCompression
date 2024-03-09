
#ifndef FILECOMPRESSION_DICTIONARY_H
#define FILECOMPRESSION_DICTIONARY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include "BinaryTree.h"

typedef struct DictNode {
    char *key;
    char *value;
    struct DictionaryNode *left, *right;
} DictionaryNode;

typedef struct Dictionary {
    DictionaryNode *root;
} Dictionary;

Dictionary *make_dictionary();

// Add a key to the dictionary along with its value.
// if key exists, the value is updated.
void addKeyValueToDictionary(Dictionary *dict, char *key, char *value);

char* fetchKeyValueFromDictionary(Dictionary *ditionary, char *key);

Dictionary* fetchDictionaryInReverseOrder(Dictionary *dictionary);

void saveDictionaryToCodebook(Dictionary *dictionary, char *files);

Dictionary *fetch_huffman_codebook(char *code_book);

Dictionary *makeDictionaryFromHuffmanTree(BTNode *root);

void printDict(Dictionary *dict);

void clear_dictionary(Dictionary *dictionary);
#endif //FILECOMPRESSION_DICTIONARY_H
