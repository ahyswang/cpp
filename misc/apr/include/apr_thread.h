#ifndef APR_THREAD_H
#define APR_THREAD_H

#include "apr.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#if APR_HAS_THREADS 

/** �߳̽ṹ���� */
typedef struct apr_thread_t apr_thread_t;

/**
 * �̺߳�������
 */
typedef void *(*apr_thread_start_t)(apr_thread_t*, void*);

/**
 * �����̡߳�
 * @param new �߳�ָ��
 * @param func �̺߳���
 * @param data �û�����
 */
APR_DECLARE(apr_status_t) apr_thread_create(apr_thread_t **new,
	apr_thread_start_t func,
	void *data);

/**
 * �˳���ǰ�̣߳��̺߳�����ִ�У��ȴ��߳̿��Ի�ȡ������)��
 * @param thd �߳�ָ��
 * @param retval ������
 */
APR_DECLARE(apr_status_t) apr_thread_exit(apr_thread_t *thd,
	apr_status_t retval);

/**
 * �����ȴ�ֱ���߳��˳���
 * @param retval ���ȴ��̵߳ķ�����
 * @param thd �߳�ָ��
 */
APR_DECLARE(apr_status_t) apr_thread_join(apr_status_t *retval,
	apr_thread_t *thd);

/**
 * �����߳�(�ͷŲ�����Դ�����������߳̿���)��
 * @param thd �߳�ָ��
 */
APR_DECLARE(apr_status_t) apr_thread_detach(apr_thread_t *thd);

/**
 * �ͷ�ʱ��Ƭ
 */
APR_DECLARE(void) apr_thread_yield();

/**
 * �����߳���Դ
 * @param thd �߳�ָ��
 */
APR_DECLARE(void) apr_thread_destory(apr_thread_t *thd);


/** ����ִ��һ�νṹ���͡� */
typedef struct apr_thread_once_t      apr_thread_once_t;

/**
 * �����������ж���
 * @param control �������ж���ָ��
 */
APR_DECLARE(apr_status_t) apr_thread_once_init(apr_thread_once_t **control);

/**
 * ����һ��ָ����������ʹ������̵߳��á�
 * @param control �������ж���ָ��
 * @param func ��ִ�еĺ���
 */
APR_DECLARE(apr_status_t) apr_thread_once(apr_thread_once_t *control,
	void(*func)(void));


/** ͬ�����ṹ���� */
typedef struct apr_thread_mutex_t apr_thread_mutex_t;

/**
 * ����ͬ������
 * @param mutex ͬ����ָ��
 */
APR_DECLARE(apr_status_t) apr_thread_mutex_create(apr_thread_mutex_t **mutex);

/**
 * ��ȡͬ������������Ѿ�����ȡ�������ȴ�ֱ�����ͷš�
 * @param mutex ͬ����ָ��
 */
APR_DECLARE(apr_status_t) apr_thread_mutex_lock(apr_thread_mutex_t *mutex);

/**
 * ���Ի�ȡͬ������������Ѿ�����ȡ������ֱ�ӷ��أ�APR_EBUSY����
 * @param mutex ͬ����ָ��
 */
APR_DECLARE(apr_status_t) apr_thread_mutex_trylock(apr_thread_mutex_t *mutex);

/**
 * �ͷ�ͬ������
 * @param mutex ͬ����ָ��
 */
APR_DECLARE(apr_status_t) apr_thread_mutex_unlock(apr_thread_mutex_t *mutex);

/**
 * ����ͬ����
 * @param mutex ͬ����ָ��
 */
APR_DECLARE(apr_status_t) apr_thread_mutex_destroy(apr_thread_mutex_t *mutex);

/** ������������ */
typedef struct apr_thread_cond_t apr_thread_cond_t;

/**
 * ������������
 * @param ��������ָ��
 */
APR_DECLARE(apr_status_t) apr_thread_cond_create(apr_thread_cond_t **cond);

/**
 * �����ȴ������ѡ�
 * ��������״ֱ̬�������ѣ�ÿ��������������Ҫ��ͬ������������ͬ�����������ǰ�ȱ�
 * ��ȡ����������δ֪���⡣�����߳̽�������״̬��ͬ����Ҳ��ͬʱ���ͷš���������
 * �̱����ѵ�ʱ��ͬ����Ҳ��ͬʱ����ȡ��
 * @param cond ��������ָ��
 * @param mutex �ڵ����������ǰ��ͬ������Ҫ�ȱ���ȡ��
 */
APR_DECLARE(apr_status_t) apr_thread_cond_wait(apr_thread_cond_t *cond,
	apr_thread_mutex_t *mutex);

/**
 * ��ʱ�����ȴ������ѡ�
 * @param cond ��������ָ��
 * @param mutex �ڵ����������ǰ��ͬ������Ҫ�ȱ���ȡ��
 * @param timeout ���ʱʱ�䣨���룩����ʱ����APR_TIMEUP��
 */
APR_DECLARE(apr_status_t) apr_thread_cond_timedwait(apr_thread_cond_t *cond,
	apr_thread_mutex_t *mutex,
	long long timeout);

/**
 * ֪ͨһ���ȴ����̡߳�
 * �ȴ����̱߳����ѣ�ͬʱ���ȡ��������ͬ����������������������Ȼ�ȡͬ������
 * @param cond ��������ָ��
 * @remark ���û�еȴ��̣߳����ᷢ���κβ���.
 */
APR_DECLARE(apr_status_t) apr_thread_cond_signal(apr_thread_cond_t *cond);


/**
 * ֪ͨ���еȴ����̡߳�
 * �ȴ����̻߳��Ŷӱ����ѣ�ͬʱ���ȡ��������ͬ������
 * @param cond ��������ָ��
 * @remark ���û�еȴ��̣߳����ᷢ���κβ���.
 */
APR_DECLARE(apr_status_t) apr_thread_cond_broadcast(apr_thread_cond_t *cond);

/**
 * ��������������
 * @param cond ��������ָ��
 */
APR_DECLARE(apr_status_t) apr_thread_cond_destroy(apr_thread_cond_t *cond);

/** ��д������ */
typedef struct apr_thread_rwlock_t apr_thread_rwlock_t;

/**
 * ������д����
 * @param ��д��ָ��
 */
APR_DECLARE(apr_status_t) apr_thread_rwlock_create(apr_thread_rwlock_t **rwlock);

/**
 * ��ȡ������
 * ���������߳�ͬʱ��ȡ��������
 * @param rwlock ��д��ָ��
 */
APR_DECLARE(apr_status_t) apr_thread_rwlock_rdlock(apr_thread_rwlock_t *rwlock);

/**
 * ���Ի�ȡ������
 * @param ��д��ָ��
 */
APR_DECLARE(apr_status_t) apr_thread_rwlock_tryrdlock(apr_thread_rwlock_t *rwlock);

/**
 * ��ȡд����
 * @param rwlock ��д��ָ��
 */
APR_DECLARE(apr_status_t) apr_thread_rwlock_wrlock(apr_thread_rwlock_t *rwlock);

/**
 * ���Ի�ȡд����
 * @param rwlock ��д��ָ��
 */
APR_DECLARE(apr_status_t) apr_thread_rwlock_trywrlock(apr_thread_rwlock_t *rwlock);

/**
 * �ͷŶ�д����
 * @param rwlock ��д��ָ��
 */
APR_DECLARE(apr_status_t) apr_thread_rwlock_unlock(apr_thread_rwlock_t *rwlock);

/**
 * ���ٶ�д����
 * @param rwlock ��д��ָ��
 */
APR_DECLARE(apr_status_t) apr_thread_rwlock_destroy(apr_thread_rwlock_t *rwlock);

#endif /* APR_HAS_THREADS */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* APR_THREAD_H */