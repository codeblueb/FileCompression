#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>

#include "AugmentedTree.h"
#include "BinaryTree.h"
#include "CustomHeap.h"
#include "Dictionary.h"

#define MAXIMUM_FILES 1000000

//This function stores the path of all the files in directories
void storeAllFilesPaths(char *path_of_directory, char **files, int *counter, int use_recursion) {

    struct stat link_path;
    stat(path_of_directory, &link_path);
    // if given path represents a file, just add the path to list and return
    if(S_ISREG(link_path.st_mode)) {
        files[*counter] = strdup(path_of_directory);
        //printf("%s\n", startPath);
        *counter += 1;
        return;
    }

    struct dirent * directory_entry;
    DIR * directory_path = opendir(path_of_directory); // open the directory
    if(directory_path == NULL) {
        printf("Check Input. Invalid path. Unable to open: %s\n", path_of_directory);
        exit(1);
    }

    // read entries from directory.
    while ((directory_entry = readdir(directory_path)) != NULL) {

        char *newPathString = malloc(sizeof(char) * (10 + strlen(path_of_directory) + strlen(directory_entry->d_name)));
        newPathString[0] = '\0';
        sprintf(newPathString, "%s/%s", path_of_directory, directory_entry->d_name);

        if(directory_entry-> d_type != DT_DIR) {
            files[*counter] = strdup(newPathString);
            *counter += 1;


        } else if(use_recursion && directory_entry -> d_type == DT_DIR &&
                  strcmp(directory_entry->d_name, ".") != 0 &&
                  strcmp(directory_entry->d_name, "..") != 0 ) {

            // This is called when recursion is happening
            storeAllFilesPaths(newPathString, files, counter, use_recursion);
        }
        free(newPathString);
    }
    closedir(directory_path);
}

//This function will check the extension of file
int checkFileExtensionForCompressedFile(char *file ) {
    file = strrchr(file, '.');
    if(file != NULL )
        return (strcmp(file, ".hcz") == 0);
    return 0;
}


//Compress Files function is used to Compress Files with the help of Codebook
void compress_files(char **files, int counter, char *code_book) {
    Dictionary *wordToCode = fetch_huffman_codebook(code_book);
    int count = 0;
    int processedFiles = 0;
    while(count < counter) {

        if(!checkFileExtensionForCompressedFile(files[count])) {
            printf("Compressing: %s\n", files[count]);

            int readerDesc = open(files[count], O_RDONLY);
            if(readerDesc < 0){
                printf("Error in reading file: %s\n", files[count]);
                //fflush(stdout);
                return;
            }

            // open file for writing the compressed version
            char *path = malloc(sizeof(char) * (strlen(files[count]) + 10));
            path[0] = '\0';
            strcat(path, files[count]);
            strcat(path, ".hcz");

            int writerDesc = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if(writerDesc < 0){
                printf("Error in writing file: %s\n", path);
                //fflush(stdout);
                free(path);
                return;
            }

            char word[500];
            int len = 0;
            char c;
            char *code;

            // read character by character
            while (read(readerDesc, &c, 1) == 1) {
                if(isspace(c)) {
                    // we need to break our character_array
                    if(len != 0) {
                        word[len] = '\0';
                        code = fetchKeyValueFromDictionary(wordToCode, word);
                        if(code != NULL) {
                            write(writerDesc, code, strlen(code));
                        }
                    }
                    len = 0;

                    // now insert your whitespace also to the tree.
                    word[0] = c;
                    word[1] = '\0';
                    // check if currently read character_array exists in dictionary
                    code = fetchKeyValueFromDictionary(wordToCode, word);
                    if(code != NULL) {
                        write(writerDesc, code, strlen(code));
                    }
                } else {
                    // add current character to the current character_array
                    word[len++] = c;
                }
            }

            // insert last character_array
            if(len != 0) {
                word[len] = '\0';
                code = fetchKeyValueFromDictionary(wordToCode, word);
                if(code != NULL) {
                    write(writerDesc, code, strlen(code));
                }
            }

            free(path);
            close(writerDesc);
            close(readerDesc);


            processedFiles++;
        }
        // move to next file
        count++;
    }

    clear_dictionary(wordToCode);
    printf("Compressed %d files successfully using %s codebook.\n", processedFiles, code_book);
}


//This function decompress a File
void decompress_files(char **filePaths, int numFiles, char *codeBook) {
    Dictionary *wordToCode = fetch_huffman_codebook(codeBook);
    Dictionary *CodeToWord = fetchDictionaryInReverseOrder(wordToCode);

    int count = 0;
    int processedFiles = 0;
    while(count < numFiles) {

        if(checkFileExtensionForCompressedFile(filePaths[count])) {
            printf("Decompressing: %s\n", filePaths[count]);
            int readerDesc = open(filePaths[count], O_RDONLY);
            if(readerDesc < 0){
                printf("Error in reading file: %s\n", filePaths[count]);
                return;
            }


            // create a path for output file with .hcz removed from fileName.
            char *outPath = malloc(sizeof(char) * (10 + strlen(filePaths[count])));
            strncpy(outPath, filePaths[count], strlen(filePaths[count]) - 4);
            outPath[strlen(filePaths[count]) - 4] = '\0';

            int writerDesc = open(outPath, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if(writerDesc < 0){
                printf("Error in writing file: %s\n", outPath);
                free(outPath);
                return;
            }

            // Now read the input file character by character
            char word[500];
            int len = 0;
            char c;
            while (read(readerDesc, &c, 1) == 1) {
                word[len++] = c;
                word[len] = '\0';

                // check if currently read code exists in dictionary
                char *token = fetchKeyValueFromDictionary(CodeToWord, word);
                if(token != NULL) {
                    // It was a valid character_array, and hence, we can print to file
                    write(writerDesc, token, strlen(token));

                    // reset current code,
                    len = 0;
                }
            }

            // close files.
            free(outPath);
            close(writerDesc);
            close(readerDesc);

            processedFiles++;
        }

        // move to next file
        count++;
    }

    // free the dictionaries
    clear_dictionary(wordToCode);
    clear_dictionary(CodeToWord);
    printf("Decompressed %d files successfully using %s codebook.\n", processedFiles, codeBook);
}




//This function is used to create codebook
void make_code_book(char **filePaths, int numFiles, char *outputCodeBook) {

    AugmentTree *wordCountTree = makeAugmentTree();

    int count = 0;
    int processedFiles = 0;
    while(count < numFiles) {

        if(!checkFileExtensionForCompressedFile(filePaths[count])) {
            printf("Processing: %s\n", filePaths[count]);
            int curr_file = open(filePaths[count], O_RDONLY);
            if(curr_file < 0){
                printf("File Not Found %s\n",  filePaths[count]);
                return;
            }

            char word[500];
            int len = 0;
            char c;

            while(read(curr_file, &c, 1) == 1) {
                if(isspace(c)) {
                    if(len != 0) {
                        word[len] = '\0';
                        addToAugmentTree(wordCountTree, word);
                    }
                    len = 0;

                    word[0] = c;
                    word[1] = '\0';
                    addToAugmentTree(wordCountTree, word);
                } else {
                    word[len++] = c;
                }
            }

            if(len != 0) {
                word[len] = '\0';
                addToAugmentTree(wordCountTree, word);
            }
            close(curr_file);

            processedFiles++;
        }
        count++;
    }
    CustomMinHeap *min_heap = changeAugmentToHeap(wordCountTree);
    clearAugmentTree(wordCountTree);
    BTNode *huff_tree = changeCustomHeapToHuffManTree(min_heap);
    clearCustomHeap(min_heap);

    Dictionary *codeDictionary = makeDictionaryFromHuffmanTree(huff_tree);
    clearBinaryTree(huff_tree);
    saveDictionaryToCodebook(codeDictionary, outputCodeBook);
    clear_dictionary(codeDictionary);

    printf("Processed %d files.\n", processedFiles);
    printf("Saved Codebook %s successfully\n", outputCodeBook);
}




//This fucntion is a utility function called when unwanted argument variables are passed
void usageUtils() {
    printf("./fileCompressor [-R] <flag> <path or file> |codebook|\n");
    printf("Valid Flags are :  b, c, d\n");
    printf("-R: recursive\n");
    printf("Codebook is required if flag is c or d\n");
}



// Main mthod to fetch arguments and rexecte require action
int main(int argc, char *argv[]) {

    int arguments;
    /**
     * Basically there are three operations
     * 1. Building CodeBook
     * 2. Compressing File
     * 3. Decompressing File
     * Recursion will be used if all files are not inside the same folder
     */
    int flag_for_file_build = 0;
    int flag_for_compress_file = 0;
    int flag_for_decompress_file = 0;


    int use_recursion = 0;
    int counter = 0;

    /**
     * Next we check for thr givrn correct files
     * 1. Directory Path to fethc Files
     * 2. Codebook Path
     */

    char *path_of_directory = NULL;
    char *path_of_codebook = NULL;

    /**
     * We will use getopt function here. getopt generally shows his own errors. but because we are
     * making out own error description, so we will not show default error, for that we need to set false
     * to show error.
     */
    opterr = 0;

   /**
    * Check the arguments given in command line, and set flags accoringly to the variables initialised above
    */
    while  ((arguments = getopt(argc, argv, "bcdR")) != -1) {
        // Switch statement to check the arguments and set flags
        switch (arguments)
        {
            case 'b':
                flag_for_file_build = 1;
                break;
            case 'c':
                flag_for_compress_file = 1;
                break;
            case 'd':
                flag_for_decompress_file = 1;
                break;
            case 'R':
                use_recursion = 1;
                break;
            case '?':
                if (isprint(optopt))
                    printf ("Unknown option `-%c'.\n", optopt);
                else
                    printf ("Unknown option character `\\x%x'.\n", optopt);
                exit(1);
            default:
                exit(1);
        }
    }

    while (optind < argc) {
        // Set the directory path and codebook path
        if(path_of_directory == NULL) {
            path_of_directory = argv[optind];
        } else if(path_of_codebook == NULL) {
            path_of_codebook = argv[optind];
        }
        optind++;
    }

    /**
     * Check how many total flags are set, for this
     * make a counter and increment setFlag when it hits banglasef ko ni magaa
     */
    int total_flags = 0;
    // Increment total flags if flag_for build is true
    if (flag_for_file_build) total_flags++;
    // Increment total flags for the  compresion flag true
    if (flag_for_compress_file) total_flags++;
    // Incremenet total flags for the decompression flag true
    if (flag_for_decompress_file) total_flags++;

    // If one flag was not passed in arguments then print error
    if(total_flags != 1) {
        printf("Please Choose one flag from b, c, d.\n");
        usageUtils();
        return 1;
    }

    /**
     * Compression and Decompression can be done using Codebook, So codebook shoudl be passed when
     * Flag is set to c, or d
     */

    if((flag_for_compress_file || flag_for_decompress_file) && (path_of_codebook == NULL)) {
        printf("Codebook is required while padding flags c or d.\n");
        usageUtils();
        return 1;
    }

    printf("Program Started : \n");
    char **files = malloc(sizeof(char *) * MAXIMUM_FILES);

       storeAllFilesPaths(path_of_directory, files, &counter, use_recursion);

    /**
     * Below operation is dependent on the king of operation chosen by the user
     * It can be -
     * 1. Building codebook
     * 2. Compressing Files
     * 3. Decompressing Files
     */
    if(flag_for_file_build) {

        make_code_book(files, counter, "HuffmanCodebook");

    } else if(flag_for_compress_file) {

        compress_files(files, counter, path_of_codebook);

    } else if(flag_for_decompress_file) {

        decompress_files(files, counter, path_of_codebook);

    }
    int i;
    for(i=0; i < counter; i++) {
        free(files[i]);
    }
    free(files);

    return 0;
}