// do not change this part, if you do, your code may not compile
//
/* test for structure defined by student */
#ifndef NTEST_STRUCT 
#include "numint.h"
#else 
#include "numint_key.h"
#endif /* NTEST_STRUCT */
//
// do not change above this line, if you do, your code may not compile
// This is the only file you have to include 

double simpson_numerical_integration(Integrand intg_arg)
{
	//declarations
	double integral = 0.0;
	double length_interval;
	double temp_lower_limit;
	double temp_upper_limit;
	int intervals_calculated = 0;

	//executables
	length_interval = (intg_arg.upper_limit - intg_arg.lower_limit) / intg_arg.n_intervals;
	while(intervals_calculated != intg_arg.n_intervals)
	{
		temp_lower_limit = intg_arg.lower_limit + intervals_calculated * length_interval;
		temp_upper_limit = intg_arg.lower_limit + (intervals_calculated + 1) * length_interval;
		integral += ((length_interval) / 6.0) * (intg_arg.func_to_be_integrated(temp_lower_limit) + 4.0 * intg_arg.func_to_be_integrated((temp_lower_limit + temp_upper_limit)/ 2.0) + intg_arg.func_to_be_integrated(temp_upper_limit)); //calculates the Simpson's rule approximation integral over one of the inputted intervals for each iteration in the loop
		intervals_calculated += 1;
	}

	return integral;
}
