#ifndef APR_STRMATCH_H
#define APR_STRMATCH_H
 /**
  * @file apr_strmatch.h
  * @brief APR-UTIL string matching routines
  */

#include "apr.h"

#ifdef __cplusplus
extern "C" {
#endif

	 /** @see apr_strmatch_pattern */
	typedef struct apr_strmatch_pattern apr_strmatch_pattern;

	/**
	 * Precompiled search pattern
	 */
	struct apr_strmatch_pattern {
		/** Function called to compare */
		const char *(*compare)(const apr_strmatch_pattern *this_pattern,
			const char *s, apr_size_t slen);
		const char *pattern;    /**< Current pattern */
		apr_size_t length;      /**< Current length */
		void *context;          /**< hook to add precomputed metadata */
	};

#if defined(DOXYGEN)
	/**
	 * Search for a precompiled pattern within a string
	 * @param pattern The pattern
	 * @param s The string in which to search for the pattern
	 * @param slen The length of s (excluding null terminator)
	 * @return A pointer to the first instance of the pattern in s, or
	 *         NULL if not found
	 */
	APR_DECLARE(const char *) apr_strmatch(const apr_strmatch_pattern *pattern,
		const char *s, apr_size_t slen);
#else
#define apr_strmatch(pattern, s, slen) (*((pattern)->compare))((pattern), (s), (slen))
#endif

	/**
	 * Precompile a pattern for matching using the Boyer-Moore-Horspool algorithm
	 * @param s The pattern string
	 * @param case_sensitive Whether the matching should be case-sensitive
	 * @return a pointer to the compiled pattern, or NULL if compilation fails
	 */
	APR_DECLARE(const apr_strmatch_pattern *) apr_strmatch_precompile(const char *s, int case_sensitive);


	APR_DECLARE(void) apr_strmatch_destory(const apr_strmatch_pattern *pattern);

	/** @} */
#ifdef __cplusplus
}
#endif

#endif	/* !APR_STRMATCH_H */
