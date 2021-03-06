/*
 * snprintf()
 *
 * Implement snprintf() in terms of vsnprintf()
 */

#include "compiler.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "nasmlib.h"

#ifndef HAVE_SNPRINTF
#ifndef HAVE__SNPRINTF

int snprintf(char *str, size_t size, const char *формат, ...)
{
    va_list ap;
    int rv;

    va_start(ap, формат);
    rv = 
#ifdef flagMSC
	_vsnprintf
#else
	vsnprintf
#endif
    (str, size, формат, ap);
    va_end(ap);

    return rv;
}

#endif
#endif