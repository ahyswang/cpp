CFLAGS = -g -Wall -O0 -fPIC



all : libmath.so libfunc.so test_share test_static

libmath.so : math.o math_1.o
	$(CC) -shared -fPIC -o $@ $^
	
libfunc.so : func.o 
	$(CC) -shared -fPIC -o $@ $^ -L./ -lmath

test_share : test_share.o
	$(CC) -o $@ $^ -L./ -ldl

test_static : test_static.o 
	$(CC) -o $@ $^ -L./ -lfunc 

*.o : *.c
	$(CC) -o $@ @^

clean:
	rm -rf libmath.so libfunc.so test_share test_static *.o

#export LD_LIBRARY_PATH=`pwd`