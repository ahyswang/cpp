#include <stdio.h>

typedef enum type_t 
{
	TYPE_NONE = 1,
	TYPE_YES,
	TYPE_NO,
	TYPE_UNKNOW
}type_t;

typedef struct outer_t
{
	int id;
	char name[64];
	type_t type;
	struct inner
	{
		int id;
		char name[64];
	} i;
}outer_t;

outer_t o = {123, "outer", (type_t)2, {456, "inner"}};

int main()
{
	o.id = 123;
	printf("o.id = %d o.name = %s o.type = %d\n", o.id, o.name, o.type);

	return 0;
}

//g++ struct.c