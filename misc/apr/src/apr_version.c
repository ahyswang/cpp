#include "apr_version.h"

APR_DECLARE(void) apr_version(apr_version_t *pvsn)
{
	pvsn->major = APR_MAJOR_VERSION;
	pvsn->minor = APR_MINOR_VERSION;
	pvsn->patch = APR_PATCH_VERSION;
#ifdef APR_IS_DEV_VERSION
	pvsn->is_dev = 1;
#else
	pvsn->is_dev = 0;
#endif
}

APR_DECLARE(const char *) apr_version_string(void)
{
	return APR_VERSION_STRING;
}