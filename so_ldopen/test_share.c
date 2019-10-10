#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

typedef int (* func_t)(int, int);

int one()
{
	printf("main one");
}

int main(void) 
{
	void *handle = NULL;
	char *so = "./libfunc.so";
	func_t func = NULL;
	
	handle = dlopen(so, RTLD_NOW);
	if (handle == NULL) {
		printf("dlopen - %s\n", dlerror());
		exit(-1);
	}
	printf("dlopen ok\n");
	func = dlsym(handle, "add");
	if (func == NULL) {
		printf("dlsym - %s\n", dlerror());
		dlclose(handle);
		exit(-1);
	}	

	
	printf("add(10, 20) = %d\n", func(10, 20));

	dlclose(handle);

	return 0;
}
