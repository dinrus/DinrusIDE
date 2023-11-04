/////////////////////////////////////////////////////////////////////////////
// Name:        src/palmos/cursor.cpp
// Purpose:     wxCursor class
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: cursor.cpp 42752 2006-10-30 19:26:48Z VZ $
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

#include <wxCtrls/cursor.h>

#ifndef WX_PRECOMP
    #include <wxCtrls/utils.h>
    #include <wxCtrls/app.h>
    #include <wxCtrls/bitmap.h>
    #include <wxCtrls/icon.h>
    #include <wxCtrls/settings.h>
    #include <wxCtrls/intl.h>
    #include <wxCtrls/image.h>
    #include <wxCtrls/module.h>
#endif

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// wxWin macros
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxCursor, wxGDIObject)

// ----------------------------------------------------------------------------
// globals
// ----------------------------------------------------------------------------

// Current cursor, in order to hang on to cursor handle when setting the cursor
// globally
static wxCursor *gs_globalCursor = NULL;

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------


// ============================================================================
// implementation
// ============================================================================


// ----------------------------------------------------------------------------
// Cursors
// ----------------------------------------------------------------------------

wxCursor::wxCursor()
{
}

#if wxUSE_IMAGE
wxCursor::wxCursor(const wxImage& image)
{
}
#endif

wxCursor::wxCursor(const char WXUNUSED(bits)[],
                   int WXUNUSED(width),
                   int WXUNUSED(height),
                   int WXUNUSED(hotSpotX), int WXUNUSED(hotSpotY),
                   const char WXUNUSED(maskBits)[])
{
}

wxCursor::wxCursor(const wxString& filename,
                   long kind,
                   int hotSpotX,
                   int hotSpotY)
{
}

// Cursors by stock number
wxCursor::wxCursor(int idCursor)
{
}

wxCursor::~wxCursor()
{
}

// ----------------------------------------------------------------------------
// other wxCursor functions
// ----------------------------------------------------------------------------

wxGDIImageRefData *wxCursor::CreateData() const
{
    return NULL;
}

// ----------------------------------------------------------------------------
// Global cursor setting
// ----------------------------------------------------------------------------

const wxCursor *wxGetGlobalCursor()
{
    return NULL;
}

void wxSetCursor(const wxCursor& cursor)
{
}
