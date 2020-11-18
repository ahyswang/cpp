int add(int a, int b);

__attribute__ ((visibility ("default"))) int funcB(int a, int b) {
	return add(a,b);
}