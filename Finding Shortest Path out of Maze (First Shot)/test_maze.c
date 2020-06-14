#include <stdio.h>
#include <stdlib.h>
#include "maze.h"

int main(int argc, char** argv)
{
	FILE* fp = fopen("samp", "r");
	char* filename = "filename";
	int* a_num_rows = malloc(sizeof(int));
	int* a_num_cols = malloc(sizeof(int));
	find_maze_dimensions(fp, a_num_rows, a_num_cols);
	printf("rows: %d cols: %d\n", *a_num_rows, *a_num_cols);

	find_opening_location(fp);
	count_path_locations(fp);
	int row = 1;
	int col = 5;
	char butt = get_location_type(fp, row, col);
	printf("type: %c.\n", butt);

	represent_maze_in_one_line(filename, fp);
	free(a_num_rows);
	free(a_num_cols);
	fclose(fp); 
	return EXIT_SUCCESS;
}
