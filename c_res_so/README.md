## ˵��
�Ѷ�̬����Ϊ��Դ�����ƴ���������У����ٳ����е��ö�̬��ĺ�����

## ����
objdump -x test.o  | more
objcopy --input-target binary --output-target elf64-x86-64 --binary-architecture i386:x86-64 data.txt  data.o
gcc test.c data.o -o test