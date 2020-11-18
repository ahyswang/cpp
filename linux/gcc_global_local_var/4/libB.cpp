int g = 200;

__attribute__ ((visibility ("default"))) int funcB(int a, int b) {
	return a + b + g;
}