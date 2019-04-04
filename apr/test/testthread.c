#include <stdio.h>
#include <stdlib.h>
#include "abts.h"
#include "apr_thread.h"

int num_1 = 0;
apr_thread_mutex_t* mu_1;

void* thread_1(apr_thread_t * td, void* data)
{
	int* int_ptr;

	apr_thread_mutex_lock(mu_1);
	int_ptr = (int *)data;
	*int_ptr += 1;
	apr_thread_mutex_unlock(mu_1);
	apr_thread_exit(td, 1);
	return NULL;
}

void* thread_2(apr_thread_t * td, void* data)
{
	int* int_ptr;
	apr_thread_mutex_lock(mu_1);
	int_ptr = (int *)data;
	*int_ptr += 1;
	apr_thread_mutex_unlock(mu_1);
	apr_thread_exit(td, 2);
	return NULL;
}

/* ²âÊÔÓÃÀı */
static void thread_create_1(abts_case *tc, void *data)
{
	apr_thread_t *thd1, *thd2;
	int retval1, retval2;

	ABTS_TRUE(tc, apr_thread_mutex_create(&mu_1) == 0);

	ABTS_TRUE(tc, apr_thread_create(&thd1, thread_1, &num_1) == 0);
	ABTS_TRUE(tc, apr_thread_create(&thd2, thread_2, &num_1) == 0);

	apr_thread_join(&retval1, thd1);
	apr_thread_join(&retval2, thd2);

	ABTS_TRUE(tc, retval1 == 1);
	ABTS_TRUE(tc, retval2 == 2);
	ABTS_TRUE(tc, num_1 == 2);
}

/* ²âÊÔÌ×¼ş */
abts_suite *testthread(abts_suite *suite)
{
	suite = ADD_SUITE(suite)

	abts_run_test(suite, thread_create_1, NULL);

	return suite;
}