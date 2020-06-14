#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdio.h>

typedef struct _Node
{
	unsigned char ascii;
	long int occurs;
	struct _Node* next;
} Node;

typedef struct _leaf
{
	int ascii;
	long int occurs;
	struct _leaf* left;
	struct _leaf* right;
	struct _leaf* next;
} leaf;

void file256(char* input_file, char* output_file);

long int char_in_string(char* string, unsigned char character, long int length);

void print_node(Node* list, FILE* output);

Node* enqueue(Node** node, unsigned char character, long int occurence);

void one_line_file(char* input_file, char* output_file);

void destroy_node(Node* list);

void huffman(char* input_file, char* output_file);

leaf* tree_enqueue(leaf** tree, int character, long int occurence);

leaf* dequeue(leaf** tree);

leaf* root_enqueue(leaf** tree, leaf** root);

void file_printer(FILE* output, int num, leaf* tree, char* string);

#endif
