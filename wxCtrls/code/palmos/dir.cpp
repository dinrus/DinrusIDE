/////////////////////////////////////////////////////////////////////////////
// Name:        src/palmos/dir.cpp
// Purpose:     wxDir implementation for PalmOS
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10.13.04
// RCS-ID:      $Id: dir.cpp 35650 2005-09-23 12:56:45Z MR $
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

#ifndef WX_PRECOMP
    #include <wxCtrls/intl.h>
    #include <wxCtrls/log.h>
#endif // PCH

#include <wxCtrls/dir.h>
#include <wxCtrls/filefn.h>          // for wxDirExists()

// ----------------------------------------------------------------------------
// define the types and functions used for file searching
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

#ifndef MAX_PATH
    #define MAX_PATH 260        // from VC++ headers
#endif

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#define M_DIR       ((wxDirData *)m_data)

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxDir helpers
// ----------------------------------------------------------------------------

/* static */
bool wxDir::Exists(const wxString& dir)
{
    return false;
}

// ----------------------------------------------------------------------------
// wxDir construction/destruction
// ----------------------------------------------------------------------------

wxDir::wxDir(const wxString& dirname)
{
}

bool wxDir::Open(const wxString& dirname)
{
    return false;
}

bool wxDir::IsOpened() const
{
    return false;
}

wxString wxDir::GetName() const
{
    wxString name;

    return name;
}

wxDir::~wxDir()
{
}

// ----------------------------------------------------------------------------
// wxDir enumerating
// ----------------------------------------------------------------------------

bool wxDir::GetFirst(wxString *filename,
                     const wxString& filespec,
                     int flags) const
{
    return false;
}

bool wxDir::GetNext(wxString *filename) const
{
    return false;
}

