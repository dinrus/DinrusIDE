/////////////////////////////////////////////////////////////////////////////
// Name:        src/mgl/settings.cpp
// Author:      Vaclav Slavik, Robert Roebling
// Id:          $Id: settings.cpp 40998 2006-09-04 15:22:04Z VS $
// Copyright:   (c) 2001-2002 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h>.
#include <wxCtrls/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include <wxCtrls/settings.h>

#ifndef WX_PRECOMP
    #include <wxCtrls/colour.h>
    #include <wxCtrls/font.h>
    #include <wxCtrls/gdicmn.h>
    #include <wxCtrls/module.h>
#endif

// ----------------------------------------------------------------------------
// global data
// ----------------------------------------------------------------------------

static wxFont *gs_fontDefault = NULL;

class wxSystemSettingsModule : public wxModule
{
public:
    virtual bool OnInit() { return true; }
    virtual void OnExit()
    {
        delete gs_fontDefault;
        gs_fontDefault = NULL;
    }

private:
    DECLARE_DYNAMIC_CLASS(wxSystemSettingsModule)
};

IMPLEMENT_DYNAMIC_CLASS(wxSystemSettingsModule, wxModule)



wxColour wxSystemSettingsNative::GetColour(wxSystemColour WXUNUSED(index))
{
    // overridden by wxSystemSettings::GetColour in wxUniversal
    return wxColour(0,0,0);
}

wxFont wxSystemSettingsNative::GetFont(wxSystemFont index)
{
    switch (index)
    {
        case wxSYS_OEM_FIXED_FONT:
        case wxSYS_ANSI_FIXED_FONT:
        case wxSYS_SYSTEM_FIXED_FONT:
        {
            return *wxNORMAL_FONT;
        }
        case wxSYS_ANSI_VAR_FONT:
        case wxSYS_SYSTEM_FONT:
        case wxSYS_DEVICE_DEFAULT_FONT:
        case wxSYS_DEFAULT_GUI_FONT:
        {
            if ( !gs_fontDefault )
                gs_fontDefault = new wxFont(10, wxSWISS, wxNORMAL, wxNORMAL, false, "Arial");
            return *gs_fontDefault;
        }
        default:
        {
        }
    }

    return wxNullFont;
}

int wxSystemSettingsNative::GetMetric(wxSystemMetric index, wxWindow* WXUNUSED(win))
{
    int val;

    switch (index)
    {
        case wxSYS_SCREEN_X:
            wxDisplaySize(&val, NULL);
            return val;
        case wxSYS_SCREEN_Y:
            wxDisplaySize(NULL, &val);
            return val;
        case wxSYS_VSCROLL_X:
        case wxSYS_HSCROLL_Y:
            return 15;
        default:
        {
        }
    }

    return -1;  // unsupported metric
}

bool wxSystemSettingsNative::HasFeature(wxSystemFeature index)
{
    switch (index)
    {
        case wxSYS_CAN_ICONIZE_FRAME:
        case wxSYS_CAN_DRAW_FRAME_DECORATIONS:
        case wxSYS_TABLET_PRESENT:
            return false;

        default:
            wxFAIL_MSG( _T("unknown feature") );
    }

    return false;
}
