#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

int main(int argc, char* argv[])
{
	int* first = malloc(sizeof(*first));
	*first = 1;
	int* second = malloc(sizeof(*second));
	*second = 4;
	int* third = malloc(sizeof(*third));
	*third = 6;
	Node* head = malloc(sizeof(*head));
	head -> ptr = first;
	head -> next = malloc(sizeof(*head));
	head -> next -> ptr = second;
	head -> next -> next = malloc(sizeof(*head));
	head -> next -> next -> ptr = third;
	head -> next -> next -> next = NULL;
	
	int* insert1 = malloc(sizeof(*insert1));
	*insert1 = -1;
	const void* new_object = insert1;
	static int (*cmp_fn)(const void*, const void*) = int_cmp;
	void (*print)(const void*) = int_print;
	
	head = pq_enqueue(&head, new_object, cmp_fn);
	if(head == NULL)
	{
		return EXIT_FAILURE;
	}
	int* insert2 = malloc(sizeof(*insert2));
	*insert2 = 7;
	new_object = insert2;
	head = pq_enqueue(&head, new_object, cmp_fn);
	if(head == NULL)
	{
		return EXIT_FAILURE;
	}
	int* insert3 = malloc(sizeof(*insert3));
	*insert3 = 5;
	new_object = insert3;
	head = pq_enqueue(&head, new_object, cmp_fn);
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
	print_node(head2, print);
	print_node(head, print);

	int* insert4 = malloc(sizeof(*insert4));
	*insert4 = -10;
	new_object = insert4;
	head = stack_push(&head, new_object);
	if(head == NULL)
	{
		return EXIT_FAILURE;
	}
	int* insert5 = malloc(sizeof(*insert5));
	*insert5 = -100;
	new_object = insert5;
	head = stack_push(&head, new_object);
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

	printf("\n");
	print_node(head,  print);
	print_node(head2, print);
	print_node(head3, print);
	print_node(head4, print);
	print_node(head5, print);

	void (*destroy_fn)(void*) = free;
	destroy_node(head,  destroy_fn);
	destroy_node(head2, destroy_fn);
	destroy_node(head3, destroy_fn);
	destroy_node(head4, destroy_fn);
	destroy_node(head5, destroy_fn);

	return EXIT_SUCCESS;
}
