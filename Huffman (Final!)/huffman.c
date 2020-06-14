#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "huffman.h"

long int _char_in_string(unsigned char* string, unsigned char character, long int length)
{
	long int char_counter = 0;
	for(long int i = 0; i < length; i++)
	{
		if(string[i] == character)
		{
			char_counter += 1;
		}
	}

	return char_counter;
}

void _destroy_tree(leaf* tree)
{
	if(tree->left != NULL)
	{
		leaf* left_branch = tree->left;
		_destroy_tree(left_branch);
	}
	if(tree->right != NULL)
	{
		leaf* right_branch = tree->right;
		_destroy_tree(right_branch);
	}
	free(tree);
}

void huffman(char* input_file, char* output_file_1, char* output_file_2, char* output_file_3)
{
	FILE* input = fopen(input_file, "r");
	FILE* output1 = fopen(output_file_1, "w");
	fseek(input, 0, SEEK_SET);
	leaf* char_leaf = NULL;

	long int total_chars = 0;
	while(true)
	{
		char checker = fgetc(input);
		if(checker == EOF)
		{
			break;
		}
		total_chars += 1;
	}
	unsigned char* input_str = malloc(sizeof(*input_str) * total_chars);
	fseek(input, 0, SEEK_SET);
	for(long int i = 0; i < total_chars; i++)
	{
		input_str[i] = fgetc(input);
	}

	for(long int i = 0; i < 256; i++)
	{
		long int chars_occur = _char_in_string(input_str, (unsigned char)(i), total_chars);
		if(chars_occur > 0)
		{
			char_leaf = _tree_enqueue(&char_leaf, (unsigned char)(i), chars_occur);
		}
	}

	while(char_leaf -> next != NULL)
	{
		leaf* branch = malloc(sizeof(*branch));
		branch -> ascii = -1;
		branch->occurs = char_leaf->occurs + char_leaf->next->occurs;
		
		branch->left = char_leaf;
		branch->right = char_leaf->next;

		char_leaf = _root_enqueue(&char_leaf, &branch);
		char_leaf = char_leaf->next->next;
	}
	
	unsigned char* zeros_and_ones = malloc(sizeof(*zeros_and_ones) * total_chars);
	_file_printer_1(output1, 0, char_leaf, zeros_and_ones);

	FILE* output2 = fopen(output_file_2, "w");
	bin_char* counter = malloc(sizeof(*counter));
	counter->num_ascii = 0;
	counter->num_binary = 1;

	_file_printer_2(output2, char_leaf, &counter);
	fputc('0', output2);
	fclose(output2);

	if(counter->num_binary % 8 != 0)
	{
		counter->num_binary += 8 - (counter->num_binary % 8);
	}
	long int total_bits = 8 * counter->num_ascii + counter->num_binary;
	long int char_bits = total_bits / 8;
	unsigned char* bit_str = malloc(sizeof(*bit_str) * char_bits);
	for(long int i = 0; i < char_bits; i++)
	{
		bit_str[i] = '\0';
	}
	long int* bit_loc = malloc(sizeof(*bit_loc)); //is any number between 0 and 7 including the extremes
	long int* str_loc = malloc(sizeof(*str_loc)); //index of string
	*bit_loc = 0;
	*str_loc = 0;
	_file_3_maker(&bit_str, bit_loc, char_leaf, str_loc);

	FILE* output3 = fopen(output_file_3, "w");
	for(long int i = 0; i < char_bits; i++)
	{
		fputc(bit_str[i], output3);
	}

	fclose(input);
	fclose(output1);
	fclose(output3);
	free(input_str);
	free(zeros_and_ones);
	free(counter);
	free(bit_str);
	free(bit_loc);
	free(str_loc);
	_destroy_tree(char_leaf);
}

void _file_3_maker(unsigned char** bit_str, long int* bit_loc, leaf* tree, long int* str_loc)
{
	if(tree->right != NULL && tree->left != NULL)
	{
		_file_3_maker(bit_str, bit_loc, tree->left, str_loc);
		_file_3_maker(bit_str, bit_loc, tree->right, str_loc);
	}
	if(tree->ascii == -1)
	{
		*bit_loc += 1;
		if(*bit_loc == 8)
		{
			*bit_loc = 0;
			*str_loc += 1;
		}
	}
	else
	{
		//put the 1 bit in
		unsigned char bit = 1;
		for(long int i = 0; i < 7 - *bit_loc; i++)
		{
			bit *= 2;
		}
		(*bit_str)[*str_loc] += bit;
		*bit_loc += 1;
		if(*bit_loc == 8)
		{
			*bit_loc = 0;
			*str_loc += 1;
		}

		//split the char into 2 separate ones (if needed)
		unsigned char split_char = (unsigned char)(tree->ascii);
		if(*bit_loc == 0) //not needed
		{
			(*bit_str)[*str_loc] = split_char;
			*str_loc += 1;
		}
		else //needed
		{
			unsigned char left_char = split_char >> *bit_loc;
			unsigned char right_char = split_char << (8 - *bit_loc);
			(*bit_str)[*str_loc] = (*bit_str)[*str_loc] | left_char;
			*str_loc += 1;
			(*bit_str)[*str_loc] = right_char;
		}
	}
}

void _file_printer_2(FILE* output, leaf* tree, bin_char** counter)
{
	if(tree->right != NULL && tree->left != NULL)
	{
		_file_printer_2(output, tree->left, counter);
		_file_printer_2(output, tree->right, counter);
	}
	(*counter)->num_binary += 1;
	if(tree->ascii == -1)
	{
		fputc('0', output);
	}
	else
	{
		fputc('1', output);
		fputc(tree->ascii, output);
		(*counter)->num_ascii += 1;
	}
}

void _file_printer_1(FILE* output, long int num, leaf* tree, unsigned char* string)
{
	if(tree->ascii != -1)
	{
		fputc(tree->ascii, output);
		fputc(':', output);
		for(long int i = 0; i < num; i++)
		{
			fputc(string[i], output);
		}
		fputc('\n', output);
	}
	else
	{
		string[num] = '0';
		_file_printer_1(output, num + 1, tree->left, string);
		string[num] = '1';
		_file_printer_1(output, num + 1, tree->right, string);
	}
}

leaf* _tree_enqueue(leaf** tree, long int character, long int occurence)
{
	leaf* curr = *tree;
	long int num_leaf = -1;
	long int comp = 1;
	while(comp > 0 && curr != NULL)
	{
		num_leaf += 1;
		comp = occurence - curr->occurs;
		if(comp == 0)
		{
			comp = (long int)(character - curr->ascii);
		}
		curr = curr->next;
	}
	if(comp > 0)
	{
		num_leaf += 1;
	}
	if(num_leaf == 0)
	{
		leaf* new_head   = malloc(sizeof(*new_head));
		if(new_head == NULL)
		{
			return NULL;
		}
		new_head->occurs = occurence;
		new_head->ascii  = character;
		new_head->next   = *tree;
		new_head->left   = NULL;
		new_head->right  = NULL;
		*tree = new_head;
		return *tree;
	}
	leaf* left_leaf = *tree;
	for(long int i = 1; i < num_leaf; i++)
	{
		left_leaf = left_leaf->next;
	}
	leaf* right_leaf = left_leaf->next;
	leaf* new_leaf = malloc(sizeof(*new_leaf));
	if(new_leaf == NULL)
	{
		return NULL;
	}
	new_leaf->occurs = occurence;
	new_leaf->ascii  = character;
	new_leaf->next   = right_leaf;
	new_leaf->left   = NULL;
	new_leaf->right  = NULL;
	left_leaf->next  = new_leaf;

	return *tree;
}

leaf* _root_enqueue(leaf** tree, leaf** root)
{
	leaf* curr = *tree;
	long int num_leaf = -1;
	long int comp = 1;
	while(comp >= 0 && curr != NULL)
	{
		num_leaf += 1;
		comp = (*root)->occurs - curr->occurs;
		curr = curr->next;
	}
	if(comp >= 0)
	{
		num_leaf += 1;
	}
	if(num_leaf == 0)
	{
		leaf* new_head   = *root;
		(*root)->next   = *tree;
		*tree = new_head;
		return *tree;
	}
	leaf* left_leaf = *tree;
	for(long int i = 1; i < num_leaf; i++)
	{
		left_leaf = left_leaf->next;
	}
	leaf* right_leaf = left_leaf->next;
	leaf* new_leaf = *root;
	(*root)->next   = right_leaf;
	left_leaf->next  = new_leaf;

	return *tree;
}
