/////////////////////////////////////////////////////////////////////////////
// Name:        src/mgl/fontutil.cpp
// Purpose:     Font helper functions for MGL
// Author:      Vaclav Slavik
// Created:     2001/04/29
// RCS-ID:      $Id: fontutil.cpp 43550 2006-11-20 20:45:57Z VS $
// Copyright:   (c) 2001-2002 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h>.
#include <wxCtrls/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wxCtrls/hash.h>
    #include <wxCtrls/log.h>
#endif

#include <wxCtrls/fontutil.h>
#include <wxCtrls/encinfo.h>
#include <wxCtrls/fontmap.h>
#include <wxCtrls/tokenzr.h>

#include <wxCtrls/listimpl.cpp>
#include <wxCtrls/sysopt.h>
#include <wxCtrls/mgl/private.h>
#include <wxCtrls/private/fontmgr.h>

#include <mgraph.h>

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxNativeEncodingInfo
// ----------------------------------------------------------------------------

// convert to/from the string representation: format is
//      encoding[;facename]
bool wxNativeEncodingInfo::FromString(const wxString& s)
{
    wxStringTokenizer tokenizer(s, _T(";"));

    wxString encid = tokenizer.GetNextToken();
    long enc;
    if ( !encid.ToLong(&enc) )
        return false;
    encoding = (wxFontEncoding)enc;

    // ok even if empty
    facename = tokenizer.GetNextToken();

    return true;
}

wxString wxNativeEncodingInfo::ToString() const
{
    wxString s;
    s << (long)encoding;
    if ( !facename.empty() )
    {
        s << _T(';') << facename;
    }

    return s;
}

// ----------------------------------------------------------------------------
// common functions
// ----------------------------------------------------------------------------

bool wxGetNativeFontEncoding(wxFontEncoding encoding,
                             wxNativeEncodingInfo *info)
{
    wxCHECK_MSG( info, false, _T("bad pointer in wxGetNativeFontEncoding") );

    if ( encoding == wxFONTENCODING_DEFAULT )
    {
        encoding = wxFont::GetDefaultEncoding();
    }

    switch ( encoding )
    {
        case wxFONTENCODING_ISO8859_1:
        case wxFONTENCODING_ISO8859_2:
        case wxFONTENCODING_ISO8859_3:
        case wxFONTENCODING_ISO8859_4:
        case wxFONTENCODING_ISO8859_5:
        case wxFONTENCODING_ISO8859_6:
        case wxFONTENCODING_ISO8859_7:
        case wxFONTENCODING_ISO8859_8:
        case wxFONTENCODING_ISO8859_9:
        case wxFONTENCODING_ISO8859_10:
        case wxFONTENCODING_ISO8859_11:
        case wxFONTENCODING_ISO8859_13:
        case wxFONTENCODING_ISO8859_14:
        case wxFONTENCODING_ISO8859_15:
            info->mglEncoding = MGL_ENCODING_ISO8859_1 +
                                (encoding - wxFONTENCODING_ISO8859_1);
            break;

        case wxFONTENCODING_KOI8:
            info->mglEncoding = MGL_ENCODING_KOI8;
            break;

        case wxFONTENCODING_CP1250:
        case wxFONTENCODING_CP1251:
        case wxFONTENCODING_CP1252:
        case wxFONTENCODING_CP1253:
        case wxFONTENCODING_CP1254:
        case wxFONTENCODING_CP1255:
        case wxFONTENCODING_CP1256:
        case wxFONTENCODING_CP1257:
            info->mglEncoding = MGL_ENCODING_CP1250 +
                                (encoding - wxFONTENCODING_CP1250);
            break;

        case wxFONTENCODING_SYSTEM:
            info->mglEncoding = MGL_ENCODING_ASCII;
            break;

        default:
            // encoding not known to MGL
            return false;
    }

    info->encoding = encoding;

    return true;
}

bool wxTestFontEncoding(const wxNativeEncodingInfo& info)
{
    if ( !info.facename )
        return true;

    wxFontBundle *bundle = wxFontsManager::Get()->GetBundle(info.facename);
    if ( !bundle )
        return false;
    if ( bundle->GetInfo()->fontLibType == MGL_BITMAPFONT_LIB )
    {
        return (info.mglEncoding == MGL_ENCODING_ASCII ||
                info.mglEncoding == MGL_ENCODING_ISO8859_1 ||
                info.mglEncoding == MGL_ENCODING_ISO8859_15 ||
                info.mglEncoding == MGL_ENCODING_CP1252);
    }
    else
        return true;
}
