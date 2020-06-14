#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "mintf.h"

void mintf(const char *format, ...){
	va_list arguments;
	va_start(arguments, format);
	
	for(int i = 0; format[i] != '\0'; i++){
		if(format[i] == '%'){ //outer if
			if(format[++i] == '%'){
				fputc('%', stdout);

			}else if(format[i] == 'd'){
				int printed_int = va_arg(arguments, int);
				print_integer(printed_int, 10, "");

			}else if(format[i] == 'x'){
				int printed_int = va_arg(arguments, int);
				print_integer(printed_int, 16, "0x");

			}else if(format[i] == 'b'){
				int printed_int = va_arg(arguments, int);
				print_integer(printed_int, 2, "0b");

			}else if(format[i] == '$'){
				double printed_double = va_arg(arguments, double);
				print_integer((int) printed_double, 10, "$"); //prints everything before the decimal place
				fputc('.',stdout);
				int decimals = abs(((long int) (printed_double * 100.0)) % 100);
				if(decimals < 10){
					fputc('0', stdout);
				}
				print_integer(decimals, 10, ""); //prints the first 2 decimals

			}else if(format[i] == 's'){
				char* printed_string = va_arg(arguments, char*);
				for(int index = 0; printed_string[index] != '\0'; index++){
					fputc(printed_string[index], stdout);
				}//prints printed_string character-by-character

			}else if(format[i] == 'c'){
				char printed_char = va_arg(arguments, int);
				fputc(printed_char, stdout);

			}else{
				fputc(format[--i], stdout);
			}
		}

		else{ //outer else
			fputc(format[i], stdout);
		}
	}
	va_end(arguments);
}

void print_integer(int n, int radix, char* prefix){
	long int largest_digit = 1; //must be a long int to account for max and min values of n as an int
	unsigned int num = abs(n); //becomes equal to the absolute value of n

	if(n < 0){
		fputc('-', stdout);
	}

	for(int index_prefix = 0; prefix[index_prefix] != '\0'; index_prefix++){
		fputc(prefix[index_prefix], stdout);
	}//prints the prefix to the terminal window character by character
	
	for(; num >= largest_digit * radix; largest_digit *= radix){}//finds the largest digit of n based on the given radix
	
	for(char counter = '0'; largest_digit != 0; largest_digit /= radix){
		for(; num >= largest_digit; num -= largest_digit){
			counter += 1;
			counter = (counter == ':') ? 'a' : counter; //jumps from numerical ASCII values to lowercase letter ASCII values
		}//finds the appropriate value of the digit
		fputc(counter, stdout);
		counter = '0';
	}//prints digit by digit to the screen
}

/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
