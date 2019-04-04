#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "abts.h"
#include "apr_strings.h"
#include "apr_strmatch.h"
#include "apr_allocater.h"

static void string_cmp(abts_case *tc, void *data)
{
	ABTS_TRUE(tc, apr_strcmp("cd", "bc") > 0);
	ABTS_TRUE(tc, apr_strcmp("bc", "bc") == 0);
	ABTS_TRUE(tc, apr_strcmp("c", "cd") < 0);
}

static void string_asprintf(abts_case *tc, void *data)
{
	char * str = apr_asprintf("name = %s, age = %d", "hello", 20);
	apr_free(str);
	str = apr_asprintf("key=%s", "value");
	ABTS_STR_EQUAL(tc, str, "key=value");
	apr_free(str);
}

static void string_asprintf_2(abts_case *tc, void *data)
{
	int argument1 = 3;
	char* argument2 = "hello";
	char * str;

	str = apr_asprintf("format string %d", argument1);
	ABTS_ASSERT(tc, "str != NULL", str != NULL);
	//printf(str);  
	apr_free(str);

	str = apr_asprintf("hello world");
	ABTS_ASSERT(tc, "str != NULL", str != NULL);
	//printf(str);  
	apr_free(str);

	str = apr_asprintf("%d %s!!", argument1, argument2);
	ABTS_ASSERT(tc, "str != NULL", str != NULL);
	//printf(str);  
	apr_free(str);
}

static void string_strmatch(abts_case *tc, void *data)
{
	const apr_strmatch_pattern *pattern;
	const apr_strmatch_pattern *pattern_nocase;
	const apr_strmatch_pattern *pattern_onechar;
	const apr_strmatch_pattern *pattern_zero;
	const char *match = NULL;
	const char *input1 = "string that contains a patterN...";
	const char *input2 = "string that contains a pattern...";
	const char *input3 = "pattern at the start of a string";
	const char *input4 = "string that ends with a pattern";
	const char *input5 = "patter\200n not found, negative chars in input";
	const char *input6 = "patter\200n, negative chars, contains pattern...";

	pattern = apr_strmatch_precompile("pattern", 1);
	ABTS_PTR_NOTNULL(tc, pattern);

	pattern_nocase = apr_strmatch_precompile("pattern", 0);
	ABTS_PTR_NOTNULL(tc, pattern_nocase);

	pattern_onechar = apr_strmatch_precompile("g", 0);
	ABTS_PTR_NOTNULL(tc, pattern_onechar);

	pattern_zero = apr_strmatch_precompile("", 0);
	ABTS_PTR_NOTNULL(tc, pattern_zero);

	match = apr_strmatch(pattern, input1, strlen(input1));
	ABTS_PTR_EQUAL(tc, NULL, match);

	match = apr_strmatch(pattern, input2, strlen(input2));
	ABTS_PTR_EQUAL(tc, input2 + 23, match);

	match = apr_strmatch(pattern_onechar, input1, strlen(input1));
	ABTS_PTR_EQUAL(tc, input1 + 5, match);

	match = apr_strmatch(pattern_zero, input1, strlen(input1));
	ABTS_PTR_EQUAL(tc, input1, match);
}

static void string_strtok(abts_case *tc, void *data)
{
	char *plain = apr_strdup("abc|123||women||||");
	char *token;
	char *last = plain;
	int i = 0;

	//printf("string_strtok");
	while ((token = apr_strtok(last, "|", &last)))
	{
		if (i == 0) {
			ABTS_STR_EQUAL(tc, token, "abc");
		}
		else if (i == 1) {
			ABTS_STR_EQUAL(tc, token, "123");
		}
		else if (i == 2) {
			ABTS_STR_EQUAL(tc, token, "women");
		}
		//printf("token = %s\n", token);
		++i;
	}

	apr_free(plain);
}


/* ²âÊÔÌ×¼þ */
abts_suite *teststring(abts_suite *suite)
{
	suite = ADD_SUITE(suite)

	abts_run_test(suite, string_asprintf, NULL);
	abts_run_test(suite, string_asprintf_2, NULL);
	abts_run_test(suite, string_cmp, NULL);
	abts_run_test(suite, string_strmatch, NULL);
	abts_run_test(suite, string_strtok, NULL);

	return suite;
}