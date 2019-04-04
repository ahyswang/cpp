#include <stdio.h>
#include <stdlib.h>
#include "abts.h"

/* ²âÊÔÓÃÀı */
static void hello_one(abts_case *tc, void *data)
{
	ABTS_TRUE(tc, 100 > 0);
}

/* ²âÊÔÌ×¼ş */
abts_suite *testhello(abts_suite *suite)
{
	suite = ADD_SUITE(suite)
	
	abts_run_test(suite, hello_one, NULL);

	return suite;
}