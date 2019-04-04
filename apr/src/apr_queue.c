#include <stdio.h>
#include <stdlib.h>

#include "apr.h"
#include "apr_allocater.h"
#include "apr_errno.h"
#include "apr_thread.h"
#include "apr_queue.h"

#if APR_HAS_THREADS
/*
 * ����������Ϣ
 */
//#define QUEUE_DEBUG

struct apr_queue_t {
	void              **data;
	unsigned int        nelts; /**< ��ǰԪ�ظ��� */ 
	unsigned int        in;    /**< ��һ���յ�λ�� */ 
	unsigned int        out;   /**< ��һ���е�λ�� */  
	unsigned int        bounds;/**< ����������� */  
	unsigned int        full_waiters; /**< д��ȴ��߳��� */
	unsigned int        empty_waiters; /**< ��ȡ�ȴ��߳��� */
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
 * �ж϶����Ƿ�Ϊ��.
 */
#define apr_queue_full(queue) ((queue)->nelts == (queue)->bounds)

 /**
  * �ж϶����Ƿ�Ϊ��.
  */
#define apr_queue_empty(queue) ((queue)->nelts == 0)

/**
 * ��ʼ������
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

	/* ����Ԫ�س�ʼ��Ϊ�� */
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
	/* ��������ʱ�Ĵ���ͣ������д���Ҳû�취 */

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
		return APR_EOF; /* �����Ѿ�����ֹ */
	}

	rv = apr_thread_mutex_lock(queue->one_big_mutex);
	if (rv != APR_SUCCESS) {
		return rv;
	}

	if (apr_queue_full(queue)) {
		if (!queue->terminated) {
			queue->full_waiters++;
			rv = apr_thread_cond_wait(queue->not_full, queue->one_big_mutex); /* �ȴ���Ϊ�� */
			queue->full_waiters--;
			if (rv != APR_SUCCESS) {
				apr_thread_mutex_unlock(queue->one_big_mutex);
				return rv;
			}
		}
		/* ��������Ѻ�����������ģ����ر����Ѳ�û�в����ɹ� */
		if (apr_queue_full(queue)) {
			Q_DBG("queue full (intr)", queue);
			rv = apr_thread_mutex_unlock(queue->one_big_mutex);
			if (rv != APR_SUCCESS) {
				return rv;
			}
			if (queue->terminated) {
				return APR_EOF; /* �����Ѿ�����ֹ */
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

	if (queue->empty_waiters) { /* ������������̣߳�֪ͨ���߳� */
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
		return APR_EOF; /* �����Ѿ�����ֹ */
	}

	rv = apr_thread_mutex_lock(queue->one_big_mutex);
	if (rv != APR_SUCCESS) {
		return rv;
	}

	if (apr_queue_full(queue)) {
		rv = apr_thread_mutex_unlock(queue->one_big_mutex);
		return APR_EAGAIN; /* ��������һ�� */
	}

	queue->data[queue->in] = data;
	queue->in++;
	if (queue->in >= queue->bounds)
		queue->in -= queue->bounds;
	queue->nelts++;

	if (queue->empty_waiters) {  /* ������������̣߳�֪ͨ���߳� */
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
		return APR_EOF; /* �����Ѿ�����ֹ */
	}

	rv = apr_thread_mutex_lock(queue->one_big_mutex);
	if (rv != APR_SUCCESS) {
		return rv;
	}

	/* ����Ϊ�յȴ������ѣ�ֱ�����в�Ϊ�� */
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
		/* ��������Ѻ�������ǿյģ����ر����Ѳ�û�в����ɹ� */
		if (apr_queue_empty(queue)) {
			Q_DBG("queue empty (intr)", queue);
			rv = apr_thread_mutex_unlock(queue->one_big_mutex);
			if (rv != APR_SUCCESS) {
				return rv;
			}
			if (queue->terminated) {
				return APR_EOF; /* �����Ѿ�����ֹ */
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
	if (queue->full_waiters) {  /* ���������д�̣߳�֪ͨд�߳� */
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
		return APR_EOF; /* �����Ѿ�����ֹ */
	}

	rv = apr_thread_mutex_lock(queue->one_big_mutex);
	if (rv != APR_SUCCESS) {
		return rv;
	}

	if (apr_queue_empty(queue)) {
		rv = apr_thread_mutex_unlock(queue->one_big_mutex);
		return APR_EAGAIN;  /* �ⲿ�ڲ���һ�� */
	}

	*data = queue->data[queue->out];
	queue->nelts--;

	queue->out++;
	if (queue->out >= queue->bounds)
		queue->out -= queue->bounds;
	if (queue->full_waiters) { /* ���������д�̣߳�֪ͨд�߳� */
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
	apr_thread_cond_broadcast(queue->not_empty); /* ֪ͨ���߳� */
	apr_thread_cond_broadcast(queue->not_full); /* ֪ͨд�߳� */

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
	 * ����ͨ��ȫ�������������޸������־
	 */
	queue->terminated = 1;
	if ((rv = apr_thread_mutex_unlock(queue->one_big_mutex)) != APR_SUCCESS) {
		return rv;
	}
	return apr_queue_interrupt_all(queue);
}

#endif /* APR_HAS_THREADS */