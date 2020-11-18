hello:
	gcc -E hello.c -o hello.i
	gcc -S hello.i -o hello.s
	gcc -c hello.s -o hello.o
	ld -static crt1.o crti.o crtbeginT.o hello.o -start-group -lgcc -lgcc_eh -lc-end-group crtend.o crtn.o
