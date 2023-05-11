/////////////////////////////////////////////////////////////////////////////
// Name:        src/palmos/nativdlg.cpp
// Purpose:     Native dialog loading code (part of wxWindow)
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: nativdlg.cpp 35650 2005-09-23 12:56:45Z MR $
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h>.
#include <wxCtrls/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <stdio.h>

    #include <wxCtrls/wx.h>
#endif

#include <wxCtrls/spinbutt.h>

// ---------------------------------------------------------------------------
// global functions
// ---------------------------------------------------------------------------

// ===========================================================================
// implementation
// ===========================================================================

bool wxWindow::LoadNativeDialog(wxWindow* parent, wxWindowID& id)
{
    return false;
}

bool wxWindow::LoadNativeDialog(wxWindow* parent, const wxString& name)
{
    return false;
}

// ---------------------------------------------------------------------------
// look for child by id
// ---------------------------------------------------------------------------

wxWindow* wxWindow::GetWindowChild1(wxWindowID id)
{
    return NULL;
}

wxWindow* wxWindow::GetWindowChild(wxWindowID id)
{
    return NULL;
}

// ---------------------------------------------------------------------------
// create wxWin window from a native HWND
// ---------------------------------------------------------------------------

wxWindow* wxWindow::CreateWindowFromWinHandle(wxWindow* parent, WXWINHANDLE handle)
{
    return NULL;
}

// Make sure the window style (etc.) reflects the WinHandle style (roughly)
void wxWindow::AdoptAttributesFromWinHandle(void)
{
}
