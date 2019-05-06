CFLAGS = -g -Wall -O0 



all : libmath.a libfunc.so test_share test_static
#all : libmath.so libmath.a libfunc.so test_share test_static

#libmath.so : math.o
#	$(CC) -shared -o $@ $^

libmath.a : math.o
	$(AR) -crv $@ $^
	
libfunc.so : func.o 
	$(CC) -shared -fPIC -o $@ $^ -L./ -lmath

test_share : test_share.o
	$(CC) -o $@ $^ -L./ -ldl

test_static : test_static.o 
	$(CC) -o $@ $^ -L./ -lfunc

*.o : *.c
	$(CC) -o $@ @^

clean:
	rm -rf libmath.so libmath.a libfunc.so test_share test_static *.o

#export LD_LIBRARY_PATH=`pwd`