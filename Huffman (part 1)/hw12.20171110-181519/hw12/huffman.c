#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"


Node *pq_enqueue(Node **pq, const void *new_object, 
                  int (*cmp_fn)(const void *, const void *))
{
   return NULL;
}

                       
Node *pq_dequeue(Node **pq)
{
   return NULL;
}


Node *stack_push(Node **stack, const void *new_object)
{
   return NULL;
}

                       
Node *stack_pop(Node **stack)
{
   return NULL;
}


Node *remove_last_node(Node **list)
{
   return NULL;
}


void destroy_node(Node *list, void (*destroy_fn)(void *))
{
   return;
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

