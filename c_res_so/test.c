#include <stdio.h>
#include <dlfcn.h>
#include <sys/stat.h>
// 这些是在资源文件的目标中定义的，注意文本和二进制的区别
//extern char _binary_data_txt_start;
//extern char _binary_data_txt_end;
//extern char _binary_data_bin_start;
//extern char _binary_data_bin_end;

extern char _binary_libmyso_so_start;
extern char _binary_libmyso_so_end;

typedef int (* func_t)(int, int);

void save_myso(char* start, char*end)
{
	char *so = "/tmp/libmyso.so";
	FILE *fp = fopen(so, "wb");
	if (fp == NULL) {
		printf("create libmyso error");
	}
	fwrite(start, 1, end - start + 1, fp); 
	fflush(fp);
	fclose(fp);
}

void remove_myso()
{
	char *so = "/tmp/libmyso.so";
	remove(so);
}

void load_call_myso()
{
	void *handle = NULL;
	char *so = "/tmp/libmyso.so";
	func_t func = NULL;
	
	handle = dlopen(so, RTLD_NOW);
	if (handle == NULL) {
		printf("dlopen - %s\n", dlerror());
		exit(-1);
	}
	printf("dlopen ok\n");
	func = dlsym(handle, "myso_add");
	if (func == NULL) {
		printf("dlsym - %s\n", dlerror());
		dlclose(handle);
		exit(-1);
	}	
	
	remove(so);

	printf("add(10, 20) = %d\n", func(10, 20));

	dlclose(handle);

	return 0;
}

int main(){
	//二进制字节
    //for (char* my_file = &_binary_data_txt_start; my_file <= &_binary_data_txt_end; my_file++)
    //    putchar(*my_file);
    printf("len = %d\n", &_binary_libmyso_so_end - &_binary_libmyso_so_start);
	
	save_myso(&_binary_libmyso_so_start, &_binary_libmyso_so_end);
	
	load_call_myso();
}
