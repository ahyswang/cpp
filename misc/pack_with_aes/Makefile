OBJCOPY = objcopy


main : main.cpp apr_allocater.cpp apr_aes.cpp apr_util.cpp
	$(CC) -o $@ $^

test: test.cpp pack.o
	$(CC) -o $@ $^

%.o : %.cpp 
	$(CXX) -o $@ -c $< 
%.o : %.bin
	$(OBJCOPY) --input-target binary --output-target elf64-x86-64 --binary-architecture i386:x86-64 $^ $@

clean :
	rm -rf main