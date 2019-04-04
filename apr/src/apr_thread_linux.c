#include "apr_thread.h"
#include "apr_allocater.h"
#include "apr_errno.h"

#ifdef APR_HAS_THREAD_LINUX

#include <pthread.h>
#include <errno.h>

struct apr_thread_t {
	pthread_t td;
	void *data;
	apr_thread_start_t func;
	apr_status_t exitval; /**< 线程返回码 */
};

static void *dummy_worker(void *opaque)
{
	apr_thread_t *thd = (apr_thread_t *)opaque;
	return thd->func(thd, thd->data);
}

APR_DECLARE(apr_status_t) apr_thread_create(apr_thread_t **new,
	apr_thread_start_t func,
	void *data)
{
	apr_status_t rv;
	apr_thread_t *new_thread;
	pthread_attr_t attr;

	new_thread = (apr_thread_t *)apr_zalloc(sizeof(apr_thread_t));
	if (new_thread == NULL) {
		return APR_ENOMEM;
	}
	new_thread->td = 0;
	new_thread->exitval = 0;
	new_thread->data = data;
	new_thread->func = func;

	rv = pthread_attr_init(&attr);
	if (rv) {
		apr_free(new_thread);
		return rv;
	}
	rv = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	if (rv) {
		apr_free(new_thread);
		pthread_attr_destroy(&attr);
		return rv;
	}
	rv = pthread_create(&new_thread->td, &attr, dummy_worker, new_thread);
	pthread_attr_destroy(&attr);
	if (rv) {
		apr_free(new_thread);
		return rv;
	}

	*new = new_thread;
	
	return APR_SUCCESS;
}

APR_DECLARE(void) apr_thread_destory(apr_thread_t *thd)
{
	apr_free(thd);
}


APR_DECLARE(apr_status_t) apr_thread_exit(apr_thread_t *thd,
	apr_status_t retval)
{
	thd->exitval = retval;
	pthread_exit(NULL);
	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_thread_join(apr_status_t *retval,
	apr_thread_t *thd)
{
	apr_status_t stat;
	apr_status_t *thread_stat;

	if ((stat = pthread_join(thd->td, (void *)&thread_stat)) == 0) {
		*retval = thd->exitval;  //返回码
		return APR_SUCCESS;
	}
	else {
		return stat;
	}
}

APR_DECLARE(apr_status_t) apr_thread_detach(apr_thread_t *thd)
{
	apr_status_t stat;

	if ((stat = pthread_detach(thd->td)) == 0) {
		return APR_SUCCESS;
	}
	else {
		return stat;
	}
}

APR_DECLARE(void) apr_thread_yield(void)
{
	// mips 不支持这个函数
	//pthread_yield();
}


struct apr_thread_mutex_t {
	pthread_mutex_t mutex;
};


APR_DECLARE(apr_status_t) apr_thread_mutex_create(apr_thread_mutex_t **mutex)
{
	apr_thread_mutex_t *new_mutex;
	apr_status_t rv;

	new_mutex = apr_zalloc(sizeof(apr_thread_mutex_t));

#ifdef HAVE_PTHREAD_MUTEX_RECURSIVE
	{   
		pthread_mutexattr_t mattr;

		rv = pthread_mutexattr_init(&mattr);
		if (rv) return rv;
		/* 可重入锁，同一个线程可以多次获取同一个递归锁，不会产生死锁 */
		rv = pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
		if (rv) {
			pthread_mutexattr_destroy(&mattr);
			return rv;
		}

		rv = pthread_mutex_init(&new_mutex->mutex, &mattr);

		pthread_mutexattr_destroy(&mattr);

		if (rv) {
			return rv;	
		}
	}
	
#else  
	rv = pthread_mutex_init(&new_mutex->mutex, NULL);
	if (rv) {
		return rv;
	}
#endif /* HAVE_PTHREAD_MUTEX_RECURSIVE */
	
	*mutex = new_mutex;
	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_thread_mutex_lock(apr_thread_mutex_t *mutex)
{
	apr_status_t rv;

	rv = pthread_mutex_lock(&mutex->mutex);

	return rv;
}

APR_DECLARE(apr_status_t) apr_thread_mutex_trylock(apr_thread_mutex_t *mutex)
{
	apr_status_t rv;

	rv = pthread_mutex_trylock(&mutex->mutex);
	if (rv) {
		return (rv == EBUSY) ? APR_EBUSY : rv;
	}

	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_thread_mutex_unlock(apr_thread_mutex_t *mutex)
{
	apr_status_t rv;

	rv = pthread_mutex_unlock(&mutex->mutex);

	return rv;
}

APR_DECLARE(apr_status_t) apr_thread_mutex_destroy(apr_thread_mutex_t *mutex)
{
	apr_status_t rv;

	rv = pthread_mutex_destroy(&mutex->mutex);
#ifdef HAVE_ZOS_PTHREADS
	if (rv) {
		rv = errno;
	}
#endif
	return rv;
}

struct apr_thread_cond_t {
	pthread_cond_t cond;
};

APR_DECLARE(apr_status_t) apr_thread_cond_create(apr_thread_cond_t **cond)
{
	apr_thread_cond_t *new_cond;
	apr_status_t rv;

	new_cond = apr_zalloc(sizeof(apr_thread_cond_t));

	if ((rv = pthread_cond_init(&new_cond->cond, NULL))) {
		return rv;
	}

	*cond = new_cond;
	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_thread_cond_wait(apr_thread_cond_t *cond,
	apr_thread_mutex_t *mutex)
{
	apr_status_t rv;

	rv = pthread_cond_wait(&cond->cond, &mutex->mutex);

	return rv;
}

APR_DECLARE(apr_status_t) apr_thread_cond_timedwait(apr_thread_cond_t *cond,
	apr_thread_mutex_t *mutex,
	long long timeout) //ms
{
	apr_status_t rv;
	long  then;
	struct timespec abstime;

	then = time(NULL) + timeout / 1000;
	abstime.tv_sec = then;
	abstime.tv_nsec = (timeout % 1000) * 1000000; /* nanoseconds */

	rv = pthread_cond_timedwait(&cond->cond, &mutex->mutex, &abstime);
	if (ETIMEDOUT == rv) {
		return APR_TIMEUP;
	}
	return rv;
}


APR_DECLARE(apr_status_t) apr_thread_cond_signal(apr_thread_cond_t *cond)
{
	apr_status_t rv;

	rv = pthread_cond_signal(&cond->cond);
#ifdef HAVE_ZOS_PTHREADS
	if (rv) {
		rv = errno;
	}
#endif
	return rv;
}

APR_DECLARE(apr_status_t) apr_thread_cond_broadcast(apr_thread_cond_t *cond)
{
	apr_status_t rv;

	rv = pthread_cond_broadcast(&cond->cond);
#ifdef HAVE_ZOS_PTHREADS
	if (rv) {
		rv = errno;
	}
#endif
	return rv;
}

APR_DECLARE(apr_status_t) apr_thread_cond_destroy(apr_thread_cond_t *cond)
{
	apr_status_t rv;

	rv = pthread_cond_destroy(&cond->cond);
#ifdef HAVE_ZOS_PTHREADS
	if (rv) {
		rv = errno;
	}
#endif
	apr_free(cond);

	return rv;
}

struct apr_thread_rwlock_t {
	pthread_rwlock_t rwlock;
};

APR_DECLARE(apr_status_t) apr_thread_rwlock_create(apr_thread_rwlock_t **rwlock)
{
	apr_thread_rwlock_t *new_rwlock;
	apr_status_t rv;

	new_rwlock = apr_zalloc(sizeof(apr_thread_rwlock_t));
	
	if ((rv = pthread_rwlock_init(&new_rwlock->rwlock, NULL))) {
#ifdef HAVE_ZOS_PTHREADS
		rv = errno;
#endif
		return rv;
	}

	*rwlock = new_rwlock;
	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_thread_rwlock_rdlock(apr_thread_rwlock_t *rwlock)
{
	apr_status_t rv;

	rv = pthread_rwlock_rdlock(&rwlock->rwlock);
#ifdef HAVE_ZOS_PTHREADS
	if (rv) {
		stat = errno;
	}
#endif
	return rv;
}

APR_DECLARE(apr_status_t) apr_thread_rwlock_tryrdlock(apr_thread_rwlock_t *rwlock)
{
	apr_status_t rv;

	rv = pthread_rwlock_tryrdlock(&rwlock->rwlock);
#ifdef HAVE_ZOS_PTHREADS
	if (rv) {
		rv = errno;
	}
#endif
	/* Normalize the return code. */
	if (rv == EBUSY)
		rv = APR_EBUSY;
	return rv;
}

APR_DECLARE(apr_status_t) apr_thread_rwlock_wrlock(apr_thread_rwlock_t *rwlock)
{
	apr_status_t rv;

	rv = pthread_rwlock_wrlock(&rwlock->rwlock);
#ifdef HAVE_ZOS_PTHREADS
	if (rv) {
		rv = errno;
	}
#endif
	return rv;
}

APR_DECLARE(apr_status_t) apr_thread_rwlock_trywrlock(apr_thread_rwlock_t *rwlock)
{
	apr_status_t rv;

	rv = pthread_rwlock_trywrlock(&rwlock->rwlock);
#ifdef HAVE_ZOS_PTHREADS
	if (rv) {
		rv = errno;
	}
#endif
	/* 忙状态 */
	if (rv == EBUSY)
		rv = APR_EBUSY;
	return rv;
}

APR_DECLARE(apr_status_t) apr_thread_rwlock_unlock(apr_thread_rwlock_t *rwlock)
{
	apr_status_t rv;

	rv = pthread_rwlock_unlock(&rwlock->rwlock);
#ifdef HAVE_ZOS_PTHREADS
	if (rv) {
		rv = errno;
	}
#endif
	return rv;
}

APR_DECLARE(apr_status_t) apr_thread_rwlock_destroy(apr_thread_rwlock_t *rwlock)
{
	apr_status_t rv;

	rv = pthread_rwlock_destroy(&rwlock->rwlock);
#ifdef HAVE_ZOS_PTHREADS
	if (rv) {
		rv = errno;
	}
#endif
	return rv;
}

#endif /* APR_HAS_THREAD_LINUX */