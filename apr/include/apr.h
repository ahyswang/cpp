#ifndef APR_H
#define APR_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* Typedefs that APR needs. */

typedef  unsigned char     apr_byte_t;

typedef  short             apr_int16_t;
typedef  unsigned short    apr_uint16_t;

typedef  int               apr_int32_t;
typedef  unsigned int      apr_uint32_t;

typedef  long long          apr_int64_t;
typedef  unsigned long long apr_uint64_t;

typedef  size_t      apr_size_t;

#ifdef _WIN64
#define APR_SIZEOF_VOIDP   8
#else
#define APR_SIZEOF_VOIDP   4
#endif

#if APR_SIZEOF_VOIDP == 8
typedef  apr_uint64_t            apr_uintptr_t;
#else
typedef  apr_uint32_t            apr_uintptr_t;
#endif

/* Are we big endian? */
/* XXX: Fatal assumption on Alpha platforms */
#define APR_IS_BIGENDIAN	0

#define APR_HAS_THREADS 1
#define APR_HAS_DSO 1
#define APR_HAS_LOG 1

#ifdef WIN32
#define APR_HAS_THREAD_WIN32 1
#define APR_HAS_DSO_WIN32 1
#define APR_HAS_TIME_WIN32 1
#define APR_HAS_LOG_WIN32 1
#else 
#define APR_HAS_THREAD_LINUX 1
#define APR_HAS_DSO_LINUX 1
#define APR_HAS_TIME_LINUX 1
#define APR_HAS_LOG_LINUX 1
#endif 

#define apr_status_t int

#define APR_DECLARE(type) type

#ifdef APR_HAS_ASSERT
#define APR_ASSERT(x) assert(x)
#else
#define APR_ASSERT(x)
#endif

#endif /* APR_H */