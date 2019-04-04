#include <stdio.h>
#include <stdlib.h>

#include "apr.h"
#include "apr_allocater.h"
#include "apr_errno.h"
#include "apr_thread.h"
#include "apr_queue.h"

#if APR_HAS_THREADS
/*
 * 开启调试信息
 */
//#define QUEUE_DEBUG

struct apr_queue_t {
	void              **data;
	unsigned int        nelts; /**< 当前元素个数 */ 
	unsigned int        in;    /**< 下一个空的位置 */ 
	unsigned int        out;   /**< 下一个有的位置 */  
	unsigned int        bounds;/**< 队列最大容量 */  
	unsigned int        full_waiters; /**< 写入等待线程数 */
	unsigned int        empty_waiters; /**< 读取等待线程数 */
	apr_thread_mutex_t *one_big_mutex;
	apr_thread_cond_t  *not_empty;
	apr_thread_cond_t  *not_full;
	int                 terminated;
};

#ifdef QUEUE_DEBUG
static void Q_DBG(char*msg, apr_queue_t *q) {
	fprintf(stderr, "%ld\t#%d in %d out %d\t%s\n",
		apr_os_thread_current(),
		q->nelts, q->in, q->out,
		msg
	);
}
#else
#define Q_DBG(x,y) 
#endif

/**
 * 判断队列是否为满.
 */
#define apr_queue_full(queue) ((queue)->nelts == (queue)->bounds)

 /**
  * 判断队列是否为空.
  */
#define apr_queue_empty(queue) ((queue)->nelts == 0)

/**
 * 初始化队列
 */
APR_DECLARE(apr_status_t) apr_queue_create(apr_queue_t **q,
	unsigned int queue_capacity)
{
	apr_status_t rv;
	apr_queue_t *queue;
	queue = apr_zalloc(sizeof(apr_queue_t));
	*q = queue;

	rv = apr_thread_mutex_create(&queue->one_big_mutex);
	if (rv != APR_SUCCESS) {
		return rv;
	}

	rv = apr_thread_cond_create(&queue->not_empty);
	if (rv != APR_SUCCESS) {
		return rv;
	}

	rv = apr_thread_cond_create(&queue->not_full);
	if (rv != APR_SUCCESS) {
		return rv;
	}

	/* 所有元素初始化为空 */
	queue->data = apr_zalloc(queue_capacity * sizeof(void*));
	queue->bounds = queue_capacity;
	queue->nelts = 0;
	queue->in = 0;
	queue->out = 0;
	queue->terminated = 0;
	queue->full_waiters = 0;
	queue->empty_waiters = 0;

	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_queue_destroy(apr_queue_t *queue)
{
	/* 忽略销毁时的错误就，就算有错误也没办法 */

	apr_thread_cond_destroy(queue->not_empty);
	apr_thread_cond_destroy(queue->not_full);
	apr_thread_mutex_destroy(queue->one_big_mutex);

	apr_free(queue);

	return APR_SUCCESS;
}


APR_DECLARE(apr_status_t) apr_queue_push(apr_queue_t *queue, void *data)
{
	apr_status_t rv;

	if (queue->terminated) {
		return APR_EOF; /* 队列已经被中止 */
	}

	rv = apr_thread_mutex_lock(queue->one_big_mutex);
	if (rv != APR_SUCCESS) {
		return rv;
	}

	if (apr_queue_full(queue)) {
		if (!queue->terminated) {
			queue->full_waiters++;
			rv = apr_thread_cond_wait(queue->not_full, queue->one_big_mutex); /* 等待不为空 */
			queue->full_waiters--;
			if (rv != APR_SUCCESS) {
				apr_thread_mutex_unlock(queue->one_big_mutex);
				return rv;
			}
		}
		/* 如果被唤醒后队列仍是满的，返回被唤醒并没有操作成功 */
		if (apr_queue_full(queue)) {
			Q_DBG("queue full (intr)", queue);
			rv = apr_thread_mutex_unlock(queue->one_big_mutex);
			if (rv != APR_SUCCESS) {
				return rv;
			}
			if (queue->terminated) {
				return APR_EOF; /* 队列已经被中止 */
			}
			else {
				return APR_EINTR;
			}
		}
	}

	queue->data[queue->in] = data;
	queue->in++;
	if (queue->in >= queue->bounds)
		queue->in -= queue->bounds;
	queue->nelts++;

	if (queue->empty_waiters) { /* 如果有阻塞读线程，通知读线程 */
		Q_DBG("sig !empty", queue);
		rv = apr_thread_cond_signal(queue->not_empty);
		if (rv != APR_SUCCESS) {
			apr_thread_mutex_unlock(queue->one_big_mutex);
			return rv;
		}
	}

	rv = apr_thread_mutex_unlock(queue->one_big_mutex);
	return rv;
}

APR_DECLARE(apr_status_t) apr_queue_trypush(apr_queue_t *queue, void *data)
{
	apr_status_t rv;

	if (queue->terminated) {
		return APR_EOF; /* 队列已经被中止 */
	}

	rv = apr_thread_mutex_lock(queue->one_big_mutex);
	if (rv != APR_SUCCESS) {
		return rv;
	}

	if (apr_queue_full(queue)) {
		rv = apr_thread_mutex_unlock(queue->one_big_mutex);
		return APR_EAGAIN; /* 重新再来一次 */
	}

	queue->data[queue->in] = data;
	queue->in++;
	if (queue->in >= queue->bounds)
		queue->in -= queue->bounds;
	queue->nelts++;

	if (queue->empty_waiters) {  /* 如果有阻塞读线程，通知读线程 */
		Q_DBG("sig !empty", queue);
		rv = apr_thread_cond_signal(queue->not_empty);
		if (rv != APR_SUCCESS) {
			apr_thread_mutex_unlock(queue->one_big_mutex);
			return rv;
		}
	}

	rv = apr_thread_mutex_unlock(queue->one_big_mutex);
	return rv;
}


APR_DECLARE(unsigned int) apr_queue_size(apr_queue_t *queue) {
	return queue->nelts;
}

APR_DECLARE(apr_status_t) apr_queue_pop(apr_queue_t *queue, void **data)
{
	apr_status_t rv;

	if (queue->terminated) {
		return APR_EOF; /* 队列已经被中止 */
	}

	rv = apr_thread_mutex_lock(queue->one_big_mutex);
	if (rv != APR_SUCCESS) {
		return rv;
	}

	/* 队列为空等待被唤醒，直到队列不为空 */
	if (apr_queue_empty(queue)) {
		if (!queue->terminated) {
			queue->empty_waiters++;
			rv = apr_thread_cond_wait(queue->not_empty, queue->one_big_mutex);
			queue->empty_waiters--;
			if (rv != APR_SUCCESS) {
				apr_thread_mutex_unlock(queue->one_big_mutex);
				return rv;
			}
		}
		/* 如果被唤醒后队列仍是空的，返回被唤醒并没有操作成功 */
		if (apr_queue_empty(queue)) {
			Q_DBG("queue empty (intr)", queue);
			rv = apr_thread_mutex_unlock(queue->one_big_mutex);
			if (rv != APR_SUCCESS) {
				return rv;
			}
			if (queue->terminated) {
				return APR_EOF; /* 队列已经被中止 */
			}
			else {
				return APR_EINTR;
			}
		}
	}

	*data = queue->data[queue->out];
	queue->nelts--;

	queue->out++;
	if (queue->out >= queue->bounds)
		queue->out -= queue->bounds;
	if (queue->full_waiters) {  /* 如果有阻塞写线程，通知写线程 */
		Q_DBG("signal !full", queue);
		rv = apr_thread_cond_signal(queue->not_full);
		if (rv != APR_SUCCESS) {
			apr_thread_mutex_unlock(queue->one_big_mutex);
			return rv;
		}
	}

	rv = apr_thread_mutex_unlock(queue->one_big_mutex);
	return rv;
}


APR_DECLARE(apr_status_t) apr_queue_trypop(apr_queue_t *queue, void **data)
{
	apr_status_t rv;

	if (queue->terminated) {
		return APR_EOF; /* 队列已经被中止 */
	}

	rv = apr_thread_mutex_lock(queue->one_big_mutex);
	if (rv != APR_SUCCESS) {
		return rv;
	}

	if (apr_queue_empty(queue)) {
		rv = apr_thread_mutex_unlock(queue->one_big_mutex);
		return APR_EAGAIN;  /* 外部在操作一次 */
	}

	*data = queue->data[queue->out];
	queue->nelts--;

	queue->out++;
	if (queue->out >= queue->bounds)
		queue->out -= queue->bounds;
	if (queue->full_waiters) { /* 如果有阻塞写线程，通知写线程 */
		Q_DBG("signal !full", queue);
		rv = apr_thread_cond_signal(queue->not_full);
		if (rv != APR_SUCCESS) {
			apr_thread_mutex_unlock(queue->one_big_mutex);
			return rv;
		}
	}

	rv = apr_thread_mutex_unlock(queue->one_big_mutex);
	return rv;
}

APR_DECLARE(apr_status_t) apr_queue_interrupt_all(apr_queue_t *queue)
{
	apr_status_t rv;
	Q_DBG("intr all", queue);
	if ((rv = apr_thread_mutex_lock(queue->one_big_mutex)) != APR_SUCCESS) {
		return rv;
	}
	apr_thread_cond_broadcast(queue->not_empty); /* 通知读线程 */
	apr_thread_cond_broadcast(queue->not_full); /* 通知写线程 */

	if ((rv = apr_thread_mutex_unlock(queue->one_big_mutex)) != APR_SUCCESS) {
		return rv;
	}

	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_queue_term(apr_queue_t *queue)
{
	apr_status_t rv;

	if ((rv = apr_thread_mutex_lock(queue->one_big_mutex)) != APR_SUCCESS) {
		return rv;
	}

	/* 
	 * 必须通过全局锁来保护来修改这个标志
	 */
	queue->terminated = 1;
	if ((rv = apr_thread_mutex_unlock(queue->one_big_mutex)) != APR_SUCCESS) {
		return rv;
	}
	return apr_queue_interrupt_all(queue);
}

#endif /* APR_HAS_THREADS */