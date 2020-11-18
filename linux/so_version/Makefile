
all : libfunc.so libfunc.a test

libfunc.so : func.cpp 
	$(CC) -shared -fPIC -o $@ $^

libfunc.a : func.cpp
	$(AR) cr $@ $^

test : test.cpp
	$(CC) -o $@ $^ -L./ -lfunc -ldl

clean : 
	rm libfunc.so libfunc.a test
