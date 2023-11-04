/////////////////////////////////////////////////////////////////////////////
// Name:        src/palmos/filedlg.cpp
// Purpose:     wxFileDialog
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: filedlg.cpp 39651 2006-06-09 17:50:46Z ABX $
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

#if wxUSE_FILEDLG && !defined(__SMARTPHONE__)

#include <wxCtrls/filedlg.h>

#ifndef WX_PRECOMP
    #include <wxCtrls/utils.h>
    #include <wxCtrls/msgdlg.h>
    #include <wxCtrls/filefn.h>
    #include <wxCtrls/intl.h>
    #include <wxCtrls/log.h>
    #include <wxCtrls/app.h>
    #include <wxCtrls/math.h>
#endif

#include <wxCtrls/palmos/private.h>

#include <stdlib.h>
#include <string.h>

#include <wxCtrls/filename.h>
#include <wxCtrls/tokenzr.h>

#ifndef OFN_EXPLORER
    #define OFN_EXPLORER 0x00080000
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

#define wxMAXPATH   1024

# define wxMAXFILE   1024

# define wxMAXEXT    5

// ============================================================================
// implementation
// ============================================================================

IMPLEMENT_CLASS(wxFileDialog, wxFileDialogBase)

// ----------------------------------------------------------------------------
// wxFileDialog
// ----------------------------------------------------------------------------

wxFileDialog::wxFileDialog(wxWindow *parent,
                           const wxString& message,
                           const wxString& defaultDir,
                           const wxString& defaultFileName,
                           const wxString& wildCard,
                           long style,
                           const wxPoint& pos,
                           const wxSize& sz,
                           const wxString& name)
             :wxFileDialogBase(parent, message, defaultDir, defaultFileName, wildCard, style, pos, sz, name)

{
}

void wxFileDialog::GetPaths(wxArrayString& paths) const
{
}

void wxFileDialog::GetFilenames(wxArrayString& files) const
{
}

void wxFileDialog::SetPath(const wxString& path)
{
}

int wxFileDialog::ShowModal()
{
    return wxID_CANCEL;
}

#endif // wxUSE_FILEDLG
