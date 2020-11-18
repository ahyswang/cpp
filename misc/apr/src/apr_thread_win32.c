#include "apr_thread.h"
#include "apr_allocater.h"
#include "apr_errno.h"

#if APR_HAS_THREAD_WIN32

#include <Windows.h>

struct apr_thread_t {
	HANDLE td;
	apr_int32_t cancel;
	apr_int32_t cancel_how;
	void *data;
	apr_thread_start_t func;
	apr_status_t exitval;
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
	unsigned temp;
	HANDLE handle;

	(*new) = (apr_thread_t *)apr_zalloc(sizeof(apr_thread_t));

	if ((*new) == NULL) {
		return APR_ENOMEM;
	}

	(*new)->data = data;
	(*new)->func = func;
	(*new)->td = NULL;

	/* 堆栈大小为0使用默认的堆栈大小
	 */
	if ((handle = CreateThread(NULL, 0,
		(unsigned int(*)(void *))dummy_worker,
		(*new), 0, &temp)) == 0) {
		return GetLastError();
	}

	(*new)->td = handle;

	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_thread_exit(apr_thread_t *thd,
	apr_status_t retval)
{
	/* 线程函数来调用 */
	thd->exitval = retval;
	ExitThread(0);
	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_thread_join(apr_status_t *retval,
	apr_thread_t *thd)
{
	apr_status_t rv = APR_SUCCESS;

	if (!thd->td) {
		/* 不能等待分离的线程 */
		return -1;
	}
	rv = WaitForSingleObject(thd->td, INFINITE);
	if (rv == WAIT_OBJECT_0 || rv == WAIT_ABANDONED) {
		/* 返回码在线程函数中返回 */
		*retval = thd->exitval;
	}
	else
		rv = GetLastError();

	CloseHandle(thd->td);
	thd->td = NULL;

	return rv;
}

APR_DECLARE(apr_status_t) apr_thread_detach(apr_thread_t *thd)
{
	if (thd->td && CloseHandle(thd->td)) {
		thd->td = NULL;
		return APR_SUCCESS;
	}
	else {
		return -1;
	}
}

APR_DECLARE(void) apr_thread_yield()
{
	SwitchToThread();
}

APR_DECLARE(void) apr_thread_destory(apr_thread_t *thd)
{
	if (thd->td){
		CloseHandle(thd->td);
	}
	apr_free(thd);
}

struct apr_thread_once_t {
	long value;
};

APR_DECLARE(apr_status_t) apr_thread_once_init(apr_thread_once_t **control)
{
	(*control) = apr_zalloc(sizeof(**control));
	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_thread_once(apr_thread_once_t *control,
	void(*func)(void))
{
	/* 
	 * InterlockedExchange函数用于原子地将32位整数设为指定的值 
	 * 返回值是指向原先的32位整数值。
	 */
	if (!InterlockedExchange(&control->value, 1)) {
		func();
	}
	return APR_SUCCESS;
}

struct apr_thread_mutex_t {
	CRITICAL_SECTION  section;
};

APR_DECLARE(apr_status_t) apr_thread_mutex_create(apr_thread_mutex_t **mutex)
{
	(*mutex) = (apr_thread_mutex_t *)apr_zalloc(sizeof(**mutex));

	InitializeCriticalSection(&(*mutex)->section);

	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_thread_mutex_lock(apr_thread_mutex_t *mutex)
{
	/* 重复调用EnterCriticalSection不会锁住自己，但需要调用同样次数的LeaveCriticalSection */
	EnterCriticalSection(&mutex->section);
	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_thread_mutex_trylock(apr_thread_mutex_t *mutex)
{
	if (!TryEnterCriticalSection(&mutex->section)) {
		return APR_EBUSY;
	}
	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_thread_mutex_unlock(apr_thread_mutex_t *mutex)
{
	LeaveCriticalSection(&mutex->section);
	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_thread_mutex_destroy(apr_thread_mutex_t *mutex)
{
	DeleteCriticalSection(&mutex->section);
	apr_free(mutex);
	return APR_SUCCESS;
}

struct apr_thread_cond_t {
	HANDLE semaphore;
	CRITICAL_SECTION csection;
	unsigned long num_waiting;
	unsigned long num_wake;
	unsigned long generation;
};

APR_DECLARE(apr_status_t) apr_thread_cond_create(apr_thread_cond_t **cond)
{
	apr_thread_cond_t *cv;

	cv = apr_zalloc(sizeof(**cond));
	if (cv == NULL) {
		return APR_ENOMEM;
	}

	cv->semaphore = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
	if (cv->semaphore == NULL) {
		return GetLastError();
	}

	*cond = cv;

	InitializeCriticalSection(&cv->csection);

	return APR_SUCCESS;
}

static apr_status_t _thread_cond_timedwait(apr_thread_cond_t *cond,
	apr_thread_mutex_t *mutex,
	DWORD timeout_ms)
{
	DWORD res;
	apr_status_t rv;
	unsigned int wake = 0;
	unsigned long generation;

	EnterCriticalSection(&cond->csection);
	cond->num_waiting++;
	generation = cond->generation;
	LeaveCriticalSection(&cond->csection);

	apr_thread_mutex_unlock(mutex);

	do {
		res = WaitForSingleObject(cond->semaphore, timeout_ms);

		EnterCriticalSection(&cond->csection);

		if (cond->num_wake) {
			if (cond->generation != generation) {
				cond->num_wake--;
				cond->num_waiting--;
				rv = APR_SUCCESS;
				break;
			}
			else {
				wake = 1;
			}
		}
		else if (res != WAIT_OBJECT_0) {
			cond->num_waiting--;
			rv = APR_TIMEUP;
			break;
		}

		LeaveCriticalSection(&cond->csection);

		if (wake) {
			wake = 0;
			ReleaseSemaphore(cond->semaphore, 1, NULL);
		}
	} while (1);

	LeaveCriticalSection(&cond->csection);
	apr_thread_mutex_lock(mutex);

	return rv;
}

APR_DECLARE(apr_status_t) apr_thread_cond_wait(apr_thread_cond_t *cond,
	apr_thread_mutex_t *mutex)
{
	return _thread_cond_timedwait(cond, mutex, INFINITE);
}

APR_DECLARE(apr_status_t) apr_thread_cond_timedwait(apr_thread_cond_t *cond,
	apr_thread_mutex_t *mutex,
	long long timeout)
{
	return _thread_cond_timedwait(cond, mutex, (DWORD)timeout);
}

APR_DECLARE(apr_status_t) apr_thread_cond_signal(apr_thread_cond_t *cond)
{
	unsigned int wake = 0;

	EnterCriticalSection(&cond->csection);
	if (cond->num_waiting > cond->num_wake) {
		wake = 1;
		cond->num_wake++;
		cond->generation++;
	}
	LeaveCriticalSection(&cond->csection);

	if (wake) {
		ReleaseSemaphore(cond->semaphore, 1, NULL);
	}

	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_thread_cond_broadcast(apr_thread_cond_t *cond)
{
	unsigned long num_wake = 0;

	EnterCriticalSection(&cond->csection);
	if (cond->num_waiting > cond->num_wake) {
		num_wake = cond->num_waiting - cond->num_wake;
		cond->num_wake = cond->num_waiting;
		cond->generation++;
	}
	LeaveCriticalSection(&cond->csection);

	if (num_wake) {
		ReleaseSemaphore(cond->semaphore, num_wake, NULL);
	}

	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_thread_cond_destroy(apr_thread_cond_t *cond)
{
	CloseHandle(cond->semaphore);
	DeleteCriticalSection(&cond->csection);
	apr_free(cond);
	return APR_SUCCESS;
}

struct apr_thread_rwlock_t {
	HANDLE      write_mutex;
	HANDLE      read_event;
	LONG        readers;
};

APR_DECLARE(apr_status_t) apr_thread_rwlock_create(apr_thread_rwlock_t **rwlock)
{
	*rwlock = apr_zalloc(sizeof(**rwlock));

	(*rwlock)->readers = 0;

	if (!((*rwlock)->read_event = CreateEvent(NULL, TRUE, FALSE, NULL))) {
		*rwlock = NULL;
		return GetLastError();
	}

	if (!((*rwlock)->write_mutex = CreateMutex(NULL, FALSE, NULL))) {
		CloseHandle((*rwlock)->read_event);
		*rwlock = NULL;
		return GetLastError();
	}

	return APR_SUCCESS;
}

static apr_status_t apr_thread_rwlock_rdlock_core(apr_thread_rwlock_t *rwlock,
	DWORD  milliseconds)
{
	DWORD   code = WaitForSingleObject(rwlock->write_mutex, milliseconds);

	if (code == WAIT_FAILED || code == WAIT_TIMEOUT)
		return code;

	/* We've successfully acquired the writer mutex, we can't be locked
	 * for write, so it's OK to add the reader lock.  The writer mutex
	 * doubles as race condition protection for the readers counter.
	 */
	InterlockedIncrement(&rwlock->readers);

	if (!ResetEvent(rwlock->read_event))
		return GetLastError();

	if (!ReleaseMutex(rwlock->write_mutex))
		return GetLastError();

	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_thread_rwlock_rdlock(apr_thread_rwlock_t *rwlock)
{
	return apr_thread_rwlock_rdlock_core(rwlock, INFINITE);
}

APR_DECLARE(apr_status_t) apr_thread_rwlock_tryrdlock(apr_thread_rwlock_t *rwlock)
{
	return apr_thread_rwlock_rdlock_core(rwlock, 0);
}

static apr_status_t apr_thread_rwlock_wrlock_core(apr_thread_rwlock_t *rwlock, DWORD milliseconds)
{
	DWORD   code = WaitForSingleObject(rwlock->write_mutex, milliseconds);

	if (code == WAIT_FAILED || code == WAIT_TIMEOUT)
		return code;

	/* We've got the writer lock but we have to wait for all readers to
	 * unlock before it's ok to use it.
	 */
	if (rwlock->readers) {
		/* Must wait for readers to finish before returning, unless this
		 * is an trywrlock (milliseconds == 0):
		 */
		code = milliseconds
			? WaitForSingleObject(rwlock->read_event, milliseconds)
			: WAIT_TIMEOUT;

		if (code == WAIT_FAILED || code == WAIT_TIMEOUT) {
			/* Unable to wait for readers to finish, release write lock: */
			if (!ReleaseMutex(rwlock->write_mutex))
				return GetLastError();

			return code;
		}
	}

	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_thread_rwlock_wrlock(apr_thread_rwlock_t *rwlock)
{
	return apr_thread_rwlock_wrlock_core(rwlock, INFINITE);
}

APR_DECLARE(apr_status_t)apr_thread_rwlock_trywrlock(apr_thread_rwlock_t *rwlock)
{
	return apr_thread_rwlock_wrlock_core(rwlock, 0);
}

APR_DECLARE(apr_status_t) apr_thread_rwlock_unlock(apr_thread_rwlock_t *rwlock)
{
	apr_status_t rv = 0;

	/* First, guess that we're unlocking a writer */
	if (!ReleaseMutex(rwlock->write_mutex))
		rv = GetLastError();

	if (rv == ERROR_NOT_OWNER) {
		// Nope, we must have a read lock
		if (rwlock->readers &&
			!InterlockedDecrement(&rwlock->readers) &&
			!SetEvent(rwlock->read_event)) {
			rv = GetLastError();
		}
		else {
			rv = 0;
		}
	}
	return rv;
}

APR_DECLARE(apr_status_t) apr_thread_rwlock_destroy(apr_thread_rwlock_t *rwlock)
{
	CloseHandle(rwlock->read_event);
	CloseHandle(rwlock->write_mutex);
	apr_free(rwlock);
	return APR_SUCCESS;
}


#endif /* APR_HAS_THREAD_WIN32 */

