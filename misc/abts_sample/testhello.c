/*
 * testhello.cc
 *
 *  Created on: 2019年3月9日
 *      Author: Administrator
 */

#include "abts_tests.h"

// 测试用例函数
static void snprintf_hello(abts_case *tc, void *data)
{
	//printf("hello");
	ABTS_STR_EQUAL(tc, "hello1", "hello1");
	ABTS_STR_EQUAL(tc, "hello1", "hello2");
}

// 测试套件函数
abts_suite *testhello(abts_suite *suite)
{
	suite = ADD_SUITE(suite)

	abts_run_test(suite, snprintf_hello, NULL);

	return suite;
}

