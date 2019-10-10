all: test

test: test.cpp
	$(CXX) -Wl,-Map=test.map -g test.cpp -o test
	
clean: 
	rm -f test
