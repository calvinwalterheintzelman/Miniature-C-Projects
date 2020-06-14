#include <stdio.h>
#include "hw02.h"

void print_integer(int n, int radix, char* prefix){
	long int largest_digit = 1; //must be a long int to account for max and min values of n as an int
	unsigned int num = n; //becomes equal to the absolute value of n
	if(n < 0){
		num = -n;
		fputc('-', stdout);
	}
	for(int index_prefix = 0; prefix[index_prefix] != '\0'; index_prefix++){
		fputc(prefix[index_prefix], stdout);
	}//prints the prefix to the terminal window character by character
	for(; num >= largest_digit * radix; largest_digit *= radix) {}//finds the largest digit of n based on the given radix
	for(char counter = '0'; largest_digit != 0; largest_digit /= radix){
		for(; num >= largest_digit; counter++){
			num -= largest_digit;
			counter = (counter == ':') ? 'a': counter; //jumps from numerical ASCII values to lowercase letter ASCII values	
		}//finds the appropriate value of the digit
		fputc(counter, stdout);
		counter = '0';
	}//prints digit by digit to the screen
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
