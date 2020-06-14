#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "hw02.h"

int main(int argc, char* argv[])
{
	int max = INT_MAX; //largest possible integer
	int min = INT_MIN; //smallest possible integer

	printf("Maximum int is %d.\nMinimum int is %d.\n", max, min);

	print_integer(-40, 10, "er");
	printf("\n");
	print_integer(698, 10, "foogle");
	printf("\n");
	print_integer(100, 5, "&^#");
	printf("\n");
	print_integer(11, 12, "Who?");
	printf("\n");
	print_integer(0, 36, "baaah");
	printf("\n");
	print_integer(0, 4, "is zero");
	printf("\n");
	print_integer(max, 10, "hi");
	printf("\n");
	print_integer(min, 10, "bye");
	printf("\n");
	print_integer(max, 2, "long");
	printf("\n");
	print_integer(min, 30, "tiny");
	printf("\n");
	print_integer(99, 3, "");
	printf("\n");
	print_integer(100, 5, "6");
	printf("\n");
	print_integer(26755, 36, "toomany?");

	return EXIT_SUCCESS;
}
