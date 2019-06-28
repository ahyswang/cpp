LIBVERSION=1.0.1
LIBVERSION_MARJOR=1

VPATH=.

.PHONY: all
all: libmylib.so.$(LIBVERSION) test_static

libmylib.so: mylib.o
	$(CC) -shared -fPIC -Wl,-soname,libhlw.so.$(LIBVERSION_MARJOR) $^ -o $@.$(LIBVERSION)

test_static: test_static.o
	$(CC) $^ -o $@ -L./ -lmylib 
	
.PHONY: clean
clean:
	rm -f libmylib.so.$(LIBVERSION) test_static