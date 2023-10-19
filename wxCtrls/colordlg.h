/////////////////////////////////////////////////////////////////////////////
// Name:        wx/colrdlgg.h
// Purpose:     wxColourDialog
// Author:      Vadim Zeitiln
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id: colordlg.h 36623 2006-01-02 14:26:36Z JS $
// Copyright:   (c) wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_COLORDLG_H_BASE_
#define _WX_COLORDLG_H_BASE_

#include <wxCtrls/defs.h>

#if wxUSE_COLOURDLG

#if defined(__WXMSW__) && !defined(__WXUNIVERSAL__)
    #include <wxCtrls/msw/colordlg.h>
#elif defined(__WXMAC__) && !defined(__WXUNIVERSAL__)
    #include <wxCtrls/mac/colordlg.h>
#elif defined(__WXGTK20__) && !defined(__WXUNIVERSAL__)
    #include <wxCtrls/gtk/colordlg.h>
#elif defined(__WXPALMOS__)
    #include <wxCtrls/palmos/colordlg.h>
#else
    #include <wxCtrls/generic/colrdlgg.h>

    #define wxColourDialog wxGenericColourDialog
#endif

// get the colour from user and return it
wxColour WXDLLEXPORT
wxGetColourFromUser(wxWindow *parent = (wxWindow *)NULL,
                    const wxColour& colInit = wxNullColour, const wxString& caption = wxEmptyString);

#endif // wxUSE_COLOURDLG

#endif
    // _WX_COLORDLG_H_BASE_
