#include "math.h"

int one()
{
	printf("func one");
	return 0;	
}

int add(int a, int b)
{
	one();
	return math_add(a, b);
}
