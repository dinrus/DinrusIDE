///////////////////////////////////////////////////////////////////////////////
// Name:        src/palmos/fontenum.cpp
// Purpose:     wxFontEnumerator class for Palm OS
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: fontenum.cpp 39464 2006-05-29 21:45:52Z ABX $
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

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

#include <wxCtrls/fontenum.h>

#ifndef WX_PRECOMP
    #include <wxCtrls/font.h>
    #include <wxCtrls/encinfo.h>
#endif

#include <wxCtrls/fontutil.h>
#include <wxCtrls/fontmap.h>

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxFontEnumerator
// ----------------------------------------------------------------------------

bool wxFontEnumerator::EnumerateFacenames(wxFontEncoding encoding,
                                          bool fixedWidthOnly)
{
    return false;
}

bool wxFontEnumerator::EnumerateEncodings(const wxString& family)
{
    return false;
}
