#ifndef APR_TIME_H
#define APR_TIME_H

#include "apr.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	/**
	 * 获取当前的时间。
	 * 返回自纪元 Epoch（1970-01-01 00:00:00 UTC）起经过的时间，以微秒为单位。
	 * @return 返回当前时间（微秒）
	 */
	APR_DECLARE(uint64_t) apr_time_now(void);

	/**
	 * 休眠指定的时间（微秒）。
	 * @param t 休眠时间
	 */
	APR_DECLARE(void) apr_sleep(uint64_t t);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* APR_TIME_H */