CFLAGS = -g -Wall -O0 -fPIC


all : libmath.a libfunc.so test_share test_static

# 如果math_1.o（math.o内部使用math_1.o）没有，生成libmath.a、libfunc.so都不会有问题
# 生成test_static的链接阶段会提示undefined reference
# 运行test_share的阶段也会提示undefined symbol
libmath.a : math.o math_1.o
	$(AR) -cr -o $@ $^ 
	
# 如果不链接math库编译时是没问题，但是运行会失败
libfunc.so : func.o 
	$(CC) -shared -fPIC -o $@ $^ -L./ -lmath

test_share : test_share.o
	$(CC) -o $@ $^ -L./ -ldl

test_static : test_static.o 
	$(CC) -o $@ $^ -L./ -lfunc 

*.o : *.c
	$(CC) -o $@ @^

clean:
	rm -rf libmath.a libfunc.so test_share test_static  *.o

#export LD_LIBRARY_PATH=`pwd`