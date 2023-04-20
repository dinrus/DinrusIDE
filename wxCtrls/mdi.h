/////////////////////////////////////////////////////////////////////////////
// Name:        wx/mdi.h
// Purpose:     wxMDI base header
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// RCS-ID:      $Id: mdi.h 37066 2006-01-23 03:27:34Z MR $
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MDI_H_BASE_
#define _WX_MDI_H_BASE_

#include <wxCtrls/defs.h>

#if wxUSE_MDI

#if defined(__WXUNIVERSAL__)
    #include <wxCtrls/generic/mdig.h>
#elif defined(__WXMSW__)
    #include <wxCtrls/msw/mdi.h>
#elif defined(__WXMOTIF__)
    #include <wxCtrls/motif/mdi.h>
#elif defined(__WXGTK20__)
    #include <wxCtrls/gtk/mdi.h>
#elif defined(__WXGTK__)
    #include <wxCtrls/gtk1/mdi.h>
#elif defined(__WXMAC__)
    #include <wxCtrls/mac/mdi.h>
#elif defined(__WXCOCOA__)
    #include <wxCtrls/cocoa/mdi.h>
#elif defined(__WXPM__)
    #include <wxCtrls/generic/mdig.h>
#endif

#endif // wxUSE_MDI

#endif
    // _WX_MDI_H_BASE_
