#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
#include <stdbool.h>

void find_maze_dimensions(FILE * fp, int * a_num_rows, int * a_num_cols)
{
	fseek(fp, 0, SEEK_SET);
	int num_cols = 0;
	char char_check1 = fgetc(fp);
	while(char_check1 != '\n')
	{
		char_check1 = fgetc(fp);
		num_cols += 1;
	}
	*a_num_cols = num_cols;

	fseek(fp, 0, SEEK_SET);
	int num_rows = 0;
	while(true)
	{
		char char_check2 = fgetc(fp);
		if(char_check2 == '\n')
		{
			num_rows += 1;
		}
		if(char_check2 == EOF)
		{
			break;
		}
	}
	*a_num_rows = num_rows;
}

int find_opening_location(FILE * fp)
{
	fseek(fp, 0, SEEK_SET);
	int col_loc = 0;
	while(true)
	{
		char char_check = fgetc(fp);
		if(char_check == PATH)
		{
			break;
		}
		col_loc += 1;
	}

	return col_loc;
}

int count_path_locations(FILE * fp)
{
	fseek(fp, 0, SEEK_SET);
	int num_paths = 0;
	while(true)
	{
		char char_check = fgetc(fp);
		if(char_check == PATH)
		{
			num_paths += 1;
		}
		if(char_check == EOF)
		{
			break;
		}
	}

	return num_paths;
}

char get_location_type(FILE * fp, int row, int col)
{
	fseek(fp, 0, SEEK_SET);
	char char_at_loc = fgetc(fp);

	for(int i = 0; i < row; i++)
	{
		while(true)
		{
			char char_check = fgetc(fp);
			if(char_check == '\n')
			{
				fgetc(fp);
				break;
			}
		}
	}

	for(int i = 0; i < col; i++)
	{
		char_at_loc = fgetc(fp);
	}

	return char_at_loc;
}
 
int represent_maze_in_one_line(char * filename, FILE * fp)
{
	FILE* filename_p = fopen(filename, "w");
	if(filename_p == NULL)
	{
		return MAZE_WRITE_FAILURE;
	}
	
	int num_chars = 0;
	fseek(fp, 0, SEEK_SET);
	while(true)
	{
		char maze_char = fgetc(fp);
		if(maze_char == EOF)
		{
			break;
		}
		if(maze_char != '\n')
		{
			fputc(maze_char, filename_p);
			num_chars += 1;
		}
	}

	fclose(filename_p);
	return num_chars;
}
