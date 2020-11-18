#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main1()
{
	int64_t int64_max = INT64_MAX;
	uint64_t uint64_max = UINT64_MAX;
	const int32_t buf_size = 1024;
	char buf[buf_size];
	

#ifdef WIN32
	printf("int64_max = %I64d\n",int64_max);
	printf("uint64_max = %I64u\n",uint64_max);
#else 

/*
//32
	printf("sizeof(int) == %d\n", sizeof(int));
	printf("int64_max = %lld\n",int64_max);
	printf("uint64_max = %llu\n",uint64_max);
	snprintf(buf, buf_size, "int64_max = %lld\n",int64_max);
	puts(buf);
	snprintf(buf, buf_size, "uint64_max = %llu\n",uint64_max);
	puts(buf);
*/
//64
	printf("sizeof(int) == %lu\n", sizeof(int));
	printf("int64_max = %ld\n",int64_max);
	printf("uint64_max = %lu\n",uint64_max);
	snprintf(buf, buf_size, "int64_max = %ld\n",int64_max);
	puts(buf);
	snprintf(buf, buf_size, "uint64_max = %lu\n",uint64_max);
	puts(buf);

#endif 

	return 0;
}


int main()
{
	//KEY_INDEX|KEY_C1|KEY_C2
/*
#define KEY_INDEX 1
#define KEY_C1 0x087C5C45
#define KEY_C2 0x629AC7AC
#define KEY_C3 0xF4A4C4EE
#define KEY_C4 0xF4B59523
#define KEY_C5 0x95C376B1
#define KEY_C6 0x5DE20245
#define KEY_C7 0x5463D324
#define KEY_C8 0x803A5DB5
*/

	unsigned KEY_INDEX = 0;
	unsigned KEY_C1 = 0;
	unsigned KEY_C2 = 0;
	unsigned KEY_C3 = 0;
	unsigned KEY_C4 = 0;
	unsigned KEY_C5 = 0;
	unsigned KEY_C6 = 0;
	unsigned KEY_C7 = 0;
	unsigned KEY_C8 = 0;

	const char* key = "1|0x087C5C45|0x629AC7AC|0xF4A4C4EE|0xF4B59523|0x95C376B1|0x5DE20245|0x5463D324|0x803A5DB5";
	sscanf(key, "%d|0x%x|0x%x|0x%x|0x%x|0x%x|0x%x|0x%x|0x%x", &KEY_INDEX, &KEY_C1, &KEY_C2, &KEY_C3, &KEY_C4, &KEY_C5, &KEY_C6, &KEY_C7, &KEY_C8);
	printf("%d|0x%x|0x%x|0x%x|0x%x|0x%x|0x%x|0x%x|0x%x\n", KEY_INDEX, KEY_C1, KEY_C2, KEY_C3, KEY_C4, KEY_C5, KEY_C6, KEY_C7, KEY_C8);
	//printf("%d|0x%x|0x%x|0x%x|0x%x|0x%x|0x%x|0x%x|0x%x", KEY_INDEX, KEY_C1, KEY_C2, KEY_C3, KEY_C4, KEY_C5, KEY_C6, KEY_C7, KEY_C8);

	return 0;
}
