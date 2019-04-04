#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "abts.h"
#include "apr_strings.h"
#include "apr_allocater.h"
#include "apr_log.h"

#if APR_HAS_LOG

static void testlog_log(abts_case *tc, void *data)
{
	int argument1 = 3;
	char* argument2 = "hello";

	apr_log_set_level(APR_LOG_LEVEL_ERROR);

	//ALOGD("info format string %d", argument1);
	//ALOGD("info hello world");
	//ALOGD("info %d %s!!", argument1, argument2);

	//ALOGE("error %d %s!!", argument1, argument2);
}

#endif 

/* ²âÊÔÌ×¼þ */
abts_suite *testlog(abts_suite *suite)
{
	suite = ADD_SUITE(suite)
#if APR_HAS_LOG
	abts_run_test(suite, testlog_log, NULL);
#endif 
	return suite;
}