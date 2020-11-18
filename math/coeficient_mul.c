/*
*/
#include <stdio.h>
#include <math.h>

int coeficient_mul(float a[], float b[], float c[], int N)
{
	int j, k;
	float Cj = .0;
	for (j = 0; j < 2*N; j++)
	{
		Cj = 0;
		for (k = 0; k <= j; k++)
		{
			Cj += a[k]*b[j-k];
			printf("%f * %f\t", a[k], b[j-k]);
		}
		c[j] = Cj;
		printf("c[%d] = %f\n", j, c[j]);
	}
	return 0;
}

int point_mul(float a[], float b[], float c[], int N)
{

}

float func(float coeficient[], int n, int x)
{
	int i;
	float result = 0;
	for (i = 0; i < n; i++)
	{
		//printf("%d * (%d ^ %d)\n", coeficient[i], x, i);
		result += coeficient[i] * pow(x, i);
	}
	return result;
}

int main()
{
	int i;
	const int N = 4;
	// 系数表达法 n*n
	// A(x) = 7x^3-x^2+x-10
	// B(x) = 8x^3-6x+3
	float a[2*N] = {-10, 1, -1, 7, 0, 0, 0, 0};
	float b[2*N] = {3, -6, 0, 8, 0, 0, 0, 0};
	float xa[2*N] = {0};
	float ya[2*N] = {0};
	float xb[2*N] = {0};
	float yb[2*N] = {0};
	float yc[2*N] = {0};
	float c[2*N];
	// 系数乘法
	coeficient_mul(a, b, c, N);
	// 点值表达法 n*n
	for (i = 0; i < 2*N; i++)
	{
		xa[i] = i;
		ya[i] = func(a, 2*N, xa[i]);
		xb[i] = i;
		yb[i] = func(b, 2*N, xb[i]);
		// 点值乘法
		yc[i] = ya[i] * yb[i];
	}

	return 0;
}