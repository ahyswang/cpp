int subfunc(int a, int b) {
    return a - b;
}

__attribute__ ((visibility ("default"))) int funcB(int a, int b) {
    return subfunc(a, b);
}