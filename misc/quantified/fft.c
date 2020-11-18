#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

int ones_32(uint32_t n)
{
	unsigned int c = 0;
	for (c = 0; n; ++c)
	{
		n &= (n-1);
	}
	return c;
}

uint32_t floor_log2_32(uint32_t x)
{
	x |= (x>>1);
	x |= (x>>2);
	x |= (x>>4);
	x |= (x>>8);
	x |= (x>>16);

	return (ones_32(x>>1));
}

float log2_bak(float n)
{
	return log(n)/log(2);
}

void digitrevorder(int32_t n)
{
	int32_t input_index, output_index;
	int32_t S = floor_log2_32(n);
	int32_t shf = 0;
	int32_t temp;
	for (input_index = 0; input_index < n; input_index++)
	{
		output_index = 0;
		temp = input_index;

		for ( shf=0; shf < S; shf++)
		{
			output_index = output_index*4 + temp%4;
			temp = temp/4;
		}
		char s[64];
		itoa(input_index, s, 2);
		printf("input_index = %d, 0b%s\n", input_index, s);

		itoa(output_index, s, 2);
		printf("output_index = %d, 0b%s\n", output_index, s);
	}
	
}

int main()
{
	printf("!!!!%f\n", floor(log2(32)));
	printf("!!!!%x\n", ones_32(5));
	printf("!!!!%d\n", floor_log2_32(32));

	digitrevorder(64);
	return 0;
}