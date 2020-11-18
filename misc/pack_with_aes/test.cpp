#include <stdio.h>
#include <stdlib.h>

extern char _binary_pack_bin_start;
extern char _binary_pack_bin_end;

void save_pack(const char* fn, char* start, char*end)
{
	FILE *fp = fopen(fn, "wb");
	if (fp == NULL) {
		printf("create pack error");
	}
	fwrite(start, 1, end - start, fp); 
	fflush(fp);
	fclose(fp);
}


int main(int argc, char** argv)
{
	printf("main");
	save_pack("pack_out.bin", &_binary_pack_bin_start, &_binary_pack_bin_end);
	return 0;
}