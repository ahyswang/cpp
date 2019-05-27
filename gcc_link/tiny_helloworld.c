/*
gcc -c -fno-builtin tiny_helloworld.c
ld -static -e nomain -o tiny_helloworld tiny_helloworld.o 
ld -static -T tiny_helloworld.lds -e nomain -o tiny_helloworld tiny_helloworld.o 
*/
char *str = "hello world!\n";
/*
void print()
{
	asm("movl $13,%%edx \n\t"
	"movl %0,%%ecx \n\t"
	"movl $0,%%ebx \n\t"
	"movl $4,%%eax \n\t"
	"int $0x80 \n\t"
	::"r"(str):"edx", "ecx", "ebx");
}
*/
void exit()
{
	asm("movl $42,%ebx \n\t"
	"movl $1,%eax \n\t"
	"int $0x80 \n\t");
}

void nomain()
{
//	print();
	exit();
}
