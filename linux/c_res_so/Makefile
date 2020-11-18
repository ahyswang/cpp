test : test.o libmyso.o -ldl
	gcc -o $@ $^ 

test.o : test.c
	cc -o $@ -c $^

libmyso.o : libmyso.so
	objcopy --input-target binary --output-target elf64-x86-64 --binary-architecture i386:x86-64 libmyso.so  libmyso.o

libmyso.so : myso.c 
	gcc -shared -fPIC -o $@ $^ 
	
clean : 
	rm test test.o libmyso.o libmyso.so
