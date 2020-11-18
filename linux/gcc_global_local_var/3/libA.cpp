int subfunc(int a, int b) {
    return a + b;
}

__attribute__ ((visibility ("default"))) int funcA(int a, int b) {
    return subfunc(a, b);
}