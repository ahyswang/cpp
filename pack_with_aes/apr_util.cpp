#include "apr_util.h"
#include "apr_allocater.h"
#include "apr_aes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32

#include <Windows.h>

int apr_file_remove(const char* path)
{
	if (!DeleteFile(path)) {
		return -1;
	}
	return 0;
}

int apr_file_rename(const char *from_path, const char *to_path)
{
	if (!MoveFileEx(from_path, to_path, MOVEFILE_REPLACE_EXISTING |
		MOVEFILE_COPY_ALLOWED))
		return -1;
	return 0;
}

#else

#include <unistd.h> 

int apr_file_remove(const char* path)
{
	if (unlink(path) != 0) {
		return -1;
	}
	return 0;
}

int apr_file_rename(const char *from_path, const char *to_path)
{
	if (rename(from_path, to_path) != 0) {
		return -1;
	}
	return 0;
}

#endif 

int apr_file_get_size(const char* filename)
{
	int size = 0;
	FILE * fp = fopen(filename, "rb");
	if (fp == NULL){
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fclose(fp);
	return size;
}

int apr_file_read_all(const char* filename, char* data, int len)
{
	int size = 0;
	FILE * fp = fopen(filename, "rb");
	if (fp == NULL){
		return -1;
	}
	fseek(fp, 0, SEEK_SET);
	size = fread(data, 1, len, fp);
	if (size != len){
		fclose(fp);
		return -1;
	}
	fclose(fp);
	return 0;
}

int apr_file_write_all(const char* filename, char* data, int len)
{
	int size = 0;
	FILE * fp = fopen(filename, "wb");
	if (fp == NULL){
		return -1;
	}
	size = fwrite(data, 1, len, fp);
	if (size != len){
		fclose(fp);
		return -1;
	}
	fclose(fp);
	return 0;
}

int apr_enc_by_aes(const char* key, const char* input, int ilen, char** output_ptr, int* olen_ptr)
{
	struct AES_ctx ctx;
	int enc_len = 0;
	int i = 0;
	char* enc_buf = 0;
	char  key_safe[AES_KEYLEN + 1] = {0};

	strncpy(key_safe, key, AES_KEYLEN + 1);

	AES_init_ctx(&ctx, (const uint8_t *)key_safe);
	enc_len = ((ilen + AES_BLOCKLEN - 1) / AES_BLOCKLEN) * AES_BLOCKLEN + AES_BLOCKLEN;
	enc_buf = (char *)apr_zalloc(enc_len);
	enc_buf[enc_len - 1] = enc_len - ilen;
	if (0 == enc_buf) return -1;
	memcpy(enc_buf, input, ilen);
	for (i = 0; i < enc_len; i += AES_BLOCKLEN){
		AES_ECB_encrypt(&ctx, (uint8_t *)(enc_buf + i));
	}
	*output_ptr = enc_buf;
	*olen_ptr = enc_len;

	return 0;
}

int apr_dec_by_aes(const char* key, const char* input, int ilen, char** output_ptr, int* olen_ptr)
{
	struct AES_ctx ctx;
	int enc_len = 0;
	int i = 0;
	char* enc_buf = 0;
	char  key_safe[AES_KEYLEN + 1] = {0};

	strncpy(key_safe, key, AES_KEYLEN + 1);

	AES_init_ctx(&ctx, (const uint8_t *)key_safe);
	enc_len = ((ilen + AES_BLOCKLEN - 1) / AES_BLOCKLEN) * AES_BLOCKLEN;
	enc_buf = (char *)apr_zalloc(enc_len);
	if (0 == enc_buf) return -1;
	memcpy(enc_buf, input, ilen);
	for (i = 0; i < enc_len; i += AES_BLOCKLEN){
		AES_ECB_decrypt(&ctx, (uint8_t *)(enc_buf + i));
	}
	*output_ptr = enc_buf;
	*olen_ptr = enc_len - enc_buf[enc_len - 1];

	return 0;
}