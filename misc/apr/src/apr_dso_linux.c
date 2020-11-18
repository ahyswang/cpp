#include "apr_dso.h"
#include "apr_allocater.h"
#include "apr_errno.h"

#ifdef APR_HAS_DSO_LINUX

#include <dlfcn.h>

struct apr_dso_handle_t {
	void          *handle;
};

APR_DECLARE(apr_status_t) apr_dso_load(apr_dso_handle_t **dso,
	const char *path)
{
	apr_dso_handle_t *new_dso;
	
	void *os_handle = dlopen((char *)path, RTLD_NOW | RTLD_GLOBAL);
	if (os_handle == NULL) {
		return APR_EDSOOPEN;
	}
	
	new_dso = apr_zalloc(sizeof(apr_dso_handle_t));
	if (new_dso == NULL) {
		return APR_ENOMEM;
	}

	new_dso->handle = (void*)os_handle;

	*dso = new_dso;

	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_dso_unload(apr_dso_handle_t *dso)
{
	if (dlclose(dso->handle) != 0)
		return APR_EINIT;
	apr_free(dso);
	return APR_SUCCESS;
}

APR_DECLARE(apr_status_t) apr_dso_sym(apr_dso_handle_sym_t *ressym,
	apr_dso_handle_t *dso,
	const char *symname)
{
	void *retval = dlsym(dso->handle, (char *)symname);
	if (retval == NULL) {
		return APR_ESYMNOTFOUND;
	}
	*ressym = retval;
	return APR_SUCCESS;
}

#endif /* APR_HAS_DSO_LINUX */