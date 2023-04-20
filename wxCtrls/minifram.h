/////////////////////////////////////////////////////////////////////////////
// Name:        wx/minifram.h
// Purpose:     wxMiniFrame base header
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// RCS-ID:      $Id: minifram.h 49299 2007-10-21 18:07:29Z PC $
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MINIFRAM_H_BASE_
#define _WX_MINIFRAM_H_BASE_

#include <wxCtrls/defs.h>

#if wxUSE_MINIFRAME

#if defined(__WXPALMOS__)
#include <wxCtrls/palmos/minifram.h>
#elif defined(__WXMSW__)
#include <wxCtrls/msw/minifram.h>
#elif defined(__WXMOTIF__)
#include <wxCtrls/motif/minifram.h>
#elif defined(__WXGTK20__)
#include <wxCtrls/gtk/minifram.h>
#elif defined(__WXGTK__)
#include <wxCtrls/gtk1/minifram.h>
#elif defined(__WXX11__)
#include <wxCtrls/x11/minifram.h>
#elif defined(__WXMAC__)
#include <wxCtrls/mac/minifram.h>
#elif defined(__WXPM__)
#include <wxCtrls/os2/minifram.h>
#else
// TODO: it seems that wxMiniFrame could be just defined here generically
//       instead of having all the above port-specific headers
#include <wxCtrls/frame.h>
typedef wxFrame wxMiniFrame;
#endif

#endif // wxUSE_MINIFRAME
#endif // _WX_MINIFRAM_H_BASE_
