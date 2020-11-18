#ifndef APR_UTIL_H
#define APR_UTIL_H


#define PARAM_AES_KEY			"hlw_sdk@iflytek.com"

#ifdef __cplusplus
extern "C" {
#endif

	int apr_file_remove(const char* path);
	int apr_file_rename(const char *from_path, const char *to_path);
	int apr_file_get_size(const char* filename);
	int apr_file_read_all(const char* filename, char* data, int len);
	int apr_file_write_all(const char* filename, char* data, int len);

	int apr_enc_by_aes(const char* key, const char* input, int ilen, char** output_ptr, int* olen_ptr);
	int apr_dec_by_aes(const char* key, const char* input, int ilen, char** output_ptr, int* olen_ptr);

#ifdef __cplusplus
}
#endif


#endif //APR_UTIL_H