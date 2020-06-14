#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "maze.h"

int main(int argc, char* argv[]) 
{
	if(argc < 8)
	{
		return EXIT_FAILURE;
	}
	if(argv[1][0] != '-')
	{
		return EXIT_FAILURE;
	}
	if(argv[1][1] != 's' && argv[1][1] != 'm')
	{
		return EXIT_FAILURE;
	}
	if(argv[1][2] != '\0')
	{
		return EXIT_FAILURE;
	}
	
	if(argv[1][1] == 'm')
	{
		if(argc != 8)
		{
			return EXIT_FAILURE;
		}
		char* string_part_row;
		char* string_part_col;
		Coord source = {
			.row = strtol(argv[4], &string_part_row, 10),
			.col = strtol(argv[5], &string_part_col, 10)};
		if(string_part_row[0] != '\0' || string_part_col[0] != '\0')
		{
			return EXIT_FAILURE;
		}
		Coord destination = {
			.row = strtol(argv[6], &string_part_row, 10),
			.col = strtol(argv[7], &string_part_col, 10)};
		if(string_part_row[0] != '\0' || string_part_col[0] != '\0')
		{
			return EXIT_FAILURE;
		}
		char* maze_file = argv[2];
		char* directions_file = argv[3];
		int success = dfs_shortest_path_directions(maze_file, directions_file, source, destination);
		if(success == -1)
		{
			return EXIT_FAILURE;
		}
		else
		{
			printf("%d\n", success);
		}
	}

	if(argv[1][1] == 's')
	{
		if(argc != 9)
		{
			return EXIT_FAILURE;
		}
		char* string_part_row;
		char* string_part_col;
		Coord source = {
			.row = strtol(argv[5], &string_part_row, 10),
			.col = strtol(argv[6], &string_part_col, 10)};
		if(string_part_row[0] != '\0' || string_part_col[0] != '\0')
		{
			return EXIT_FAILURE;
		}
		Coord destination = {
			.row = strtol(argv[7], &string_part_row, 10),
			.col = strtol(argv[8], &string_part_col, 10)};
		if(string_part_row[0] != '\0' || string_part_col[0] != '\0')
		{
			return EXIT_FAILURE;
		}
		char* maze_file = argv[2];
		char* directions_file = argv[3];
		char* visited_file = argv[4];
		int visited_locations = simulate_movement(maze_file, directions_file, visited_file, source, destination);
		if(visited_locations == -1)
		{
			return EXIT_FAILURE;
		}
		else
		{
			printf("%d\n", visited_locations);
		}
	}

	return EXIT_SUCCESS;
}
