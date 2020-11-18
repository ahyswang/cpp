#include <stdio.h>
#include <stdlib.h>
#include "abts.h"
#include "apr_rmm.h"
#include "apr_allocater.h"   // 使用到自定义内存的必须包含这个头文件

static void rmm_2(abts_case *tc, void *data)
{
	char *base, *base1;
	apr_size_t size = 100;
	base1 = (char *)apr_zalloc(size);
	printf("base1 = %p\n", base1);
	base = malloc(size);
	printf("base = %p\n", base);
}

#define BLOCK_SIZE 1024
#define BLOCK_COUNT  10
/* 测试用例 */
static void rmm_1(abts_case *tc, void *data)
{
	char *base;
	apr_size_t size;
	apr_rmm_t *rmm;
	apr_thread_mutex_t *mu;
	apr_rmm_off_t off;
	int i;

	size = apr_rmm_overhead_get(BLOCK_COUNT) + BLOCK_SIZE * BLOCK_COUNT;

	base = (char *)apr_zalloc(size);
	//printf("base = %p\n", base);
	
	ABTS_TRUE(tc, apr_thread_mutex_create(&mu) == 0);

	ABTS_TRUE(tc, apr_rmm_init(&rmm, mu, base, size) == 0);

	for (i = 0; i < BLOCK_COUNT + 1; ++i)
	{
		off = apr_rmm_malloc(rmm, BLOCK_SIZE);
		if (i < BLOCK_COUNT) {
			ABTS_ASSERT(tc, "off > 0", off > 0);
		}
		else {
			ABTS_ASSERT(tc, "off == 0", off == 0);
		}
		//printf("i = %d, off = %ld\n", i, off);
	}

	apr_rmm_destroy(rmm);
	apr_free(base);
	apr_thread_mutex_destroy(mu);

	/*
	for (i = 0; i < BLOCK_COUNT + 10; ++i)
	{
		apr_rmm_off_t off1, off2;

		off1 = apr_rmm_malloc(rmm, BLOCK_SIZE);
		off2 = apr_rmm_malloc(rmm, BLOCK_SIZE);

		apr_rmm_free(rmm, off1);
		apr_rmm_free(rmm, off2);

		printf("i = %d, off1 = %ld, off2 = %ld\n", i, off1, off2);
	}
	*/
}

/* 测试套件 */
abts_suite *testrmm(abts_suite *suite)
{
	suite = ADD_SUITE(suite)

	abts_run_test(suite, rmm_1, NULL);

	return suite;
}
