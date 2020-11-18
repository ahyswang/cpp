/*
gcc -c simple_section.c -o simple_section.o
*/
int printf( const char* format, ...);
int global_init_var = 84;
int global_uninit_var;  //unused
__attribute__((section("FOO"))) int global = 42;

void func1( int i )
{
	printf( "%d\n", i);
}

int main(void)
{
	static int static_init_var = 85;
	static int static_uninit_var;

	int a = 1;
	int b;

	func1( static_init_var + static_uninit_var + a + b);

	return a;
}
