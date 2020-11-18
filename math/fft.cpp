#include <complex>
#include <math.h>

#define PI 3.1415926
#define N 1024

using namespace std;

#define cp complex<double>

cp omega(int n, int k){
    return cp(cos(2 * PI * k / n), sin(2 * PI * k / n));
}

/*
inv表示这次用的单位根是否要取倒数。
*/
void fft(cp *a, int n, int inv) //inv是取共轭复数的符号
{
	if (n == 1) return;
	static cp buf[N];
	int m = n / 2;
	for (int i = 0; i < m; i++) { //将每一项按照奇偶分为两组
		buf[i] = a[2*i];
		buf[i + m] = a[2 * i + 1];
	}
	for (int i = 0; i < n; i++)
		a[i] = buf[i];
	fft(a, m, inv); //递归处理两个子问题
	fft(a + m, m, inv);
	for (int i = 0; i < m; i++) { //枚举x，计算A(x)
		cp x = omega(n, i);
		if (inv) x = conj(x);
		//conj是一个自带的求共轭复数的函数，精度较高。当复数模为1时，共轭复数等于倒数
		buf[i] = a[i] + x * a[i + m]; //根据之前推出的结论计算
		buf[i + m] = a[i] - x * a[i + m];
	}
	for (int i = 0; i < n; i++)
		a[i] = buf[i];
}

int main()
{
	return 0;
}