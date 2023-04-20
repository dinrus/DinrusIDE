/////////////////////////////////////////////////////////////////////////////
// Name:        wx/brush.h
// Purpose:     Includes platform-specific wxBrush file
// Author:      Julian Smart
// Modified by:
// Created:
// RCS-ID:      $Id: brush.h 40865 2006-08-27 09:42:42Z VS $
// Copyright:   Julian Smart
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_BRUSH_H_BASE_
#define _WX_BRUSH_H_BASE_

#include <wxCtrls/defs.h>
#include <wxCtrls/object.h>
#include <wxCtrls/gdiobj.h>

// wxBrushBase
class WXDLLEXPORT wxBrushBase: public wxGDIObject
{
public:
    virtual ~wxBrushBase() { }

    virtual int GetStyle() const = 0;

    virtual bool IsHatch() const
        { return (GetStyle()>=wxFIRST_HATCH) && (GetStyle()<=wxLAST_HATCH); }
};

#if defined(__WXPALMOS__)
    #include <wxCtrls/palmos/brush.h>
#elif defined(__WXMSW__)
    #include <wxCtrls/msw/brush.h>
#elif defined(__WXMOTIF__) || defined(__WXX11__)
    #include <wxCtrls/x11/brush.h>
#elif defined(__WXGTK20__)
    #include <wxCtrls/gtk/brush.h>
#elif defined(__WXGTK__)
    #include <wxCtrls/gtk1/brush.h>
#elif defined(__WXMGL__)
    #include <wxCtrls/mgl/brush.h>
#elif defined(__WXDFB__)
    #include <wxCtrls/dfb/brush.h>
#elif defined(__WXMAC__)
    #include <wxCtrls/mac/brush.h>
#elif defined(__WXCOCOA__)
    #include <wxCtrls/cocoa/brush.h>
#elif defined(__WXPM__)
    #include <wxCtrls/os2/brush.h>
#endif

#endif
    // _WX_BRUSH_H_BASE_
