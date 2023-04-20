/////////////////////////////////////////////////////////////////////////////
// Name:        src/mac/carbon/gdiobj.cpp
// Purpose:     wxGDIObject class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: gdiobj.cpp 40943 2006-08-31 19:31:43Z ABX $
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include <wxCtrls/wxprec.h>

#include <wxCtrls/gdiobj.h>

#ifndef WX_PRECOMP
    #include <wxCtrls/gdicmn.h>
    #include <wxCtrls/module.h>
#endif

#include <wxCtrls/link.h>
#include <wxCtrls/mac/private.h>

// Linker will discard entire object file without this
wxFORCE_LINK_THIS_MODULE(gdiobj)

class wxStockGDIMac: public wxStockGDI, public wxModule
{
public:
    virtual const wxFont* GetFont(Item item);

    virtual bool OnInit();
    virtual void OnExit();

private:
    typedef wxStockGDI super;
    DECLARE_DYNAMIC_CLASS(wxStockGDIMac)
};

IMPLEMENT_DYNAMIC_CLASS(wxStockGDIMac, wxModule)

bool wxStockGDIMac::OnInit()
{
    // Override default instance
    ms_instance = this;
    return true;
}

void wxStockGDIMac::OnExit()
{
}

const wxFont* wxStockGDIMac::GetFont(Item item)
{
    wxFont* font = static_cast<wxFont*>(ms_stockObject[item]);
    if (font == NULL)
    {
        switch (item)
        {
        case FONT_NORMAL:
            font = new wxFont;
            font->MacCreateThemeFont(kThemeSystemFont);
            break;
        case FONT_SMALL:
            font = new wxFont;
            font->MacCreateThemeFont(kThemeSmallSystemFont);
            break;
        default:
            font = const_cast<wxFont*>(super::GetFont(item));
            break;
        }
        ms_stockObject[item] = font;
    }
    return font;
}
