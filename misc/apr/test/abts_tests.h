/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef APR_TEST_INCLUDES
#define APR_TEST_INCLUDES

#include "abts.h"

/* 测试初始 */
void initialize(void);

/* 测试套件列表 */
abts_suite *testhello(abts_suite *suite);
abts_suite *testthread(abts_suite *suite);
abts_suite *teststring(abts_suite *suite);
abts_suite *testqueue(abts_suite *suite);
abts_suite *testrmm(abts_suite *suite);
abts_suite *testtime(abts_suite *suite);
abts_suite *testversion(abts_suite *suite);
abts_suite *testlog(abts_suite *suite);
abts_suite *testbase64(abts_suite *suite);

/* 列表只能定义一次 */
const struct testlist {
    abts_suite *(*func)(abts_suite *suite);
} alltests[] = {
    {testhello},
	{testthread},
	{teststring},
	{testqueue},
	{testrmm},
	{testtime},
	{testversion},
	{testlog},
	{testbase64},
};

#endif /* APR_TEST_INCLUDES */
