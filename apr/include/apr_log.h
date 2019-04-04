#ifndef APR_LOG_H
#define APR_LOG_H

#include "apr.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 
	APR log API.
	Usage:
	int argument1 = 3;
	char* argument2 = "hello";
	ALOGD("format string %d", argument1);
	ALOGD("hello world");
	ALOGD("%d %s!!", argument1, argument2);
*/
#if APR_HAS_LOG

#define APR_LOG_LEVEL_DEBUG 0
#define APR_LOG_LEVEL_INFO 1
#define APR_LOG_LEVEL_ERROR 2


/**
* 设置日志级别
*/
APR_DECLARE(void) apr_log_set_level(int level);

/**
* 日志记录函数.
* @param file 代码文件名.
* @param line 代码行号.
* @param level 日志级别.
* @param format 日志内容.
*/
APR_DECLARE(void) apr_log(const char *file, int line, int level, const char *format, ...);

/* 日志宏简单使用，类似于系统的printf函数 */
#define ALOGD(format, ...) apr_log(__FILE__, __LINE__, APR_LOG_LEVEL_DEBUG, format, ##__VA_ARGS__)
#define ALOGI(format, ...) apr_log(__FILE__, __LINE__, APR_LOG_LEVEL_INFO, format, ##__VA_ARGS__)
#define ALOGE(format, ...) apr_log(__FILE__, __LINE__, APR_LOG_LEVEL_ERROR, format, ##__VA_ARGS__)

/* 日志记录断言错误 */
#define APR_ASSERT(x)										\
	do {													\
		if (!(x)) {											\
			ALOGE("assertion failed: %s", #x)	\
			abort();										\
		}													\
	} while (0)												\

#endif /* APR_HAS_LOG */

#ifdef __cplusplus
}
#endif

#endif /* APR_LOG_H */