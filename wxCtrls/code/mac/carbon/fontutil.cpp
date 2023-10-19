///////////////////////////////////////////////////////////////////////////////
// Name:        msw/fontutil.cpp
// Purpose:     font-related helper functions for wxMSW
// Author:      Vadim Zeitlin
// Modified by:
// Created:     05.11.99
// RCS-ID:      $Id: fontutil.cpp 36879 2006-01-15 07:20:59Z vell $
// Copyright:   (c) 1999 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include <wxCtrls/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wxCtrls/string.h>
    #include <wxCtrls/log.h>
    #include <wxCtrls/intl.h>
#endif

#include <wxCtrls/fontutil.h>
#include <wxCtrls/fontmap.h>
#include <wxCtrls/encinfo.h>
#include <wxCtrls/tokenzr.h>


// convert to/from the string representation:
// format is facename[;charset]
//
bool wxNativeEncodingInfo::FromString( const wxString& s )
{
    wxStringTokenizer tokenizer(s, wxT(";"));

    facename = tokenizer.GetNextToken();
    if ( !facename )
        return false;

    wxString tmp = tokenizer.GetNextToken();
    if ( !tmp )
    {
        // default charset (don't use DEFAULT_CHARSET though because of subtle
        // Windows 9x/NT differences in handling it)
        charset = 0;
    }
    else
    {
        if ( wxSscanf( tmp, wxT("%u"), &charset ) != 1 )
            // should be a number!
            return false;
    }

    return true;
}

wxString wxNativeEncodingInfo::ToString() const
{
    wxString s(facename);
    if ( charset != 0 )
        s << wxT(';') << charset;

    return s;
}

// ----------------------------------------------------------------------------
// helper functions
// ----------------------------------------------------------------------------

bool wxGetNativeFontEncoding( wxFontEncoding encoding, wxNativeEncodingInfo *info )
{
    wxCHECK_MSG( info, false, wxT("bad pointer in wxGetNativeFontEncoding") );

    if ( encoding == wxFONTENCODING_DEFAULT )
        encoding = wxFont::GetDefaultEncoding();

    info->encoding = encoding;

    return true;
}

bool wxTestFontEncoding( const wxNativeEncodingInfo& info )
{
    // basically we should be able to support every encoding via the OS
    return true;
}
