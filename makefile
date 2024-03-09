all: FileCompression

FileCompression: FileCompression.c
	gcc FileCompression.c  AugmentedTree.c BinaryTree.c CustomHeap.c Dictionary.c  -o fileCompressor
clean:
	rm fileCompressor