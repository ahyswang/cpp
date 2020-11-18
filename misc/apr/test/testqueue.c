#include <stdio.h>
#include <stdlib.h>
#include "abts.h"
#include "apr_thread.h"
#include "apr_allocater.h"
#include "apr_queue.h"
#include "apr_errno.h"
#include "apr_time.h"

#if APR_HAS_THREADS


//queue

#define CONSUMER_ACTIVITY 4
#define PRODUCER_ACTIVITY 5
#define QUEUE_SIZE 100

static apr_queue_t *queue;

static void * consumer(apr_thread_t *thd, void *data)
{
	long sleeprate;
	apr_status_t rv;
	void *v;
	abts_case *tc = data;
	
	sleeprate = 1000000 / CONSUMER_ACTIVITY;
	while (1)
	{
		rv = apr_queue_pop(queue, &v);

		if (rv == APR_EINTR)
			continue;

		if (rv == APR_EOF)
			break;

		//不能再多线程中使用
		//ABTS_TRUE(tc, v == NULL);
		//ABTS_TRUE(tc, rv == APR_SUCCESS);

		apr_sleep(sleeprate);
	}

	return NULL;
}

static void * producer(apr_thread_t *thd, void *data)
{
	long sleeprate;
	apr_status_t rv;
	void *v;
	abts_case *tc = data;

	sleeprate = 1000000 / PRODUCER_ACTIVITY;

	while (1)
	{
		rv = apr_queue_push(queue, NULL);

		if (rv == APR_EINTR)
			continue;

		if (rv == APR_EOF)
			break;

		//ABTS_TRUE(tc, rv == APR_SUCCESS);

		apr_sleep(sleeprate);
	}

	return NULL;
}

static void test_queue_producer_consumer(abts_case *tc, void *data)
{
	apr_status_t rv;
	apr_thread_t* thd_producer;
	apr_thread_t* thd_consumer;
	int retval1, retval2;

	rv = apr_queue_create(&queue, QUEUE_SIZE);
	ABTS_INT_EQUAL(tc, APR_SUCCESS, rv);

	rv = apr_thread_create(&thd_producer, producer, NULL);
	ABTS_INT_EQUAL(tc, APR_SUCCESS, rv);

	rv = apr_thread_create(&thd_consumer, consumer, NULL);
	ABTS_INT_EQUAL(tc, APR_SUCCESS, rv);

	apr_sleep(5000000); /* sleep 5 second */

	rv = apr_queue_term(queue);
	ABTS_INT_EQUAL(tc, APR_SUCCESS, rv);

	apr_thread_join(&retval1, thd_producer);
	apr_thread_join(&retval2, thd_consumer);
	apr_thread_destory(thd_producer);
	apr_thread_destory(thd_consumer);
	apr_queue_destroy(queue);
}


#endif /* APR_HAS_THREADS */

/* 测试套件 */
abts_suite *testqueue(abts_suite *suite)
{
	suite = ADD_SUITE(suite)

#if APR_HAS_THREADS
	abts_run_test(suite, test_queue_producer_consumer, NULL);
#endif 

	return suite;
}