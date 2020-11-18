#include <stdio.h>
#include <stdlib.h>
#include "math_func.h"

int main(int argc, char **argv)
{
	double base = 10.0;
	int exponent = 2;
	double result = power(base, exponent);
	printf("%g ^ %d is %g\n", base, exponent, result);
	return 0;
}