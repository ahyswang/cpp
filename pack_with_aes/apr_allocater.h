#ifndef APR_ALLOCATER_H
#define APR_ALLOCATER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	/**
	 * 创建内存
	 *
	 * @param size 内存字节大小
	 * @returns 成功返回地址，失败返回NULL
	 */
	void* apr_malloc(size_t size);

	/**
	 * 创建内存并被初始化为0
	 *
	 * @param size 内存字节大小
	 * @returns 成功返回地址，失败返回NULL
	 */
	void* apr_zalloc(size_t size);

	/**
	 * 释放内存
	 *
	 * @param ptr 内存地址
	 */
	void apr_free(void* ptr);

	/**
	 * 重新分配内存并且数据保留
	 * 原来的地址扩展内存成功会返回原来的地址，否则开辟新的内存的空间并把
	 * 数据拷贝到新的内存空间返回新的内存地址。
	 * @param p	原来的内存地址
	 * @param size 内存字节大小
	 * @returns 成功返回地址，失败返回NULL
	 */
	void* apr_realloc(void* p, size_t size);

	/**
	 * 创建字节对齐的内存
	 *
	 * @param size 内存字节大小
	 * @param alignment 字节对齐大小
	 * @returns 成功返回地址，失败返回NULL
	 */
	void* apr_malloc_aligned(size_t size, size_t alignment);

	/**
	 * 释放字节对齐的内存
	 *
	 * @param ptr 内存地址
	 */
	void apr_free_aligned(void* ptr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* APR_ALLOCATER_H */
