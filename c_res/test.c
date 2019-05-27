#include <stdio.h>

extern char _binary_data_txt_start;
extern char _binary_data_txt_end;

int main(){
	//二进制字节
    for (char* my_file = &_binary_data_txt_start; my_file <= &_binary_data_txt_end; my_file++)
        putchar(*my_file);
    printf("\n");
}
