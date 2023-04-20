/////////////////////////////////////////////////////////////////////////////
// Name:        wx/dcclient.h
// Purpose:     wxClientDC base header
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// RCS-ID:      $Id: dcclient.h 40865 2006-08-27 09:42:42Z VS $
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCCLIENT_H_BASE_
#define _WX_DCCLIENT_H_BASE_

#include <wxCtrls/defs.h>

#if defined(__WXPALMOS__)
#include <wxCtrls/palmos/dcclient.h>
#elif defined(__WXMSW__)
#include <wxCtrls/msw/dcclient.h>
#elif defined(__WXMOTIF__)
#include <wxCtrls/motif/dcclient.h>
#elif defined(__WXGTK20__)
#include <wxCtrls/gtk/dcclient.h>
#elif defined(__WXGTK__)
#include <wxCtrls/gtk1/dcclient.h>
#elif defined(__WXX11__)
#include <wxCtrls/x11/dcclient.h>
#elif defined(__WXMGL__)
#include <wxCtrls/mgl/dcclient.h>
#elif defined(__WXDFB__)
#include <wxCtrls/dfb/dcclient.h>
#elif defined(__WXMAC__)
#include <wxCtrls/mac/dcclient.h>
#elif defined(__WXCOCOA__)
#include <wxCtrls/cocoa/dcclient.h>
#elif defined(__WXPM__)
#include <wxCtrls/os2/dcclient.h>
#endif

#endif
    // _WX_DCCLIENT_H_BASE_
