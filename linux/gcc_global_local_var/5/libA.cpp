int add(int a, int b);

__attribute__ ((visibility ("default"))) int funcA(int a, int b) {
	return add(a,b);
}