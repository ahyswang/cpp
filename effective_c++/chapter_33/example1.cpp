#include <iostream>

//变量名称遮掩
int x; //global 变量
void someFunc()
{
    double x;  // local 变量
    std::cin >> x; // 读一个新值赋予local变量x
}

int main()
{
    someFunc();
    return 0;
}
