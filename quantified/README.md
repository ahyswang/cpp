## 说明

量化编码测试例子

float内存结构：
符号位 31		  1位
阶码   30-23      8位
尾数有效位 22-0   23位   注意：小数点后面的尾数

-12.25二进制表示如下：
12.25 1100.01 = 1.10001 * 2^3
      12 0.25

-12.25 符号位：1 阶码：3+127=130 1000 0010 尾数有效位：100010...0

## 参考
- DSP定点运算
http://www.docin.com/p-296764982.html?docfrom=rrela
- float转int
https://blog.csdn.net/qq_41822235/article/details/80789361
- float打印二进制
https://www.cnblogs.com/Howe-Young/p/4160289.html