/*
霍纳法则:
y = a0 + a1*x^1 + a2*x^2 + ... + an_1*x^(n-1)

p(x)=2x^4-x^3+3x^2+x-5
=x(2x^3-x^2+3x+1)-5
=x(x(2x^2-x+3)+1)-5
=x(x(x(2x-1)+3)+1)-5
*/

#include <stdio.h>
#include <math.h>
#include <time.h>

//coefficient[]为待求多项式的系数数组，n为数组大小，x为多项式中未知数x的具体值
//注意：coefficient[0]存放系数a0，coefficient[1]存放系数a1，以此类推…
int horner(int coeficient[], int n, int x)
{
	int i;
	int result;

	result = coeficient[n -1];
	for (i = n - 2; i >= 0; i--)
	{
		//printf("%d * %d + %d\n", x, result, coeficient[i]);
		result = x * result + coeficient[i];
	}

	return result;
}

int func(int coeficient[], int n, int x)
{
	int i;
	int result = 0;
	for (i = 0; i < n; i++)
	{
		//printf("%d * (%d ^ %d)\n", coeficient[i], x, i);
		result += coeficient[i] * pow(x, i);
	}
	return result;
}

int main()
{
	int p;
	int i;
	int N = 10000;
	clock_t t1,t2;
	int a[5] = {-5, 1, 3, -1, 2};
	t1 = clock();
	for (i = 0; i < N; i++)
	{
		p = horner(a, 5, 3);
		//printf("p = %d\n", p);
	}
	t2 = clock();
	printf("consume = %f\n", (float)(t2 - t1)/CLOCKS_PER_SEC);
	
	for (i = 0; i < N; i++)
	{
		p = func(a, 5, 3);
		//printf("p = %d\n", p);
	}
	printf("consume = %f\n", (float)(t2 - t1)/CLOCKS_PER_SEC);
}