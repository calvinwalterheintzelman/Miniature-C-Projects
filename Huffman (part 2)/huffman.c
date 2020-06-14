#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "huffman.h"

void file256(char* input_file, char* output_file)
{
	FILE* input = fopen(input_file, "r");
	FILE* output = fopen(output_file, "w");
	fseek(input, 0, SEEK_SET);
	
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
	char* input_str = malloc(sizeof(*input_str) * (total_chars));
	fseek(input, 0, SEEK_SET);
	for(long int i = 0; i < total_chars; i++)
	{
		input_str[i] = fgetc(input);
	}
	
	for(int i = 0; i < 256; i++)
	{
		long int chars_occur = char_in_string(input_str, (unsigned char)(i), total_chars);
		fprintf(output, "%ld\n", chars_occur);
	}
	
	free(input_str);
	fclose(output);
	fclose(input);
}

long int char_in_string(char* string, unsigned char character, long int length)
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

void one_line_file(char* input_file, char* output_file)
{
	FILE* input = fopen(input_file, "r");
	FILE* output = fopen(output_file, "w");
	fseek(input, 0, SEEK_SET);
	Node* char_node = NULL;

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
	char* input_str = malloc(sizeof(*input_str) * total_chars);
	fseek(input, 0, SEEK_SET);
	for(long int i = 0; i < total_chars; i++)
	{
		input_str[i] = fgetc(input);
	}

	for(int i = 0; i < 256; i++)
	{
		long int chars_occur = char_in_string(input_str, (unsigned char)(i), total_chars);
		if(chars_occur > 0)
		{
			char_node = enqueue(&char_node, (unsigned char)(i), chars_occur);
		}
	}
	print_node(char_node, output);

	destroy_node(char_node);
	fclose(input);
	fclose(output);
	free(input_str);
}

Node* enqueue(Node** node, unsigned char character, long int occurence)
{
	Node* curr = *node;
	int num_node = -1;
	long int comp = 1;
	while(comp > 0 && curr != NULL)
	{
		num_node += 1;
		comp = occurence - curr->occurs;
		if(comp == 0)
		{
			comp = (long int)(character - curr->ascii);
		}
		curr = curr->next;
	}
	if(comp > 0)
	{
		num_node += 1;
	}
	if(num_node == 0)
	{
		Node* new_head   = malloc(sizeof(*new_head));
		if(new_head == NULL)
		{
			return NULL;
		}
		new_head->occurs = occurence;
		new_head->ascii  = character;
		new_head->next   = *node;
		*node = new_head;
		return *node;
	}
	Node* left_node = *node;
	for(int i = 1; i < num_node; i++)
	{
		left_node = left_node->next;
	}
	Node* right_node = left_node->next;
	Node* new_node = malloc(sizeof(*new_node));
	if(new_node == NULL)
	{
		return NULL;
	}
	new_node->occurs = occurence;
	new_node->ascii  = character;
	new_node->next   = right_node;
	left_node->next  = new_node;

	return *node;
}

void print_node(Node* list, FILE* output)
{
	while(list != NULL)
	{
		fprintf(output, "%c:%ld->", list->ascii, list->occurs);
		list = list->next;
	}
	fprintf(output, "NULL\n");
}

void destroy_node(Node* list)
{
	Node* head = list;
	if(head == NULL)
	{
		free(list);
		return;
	}

	while(head != NULL)
	{
		Node* curr = head->next;
		free(head);
		head = curr;
	}
}

void destroy_tree(leaf* tree)
{
	if(tree->left != NULL)
	{
		leaf* left_branch = tree->left;
		destroy_tree(left_branch);
	}
	if(tree->right != NULL)
	{
		leaf* right_branch = tree->right;
		destroy_tree(right_branch);
	}
	free(tree);
}

void huffman(char* input_file, char* output_file)
{
	FILE* input = fopen(input_file, "r");
	FILE* output = fopen(output_file, "w");
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
	char* input_str = malloc(sizeof(*input_str) * total_chars);
	fseek(input, 0, SEEK_SET);
	for(long int i = 0; i < total_chars; i++)
	{
		input_str[i] = fgetc(input);
	}

	for(int i = 0; i < 256; i++)
	{
		long int chars_occur = char_in_string(input_str, (unsigned char)(i), total_chars);
		if(chars_occur > 0)
		{
			char_leaf = tree_enqueue(&char_leaf, (unsigned char)(i), chars_occur);
		}
	}

	while(char_leaf -> next != NULL)
	{
		leaf* branch = malloc(sizeof(*branch));
		branch -> ascii = -1;
		branch->occurs = char_leaf->occurs + char_leaf->next->occurs;
		
		branch->left = char_leaf;
		branch->right = char_leaf->next;

		char_leaf = root_enqueue(&char_leaf, &branch);
		char_leaf = char_leaf->next->next;
	}//success!
	
	char* zeros_and_ones = malloc(sizeof(*zeros_and_ones) * total_chars);
	file_printer(output, 0, char_leaf, zeros_and_ones);

	fclose(input);
	fclose(output);
	free(input_str);
	free(zeros_and_ones);
	destroy_tree(char_leaf);
}

void file_printer(FILE* output, int num, leaf* tree, char* string)
{
	if(tree->ascii != -1)
	{
		fputc(tree->ascii, output);
		fputc(':', output);
		for(int i = 0; i < num; i++)
		{
			fputc(string[i], output);
		}
		fputc('\n', output);
	}
	else
	{
		string[num] = '0';
		file_printer(output, num + 1, tree->left, string);
		string[num] = '1';
		file_printer(output, num + 1, tree->right, string);
	}
}

leaf* tree_enqueue(leaf** tree, int character, long int occurence)
{
	leaf* curr = *tree;
	int num_leaf = -1;
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
	for(int i = 1; i < num_leaf; i++)
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

leaf* dequeue(leaf** tree)
{
	if(*tree == NULL)
	{
		return NULL;
	}
	else
	{
		leaf* tiny = *tree;
		*tree = (*tree) -> next;
		tiny -> next = NULL;
		return tiny;
	}
}

leaf* root_enqueue(leaf** tree, leaf** root)
{
	leaf* curr = *tree;
	int num_leaf = -1;
	long int comp = 1;
	while(comp >= 0 && curr != NULL)
	{
		num_leaf += 1;
		comp = (*root)->occurs - curr->occurs;
		curr = curr->next;
	}
	if(comp > 0)
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
	for(int i = 1; i < num_leaf; i++)
	{
		left_leaf = left_leaf->next;
	}
	leaf* right_leaf = left_leaf->next;
	leaf* new_leaf = *root;
	(*root)->next   = right_leaf;
	left_leaf->next  = new_leaf;

	return *tree;
}
