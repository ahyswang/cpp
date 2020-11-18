#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include "apr_strings.h"
#include "apr_allocater.h"

APR_DECLARE(int) apr_strcmp(const char* a, const char* b) 
{
	int ca, cb;
	do {
		ca = *a;
		cb = *b;
		++a;
		++b;
	} while (ca == cb && ca && cb);
	return ca - cb;
}

APR_DECLARE(int) apr_stricmp(const char* a, const char* b)
{
	int ca, cb;
	do {
		ca = tolower(*a);
		cb = tolower(*b);
		++a;
		++b;
	} while (ca == cb && ca && cb);
	return ca - cb;
}

APR_DECLARE(char *) apr_strdup(const char *src)
{
	char* dst;
	size_t len;

	if (!src) {
		return NULL;
	}

	len = strlen(src) + 1;
	dst = (char *)(apr_malloc(len));

	memcpy(dst, src, len);

	return dst;
}

APR_DECLARE(char *) apr_strtok(char *str, const char *sep, char **last)
{
	char *token;
	if (!str)           /* subsequent call */
		str = *last;    /* start where we left off */
	/* skip characters in sep (will terminate at '\0') */
	while (*str && strchr(sep, *str))
		++str;

	if (!*str)          /* no more tokens */
		return NULL;

	token = str;

	/* skip valid token characters to terminate token and
	 * prepare for the next call (will terminate at '\0)
	 */
	*last = token + 1;
	while (**last && !strchr(sep, **last))
		++*last;

	if (**last) {
		**last = '\0';
		++*last;
	}

	return token;
}

APR_DECLARE(char *) apr_vasprintf(const char* fmt, va_list args)
{
	int ret;
	char buf[64];
	char *strp;
	size_t strp_buflen;

	/* Use a constant-sized buffer to determine the length. */
	ret = vsnprintf(buf, sizeof(buf), fmt, args);
	if (ret < 0) {
		return NULL;
	}

	/* Allocate a new buffer, with space for the NUL terminator. */
	strp_buflen = ret + 1;
	if (((strp = (char *)apr_malloc(strp_buflen))) == NULL) {
		/* This shouldn't happen, because gpr_malloc() calls abort(). */
		return NULL;
	}

	/* Return early if we have all the bytes. */
	if (strp_buflen <= sizeof(buf)) {
		memcpy(strp, buf, strp_buflen);
		return strp;
	}

	/* Try again using the larger buffer. */
	ret = vsnprintf(strp, strp_buflen, fmt, args);
	if (ret == strp_buflen - 1) {
		return strp;
	}

	/* This should never happen. */
	apr_free(strp);
	return NULL;
}

APR_DECLARE(char *) apr_asprintf(const char* fmt, ...)
{
	va_list args;
	int ret;
	char buf[64];
	char *strp;
	size_t strp_buflen;

	/* Use a constant-sized buffer to determine the length. */
	va_start(args, fmt);
	ret = vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
	if (ret < 0) {
		return NULL;
	}

	/* Allocate a new buffer, with space for the NUL terminator. */
	strp_buflen = ret + 1;
	if (((strp = (char *)apr_malloc(strp_buflen))) == NULL) {
		/* This shouldn't happen, because gpr_malloc() calls abort(). */
		return NULL;
	}

	/* Return early if we have all the bytes. */
	if (strp_buflen <= sizeof(buf)) {
		memcpy(strp, buf, strp_buflen);
		return strp;
	}

	/* Try again using the larger buffer. */
	va_start(args, fmt);
	ret = vsnprintf(strp, strp_buflen, fmt, args);
	va_end(args);
	if (ret == strp_buflen - 1) {
		return strp;
	}

	/* This should never happen. */
	apr_free(strp);
	return NULL;
}