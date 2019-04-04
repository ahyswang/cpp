#include "apr_dso.h"
#include "apr_allocater.h"
#include "apr_errno.h"

#ifdef APR_HAS_DSO_WIN32

#include <Windows.h>

struct apr_dso_handle_t {
	void          *handle;
};

APR_DECLARE(apr_status_t) apr_dso_load(struct apr_dso_handle_t **res_handle,
	const char *path)
{
	HINSTANCE os_handle;
	apr_status_t rv;

	os_handle = LoadLibraryEx(path, NULL, 0);
	if (!os_handle)
		rv = APR_EDSOOPEN;
	else
		rv = APR_SUCCESS;

	*res_handle = apr_zalloc(sizeof(**res_handle));
	if (rv) {
		return rv;
	}

	(*res_handle)->handle = (void*)os_handle;

	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_dso_sym(apr_dso_handle_sym_t *ressym,
	struct apr_dso_handle_t *handle,
	const char *symname)
{
	*ressym = (apr_dso_handle_sym_t)GetProcAddress(handle->handle, symname);
	if (!*ressym) {
		return APR_ESYMNOTFOUND;
	}
	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_dso_unload(struct apr_dso_handle_t *handle)
{
	if (handle->handle != NULL && !FreeLibrary(handle->handle)) {
		return -1;
	}
	apr_free(handle);
	return APR_SUCCESS;
}

#endif /* APR_HAS_DSO_WIN32 */