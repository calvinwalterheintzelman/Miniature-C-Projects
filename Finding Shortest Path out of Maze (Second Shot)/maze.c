#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "maze.h"

Maze* malloc_maze(int num_rows, int num_cols)
{
	Maze* a_maze = malloc(sizeof(*a_maze));
	if(a_maze == NULL)
	{
		free(a_maze);
		return NULL;
	}
	a_maze -> num_rows = num_rows;
	a_maze -> num_cols = num_cols;
	a_maze -> cells = malloc(sizeof(*(a_maze -> cells)) * num_rows);
	for(int i = 0; i < num_rows; i++)
	{
		a_maze -> cells[i] = malloc(sizeof(*(a_maze -> cells[i])) * num_cols);
		if(a_maze -> cells[i] == NULL)
		{
			return NULL;
		}
	}

	if(a_maze -> cells == NULL)
	{
		free(a_maze -> cells);
		return NULL;
	}

	return a_maze;
}

void free_maze(Maze* maze)
{
	for(int i = 0; i < maze -> num_rows; i++)
	{
		if(maze -> cells[i] != NULL)
		{
			free(maze -> cells[i]);
		}
	}
	if(maze -> cells != NULL)
	{
		free(maze -> cells);
	}
		free(maze);
}

Maze* read_maze(FILE* fp) //reads file, sets it equal to a maze
{
	fseek(fp, 0, SEEK_SET);
	int num_rows = 0;
	int num_cols = 0;
	char maze_object1 = fgetc(fp); 
	while(maze_object1 != '\n')
	{
		maze_object1 = fgetc(fp);
		num_cols += 1; 
	}

	fseek(fp, 0, SEEK_SET);
	while(true)
	{
		maze_object1 = fgetc(fp);
		if(maze_object1 == '\n')
		{
			num_rows += 1;
		}
		if(maze_object1 == EOF)
		{
			break;
		}
	}

	Maze* a_maze = malloc_maze(num_rows, num_cols);
	fseek(fp, 0, SEEK_SET);
	for(int row = 0; row < num_rows; row++)
	{
		for(int col = 0; col < num_cols + 1; col++)
		{
			char maze_object2 = fgetc(fp);
			if(maze_object2 != '\n')
			{
				a_maze -> cells[row][col] = maze_object2;
			}
		}
	}

	return a_maze;
}

bool write_maze(const char* filename, const Maze* maze)
{
	FILE* fp = fopen(filename, "w");
	if(fp == NULL)
	{
		return false;
	}
	for(int row = 0; row < maze -> num_rows; row++)
	{
		for(int col = 0; col < maze -> num_cols; col++)
		{
			fputc(maze -> cells[row][col], fp);
		}
		fputc('\n', fp);
	}
	fclose(fp);

	return true;
}

Maze* make_taller(const Maze* orig)
{	
	int num_rows = orig -> num_rows * 2 - 1;
	int num_cols = orig -> num_cols;
	Maze* a_tall_maze = malloc_maze(num_rows, num_cols);
	if(a_tall_maze == NULL)
	{
		return NULL;
	}

	int row = 0;
	while(row < orig -> num_rows)
	{
		for(int col = 0; col < num_cols; col++)
		{
			a_tall_maze -> cells[row][col] = orig -> cells[row][col];
		}
		row += 1;
	}
	for(int i = 2; row < num_rows; i++)
	{
		for(int col = 0; col < num_cols; col++)
		{
			a_tall_maze -> cells[row][col] = orig -> cells[orig -> num_rows - i][col];
		}
		row += 1;
	}

	return a_tall_maze;
}

Maze* make_wider(const Maze* orig)
{
	int num_rows = orig -> num_rows;
	int num_cols = orig -> num_cols * 2 - 1;
	Maze* a_wide_maze = malloc_maze(num_rows, num_cols);
	if(a_wide_maze == NULL)
	{
		return NULL;
	}

	int col = 0;
	while(col < orig -> num_cols)
	{
		for(int row = 0; row < a_wide_maze -> num_rows; row++)
		{
			a_wide_maze -> cells[row][col] = orig -> cells[row][col];
		}
		col += 1;
	}
	for(int i = 2; col < a_wide_maze -> num_cols; i++)
	{
		for(int row = 0; row < a_wide_maze -> num_rows; row++)
		{
			a_wide_maze -> cells[row][col] = orig -> cells[row][orig -> num_cols - i];
		}
		col += 1;
	}

	a_wide_maze -> cells[num_rows / 2][num_cols / 2] = PATH;
	if(a_wide_maze -> cells[num_rows / 2][num_cols / 2 + 1] == WALL)
	{
		a_wide_maze -> cells[num_rows / 2][num_cols / 2 + 1] = PATH;
		a_wide_maze -> cells[num_rows / 2][num_cols / 2 - 1] = PATH;
	}

	return a_wide_maze;
}
