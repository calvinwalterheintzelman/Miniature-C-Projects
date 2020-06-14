#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "maze.h"

typedef struct {
	int n_row;
	int n_col;
	int** maze_array;
} num_Maze;

Maze* _malloc_maze(int num_rows, int num_cols);
void _free_maze(Maze* maze);
Maze* _read_maze(FILE* fp);
void _path_num(num_Maze* maze, int evaluated_num, Coord curr_path);
num_Maze* _malloc_num_maze(int num_rows, int num_cols);
void _free_num_maze(num_Maze* num_maze);
void _path_finder(num_Maze* maze, int num_location, char* directions, Coord location, int num_direction);
int _visit_paths(Maze* maze, char* directions, Coord location, Coord destination, int step);

int dfs_shortest_path_directions(char *maze_file, char *directions_file, Coord source, Coord destination)
{
	//read maze_file and assign it to a variable of type maze
	FILE* maze_fp = fopen(maze_file, "r");
	if(maze_fp == NULL)
	{
		fclose(maze_fp);
		return -1;
	}
	Maze* a_maze = _read_maze(maze_fp);
	if(a_maze == NULL)
	{
		fclose(maze_fp);
		_free_maze(a_maze);
		return -1;
	}
	if(source.row >= a_maze -> n_row || source.col >= a_maze -> n_col || destination.row >= a_maze -> n_row || destination.col >= a_maze -> n_col || source.row < 0 || source.col < 0 || destination.row < 0 || destination.col < 0)
	{
		fclose(maze_fp);
		_free_maze(a_maze);
		return -1;
	}
	if(a_maze -> maze_array[source.row][source.col] == WALL || a_maze -> maze_array[destination.row][destination.col] == WALL)
	{
		fclose(maze_fp);
		_free_maze(a_maze);
		return -1;
	}
	FILE* directions_fp = fopen(directions_file, "w");
	if(directions_fp == NULL)
	{
		fclose(maze_fp);
		_free_maze(a_maze);
		fclose(directions_fp);
		return -1;
	}

	num_Maze* num_maze = _malloc_num_maze(a_maze -> n_row, a_maze -> n_col);
	for(int i1 = 0; i1 < num_maze -> n_row; i1++)
	{
		for(int i2 = 0; i2 < num_maze -> n_col; i2++)
		{
			char maze_element = a_maze -> maze_array[i1][i2];
			if(maze_element == PATH)
			{
				num_maze -> maze_array[i1][i2] = 0;
			}
			else
			{
				num_maze -> maze_array[i1][i2] = -1;
			}
		}
	}

	num_maze -> maze_array[source.row][source.col] = 1;
	_path_num(num_maze, 1, source);

	int num_moves = num_maze -> maze_array[destination.row][destination.col] - 1;
	char* directions = malloc(sizeof(*directions) * num_moves);
	_path_finder(num_maze, num_moves + 1, directions, destination, num_moves - 1);

	for(int i = 0; i < num_moves; i++)
	{
		char direct = fputc(directions[i], directions_fp);
		if(direct == EOF)
		{
			free(directions);
			_free_num_maze(num_maze);
			_free_maze(a_maze);
			fclose(maze_fp);
			fclose(directions_fp);
			return num_moves;
		}
	}

	free(directions);
	_free_num_maze(num_maze);
	_free_maze(a_maze);
	fclose(maze_fp);
	fclose(directions_fp);
	return num_moves;
}

int simulate_movement(char *maze_file, char *directions_file, char *visited_file, Coord source, Coord destination)
{
	FILE* maze_fp = fopen(maze_file, "r");
	if(maze_fp == NULL)
	{
		fclose(maze_fp);
		return -1;
	}
	FILE* directions_fp = fopen(directions_file, "r");
	if(directions_fp == NULL)
	{
		fclose(maze_fp);
		fclose(directions_fp);
		return -1;
	}
	Maze* a_maze = _read_maze(maze_fp);
	if(a_maze == NULL)
	{
		fclose(directions_fp);
		fclose(maze_fp);
		_free_maze(a_maze);
		return -1;
	}
	if(source.row >= a_maze -> n_row || source.col >= a_maze -> n_col || destination.row >= a_maze -> n_row || destination.col >= a_maze -> n_col || source.row < 0 || source.col < 0 || destination.row < 0 || destination.col < 0)
	{
		fclose(maze_fp);
		fclose(directions_fp);
		_free_maze(a_maze);
		return -1;
	}
	if(a_maze -> maze_array[source.row][source.col] == WALL || a_maze -> maze_array[destination.row][destination.col] == WALL)
	{
		fclose(maze_fp);
		fclose(directions_fp);
		_free_maze(a_maze);
		return -1;
	}
	
	fseek(directions_fp, 0, SEEK_SET);
	int char_counter = 0;
	while(true)
	{
		char direction = fgetc(directions_fp);
		if(direction == EOF)
		{
			break;
		}
		char_counter += 1;
	}
	char* directions = malloc(sizeof(*directions) * char_counter);

	fseek(directions_fp, 0, SEEK_SET);
	for(int i = 0; i < char_counter; i++)
	{
		directions[i] = fgetc(directions_fp);
	}
	
	int num_locations = 1;
	int step = 0;
	num_locations += _visit_paths(a_maze, directions, source, destination, step);
	
	FILE* visited_fp = fopen(visited_file, "w");
	if(visited_fp == NULL)
	{
		fclose(visited_fp);
		fclose(maze_fp);
		fclose(directions_fp);
		_free_maze(a_maze);
		free(directions);
		return -1;
	}

	for(int row = 0; row < a_maze -> n_row; row++)
	{
		for(int col = 0; col < a_maze -> n_col; col++)
		{
			char error_check = fputc(a_maze -> maze_array[row][col], visited_fp);
			if(error_check == EOF)
			{
				fclose(visited_fp);
				fclose(maze_fp);
				fclose(directions_fp);
				_free_maze(a_maze);
				free(directions);
				return -1;
			}
		}
		char error_check = fputc('\n', visited_fp);
		if(error_check == EOF)
		{
			fclose(visited_fp);
			fclose(maze_fp);
			fclose(directions_fp);
			_free_maze(a_maze);
			free(directions);
			return -1;
		}
	}

	fclose(visited_fp);
	fclose(maze_fp);
	fclose(directions_fp);
	_free_maze(a_maze);
	free(directions);
	if(num_locations == 0)
	{
		return -1;
	}
	else
	{
		return num_locations;
	}
}

int _visit_paths(Maze* maze, char* directions, Coord location, Coord destination, int step)
{
	maze -> maze_array[location.row][location.col] = VISITED;
	if(location.row == destination.row && location.col == destination.col)
	{
		return step;
	}
	char direction = directions[step];
	if(direction != 'N' && direction != 'E' && direction != 'S' && direction != 'W')
	{
		return -1;
	}
	
	//north
	if(direction == 'N')
	{
		Coord new_location = {.row = location.row - 1, .col = location.col};
		if(new_location.row < 0)
		{
			return -1;
		}
		char north = maze -> maze_array[new_location.row][new_location.col];
		if(north == WALL)
		{
			return -1;
		}
		int num_steps = _visit_paths(maze, directions, new_location, destination, step + 1);
		return num_steps;
	}

	//east
	if(direction == 'E')
	{
		Coord new_location = {.row = location.row, .col = location.col + 1};
		if(new_location.col >= maze -> n_col)
		{
			return -1;
		}
		char east = maze -> maze_array[new_location.row][new_location.col];
		if(east == WALL)
		{
			return -1;
		}
		int num_steps = _visit_paths(maze, directions, new_location, destination, step + 1);
		return num_steps;
	}
	
	//south
	if(direction == 'S')
	{
		Coord new_location = {.row = location.row + 1, .col = location.col};
		if(new_location.row >= maze -> n_row)
		{
			return -1;
		}
		char south = maze -> maze_array[new_location.row][new_location.col];
		if(south == WALL)
		{
			return -1;
		}
		int num_steps = _visit_paths(maze, directions, new_location, destination, step + 1);
		return num_steps;
	}

	//west
	if(direction == 'W')
	{
		Coord new_location = {.row = location.row, .col = location.col - 1};
		if(new_location.col < 0)
		{
			return -1;
		}
		char west = maze -> maze_array[new_location.row][new_location.col];
		if(west == WALL)
		{
			return -1;
		}
		int num_steps = _visit_paths(maze, directions, new_location, destination, step + 1);
		return num_steps;
	}
	return -1;
}

Maze* _malloc_maze(int n_row, int n_col)
{
	Maze* a_maze = malloc(sizeof(*a_maze));
	if(a_maze == NULL)
	{
		free(a_maze);
		return NULL;
	}
	a_maze -> n_row = n_row;
	a_maze -> n_col = n_col;
	a_maze -> maze_array = malloc(sizeof(*(a_maze -> maze_array)) * n_row);
	for(int i = 0; i < n_row; i++)
	{
		a_maze -> maze_array[i] = malloc(sizeof(*(a_maze -> maze_array[i])) * n_col);
		if(a_maze -> maze_array[i] == NULL)
		{
			return NULL;
		}
	}

	if(a_maze -> maze_array == NULL)
	{
		free(a_maze -> maze_array);
		return NULL;
	}

	return a_maze;
}

void _free_maze(Maze* maze)
{
	for(int i = 0; i < maze -> n_row; i++)
	{
		if(maze -> maze_array[i] != NULL)
		{
			free(maze -> maze_array[i]);
		}
	}
	if(maze -> maze_array != NULL)
	{
		free(maze -> maze_array);
	}
		free(maze);
}

Maze* _read_maze(FILE* fp) //reads file, sets it equal to a maze
{
	fseek(fp, 0, SEEK_SET);
	int n_row = 0;
	int n_col = 0;
	char maze_object1 = fgetc(fp); 
	while(maze_object1 != '\n')
	{
		maze_object1 = fgetc(fp);
		n_col += 1; 
	}

	fseek(fp, 0, SEEK_SET);
	while(true)
	{
		maze_object1 = fgetc(fp);
		if(maze_object1 == '\n')
		{
			n_row += 1;
		}
		if(maze_object1 == EOF)
		{
			break;
		}
	}

	Maze* a_maze = _malloc_maze(n_row, n_col);
	fseek(fp, 0, SEEK_SET);
	for(int row = 0; row < n_row; row++)
	{
		for(int col = 0; col < n_col + 1; col++)
		{
			char maze_object2 = fgetc(fp);
			if(maze_object2 != '\n')
			{
				a_maze -> maze_array[row][col] = maze_object2;
			}
		}
	}

	return a_maze;
}

void _path_num(num_Maze* maze, int evaluated_num, Coord curr_path)
{
	//North
	if(curr_path.row - 1 >= 0)
	{
		int north = maze -> maze_array[curr_path.row - 1][curr_path.col];
		if(north ==  0|| north > evaluated_num + 1)
		{
			maze -> maze_array[curr_path.row - 1][curr_path.col] = evaluated_num + 1;
			Coord new_coord = {.row = curr_path.row - 1, .col = curr_path.col};
			_path_num(maze, evaluated_num + 1, new_coord);
		}
	}
	//East
	if(curr_path.col + 1 < maze -> n_col)
	{
		int east = maze -> maze_array[curr_path.row][curr_path.col + 1];
		if(east == 0 || east > evaluated_num + 1)
		{
			maze -> maze_array[curr_path.row][curr_path.col + 1] = evaluated_num + 1;
			Coord new_coord = {.row = curr_path.row, .col = curr_path.col + 1};
			_path_num(maze, evaluated_num + 1, new_coord);
		}
	}

	//South
	if(curr_path.row + 1 < maze -> n_row)
	{
		int south = maze -> maze_array[curr_path.row + 1][curr_path.col];
		if(south == 0 || south > evaluated_num + 1)
		{
			maze -> maze_array[curr_path.row + 1][curr_path.col] = evaluated_num + 1;
			Coord new_coord = {.row = curr_path.row + 1, .col = curr_path.col};
			_path_num(maze, evaluated_num + 1, new_coord);
		}
	}
	//West
	if(curr_path.col - 1 >= 0)
	{
		int west = maze -> maze_array[curr_path.row][curr_path.col - 1];
		if(west == 0 || west > evaluated_num + 1)
		{
			maze -> maze_array[curr_path.row][curr_path.col - 1] = evaluated_num + 1;
			Coord new_coord = {.row = curr_path.row, .col = curr_path.col - 1};
			_path_num(maze, evaluated_num + 1, new_coord);
		}
	}
}

num_Maze* _malloc_num_maze(int n_row, int n_col)
{
	num_Maze* a_maze = malloc(sizeof(*a_maze));
	if(a_maze == NULL)
	{
		free(a_maze);
		return NULL;
	}
	a_maze -> n_row = n_row;
	a_maze -> n_col = n_col;
	a_maze -> maze_array = malloc(sizeof(*(a_maze -> maze_array)) * n_row);
	for(int i = 0; i < n_row; i++)
	{
		a_maze -> maze_array[i] = malloc(sizeof(*(a_maze -> maze_array[i])) * n_col);
		if(a_maze -> maze_array[i] == NULL)
		{
			return NULL;
		}
	}

	if(a_maze -> maze_array == NULL)
	{
		free(a_maze -> maze_array);
		return NULL;
	}

	return a_maze;
}

void _free_num_maze(num_Maze* maze)
{
	for(int i = 0; i < maze -> n_row; i++)
	{
		if(maze -> maze_array[i] != NULL)
		{
			free(maze -> maze_array[i]);
		}
	}
	if(maze -> maze_array != NULL)
	{
		free(maze -> maze_array);
	}
		free(maze);
}

void _path_finder(num_Maze* maze, int num_location, char* directions, Coord location, int num_direction)
{
	int next_location = num_location - 1; //integer in the maze that we want to move towards
	//north
	if(location.row - 1 >= 0)
	{
		int north = maze -> maze_array[location.row - 1][location.col];
		if(north == next_location)
		{
			directions[num_direction] = 'S';
			Coord new_location = {.row = location.row - 1, .col = location.col};
			_path_finder(maze, next_location, directions, new_location, num_direction - 1);
			return;
		}
	}
	//east
	if(location.col + 1 < maze -> n_col)
	{
		int east = maze -> maze_array[location.row][location.col + 1];
		if(east == next_location)
		{
			directions[num_direction] = 'W';
			Coord new_location = {.row = location.row, .col = location.col + 1};
			_path_finder(maze, next_location, directions, new_location, num_direction - 1);
			return;
		}
	}
	//south
	if(location.row + 1 < maze -> n_row)
	{
		int south = maze -> maze_array[location.row + 1][location.col];
		if(south == next_location)
		{
			directions[num_direction] = 'N';
			Coord new_location = {.row = location.row + 1, .col = location.col};
			_path_finder(maze, next_location, directions, new_location, num_direction - 1);
			return;
		}

	}
	//west
	if(location.col - 1 >= 0)
	{
		int west = maze -> maze_array[location.row][location.col - 1];
		if(west == next_location)
		{
			directions[num_direction] = 'E';
			Coord new_location = {.row = location.row, .col = location.col - 1};
			_path_finder(maze, next_location, directions, new_location, num_direction - 1);
			return;
		}
	}
}
