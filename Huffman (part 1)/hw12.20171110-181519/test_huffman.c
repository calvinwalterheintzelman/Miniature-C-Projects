#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"
#include <string.h>

static int int_cmp(const void* p1, const void* p2);

static void int_print(const void* ptr);

static int string_cmp(const void* p1, const void* p2);

static void string_print(const void* ptr);

void useless(void* anything);

int main(int argc, char* argv[])
{
	static int (*cmp_fn)(const void*, const void*) = int_cmp;
	static void (*print)(const void*) = int_print;
	void (*really_useless)(void*) = useless;
	
	int first = 1;
	int second = 4;
	int third = 6;
	Node* head = NULL;
	head = pq_enqueue(&head, &first, cmp_fn);
	head = pq_enqueue(&head, &second, cmp_fn);
	head = pq_enqueue(&head, &third, cmp_fn);
	if(head == NULL)
	{
		return EXIT_FAILURE;
	}

	int insert1 = -1;
	head = pq_enqueue(&head, &insert1, cmp_fn);
	if(head == NULL)
	{
		return EXIT_FAILURE;
	}
	int insert2 = 7;
	head = pq_enqueue(&head, &insert2, cmp_fn);
	if(head == NULL)
	{
		return EXIT_FAILURE;
	}
	int insert3 = 5;
	head = pq_enqueue(&head, &insert3, cmp_fn);
	if(head == NULL)
	{
		return EXIT_FAILURE;
	}
	print_node(head, print);

	Node* head2 = head -> next;
	head = pq_dequeue(&head);
	if(head == NULL || head2 == NULL)
	{
		return EXIT_FAILURE;
	}
	printf("head2, then 1\n");
	print_node(head2, print);
	print_node(head, print);

	int insert4 = -10;
	head = stack_push(&head, &insert4);
	if(head == NULL)
	{
		return EXIT_FAILURE;
	}
	int insert5 = -100;
	head = stack_push(&head, &insert5);
	if(head == NULL)
	{
		return EXIT_FAILURE;
	}
	print_node(head, print);
	
	Node* head3 = head2 -> next;
	head2 = stack_pop(&head2);
	head2 = stack_pop(&head2);
	if(head2 == NULL || head3 == NULL)
	{
		return EXIT_FAILURE;
	}
	print_node(head2, print);
	print_node(head3, print);

	Node* head4 = remove_last_node(&head3);
	if(head4 == NULL || head3 == NULL)
	{
		return EXIT_FAILURE;
	}
	print_node(head4, print);
	print_node(head3, print);
	Node* head5 = remove_last_node(&head);
	if(head5 == NULL || head == NULL)
	{
		return EXIT_FAILURE;
	}
	print_node(head5, print);
	print_node(head,  print);

	print_node(head,  print);
	print_node(head2, print);
	print_node(head3, print);
	print_node(head4, print);
	print_node(head5, print);

	//void (*destroy_fn)(void*) = free;
	destroy_node(head,  really_useless);
	destroy_node(head2, really_useless);
	destroy_node(head3, really_useless);
	destroy_node(head4, really_useless);
	destroy_node(head5, really_useless);

	Node* new = malloc(sizeof(*new));
	char* word1 = "clue";
	char* word2 = "hi";
	char* word3 = "what";
	new -> ptr = word1;
	new -> next = malloc(sizeof(*new));
	new -> next -> ptr = word2;
	new -> next -> next = malloc(sizeof(*new));
	new -> next -> next -> ptr = word3;
	new -> next -> next -> next = NULL;
	static void (*p_string)(const void*) = string_print;
	static int (*cmp_string)(const void*, const void*) = string_cmp;
	print_node(new, p_string);
	char* new_word = "blahblah";
	new = pq_enqueue(&new, new_word, cmp_string);
	char* another_word = "fancy";
	new = pq_enqueue(&new, another_word, cmp_string);
	char* last_word = "zach";
	new = pq_enqueue(&new, last_word, cmp_string);
	print_node(new, p_string);

	Node* newer = pq_dequeue(&new);
	print_node(newer, p_string);
	print_node(new, p_string);
	destroy_node(new, really_useless);
	destroy_node(newer, really_useless);

	return EXIT_SUCCESS;
}

static int int_cmp(const void* p1, const void* p2)
{
	return (*(const int*)p1 - *(const int*)p2);
}

static void int_print(const void* ptr)
{
	fprintf(stdout, "%d", *(const int*)ptr);
}

static int string_cmp(const void* p1, const void* p2)
{
	return (strcmp((const char*)p1, (const char*)p2));
}

static void string_print(const void* ptr)
{
	printf("%s", (const char*)ptr);
}

void useless(void* anything)
{
	return;
}
