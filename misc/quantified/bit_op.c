#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main()
{
	{
		const int MAX = 64;  //2^5
		int base[MAX] = {0}; 
		int n =0;
		
		for (size_t i = 0; i < 100; i++)
		{
			printf("n = %d\n", n);
			n = n + 1;
			n = n & (MAX - 1); // n = n % MAX;
		}
	}

	return 0;
}