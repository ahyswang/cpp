// Copyright (C) 2004  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.

#ifndef DLIB_MISC_APi_
#define DLIB_MISC_APi_

#include "platform.h"

#ifdef WIN32
#include "./misc_api/misc_api_kernel_1.h"
#endif

#ifndef WIN32
#include "./misc_api/misc_api_kernel_2.h"
#endif

#endif // DLIB_MISC_APi_

