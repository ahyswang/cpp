#ifndef APR_VERSION_H
#define APR_VERSION_H

#define APR_COPYRIGHT "Copyright (c) 2000-2019 The Iflytek Software " \
                      "Foundation or its licensors, as applicable."

/** major version
 * 主版本号修改影响API的兼容性修改（API的参数、API的结构体）。
 */
#define APR_MAJOR_VERSION       1

/** minor version
 * 次版本号修改不影响API的兼容性修改，用于内部功能升级。
 * 升级到APR_MAJOR_VERSION，当前值设置为0。
 */
#define APR_MINOR_VERSION       6

/** patch level
 * 补丁版本修改包括BUG修复。
 * 升级到APR_MINOR_VERSION，当前值设置为0。
 */
#define APR_PATCH_VERSION       5

#if defined(APR_IS_DEV_VERSION)
/** 开发版本的内部标志 */
#ifndef APR_IS_DEV_STRING
#define APR_IS_DEV_STRING "-dev"
#endif
#else
#define APR_IS_DEV_STRING ""
#endif

#ifndef APR_STRINGIFY
/** 宏字符串化（#5 => "5"） */
#define APR_STRINGIFY(n) #n
#endif

/** 版本号的字符串 */
#define APR_VERSION_STRING \
     APR_STRINGIFY(APR_MAJOR_VERSION) "." \
     APR_STRINGIFY(APR_MINOR_VERSION) "." \
     APR_STRINGIFY(APR_PATCH_VERSION) \
     APR_IS_DEV_STRING

#include "apr.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 版本号结构
 */
typedef struct {
	int major;      /**< 主版本号 */
	int minor;      /**< 次版本号 */
	int patch;      /**< 补丁版本号 */
	int is_dev;     /**< 开发版本标志 (1 or 0) */
} apr_version_t;

/**
 * 获取版本号信息。
 *
 *  @param pvsn 版本号结构指针
 */
APR_DECLARE(void) apr_version(apr_version_t *pvsn);

/** 获取版本号字符串 */
APR_DECLARE(const char *) apr_version_string(void);

#ifdef __cplusplus
}
#endif

#endif /* APR_VERSION_H */