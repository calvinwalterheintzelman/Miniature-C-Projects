#include <stdio.h>
#include <stdlib.h>

static int int_cmp(const void* p1, const void* p2)
{
	return *(const int*)p1 - *(const int*)p2;
}

int main(int argc, char* argv[])
{
	int first = 1;
	int second = 4;
	int third = 6;
	Node* head = malloc(sizeof(*head));
	head -> ptr = &first;
	head -> next = malloc(sizeof(*head));
	head -> next -> ptr = &second;
	head -> next -> next = malloc(sizeof(*head));
	head -> next -> next -> ptr = &third;
	head -> next -> next -> next = NULL;
	
	int insert = 3;
	const void* new_object = &insert;
	static int (*cmp_fn)(const void*, const void*) = int_cmp;
	head = pq_enqueue(&head, new_object, cmp_fn);
	return EXIT_SUCCESS;
}
