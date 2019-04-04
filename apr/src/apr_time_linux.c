#include "apr_time.h"

#ifdef APR_HAS_TIME_LINUX

#include <sys/time.h>
#include <unistd.h>
#include <time.h>


APR_DECLARE(uint64_t) apr_time_now(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}


APR_DECLARE(void) apr_sleep(uint64_t t)
{
	struct timeval tv;
	tv.tv_usec = t % 1000000;
	tv.tv_sec = t / 1000000;
	select(0, NULL, NULL, NULL, &tv);
}

#endif /* APR_HAS_TIME_LINUX */