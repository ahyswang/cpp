#include <stdio.h>

int funcA(int, int);
int funcB(int, int);


int add(int a, int b)
{
    printf("add in main");
	return a + b + 300;
}


int main() {
    printf("funcA(2, 1) = %d\n,", funcA(2, 1));
    printf("funcB(2, 1) = %d\n", funcB(2, 1));
    printf("add(2, 1) = %d\n", add(2, 1));
    return 0;
}