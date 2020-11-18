#ifndef APR_DSO_H

#include "apr.h"

#ifdef __cplusplus
extern "C" {
#endif

#if APR_HAS_DSO

/**
* ��̬������
*/
typedef struct apr_dso_handle_t       apr_dso_handle_t;

/**
* ��̬�⺯����������
*/
typedef void *                        apr_dso_handle_sym_t;

/**
* ���ض�̬�⡣
* @param res_handle ��̬��ָ��
* @param path ��̬���ļ�·��
*/
APR_DECLARE(apr_status_t) apr_dso_load(apr_dso_handle_t **res_handle,
	const char *path);

/**
* ж�ض�̬�⡣
* @param handle ��̬��ָ��
*/
APR_DECLARE(apr_status_t) apr_dso_unload(apr_dso_handle_t *handle);

/**
* ��ȡ��̬�⺯�����š�
* @param ressym ��̬�⺯��ָ��
* @param handle ��̬��ָ��
* @param symname ��̬�⺯������
*/
APR_DECLARE(apr_status_t) apr_dso_sym(apr_dso_handle_sym_t *ressym,
	apr_dso_handle_t *handle,
	const char *symname);

#endif /* APR_HAS_DSO */

#ifdef __cplusplus
}
#endif

#endif