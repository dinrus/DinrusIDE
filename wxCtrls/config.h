/////////////////////////////////////////////////////////////////////////////
// Name:        config.h
// Purpose:     wxConfig base header
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// RCS-ID:      $Id: config.h 60524 2009-05-05 22:51:44Z PC $
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CONFIG_H_BASE_
#define _WX_CONFIG_H_BASE_

#include <wxCtrls/confbase.h>

#if wxUSE_CONFIG

#if defined(__WXMSW__) && wxUSE_CONFIG_NATIVE
#    ifdef __WIN32__
#        include <wxCtrls/msw/regconf.h>
#    else
#        include <wxCtrls/msw/iniconf.h>
#    endif
#elif defined(__WXPALMOS__) && wxUSE_CONFIG_NATIVE
#    include <wxCtrls/palmos/prefconf.h>
#else
#    include <wxCtrls/fileconf.h>
#endif

#endif // wxUSE_CONFIG

#endif // _WX_CONFIG_H_BASE_
