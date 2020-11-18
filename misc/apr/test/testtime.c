
#include <stdio.h>
#include <stdlib.h>
#include "abts.h"
#include "apr_time.h"

static void time_now(abts_case *tc, void *data)
{
	uint64_t now = apr_time_now();
	//printf("now = %lld\n", now);
}

/* ²âÊÔÌ×¼ş */
abts_suite *testtime(abts_suite *suite)
{
	suite = ADD_SUITE(suite)

	abts_run_test(suite, time_now, NULL);

	return suite;
}