#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
#include <stdbool.h>

void append_maze(FILE* fp, const Maze* maze)
{
	for(int row = 0; row < maze -> num_rows; row++)
	{
		for(int col = 0; col < maze -> num_cols; col++)
		{
			fputc(maze -> cells[row][col], fp);
		}
		fputc('\n', fp);
	}
}

int main(int argc, char* argv[])
{
	FILE* fp1 = fopen("samp", "r");
	Maze* a_maze = read_maze(fp1);
	bool will_write = write_maze("expected.txt", a_maze);
	if(will_write == false)
	{
		return EXIT_FAILURE;
	}

	FILE* expected = fopen("expected.txt", "a");
	fputc('\n', expected);

	Maze* a_tall_maze = make_taller(a_maze);
	append_maze(expected, a_tall_maze);
	fputc('\n', expected);
	
	Maze* a_wide_maze = make_wider(a_maze);
	append_maze(expected, a_wide_maze);
	fputc('\n', expected);

	FILE* fp2 = fopen("other_samp", "r");
	Maze* a_other_maze = read_maze(fp2);
	append_maze(expected, a_other_maze);
	fputc('\n', expected);
	Maze* a_other_tall_maze = make_taller(a_other_maze);
	append_maze(expected, a_other_tall_maze);
	fputc('\n', expected);
	Maze* a_other_wide_maze = make_wider(a_other_maze);
	append_maze(expected, a_other_wide_maze);

	bool will_write_again = write_maze("test", a_other_wide_maze);
	bool will_write_yet_again = write_maze("tester_file", a_tall_maze);
	bool will_absolutely_write = write_maze("file_to_test", a_other_wide_maze);
	if(will_write_again && will_write_yet_again && will_absolutely_write == false)
	{
		return EXIT_FAILURE;
	}

	free_maze(a_maze);
	free_maze(a_tall_maze);
	free_maze(a_wide_maze);
	fclose(fp1);
	fclose(fp2);
	fclose(expected);
	return EXIT_SUCCESS;
}
