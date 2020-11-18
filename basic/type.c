#include <stdio.h>
#include <stdlib.h>

int main()
{
	int n = -1;
	int a = 23;
	int b = -1;
	int b1 = 1;
	unsigned int  b2 = 0;
	printf("0x%x\n", n);
	printf("a = 0x%x\n", a);
	printf("b = 0x%x\n", b);
	printf("b1 = 0x%x\n", b1);
	b2 = b1;
	b2 = (~b2) + 1;
	printf("b2 = 0x%x\n", b2);

	
	{
		//补码表示负数
		unsigned char yuanma = 0b10000001;
		unsigned char fanma = 0b11111110;
		unsigned char buma = 0b11111111;
		char v1 = 0xff;
		printf("-1 yuanma = 0x%x, yuanma = 0x%x, yuanma = 0x%x\n", yuanma, fanma, buma);
		printf("v1 = 0x%x v1 = %d\n", v1, v1);
		//补码作用，减法运算统一为加法
		//1 - 1 
		//1 + abs(1)的补码
		unsigned char n1 = 0b00000001;
		unsigned char sum = n1 + buma;
		printf("1 - 1 = 0x%x\n", sum);

		float f1 = 1.2;
		unsigned int i1 = *(unsigned int *)(&f1);
		printf("1 - 1 = 0x%x\n", sum);
	}

	{
		//
		long long n1 = 0xffffffff80000000;
		printf("n1 = %lld\n", n1);
		long long n2 = -2147483648;
		printf("n2 = 0x%llx\n", n2);
	}

	{
		//
		short n1 = -1;
		int n2 = n1;
		char* ptr = &n2;
		printf("n1 = %d, 0x%x\n", n1);
		printf("n2 = %d, 0x%x\n", n2);
	}

	return 0;
}