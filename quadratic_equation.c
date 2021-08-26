// ConsoleApplication6.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <stdio.h>
#include <math.h>
#include <assert.h>
#define ACCURACY 0.001
#define INF_SOLUTION -1
#define NO_SOLUTION 0
#define NUM_OF_VARIABLE 3
int close_to_0(double arg);
int solve_quadratic(double a, double b, double c, double* x1, double* x2);
int solve_lineal(double b, double c, double* x1, double* x2);
int main()
{
	double a = 0, b = 0, c = 0, x1 = 0, x2 = 0;
	printf("Please, enter coefficients a, b, c of quadratic equation separated by whitespace\n");
	if (scanf_s("%lg %lg %lg", &a, &b, &c) != NUM_OF_VARIABLE)
	{
		printf("main(): incorect input caused by scanf()");
		return 1;
	}
	int nRoots = solve_quadratic(a, b, c, &x1, &x2);
	switch (nRoots)
	{
	case NO_SOLUTION: 
		printf("No solution");
		break;
	case 1: 
		printf("x = %lf", x1);
		break;
	case 2: 
		printf("x1 = %lf, x2 = %lf", x1, x2);
		break;
	case INF_SOLUTION:
		printf("x - any real number");
		break;
	default: 
		printf("main() ERROR: nRoots = %d", nRoots);
		return 1;

	}
}
int close_to_0(double arg)
{
	return (fabs(arg) < ACCURACY);
}
int solve_quadratic(double a, double b, double c, double* x1, double* x2)
{
	assert(isfinite(a));
	assert(isfinite(b));
	assert(isfinite(c));
	assert(x1 != x2);
	assert(x1 != NULL);
	assert(x2 != NULL);
	if (close_to_0(a))
		return solve_lineal(b, c, x1, x2);
	else
	{
		double d = b * b - 4 * a * c;
		if (close_to_0(d))
		{
			*x1 = *x2 = -b / (2 * a);
			return 1;
		}
		else if (d > 0)
		{
			double sqrt_d = sqrt(d);
			*(x1) = (-b + sqrt_d) / (2 * a);
			*(x2) = (-b - sqrt_d) / (2 * a);
			return 2;
		}
		else
			return NO_SOLUTION;

	}
}
int solve_lineal(double b, double c, double* x1, double* x2)
{
	if (close_to_0(b))
	{
		return (close_to_0(c)) ? INF_SOLUTION : NO_SOLUTION;
	}
	else
	{
		*x1 = *x2 = -c / b;
		return 1;
	}
}
//! @param[in] a a-coefficient