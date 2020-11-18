#include "math_func.h"

/**
 * power - Calculate the power of number.
 * @param base: Base value.
 * @param exponent: Exponent value.
 *
 * @return base raised to the power exponent.
 */
double power(double base, int exponent)
{
	int result = base;
	int i;

	if (exponent == 0) {
		return 0;
	}

	for (i = 0; i < exponent; ++i) {
		result = result * base;
	}

	return result;
}