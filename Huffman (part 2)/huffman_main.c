#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

int main(int argc, char* argv[]) 
{
	if(argc != 5)
	{
		return EXIT_FAILURE;
	}
	FILE* gopher = fopen("gophers", "w");
	fprintf(gopher, "go go gophers");
	fclose(gopher);
	char* input_file = argv[1];
	char* output_file_1 = argv[2];
	file256(input_file, output_file_1);
	char* output_file_2 = argv[3];
	one_line_file(input_file, output_file_2);
	char* output_file_3 = argv[4];
	huffman(input_file, output_file_3);

	return EXIT_SUCCESS;
}
