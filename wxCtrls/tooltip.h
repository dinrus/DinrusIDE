/////////////////////////////////////////////////////////////////////////////
// Name:        tooltip.h
// Purpose:     wxToolTip base header
// Author:      Robert Roebling
// Modified by:
// Created:
// Copyright:   (c) Robert Roebling
// RCS-ID:      $Id: tooltip.h 37066 2006-01-23 03:27:34Z MR $
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_TOOLTIP_H_BASE_
#define _WX_TOOLTIP_H_BASE_

#include <wxCtrls/defs.h>

#if wxUSE_TOOLTIPS

#if defined(__WXMSW__)
#include <wxCtrls/msw/tooltip.h>
#elif defined(__WXMOTIF__)
// #include <wxCtrls/motif/tooltip.h>
#elif defined(__WXGTK20__)
#include <wxCtrls/gtk/tooltip.h>
#elif defined(__WXGTK__)
#include <wxCtrls/gtk1/tooltip.h>
#elif defined(__WXMAC__)
#include <wxCtrls/mac/tooltip.h>
#elif defined(__WXCOCOA__)
#include <wxCtrls/cocoa/tooltip.h>
#elif defined(__WXPM__)
#include <wxCtrls/os2/tooltip.h>
#endif

#endif
    // wxUSE_TOOLTIPS

#endif
    // _WX_TOOLTIP_H_BASE_
