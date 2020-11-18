int g = 100;

__attribute__ ((visibility ("default"))) int funcA(int a, int b) {
	return a + b + g;
}