#include "Dictionary.h"

Dictionary *make_dictionary() {
    Dictionary *dictionary_tree = (Dictionary *) malloc(sizeof(Dictionary));
    dictionary_tree->root = NULL;
    return dictionary_tree;
}

DictionaryNode *make_dictionary_single_node(char *key, char *value) {
    DictionaryNode *new_dict_node = (DictionaryNode *) malloc(sizeof(DictionaryNode));
    new_dict_node->key = strdup(key);
    new_dict_node->value = strdup(value);
    new_dict_node->right = NULL;
    new_dict_node->left = NULL;
    return new_dict_node;
}


void addKeyValueToDictionary(Dictionary *dict, char *key, char *value) {
    if(dict->root == NULL) {
        dict->root = make_dictionary_single_node(key, value);
    } else {
        DictionaryNode *start = dict->root;

        while(1) {
            // if current node has same key
            if(strcmp(start->key, key) == 0) {
                // update value, and return
                free(start->value);
                start->value = strdup(value);
                return;

            } else if(strcmp(start->key, key) < 0) {
                if(start->right == NULL) {
                    start->right = make_dictionary_single_node(key, value);
                    return;
                } else {
                    start = start->right;
                }
            } else {
                if(start->left == NULL) {
                    start->left = make_dictionary_single_node(key, value);
                    return;
                } else {
                    start = start->left;
                }
            }
        }
    }
}

char* fetchKeyValueFromDictionary(Dictionary *ditionary, char *key) {
    DictionaryNode *start = ditionary->root;
    while(start != NULL) {
        if(strcmp(start->key, key) == 0) {
            return start->value;
        } else if(strcmp(start->key, key) < 0) {
            start = start->right;
        } else {
            start = start->left;
        }
    }
    return NULL;
}

void deepCopyNodeToDictionary(Dictionary *dictionary, DictionaryNode *node) {
    if(node != NULL) {
        deepCopyNodeToDictionary(dictionary, node->left);
        addKeyValueToDictionary(dictionary, node->value, node->key);
        deepCopyNodeToDictionary(dictionary, node->right);
    }
}

Dictionary* fetchDictionaryInReverseOrder(Dictionary *dictionary) {
    Dictionary *reverse_dictionary = make_dictionary();
    deepCopyNodeToDictionary(reverse_dictionary, dictionary->root);
    return reverse_dictionary;
}

void fetchAllTypeOfEscapeSequences(char *str, char *sequences) {
    int i = 0;
    int j = 0;
    while(str[i] != '\0') {
        if(str[i] == '\n') {
            sequences[j++] = '\\';
            sequences[j++] = 'n';
        } else if(str[i] == '\t') {
            sequences[j++] = '\\';
            sequences[j++] = 't';
        } else if(str[i] == '\a') {
            sequences[j++] = '\\';
            sequences[j++] = 'a';
        } else if(str[i] == '\b') {
            sequences[j++] = '\\';
            sequences[j++] = 'b';
        } else if(str[i] == '\f') {
            sequences[j++] = '\\';
            sequences[j++] = 'f';
        } else if(str[i] == '\v') {
            sequences[j++] = '\\';
            sequences[j++] = 'v';
        } else if(str[i] == '\r') {
            sequences[j++] = '\\';
            sequences[j++] = 'r';
        } else if(str[i] == '\\') {
            sequences[j++] = '\\';
            sequences[j++] = '\\';
        } else {
            sequences[j++] = str[i];
        }
        i++;
    }
    sequences[j] = '\0';
}

void fetchAllTypeOfUnEscapeSequences(char *str, char *result) {
    int i = 0;
    int j = 0;
    while(str[i] != '\0') {
        if(str[i] == '\\') {
            if(str[i+1] == 'n') {
                result[j++] = '\n';
            } else if(str[i+1] == 't') {
                result[j++] = '\t';
            } else if(str[i+1] == 'a') {
                result[j++] = '\a';
            } else if(str[i+1] == 'b') {
                result[j++] = '\b';
            } else if(str[i+1] == 'f') {
                result[j++] = '\f';
            } else if(str[i+1] == 'v') {
                result[j++] = '\v';
            } else if(str[i+1] == 'r') {
                result[j++] = '\r';
            } else if(str[i+1] == '\\') {
                result[j++] = '\\';
            }
            i += 2;
        } else {
            result[j++] = str[i++];
        }
    }
    result[j] = '\0';
}
void write_dictionary_node(DictionaryNode *node, int writerDesc) {
    if(node != NULL) {
        write_dictionary_node(node->left, writerDesc);

        char escaped[500];
        fetchAllTypeOfEscapeSequences(node->key, escaped);
        write(writerDesc,node->value, strlen(node->value));
        write(writerDesc,"\t", 1);
        write(writerDesc,escaped, strlen(escaped));
        write(writerDesc,"\n", 1);

        write_dictionary_node(node->right, writerDesc);
    }
}
void saveDictionaryToCodebook(Dictionary *dictionary, char *files) {

    int get_file = open(files, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(get_file < 0){
        printf("File Not Found %s\n", files);
        exit(1);
    }
    write_dictionary_node(dictionary->root, get_file);
    close(get_file);
}

//Function converts codebook to dictionary
Dictionary *fetch_huffman_codebook(char *code_book) {
    int open_file = open(code_book, O_RDONLY);
    if(open_file < 0){
        printf("File Not Found: %s\n", code_book);
        exit(1);
    }
    Dictionary *dictionary = make_dictionary();

    char token_array[500], code_array[500], unescape_sequence_token[500];
    int length_of_code = 0;
    int length_of_token = 0;
    int got_code = 1;
    int got_token = 0;
    char c;
    while (read(open_file, &c, 1) != 0) {
        if((c == '\n') && got_token) {
            token_array[length_of_token] = '\0';
            code_array[length_of_code] = '\0';
            fetchAllTypeOfUnEscapeSequences(token_array, unescape_sequence_token);
            addKeyValueToDictionary(dictionary, unescape_sequence_token, code_array);
            length_of_token = 0;
            length_of_code = 0;
            got_token = 0;
            got_code = 1;
        } else if(c == '\t' && got_code) {
            got_token = 1;
            got_code = 0;
        } else if(got_code) {
            code_array[length_of_code++] = c;
        } else if(got_token) {
            token_array[length_of_token++] = c;
        }
    };
    close(open_file);
    return dictionary;
}


void addWordsToDictionaryFromHuffman(Dictionary *dict, BTNode *node, char *path) {
    if(node != NULL) {
        if((node->left_child == NULL) && (node->right_child == NULL)) {
            addKeyValueToDictionary(dict, node->character_array, path);
        } else {
            char *newPath = malloc(sizeof(char) * (10 + strlen(path)));
            newPath[0] = '\0';
            strcat(newPath, path);
            strcat(newPath, "0");
            addWordsToDictionaryFromHuffman(dict, node->left_child, newPath);

            newPath[0] = '\0';
            strcat(newPath, path);
            strcat(newPath, "1");
            addWordsToDictionaryFromHuffman(dict, node->right_child, newPath);

            free(newPath);
        }
    }
}

Dictionary *makeDictionaryFromHuffmanTree(BTNode *root) {
    Dictionary *dictionary = make_dictionary();
    addWordsToDictionaryFromHuffman(dictionary, root, "");
    return dictionary;
}

void printDictNode(DictionaryNode *root) {

    if(root != NULL) {
        printDictNode(root->left);
        printf("(%s |%s|) ", root->key ? root->key : "", root->value);
        printDictNode(root->right);
    }
}
void printDict(Dictionary *dict) {
    printDictNode(dict->root);
    printf("\n");
}

void clear_dicitionary_node(DictionaryNode *root) {
    if(root != NULL) {
        DictionaryNode *del = root;
        clear_dicitionary_node(root->left);
        clear_dicitionary_node(root->right);
        free(root->key);
        free(root->value);
        free(root);
    }
}
void clear_dictionary(Dictionary *dictionary) {
    clear_dicitionary_node(dictionary->root);
    free(dictionary);
}