## 说明
测试符号找不到

如果math_1.o（math.o内部使用math_1.o）没有，生成libmath.a、libfunc.so都不会有问题。
生成test_static的链接阶段会提示undefined reference。
运行test_share的阶段也会提示undefined symbol。

生成libmath.a只是把需要的o文件打包到一起，不知道哪个o漏掉了。

生成libfunc.so只检查func.c中使用到的外部符号，不检查libmath.a内部使用的符号。


