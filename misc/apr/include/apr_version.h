#ifndef APR_VERSION_H
#define APR_VERSION_H

#define APR_COPYRIGHT "Copyright (c) 2000-2019 The Iflytek Software " \
                      "Foundation or its licensors, as applicable."

/** major version
 * ���汾���޸�Ӱ��API�ļ������޸ģ�API�Ĳ�����API�Ľṹ�壩��
 */
#define APR_MAJOR_VERSION       1

/** minor version
 * �ΰ汾���޸Ĳ�Ӱ��API�ļ������޸ģ������ڲ�����������
 * ������APR_MAJOR_VERSION����ǰֵ����Ϊ0��
 */
#define APR_MINOR_VERSION       6

/** patch level
 * �����汾�޸İ���BUG�޸���
 * ������APR_MINOR_VERSION����ǰֵ����Ϊ0��
 */
#define APR_PATCH_VERSION       5

#if defined(APR_IS_DEV_VERSION)
/** �����汾���ڲ���־ */
#ifndef APR_IS_DEV_STRING
#define APR_IS_DEV_STRING "-dev"
#endif
#else
#define APR_IS_DEV_STRING ""
#endif

#ifndef APR_STRINGIFY
/** ���ַ�������#5 => "5"�� */
#define APR_STRINGIFY(n) #n
#endif

/** �汾�ŵ��ַ��� */
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
 * �汾�Žṹ
 */
typedef struct {
	int major;      /**< ���汾�� */
	int minor;      /**< �ΰ汾�� */
	int patch;      /**< �����汾�� */
	int is_dev;     /**< �����汾��־ (1 or 0) */
} apr_version_t;

/**
 * ��ȡ�汾����Ϣ��
 *
 *  @param pvsn �汾�Žṹָ��
 */
APR_DECLARE(void) apr_version(apr_version_t *pvsn);

/** ��ȡ�汾���ַ��� */
APR_DECLARE(const char *) apr_version_string(void);

#ifdef __cplusplus
}
#endif

#endif /* APR_VERSION_H */