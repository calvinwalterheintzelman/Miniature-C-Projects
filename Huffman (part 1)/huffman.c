#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

//cmp_fn returns the diferrence between two random void* (regardless of type)
//in this case it returns ints
static int int_cmp(const void* p1, const void* p2);

void int_print(const void* ptr);

Node *pq_enqueue(Node **pq, const void *new_object, int (*cmp_fn)(const void *, const void *))
{
	//assume linked list is not null
	//goal is to put new_object after the node that's smaller but before the node that's bigger. it can be next to nodes of equal size.
	//use cmp_fn to determine what's bigger/smaller
	if(new_object == NULL)
	{
		return NULL;
	}
	Node* head = *pq;
	Node* curr = head;
	int num_node = -1;
	int comp = 1;
	while(comp > 0 && curr != NULL)
	{
		num_node += 1;
		comp = cmp_fn(new_object, curr->ptr);
		curr = curr->next;
	}
	if(comp > 0)
	{
		num_node += 1;
	}
	if(num_node == 0)//when the new object is first
	{
		Node* new_head = malloc(sizeof(*new_head));
		new_head -> ptr = (void*)new_object;
		new_head -> next = head;
		return new_head;
	}
	Node* left_node = head;
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
	new_node -> ptr = (void*)new_object;
	new_node -> next = right_node;
	left_node->next = new_node;

	return head;
}

                       
Node *pq_dequeue(Node **pq)
{
	Node* head = *pq;
	if(head == NULL)
	{
		return NULL;
	}
	else
	{
		head -> next = NULL;
		return head;
	}
}


Node *stack_push(Node **stack, const void *new_object)
{
	if(new_object == NULL)
	{
		return NULL;
	}
	Node* top_node = malloc(sizeof(*top_node));
	if(top_node == NULL)
	{
		return NULL;
	}
	top_node -> ptr = (void*)new_object;
	top_node -> next = *stack;
	return top_node;
}

                       
Node *stack_pop(Node **stack)
{
	Node* head = *stack;
	if(head == NULL)
	{
		return NULL;
	}
	else
	{
		head -> next = NULL;
		return head;
	}
}

Node *remove_last_node(Node **list)
{
	Node* first = *list;
	if(first == NULL)
	{
		return NULL;
	}
	Node* second = first  -> next;
	if(second == NULL)
	{
		return first;
	}
	Node* third = second -> next;
	while(third != NULL)
	{
		first = second;
		second = third;
		third = third -> next;
	}
	Node* last_node = second;
	first -> next = NULL;
	return last_node;
}

void destroy_node(Node *list, void (*destroy_fn)(void *))
{
	Node* head = list;
	if(head == NULL)
	{
		free(list);
		return;
	}

	while(head != NULL)
	{
		Node* curr = head -> next;
		destroy_fn(head -> ptr);
		free(head);
		head = curr;
	}
}

/* print an entire linked list                                           */
/* pq is the address of the first Node in the linked list               */
/* print_fn is the address of the function to print the generic object,  */
/* of which the address is stored in the ptr field of an Node in the    */
/* linked list                                                           */
/*                                                                       */
/* DO NOT CHANGE THIS FUNCTION, OTHERWISE YOUR OUTPUT WILL BE DIFFERENT  */
/* AND YOU WILL NOT RECEIVE CREDIT                                       */
 
void print_node(const Node *list, void (*print_fn)(const void *))
{
   while (list != NULL) {
      // print the memory associated with list->ptr
      print_fn(list->ptr);
      // print an arrow
      fprintf(stdout, "->");
      list = list->next;
   } 
   // print NULL and a newline after that 
   fprintf(stdout, "NULL\n");
}


static int int_cmp(const void* p1, const void* p2)
{
	return *(const int*)p1 - *(const int*)p2;
}

void int_print(const void* ptr)
{
	printf("%d", *(const int*)ptr);
}
