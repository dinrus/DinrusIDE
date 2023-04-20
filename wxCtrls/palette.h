/////////////////////////////////////////////////////////////////////////////
// Name:        wx/palette.h
// Purpose:     Common header and base class for wxPalette
// Author:      Julian Smart
// Modified by:
// Created:
// RCS-ID:      $Id: palette.h 61872 2009-09-09 22:37:05Z VZ $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PALETTE_H_BASE_
#define _WX_PALETTE_H_BASE_

#include <wxCtrls/defs.h>

#if wxUSE_PALETTE

#include <wxCtrls/object.h>
#include <wxCtrls/gdiobj.h>

// wxPaletteBase
class WXDLLEXPORT wxPaletteBase: public wxGDIObject
{
public:
    virtual ~wxPaletteBase() { }

    virtual bool Ok() const { return IsOk(); }
    virtual bool IsOk() const = 0;
    virtual int GetColoursCount() const { wxFAIL_MSG( wxT("not implemented") ); return 0; }
};

#if defined(__WXPALMOS__)
    #include <wxCtrls/palmos/palette.h>
#elif defined(__WXMSW__)
    #include <wxCtrls/msw/palette.h>
#elif defined(__WXMOTIF__)
    #include <wxCtrls/motif/palette.h>
#elif defined(__WXGTK__) || defined(__WXCOCOA__)
    #include <wxCtrls/generic/paletteg.h>
#elif defined(__WXX11__)
    #include <wxCtrls/x11/palette.h>
#elif defined(__WXMGL__)
    #include <wxCtrls/mgl/palette.h>
#elif defined(__WXMAC__)
    #include <wxCtrls/mac/palette.h>
#elif defined(__WXPM__)
    #include <wxCtrls/os2/palette.h>
#endif

#if WXWIN_COMPATIBILITY_2_4
    #define wxColorMap wxPalette
    #define wxColourMap wxPalette
#endif

#endif // wxUSE_PALETTE

#endif
    // _WX_PALETTE_H_BASE_
