#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "apr_allocater.h"

#define GPR_ASSERT

void* apr_malloc(size_t size) 
{
	void* p;
	if (size == 0) return NULL;
	p = malloc(size);
	if (!p) {
		abort();  /* 内存分配失败，直接退出 */
	}
	return p;
}

void* apr_zalloc(size_t size) 
{
	void* p;
	if (size == 0) return NULL;
	p = calloc(size, 1);
	if (!p) {
		abort();
	}
	return p;
}

void apr_free(void* p) 
{
	if (p) free(p);
}

void* apr_realloc(void* p, size_t size) 
{
	if ((size == 0) && (p == NULL)) return NULL;
	p = realloc(p, size);
	if (!p) {
		abort();
	}
	return p;
}

void* apr_malloc_aligned(size_t size, size_t alignment) 
{
	//GPR_ASSERT(((alignment - 1) & alignment) == 0);  // Must be power of 2.
	size_t extra = alignment - 1 + sizeof(void*);
	void* p = apr_malloc(size + extra);
	void** ret = (void**)(((uintptr_t)p + extra) & ~(alignment - 1));
	ret[-1] = p;
	return (void*)ret;
}

void apr_free_aligned(void* ptr) 
{ 
	apr_free(((void**)(ptr))[-1]); 
}




