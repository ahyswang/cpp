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
* ������־����
*/
APR_DECLARE(void) apr_log_set_level(int level);

/**
* ��־��¼����.
* @param file �����ļ���.
* @param line �����к�.
* @param level ��־����.
* @param format ��־����.
*/
APR_DECLARE(void) apr_log(const char *file, int line, int level, const char *format, ...);

/* ��־���ʹ�ã�������ϵͳ��printf���� */
#define ALOGD(format, ...) apr_log(__FILE__, __LINE__, APR_LOG_LEVEL_DEBUG, format, ##__VA_ARGS__)
#define ALOGI(format, ...) apr_log(__FILE__, __LINE__, APR_LOG_LEVEL_INFO, format, ##__VA_ARGS__)
#define ALOGE(format, ...) apr_log(__FILE__, __LINE__, APR_LOG_LEVEL_ERROR, format, ##__VA_ARGS__)

/* ��־��¼���Դ��� */
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