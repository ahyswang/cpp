#ifndef APR_THREAD_H
#define APR_THREAD_H

#include "apr.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#if APR_HAS_THREADS 

/** 线程结构类型 */
typedef struct apr_thread_t apr_thread_t;

/**
 * 线程函数类型
 */
typedef void *(*apr_thread_start_t)(apr_thread_t*, void*);

/**
 * 创建线程。
 * @param new 线程指针
 * @param func 线程函数
 * @param data 用户数据
 */
APR_DECLARE(apr_status_t) apr_thread_create(apr_thread_t **new,
	apr_thread_start_t func,
	void *data);

/**
 * 退出当前线程（线程函数中执行，等待线程可以获取返回码)。
 * @param thd 线程指针
 * @param retval 返回码
 */
APR_DECLARE(apr_status_t) apr_thread_exit(apr_thread_t *thd,
	apr_status_t retval);

/**
 * 阻塞等待直到线程退出。
 * @param retval 被等待线程的返回码
 * @param thd 线程指针
 */
APR_DECLARE(apr_status_t) apr_thread_join(apr_status_t *retval,
	apr_thread_t *thd);

/**
 * 分离线程(释放部分资源，脱离主控线程控制)。
 * @param thd 线程指针
 */
APR_DECLARE(apr_status_t) apr_thread_detach(apr_thread_t *thd);

/**
 * 释放时间片
 */
APR_DECLARE(void) apr_thread_yield();

/**
 * 销毁线程资源
 * @param thd 线程指针
 */
APR_DECLARE(void) apr_thread_destory(apr_thread_t *thd);


/** 函数执行一次结构类型。 */
typedef struct apr_thread_once_t      apr_thread_once_t;

/**
 * 创建单次运行对象
 * @param control 单次运行对象指针
 */
APR_DECLARE(apr_status_t) apr_thread_once_init(apr_thread_once_t **control);

/**
 * 运行一次指定函数，即使被多个线程调用。
 * @param control 单次运行对象指针
 * @param func 被执行的函数
 */
APR_DECLARE(apr_status_t) apr_thread_once(apr_thread_once_t *control,
	void(*func)(void));


/** 同步锁结构类型 */
typedef struct apr_thread_mutex_t apr_thread_mutex_t;

/**
 * 创建同步锁。
 * @param mutex 同步锁指针
 */
APR_DECLARE(apr_status_t) apr_thread_mutex_create(apr_thread_mutex_t **mutex);

/**
 * 获取同步锁，如果锁已经被获取，阻塞等待直到被释放。
 * @param mutex 同步锁指针
 */
APR_DECLARE(apr_status_t) apr_thread_mutex_lock(apr_thread_mutex_t *mutex);

/**
 * 尝试获取同步锁，如果锁已经被获取，调用直接返回（APR_EBUSY）。
 * @param mutex 同步锁指针
 */
APR_DECLARE(apr_status_t) apr_thread_mutex_trylock(apr_thread_mutex_t *mutex);

/**
 * 释放同步锁。
 * @param mutex 同步锁指针
 */
APR_DECLARE(apr_status_t) apr_thread_mutex_unlock(apr_thread_mutex_t *mutex);

/**
 * 销毁同步锁
 * @param mutex 同步锁指针
 */
APR_DECLARE(apr_status_t) apr_thread_mutex_destroy(apr_thread_mutex_t *mutex);

/** 条件变量类型 */
typedef struct apr_thread_cond_t apr_thread_cond_t;

/**
 * 创建条件变量
 * @param 条件变量指针
 */
APR_DECLARE(apr_status_t) apr_thread_cond_create(apr_thread_cond_t **cond);

/**
 * 阻塞等待被唤醒。
 * 进入休眠状态直到被唤醒，每个条件变量必须要和同步锁关联，并同步锁必须调用前先被
 * 获取，否则会出现未知问题。调用线程进入休眠状态，同步锁也会同时被释放。当调用线
 * 程被唤醒的时候，同步锁也会同时被获取。
 * @param cond 条件变量指针
 * @param mutex 在调用这个函数前，同步锁需要先被获取。
 */
APR_DECLARE(apr_status_t) apr_thread_cond_wait(apr_thread_cond_t *cond,
	apr_thread_mutex_t *mutex);

/**
 * 超时阻塞等待被唤醒。
 * @param cond 条件变量指针
 * @param mutex 在调用这个函数前，同步锁需要先被获取。
 * @param timeout 最大超时时间（毫秒），超时返回APR_TIMEUP。
 */
APR_DECLARE(apr_status_t) apr_thread_cond_timedwait(apr_thread_cond_t *cond,
	apr_thread_mutex_t *mutex,
	long long timeout);

/**
 * 通知一个等待的线程。
 * 等待的线程被唤醒，同时会获取到关联的同步锁，调用这个函数必须先获取同步锁。
 * @param cond 条件变量指针
 * @remark 如果没有等待线程，不会发生任何操作.
 */
APR_DECLARE(apr_status_t) apr_thread_cond_signal(apr_thread_cond_t *cond);


/**
 * 通知所有等待的线程。
 * 等待的线程会排队被唤醒，同时会获取到关联的同步锁。
 * @param cond 条件变量指针
 * @remark 如果没有等待线程，不会发生任何操作.
 */
APR_DECLARE(apr_status_t) apr_thread_cond_broadcast(apr_thread_cond_t *cond);

/**
 * 销毁条件变量。
 * @param cond 条件变量指针
 */
APR_DECLARE(apr_status_t) apr_thread_cond_destroy(apr_thread_cond_t *cond);

/** 读写锁类型 */
typedef struct apr_thread_rwlock_t apr_thread_rwlock_t;

/**
 * 创建读写锁。
 * @param 读写锁指针
 */
APR_DECLARE(apr_status_t) apr_thread_rwlock_create(apr_thread_rwlock_t **rwlock);

/**
 * 获取读锁。
 * 允许多个读线程同时获取到读锁。
 * @param rwlock 读写锁指针
 */
APR_DECLARE(apr_status_t) apr_thread_rwlock_rdlock(apr_thread_rwlock_t *rwlock);

/**
 * 尝试获取读锁。
 * @param 读写锁指针
 */
APR_DECLARE(apr_status_t) apr_thread_rwlock_tryrdlock(apr_thread_rwlock_t *rwlock);

/**
 * 获取写锁。
 * @param rwlock 读写锁指针
 */
APR_DECLARE(apr_status_t) apr_thread_rwlock_wrlock(apr_thread_rwlock_t *rwlock);

/**
 * 尝试获取写锁。
 * @param rwlock 读写锁指针
 */
APR_DECLARE(apr_status_t) apr_thread_rwlock_trywrlock(apr_thread_rwlock_t *rwlock);

/**
 * 释放读写锁。
 * @param rwlock 读写锁指针
 */
APR_DECLARE(apr_status_t) apr_thread_rwlock_unlock(apr_thread_rwlock_t *rwlock);

/**
 * 销毁读写锁。
 * @param rwlock 读写锁指针
 */
APR_DECLARE(apr_status_t) apr_thread_rwlock_destroy(apr_thread_rwlock_t *rwlock);

#endif /* APR_HAS_THREADS */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* APR_THREAD_H */