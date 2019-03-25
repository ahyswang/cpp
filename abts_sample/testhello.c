/*
 * testhello.cc
 *
 *  Created on: 2019��3��9��
 *      Author: Administrator
 */

#include "abts_tests.h"

// ������������
static void snprintf_hello(abts_case *tc, void *data)
{
	//printf("hello");
	ABTS_STR_EQUAL(tc, "hello1", "hello1");
	ABTS_STR_EQUAL(tc, "hello1", "hello2");
}

// �����׼�����
abts_suite *testhello(abts_suite *suite)
{
	suite = ADD_SUITE(suite)

	abts_run_test(suite, snprintf_hello, NULL);

	return suite;
}

