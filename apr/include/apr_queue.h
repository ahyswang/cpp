#ifndef APR_QUEUE_H
#define APR_QUEUE_H

/**
 * @file apr_queue.h
 * @brief �̰߳�ȫ��FIFOָ�������Ķ��С�
 * @note 
 */
#include "apr.h"

#define APR_HAS_THREADS 1

#if APR_HAS_THREADS

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * ���нṹ����
 */
typedef struct apr_queue_t apr_queue_t;

/**
 * �������У�֧���߳�ͬ����������
 * 
 * @param queue ����ָ��
 * @param queue_capacity �����е��������
 */
APR_DECLARE(apr_status_t) apr_queue_create(apr_queue_t **queue,
	unsigned int queue_capacity);

/**
 * ���ٶ���
 *
 * @param queue ����ָ��
 * @returns APR_SUCCESS ��ʾ�ɹ�
 */
APR_DECLARE(apr_status_t) apr_queue_destroy(apr_queue_t *queue);

/**
 * ����һ��Ԫ�أ�����������������ȴ���
 *
 * @param queue ����ָ��
 * @param data Ԫ��ָ��
 * @returns APR_EINTR ��ʾ�������жϣ�û�м���Ԫ�� (��Ҫ����)
 * @returns APR_EOF ��ʾ�����Ѿ��ر�
 * @returns APR_SUCCESS ��ʾ�ɹ�
 */
APR_DECLARE(apr_status_t) apr_queue_push(apr_queue_t *queue, void *data);


/**
 * ��ȡһ��Ԫ�أ�������пջ������ȴ���
 *
 * @param queue ����ָ��
 * @param data Ԫ��ָ��
 * @returns APR_EINTR ��ʾ�������жϣ�û�л�ȡ��Ԫ�� (��Ҫ����)
 * @returns APR_EOF ��ʾ�����Ѿ��ر�
 * @returns APR_SUCCESS ��ʾ�ɹ�
 */
APR_DECLARE(apr_status_t) apr_queue_pop(apr_queue_t *queue, void **data);

/**
 * ���Լ���һ��Ԫ�أ�������������������ء�
 *
 * @param queue ����ָ��
 * @param data Ԫ��ָ��
 * @returns APR_EINTR ��ʾ�������жϣ�û�л�ȡ��Ԫ�� (��Ҫ����)
 * @returns APR_EAGAIN ��ʾ������
 * @returns APR_EOF the ��ʾ�����Ѿ��ر�
 * @returns APR_SUCCESS ��ʾ�ɹ�
 */
APR_DECLARE(apr_status_t) apr_queue_trypush(apr_queue_t *queue, void *data);

/**
 * ���Ի�ȡһ��Ԫ�أ�������пջ��������ء�
 *
 * @param queue ����ָ��
 * @param data Ԫ��ָ��
 * @returns APR_EINTR ��ʾ�������жϣ�û�л�ȡ��Ԫ�� (��Ҫ����)
 * @returns APR_EAGAIN ��ʾ������
 * @returns APR_EOF the ��ʾ�����Ѿ��ر�
 * @returns APR_SUCCESS ��ʾ�ɹ�
 */
APR_DECLARE(apr_status_t) apr_queue_trypop(apr_queue_t *queue, void **data);

/**
 * ���ض��еĳ��ȡ�
 *
 * @warning ��ǰ���������̰߳�ȫ�ģ����ݲ���׼ȷ�ģ����ڼ������
 * @param queue ����ָ��
 * @returns ���ض��еĳ���
 */
APR_DECLARE(unsigned int) apr_queue_size(apr_queue_t *queue);

/**
 * �жϣ�֪ͨ�����������̡߳�
 *
 * @param queue ����ָ��
 */
APR_DECLARE(apr_status_t) apr_queue_interrupt_all(apr_queue_t *queue);

/**
 * ��ֹ���У����жϣ�֪ͨ�����������̡߳�
 *
 * @param queue ����ָ��
 */
APR_DECLARE(apr_status_t) apr_queue_term(apr_queue_t *queue);

#ifdef __cplusplus
}
#endif

#endif /* APR_HAS_THREADS */

#endif /* APRQUEUE_H */