#ifndef APR_ALLOCATER_H
#define APR_ALLOCATER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	/**
	 * �����ڴ�
	 *
	 * @param size �ڴ��ֽڴ�С
	 * @returns �ɹ����ص�ַ��ʧ�ܷ���NULL
	 */
	void* apr_malloc(size_t size);

	/**
	 * �����ڴ沢����ʼ��Ϊ0
	 *
	 * @param size �ڴ��ֽڴ�С
	 * @returns �ɹ����ص�ַ��ʧ�ܷ���NULL
	 */
	void* apr_zalloc(size_t size);

	/**
	 * �ͷ��ڴ�
	 *
	 * @param ptr �ڴ��ַ
	 */
	void apr_free(void* ptr);

	/**
	 * ���·����ڴ沢�����ݱ���
	 * ԭ���ĵ�ַ��չ�ڴ�ɹ��᷵��ԭ���ĵ�ַ�����򿪱��µ��ڴ�Ŀռ䲢��
	 * ���ݿ������µ��ڴ�ռ䷵���µ��ڴ��ַ��
	 * @param p	ԭ�����ڴ��ַ
	 * @param size �ڴ��ֽڴ�С
	 * @returns �ɹ����ص�ַ��ʧ�ܷ���NULL
	 */
	void* apr_realloc(void* p, size_t size);

	/**
	 * �����ֽڶ�����ڴ�
	 *
	 * @param size �ڴ��ֽڴ�С
	 * @param alignment �ֽڶ����С
	 * @returns �ɹ����ص�ַ��ʧ�ܷ���NULL
	 */
	void* apr_malloc_aligned(size_t size, size_t alignment);

	/**
	 * �ͷ��ֽڶ�����ڴ�
	 *
	 * @param ptr �ڴ��ַ
	 */
	void apr_free_aligned(void* ptr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* APR_ALLOCATER_H */
