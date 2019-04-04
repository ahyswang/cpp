#include "apr_log.h"

#ifdef APR_HAS_LOG_WIN32

#include <stdarg.h>
#include <time.h>

#include "apr_allocater.h"
#include "apr_strings.h"

const char* log_level_str(int level)
{
	if (level == APR_LOG_LEVEL_DEBUG) {
		return "D";
	}
	else if (level == APR_LOG_LEVEL_INFO) {
		return "I";
	}
	else if (level == APR_LOG_LEVEL_ERROR) {
		return "E";
	}
	else{
		return "O";
	}
}

static int g_level = 0;

APR_DECLARE(void) apr_log_set_level(int level)
{
	g_level = level;
}

APR_DECLARE(void) apr_log(const char *file, int line, int level, const char *format, ...)
{
	char *message = NULL;
	char time_buffer[64];
	struct tm tm;
	time_t timer = time(NULL);
	va_list args;

	if (level < g_level){
		return;
	}

	va_start(args, format);
	message = apr_vasprintf(format, args);
	if (message == NULL) {
		va_end(args);
		return;
	}
	va_end(args);
	
	if (localtime_s(&tm, &timer)) {  //win32
		strcpy(time_buffer, "error:localtime");
	}
	else if (strftime(time_buffer, sizeof(time_buffer), "%m%d %H:%M:%S", &tm) == 0) {
		strcpy(time_buffer, "error:strftime");
	}

	fprintf(stderr, "[%s%s %5lu %s:%d] %s\n",
		log_level_str(level), time_buffer, GetCurrentThreadId(), file, line, message);
	fflush(stderr);

	apr_free(message);
}

#endif /* APR_HAS_LOG_WIN32 */