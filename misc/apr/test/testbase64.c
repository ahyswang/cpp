
#include <stdio.h>
#include <stdlib.h>
#include "abts.h"
#include "apr_base64.h"
#include "apr_allocater.h"

static void base64_str(abts_case *tc, void *data)
{
	char *str_en, *str_de;
	int   len_en, len_de;
	const char *plain = "hello";

	len_en = apr_base64_encode_len(strlen(plain)); 
	str_en = (char *)apr_malloc(len_en);  
	ABTS_ASSERT(tc, "str_en != NULL", str_en != NULL);  
	len_en = apr_base64_encode(str_en, plain, strlen(plain)); 
	ABTS_ASSERT(tc, "len_en > 0", len_en > 0); 

	len_de = apr_base64_decode_len(str_en); 
	str_de = (char *)apr_malloc(len_en); 
	ABTS_ASSERT(tc, "str_de != NULL", str_de != NULL); 
	len_de = apr_base64_decode(str_de, str_en); 
	ABTS_ASSERT(tc, "len_de > 0", len_de > 0); 

	ABTS_STR_EQUAL(tc, str_de, plain);

	apr_free(str_en);
	apr_free(str_de);
}

/* ²âÊÔÌ×¼ş */
abts_suite *testbase64(abts_suite *suite)
{
	suite = ADD_SUITE(suite)

	abts_run_test(suite, base64_str, NULL);

	return suite;
}