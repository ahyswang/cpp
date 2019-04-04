#include <stdio.h>
#include <stdlib.h>
#include "abts.h"
#include "apr_version.h"

static void testversion_1(abts_case *tc, void *data)
{
	apr_version_t ver;
	apr_version(&ver);
	ABTS_TRUE(tc, ver.major == APR_MAJOR_VERSION);
	ABTS_TRUE(tc, ver.minor == APR_MINOR_VERSION);
	ABTS_TRUE(tc, ver.patch == APR_PATCH_VERSION);
	ABTS_TRUE(tc, ver.is_dev == (APR_IS_DEV_STRING != ""));
}

/* ²âÊÔÌ×¼þ */
abts_suite *testversion(abts_suite *suite)
{
	suite = ADD_SUITE(suite)

	abts_run_test(suite, testversion_1, NULL);

	return suite;
}
