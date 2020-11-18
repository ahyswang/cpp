CFLAGS = -g -Wall -O0


all : libmath.so libfunc.so libfunc.a test_share test_static

libmath.so : math.c
	$(CC) -shared -fPIC -o $@ $^
	
# 如果不链接math库编译时是没问题，但是运行会失败
libfunc.so : func.c 
	$(CC) -shared -fPIC -o $@ $^ -L./ -lmath

libfunc.a : func.c
	$(AR) -cr -o $@ $^ 

test_share : test_share.c
	$(CC) -o $@ $^ -L./ -ldl -lmath

test_static : test_static.c 
	$(CC) -o $@ $^ -L./ -lfunc -lmath

clean:
	rm -rf libmath.so libfunc.so libfunc.a test_share test_static
