#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

int main(int argc, char* argv[]) 
{
	if(argc != 5)
	{
		return EXIT_FAILURE;
	}
	char* input_file = argv[1];
	char* output_file_1 = argv[2];
	char* output_file_2 = argv[3];
	char* output_file_3 = argv[4];
	huffman(input_file, output_file_1, output_file_2, output_file_3);

	return EXIT_SUCCESS;
}
