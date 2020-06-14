#include <math.h>
#include "fourier.h"
#define M_PI 3.141592653589793115997963468544185161590576171875L

double simpson_numerical_integration(Integrand intg_arg)
{
	//declarations
	double integral = 0.0;
	double temp_lower_limit;
	double temp_upper_limit;
	int intervals_calculated = 0;

	//executables
	double length_interval = (intg_arg.upper_limit - intg_arg.lower_limit) / intg_arg.n_intervals;
	while(intervals_calculated != intg_arg.n_intervals)
	{
		temp_lower_limit = intg_arg.lower_limit + intervals_calculated * length_interval;
		temp_upper_limit = intg_arg.lower_limit + (intervals_calculated + 1) * length_interval;
		integral += ((length_interval) / 6.0) * (intg_arg.func_to_be_integrated(temp_lower_limit) + 4.0 * intg_arg.func_to_be_integrated((temp_lower_limit + temp_upper_limit)/ 2.0) + intg_arg.func_to_be_integrated(temp_upper_limit)); //calculates the Simpson's rule approximation integral over one of the inputted intervals for each iteration in the loop
		intervals_calculated += 1;
	}

	return integral;
}

double simpson_numerical_integration_for_fourier(Integrand intg_arg, int n, double (*cos_sin)(double)) 
{
	//declarations
	double integral = 0.0;
	double temp_lower_limit;
	double temp_upper_limit;
	int intervals_calculated = 0;
	double f_left;
	double f_midpoint;
	double f_right;

	//executables
	double half_period = (intg_arg.upper_limit - intg_arg.lower_limit) / 2.0;
	double length_interval = (intg_arg.upper_limit - intg_arg.lower_limit) / intg_arg.n_intervals;

	while(intervals_calculated != intg_arg.n_intervals)
	{
		temp_lower_limit = intg_arg.lower_limit + intervals_calculated * length_interval;
		temp_upper_limit = intg_arg.lower_limit + (intervals_calculated + 1) * length_interval;
		f_left = intg_arg.func_to_be_integrated(temp_lower_limit) * cos_sin((n * M_PI * temp_lower_limit) / half_period);
		f_midpoint = intg_arg.func_to_be_integrated((temp_lower_limit + temp_upper_limit) / 2.0) * cos_sin((n * M_PI * ((temp_lower_limit + temp_upper_limit) / 2.0)) / half_period);
		f_right = intg_arg.func_to_be_integrated(temp_upper_limit) * cos_sin((n * M_PI * temp_upper_limit) / half_period);
		integral += (length_interval / 6.0) * (f_left + 4.0 * f_midpoint + f_right); //calculates the Simpson's rule approximation integral over one of the inputted intervals for each iteration in the loop
		intervals_calculated += 1;
	}

	return integral;
}

void fourier_coefficients(Fourier fourier_arg)
{
	double period = fourier_arg.intg.upper_limit - fourier_arg.intg.lower_limit;
	double half_period = period / 2.0; //used to make code more readable; half_period is just half the period
	fourier_arg.a_i[0] = (1.0 / half_period) * simpson_numerical_integration(fourier_arg.intg);
	//index 0 of b_i is not defined because it will not be used
	for (int i = 1; i < fourier_arg.n_terms; i++)
	{
		fourier_arg.a_i[i] = (1.0 / half_period) * simpson_numerical_integration_for_fourier(fourier_arg.intg, i, cos);
		fourier_arg.b_i[i] = (1.0 / half_period) * simpson_numerical_integration_for_fourier(fourier_arg.intg, i, sin);
	}
}
