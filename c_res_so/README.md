## 说明
把动态库作为资源二进制打包到程序中，并再程序中调用动态库的函数。

## 步骤
objdump -x test.o  | more
objcopy --input-target binary --output-target elf64-x86-64 --binary-architecture i386:x86-64 data.txt  data.o
gcc test.c data.o -o test