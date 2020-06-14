#include <stdio.h>
#include <stdlib.h>
#include "aux.h"
#include "util.h"
#include "fourier.h"

int main(int argc, char ** argv) 
{
	//declaration
	double (*fourier_function)(double); //the program will find the fourier coefficients of this function
	
	//validates inputs and sets the appropriate function equal to fourier_function
	if(argc != 6)
	{
		return EXIT_FAILURE;
	}
	if(argv[1][1] != '\0')
	{
		return EXIT_FAILURE;
	}
	if(argv[1][0] == '1')
	{
		fourier_function = unknown_function_1;
	}
	else if(argv[1][0] == '2')
	{
		fourier_function = unknown_function_2;
	}
	else if(argv[1][0] == '3')
	{
		fourier_function = unknown_function_3;
	}
	else
	{
		return EXIT_FAILURE;
	}

	Integrand integrand = 
	{
		.lower_limit = atof(argv[2]),
		.upper_limit = atof(argv[3]),
		.n_intervals = atoi(argv[4]),	
		.func_to_be_integrated = fourier_function
	};

	int n_terms = atoi(argv[5]);

	if(integrand.lower_limit == integrand.upper_limit)
	{
		return EXIT_FAILURE;
	}

	if(integrand.n_intervals < 1)
	{
		integrand.n_intervals = 1;
	}//the number of intervals has to be at least one
	if(n_terms < 1)
	{
		n_terms = 1;
	}//the number of terms has to be at least one

	Fourier fourier = 
	{
		.intg      = integrand,
		.n_terms   = n_terms,
		.a_i       = malloc(sizeof(*fourier.a_i) * n_terms),
		.b_i       = malloc(sizeof(*fourier.b_i) * n_terms)
	};

   // If allocation of fourier.a_i and/or fourier.b_i failed, then free the
   // one that didn't fail and then exit with EXIT_FAILURE.
	if (fourier.a_i == NULL || fourier.b_i == NULL)
	{
		free(fourier.a_i);  // free(...) has no effect if its argument is NULL
		free(fourier.b_i);
		return EXIT_FAILURE;
	}

	fourier_coefficients(fourier);

	print_fourier_coefficients(fourier.a_i, fourier.b_i, fourier.n_terms);

	free(fourier.a_i);
	free(fourier.b_i);

	return EXIT_SUCCESS;
} 
