/////////////////////////////////////////////////////////////////////////////
// Name:        src/palmos/fontdlg.cpp
// Purpose:     wxFontDialog class
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: fontdlg.cpp 39337 2006-05-25 21:08:11Z ABX $
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h>.
#include <wxCtrls/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_FONTDLG

#include <wxCtrls/fontdlg.h>

#ifndef WX_PRECOMP
    #include <wxCtrls/utils.h>
    #include <wxCtrls/dialog.h>
    #include <wxCtrls/math.h>
    #include <wxCtrls/log.h>
    #include <wxCtrls/cmndata.h>
#endif

#include <wxCtrls/palmos/private.h>

#include <stdlib.h>
#include <string.h>

// ----------------------------------------------------------------------------
// wxWin macros
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxFontDialog, wxDialog)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxFontDialog
// ----------------------------------------------------------------------------

int wxFontDialog::ShowModal()
{
    return wxID_CANCEL;
}

#endif // wxUSE_FONTDLG
