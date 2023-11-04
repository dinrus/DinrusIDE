///////////////////////////////////////////////////////////////////////////////
// Name:        src/palmos/fontutil.cpp
// Purpose:     font-related helper functions
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: fontutil.cpp 35650 2005-09-23 12:56:45Z MR $
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

#ifndef WX_PRECOMP
    #include <wxCtrls/string.h>
    #include <wxCtrls/log.h>
    #include <wxCtrls/intl.h>
    #include <wxCtrls/encinfo.h>
#endif //WX_PRECOMP

#include <wxCtrls/fontutil.h>
#include <wxCtrls/fontmap.h>

#include <wxCtrls/tokenzr.h>

#ifndef HANGUL_CHARSET
#    define HANGUL_CHARSET  129
#endif

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxNativeEncodingInfo
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// helper functions
// ----------------------------------------------------------------------------

bool wxGetNativeFontEncoding(wxFontEncoding encoding,
                             wxNativeEncodingInfo *info)
{
    return false;
}

bool wxTestFontEncoding(const wxNativeEncodingInfo& info)
{
    return false;
}

// ----------------------------------------------------------------------------
// wxFontEncoding <-> CHARSET_XXX
// ----------------------------------------------------------------------------

wxFontEncoding wxGetFontEncFromCharSet(int cs)
{
    return wxFONTENCODING_SYSTEM;
}
