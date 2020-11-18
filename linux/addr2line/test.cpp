#include<stdio.h>

int divide(int x, int y)
{
    return x/y;
}

int main()
{
    printf("hello world\n");
    int x = 3;
    int y = 0;
    int div = divide(x, y); 
    printf("%d / %d = %d\n", x, y, div);
    return 0;
}

