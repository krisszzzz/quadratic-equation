// ConsoleApplication6.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <stdio.h>
#include <math.h>
#include <assert.h>
#define NO_SOLUTION 0
#define INF_SOLUTION -1
const double ACCURACY = 0.001;
const int NUM_OF_VARIABLE = 3;
int solve_quadratic(double a, double b, double c, double* x1, double* x2);// solve quadratic equation ax^2 + bx + c = 0, return number of roots
int solve_lineal(double b, double c, double* x1, double* x2); // solve lineal equation bx + c = 0, return number of roots
int isEqual(double val1, double val2); // compare two real number with some accuracy
//int RunUnitTests(); // Launch unit test and return number of failed tests
int unit_test(int num_of_test, double a, double b, double c, int nRoots, double x1ref, double x2ref); // test solve_quadratic function
int main()
{
	double a = NAN, b = NAN, c = NAN, x1 = NAN, x2 = NAN;
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
		printf("x1 = %lg, x2 = %lg", x1, x2);
		break;
	case INF_SOLUTION:
		printf("x - any real number");
		break;
	default: 
		printf("main() ERROR: nRoots = %d", nRoots);
		return 1;

	}
}
//int close_to_0(double arg)
//{
	//return (fabs(arg) < ACCURACY);
//}
int solve_quadratic(double a, double b, double c, double* x1, double* x2)
{
	assert(isfinite(a));
	assert(isfinite(b));
	assert(isfinite(c));
	assert(x1 != x2);
	assert(x1 != NULL);
	assert(x2 != NULL);
	if (isEqual(a, 0))
		return solve_lineal(b, c, x1, x2);
	else
	{
		double d = b * b - 4 * a * c;
		if (isEqual(d, 0))
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
	if (isEqual(b, 0))
	{
		return (isEqual(c, 0)) ? INF_SOLUTION : NO_SOLUTION;
	}
	else
	{
		*x1 = *x2 = -c / b;
		return 1;
	}
}
int isEqual(double val1, double val2)
{
	return (fabs(val1 - val2) < ACCURACY) ? 1 : 0;
}
/*
int RunUnitTests() 
{
	double x1 = NAN, x2 = NAN;
	int failed = 0, nRoots = 0;
	if (unit_test(1, 1, 5, -6, 2, 1, -6))
		failed++;
	return failed;
}
*/
int unit_test(int num_of_test, double a, double b, double c, int nRootsRef, double x1ref, double x2ref)
{
	double x1 = NAN, x2 = NAN;
	int nRoots = solve_quadratic(a, b, c, &x1, &x2);
	if (nRoots != nRootsRef || !isEqual(x1, x1ref) || !isEqual(x2, x2ref))
	{
		printf("Test %d FAILED\n x1 = %lg, x2 = %lg, nRoots = %d\n Should be: x1 = %lg, x2 = %lg, nRoots = %d", num_of_test,
			x1, x2, nRoots, x1ref, x2ref, nRootsRef);
		return 1;
	}
	else
		return 0;
}

