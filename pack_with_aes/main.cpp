#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "apr_util.h"
#include "apr_allocater.h"

int main_pack_with_aes(int argc, char ** argv);
int main_unpack_with_aes(int argc, char ** argv);

int main(int argc, char ** argv)
{
	if (argc < 2) {
		printf("usage: \n");
		return -1;
	}
	if (strcmp(argv[1], "pack") == 0) {
		return main_pack_with_aes(argc - 1, argv + 1);
	} else if (strcmp(argv[1], "unpack") == 0){
		return main_unpack_with_aes(argc - 1, argv + 1);
	} else  {
		printf("unknow\n");
		return -2;
	}
	return 0;
}

int main_pack_with_aes(int argc, char ** argv)
{
	int rv;
	int read_size;
	int file_size;
	char* file_data;
	int aes_size;
	char* aes_data;
	const char* input_path = "unpack.bin";
	const char* output_path = "pack.bin";
	const char* aes_key = "woshiwo123";

	if (argc == 4) {
		input_path = argv[1];
		output_path = argv[2];
		aes_key = argv[3];
	}

	file_size = apr_file_get_size(input_path);
	if (file_size <= 0){
		return -1;
	}
	file_data = (char *)apr_zalloc(file_size + 1);
	read_size = apr_file_read_all(input_path, file_data, file_size);
	if (read_size != 0){
		apr_free(file_data);
		return -2;
	}
	apr_enc_by_aes(aes_key, file_data, file_size, &aes_data, &aes_size);
	apr_file_write_all(output_path, aes_data, aes_size);
	apr_free(file_data);
	apr_free(aes_data);
	return 0;
}

int main_unpack_with_aes(int argc, char ** argv)
{
	int rv;
	int read_size;
	int file_size;
	char* file_data;
	int aes_size;
	char* aes_data;
	const char* input_path = "pack.bin";
	const char* output_path = "unpack.bin";
	const char* aes_key = "woshiwo123";

	if (argc == 4) {
		input_path = argv[1];
		output_path = argv[2];
		aes_key = argv[3];
	}

	file_size = apr_file_get_size(input_path);
	if (file_size <= 0){
		return -1;
	}
	file_data = (char *)apr_zalloc(file_size + 1);
	read_size = apr_file_read_all(input_path, file_data, file_size);
	if (read_size != 0){
		apr_free(file_data);
		return -2;
	}
	apr_dec_by_aes(aes_key, file_data, file_size, &aes_data, &aes_size);
	apr_file_write_all(output_path, aes_data, aes_size);
	apr_free(file_data);
	apr_free(aes_data);
	return 0;
}