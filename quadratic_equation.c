
#include <stdio.h>
#include <math.h>
#include <assert.h>
enum roots
{
	NO_SOLUTION = 0,
	ONE_SOLUTION = 1,
	INF_SOLUTION = -1,
	TWO_SOLUTION = 2
};
const double ACCURACY = 0.001;
const int NUM_OF_VARIABLE = 3;

int solve_quadratic(double a, double b, double c, double* x1, double* x2);// solve quadratic equation ax^2 + bx + c = 0, return number of roots
int solve_lineal(double b, double c, double* x1, double* x2); // solve lineal equation bx + c = 0, return number of roots
int isEqual(double val1, double val2); // compare two real number with some accuracy. Also if both arguments are NAN return 1, that is, considers them equal
int RunUnitTests(); // Launch unit test and return number of failed tests (only launch three special cases)
int is_unit_test_failed(int num_of_test, double a, double b, double c, int nRootsRef, double x1ref, double x2ref); // test solve_quadratic() function
int isNAN(double arg); // defines is arg NAN or not
void clean_buff(void); // clean buffer 
void get_input(double* a, double* b, double* c);
void print_num_of_roots(int nRoots, double x1, double x2);
int main()
{
	double a = NAN, b = NAN, c = NAN, x1 = NAN, x2 = NAN;
	printf("Please, enter coefficients a, b, c of quadratic equation separated by whitespace\n");
	get_input(&a, &b, &c);
	int nRoots = solve_quadratic(a, b, c, &x1, &x2);
	print_num_of_roots(nRoots, x1, x2);
	
	int fail = RunUnitTests();
	printf("\nNumber of failed tests = %d", fail);
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
	if (isNAN(val1) && isNAN(val2))
		return 1;
	return (fabs(val1 - val2) < ACCURACY) ? 1 : 0;
}
void clean_buff(void)
{
	while (getchar() != '\n' || getchar() != EOF);
}

int RunUnitTests() 
{
	/*enum special_case
	{
		a1 = 0,
		b1 = 0,
		c1 = 0,
		a2 = 0,
		b2 = 1,
		c2 = 0,
		a3 = 0,
		b3 = 0,
		c3 = 1
	};*/
	int failed = 0;
					 //num_of_test   a,   b,  c,    nRootsRef,    x1Ref,   x2Ref	
	if (is_unit_test_failed(1,        0,   0, 0,  INF_SOLUTION,    NAN,    NAN))
		failed++;
	if (is_unit_test_failed(2,        0,   1, 0,  ONE_SOLUTION,     0,      0))
		failed++;
	if (is_unit_test_failed(3,        0,   0, 1,  NO_SOLUTION,     NAN,   NAN))
		failed++;
	if (is_unit_test_failed(4,    0.0009,  1, 1,  ONE_SOLUTION,    -1,     -1))
		failed++;
	if (is_unit_test_failed(5,     1, 0.0009, 1,  NO_SOLUTION,     NAN,    NAN))
		failed++;
	if (is_unit_test_failed(6,     1, 0.0009, -1, TWO_SOLUTION,    -1,     1))
		failed++;
	if (is_unit_test_failed(7,     1,     1, 0.0001, TWO_SOLUTION,   0,     -1))
		failed++;
	return failed;
}
int isNAN(double arg)
{
	return arg != arg;
}

int is_unit_test_failed(int num_of_test, double a, double b, double c, int nRootsRef, double x1ref, double x2ref)
{
	double x1 = NAN, x2 = NAN;
	int nRoots = solve_quadratic(a, b, c, &x1, &x2);
	if (nRoots != nRootsRef || (!isEqual(x1, x1ref) && !isEqual(x2, x1ref)) || (!isEqual(x2, x2ref) && !isEqual(x1, x2ref)))
	{
		printf("\nTest %d FAILED\nx1 = %lg, x2 = %lg, nRoots = %d\nShould be: x1 = %lg, x2 = %lg, nRoots = %d", num_of_test,
			x1, x2, nRoots, x1ref, x2ref, nRootsRef);
		return 1;
	}
	else
		return 0;
}
void get_input(double* a, double* b, double* c)
{
	int incorect_input = 1;
	while (incorect_input)
	{
		if (scanf_s("%lg %lg %lg", a, b, c) != NUM_OF_VARIABLE)
		{
			printf("main(): incorect input caused by scanf()\n");
			clean_buff();
		}
		else
			incorect_input = 0;
	}
}
void print_num_of_roots(int nRoots, double x1, double x2)
{
	switch (nRoots)
	{
		case NO_SOLUTION:
			printf("No solution");
			break;
		case ONE_SOLUTION:
			printf("x = %lf", x1);
			break;
		case TWO_SOLUTION:
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