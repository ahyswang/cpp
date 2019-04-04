#include "apr_strmatch.h"
#include "apr_allocater.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define NUM_CHARS  256

/*
 * String searching functions
 */
static const char *match_no_op(const apr_strmatch_pattern *this_pattern,
	const char *s, apr_size_t slen)
{
	return s;
}

static const char *match_boyer_moore_horspool(
	const apr_strmatch_pattern *this_pattern,
	const char *s, apr_size_t slen)
{
	const char *s_end = s + slen;
	apr_size_t *shift = (apr_size_t *)(this_pattern->context);
	const char *s_next = s + this_pattern->length - 1;
	const char *p_start = this_pattern->pattern;
	const char *p_end = p_start + this_pattern->length - 1;
	while (s_next < s_end) {
		const char *s_tmp = s_next;
		const char *p_tmp = p_end;
		while (*s_tmp == *p_tmp) {
			p_tmp--;
			if (p_tmp < p_start) {
				return s_tmp;
			}
			s_tmp--;
		}
		s_next += shift[(int)*((const unsigned char *)s_next)];
	}
	return NULL;
}

static const char *match_boyer_moore_horspool_nocase(
	const apr_strmatch_pattern *this_pattern,
	const char *s, apr_size_t slen)
{
	const char *s_end = s + slen;
	apr_size_t *shift = (apr_size_t *)(this_pattern->context);
	const char *s_next = s + this_pattern->length - 1;
	const char *p_start = this_pattern->pattern;
	const char *p_end = p_start + this_pattern->length - 1;
	while (s_next < s_end) {
		const char *s_tmp = s_next;
		const char *p_tmp = p_end;
		while (tolower(*s_tmp) == tolower(*p_tmp)) {
			p_tmp--;
			if (p_tmp < p_start) {
				return s_tmp;
			}
			s_tmp--;
		}
		s_next += shift[(unsigned char)tolower(*s_next)];
	}
	return NULL;
}

APR_DECLARE(const apr_strmatch_pattern *) apr_strmatch_precompile(const char *s,
	int case_sensitive)
{
	apr_strmatch_pattern *pattern;
	apr_size_t i;
	apr_size_t *shift;

	pattern = apr_zalloc(sizeof(*pattern));
	pattern->pattern = s;
	pattern->length = strlen(s);
	if (pattern->length == 0) {
		pattern->compare = match_no_op;
		pattern->context = NULL;
		return pattern;
	}

	shift = (apr_size_t *)apr_zalloc(sizeof(apr_size_t) * NUM_CHARS);
	for (i = 0; i < NUM_CHARS; i++) {
		shift[i] = pattern->length;
	}
	if (case_sensitive) {
		pattern->compare = match_boyer_moore_horspool;
		for (i = 0; i < pattern->length - 1; i++) {
			shift[(unsigned char)s[i]] = pattern->length - i - 1;
		}
	}
	else {
		pattern->compare = match_boyer_moore_horspool_nocase;
		for (i = 0; i < pattern->length - 1; i++) {
			shift[(unsigned char)tolower(s[i])] = pattern->length - i - 1;
		}
	}
	pattern->context = shift;

	return pattern;
}

APR_DECLARE(void) apr_strmatch_destory(const apr_strmatch_pattern *pattern)
{
	if (pattern) {
		if (pattern->context) {
			apr_free(pattern->context);
		}
		apr_free(pattern);
	}
}