#include "apr_time.h"

#ifdef APR_HAS_TIME_WIN32

#include <Windows.h>

/* Number of micro-seconds between the beginning of the Windows epoch
 * (Jan. 1, 1601) and the Unix epoch (Jan. 1, 1970)
 */
#define APR_DELTA_EPOCH_IN_USEC   (uint64_t)(11644473600000000)

static void FileTimeToAprTime(uint64_t *result, FILETIME *input)
{
	/* Convert FILETIME one 64 bit number so we can work with it. */
	*result = input->dwHighDateTime;
	*result = (*result) << 32;
	*result |= input->dwLowDateTime;
	*result /= 10;    /* Convert from 100 nano-sec periods to micro-seconds. */
	*result -= APR_DELTA_EPOCH_IN_USEC;  /* Convert from Windows epoch to Unix epoch */
	return;
}

APR_DECLARE(uint64_t) apr_time_now(void)
{
	LONGLONG aprtime = 0;
	FILETIME time;
	SYSTEMTIME st;
	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &time);
	FileTimeToAprTime(&aprtime, &time);
	return aprtime;
}


APR_DECLARE(void) apr_sleep(uint64_t t)
{
	/* One of the few sane situations for a cast, Sleep
	 * is in ms, not us, and passed as a DWORD value
	 */
	Sleep((DWORD)(t / 1000));
}

#endif /* APR_HAS_TIME_LINUX */