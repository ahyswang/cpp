#ifndef APR_DSO_H

#include "apr.h"

#ifdef __cplusplus
extern "C" {
#endif

#if APR_HAS_DSO

/**
* 动态库类型
*/
typedef struct apr_dso_handle_t       apr_dso_handle_t;

/**
* 动态库函数符号类型
*/
typedef void *                        apr_dso_handle_sym_t;

/**
* 加载动态库。
* @param res_handle 动态库指针
* @param path 动态库文件路径
*/
APR_DECLARE(apr_status_t) apr_dso_load(apr_dso_handle_t **res_handle,
	const char *path);

/**
* 卸载动态库。
* @param handle 动态库指针
*/
APR_DECLARE(apr_status_t) apr_dso_unload(apr_dso_handle_t *handle);

/**
* 获取动态库函数符号。
* @param ressym 动态库函数指针
* @param handle 动态库指针
* @param symname 动态库函数名称
*/
APR_DECLARE(apr_status_t) apr_dso_sym(apr_dso_handle_sym_t *ressym,
	apr_dso_handle_t *handle,
	const char *symname);

#endif /* APR_HAS_DSO */

#ifdef __cplusplus
}
#endif

#endif