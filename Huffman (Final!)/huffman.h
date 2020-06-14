#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdio.h>

typedef struct _leaf
{
	long int ascii;
	long int occurs;
	struct _leaf* left;
	struct _leaf* right;
	struct _leaf* next;
} leaf;

typedef struct _bin_char
{
	long int num_ascii;
	long int num_binary;
} bin_char;

long int _char_in_string(unsigned char* string, unsigned char character, long int length);

void huffman(char* input_file, char* output_file_1, char* output_file_2, char* output_file_3);

leaf* _tree_enqueue(leaf** tree, long int character, long int occurence);

leaf* _root_enqueue(leaf** tree, leaf** root);

void _file_printer_1(FILE* output, long int num, leaf* tree, unsigned char* string);

void _file_printer_2(FILE* output, leaf* tree, bin_char** counter);

void _file_3_maker(unsigned char** bit_str, long int* bit_loc, leaf* tree, long int* str_loc);

void _destroy_tree(leaf* tree);

#endif
