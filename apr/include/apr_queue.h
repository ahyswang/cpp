#ifndef APR_QUEUE_H
#define APR_QUEUE_H

/**
 * @file apr_queue.h
 * @brief 线程安全的FIFO指定容量的队列。
 * @note 
 */
#include "apr.h"

#define APR_HAS_THREADS 1

#if APR_HAS_THREADS

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * 队列结构类型
 */
typedef struct apr_queue_t apr_queue_t;

/**
 * 创建队列（支持线程同步操作）。
 * 
 * @param queue 队列指针
 * @param queue_capacity 队列中的最大容量
 */
APR_DECLARE(apr_status_t) apr_queue_create(apr_queue_t **queue,
	unsigned int queue_capacity);

/**
 * 销毁队列
 *
 * @param queue 队列指针
 * @returns APR_SUCCESS 表示成功
 */
APR_DECLARE(apr_status_t) apr_queue_destroy(apr_queue_t *queue);

/**
 * 加入一个元素，如果队列满会阻塞等待。
 *
 * @param queue 队列指针
 * @param data 元素指针
 * @returns APR_EINTR 表示阻塞被中断，没有加入元素 (需要重试)
 * @returns APR_EOF 表示队列已经关闭
 * @returns APR_SUCCESS 表示成功
 */
APR_DECLARE(apr_status_t) apr_queue_push(apr_queue_t *queue, void *data);


/**
 * 获取一个元素，如果队列空会阻塞等待。
 *
 * @param queue 队列指针
 * @param data 元素指针
 * @returns APR_EINTR 表示阻塞被中断，没有获取到元素 (需要重试)
 * @returns APR_EOF 表示队列已经关闭
 * @returns APR_SUCCESS 表示成功
 */
APR_DECLARE(apr_status_t) apr_queue_pop(apr_queue_t *queue, void **data);

/**
 * 尝试加入一个元素，如果队列满会立即返回。
 *
 * @param queue 队列指针
 * @param data 元素指针
 * @returns APR_EINTR 表示阻塞被中断，没有获取到元素 (需要重试)
 * @returns APR_EAGAIN 表示队列满
 * @returns APR_EOF the 表示队列已经关闭
 * @returns APR_SUCCESS 表示成功
 */
APR_DECLARE(apr_status_t) apr_queue_trypush(apr_queue_t *queue, void *data);

/**
 * 尝试获取一个元素，如果队列空会立即返回。
 *
 * @param queue 队列指针
 * @param data 元素指针
 * @returns APR_EINTR 表示阻塞被中断，没有获取到元素 (需要重试)
 * @returns APR_EAGAIN 表示队列满
 * @returns APR_EOF the 表示队列已经关闭
 * @returns APR_SUCCESS 表示成功
 */
APR_DECLARE(apr_status_t) apr_queue_trypop(apr_queue_t *queue, void **data);

/**
 * 返回队列的长度。
 *
 * @warning 当前操作不是线程安全的，数据不是准确的，用于监控数据
 * @param queue 队列指针
 * @returns 返回队列的长度
 */
APR_DECLARE(unsigned int) apr_queue_size(apr_queue_t *queue);

/**
 * 中断（通知）所有阻塞线程。
 *
 * @param queue 队列指针
 */
APR_DECLARE(apr_status_t) apr_queue_interrupt_all(apr_queue_t *queue);

/**
 * 中止队列，并中断（通知）所有阻塞线程。
 *
 * @param queue 队列指针
 */
APR_DECLARE(apr_status_t) apr_queue_term(apr_queue_t *queue);

#ifdef __cplusplus
}
#endif

#endif /* APR_HAS_THREADS */

#endif /* APRQUEUE_H */