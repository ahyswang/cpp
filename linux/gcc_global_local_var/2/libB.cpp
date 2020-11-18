__attribute__ ((visibility ("hidden"))) int subfunc(int a, int b) {
    return a - b;
}

int funcB(int a, int b) {
    return subfunc(a, b);
}