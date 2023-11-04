/////////////////////////////////////////////////////////////////////////////
// Name:        statbox.h
// Purpose:     wxStaticBox base header
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// RCS-ID:      $Id: statbox.h 37066 2006-01-23 03:27:34Z MR $
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_STATBOX_H_BASE_
#define _WX_STATBOX_H_BASE_

#include <wxCtrls/defs.h>

#if wxUSE_STATBOX

#include <wxCtrls/control.h>

extern WXDLLEXPORT_DATA(const wxChar) wxStaticBoxNameStr[];

// ----------------------------------------------------------------------------
// wxStaticBox: a grouping box with a label
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxStaticBoxBase : public wxControl
{
public:
    wxStaticBoxBase() { }

    // overriden base class virtuals
    virtual bool AcceptsFocus() const { return false; }
    virtual bool HasTransparentBackground() { return true; }

    // implementation only: this is used by wxStaticBoxSizer to account for the
    // need for extra space taken by the static box
    //
    // the top border is the margin at the top (where the title is),
    // borderOther is the margin on all other sides
    virtual void GetBordersForSizer(int *borderTop, int *borderOther) const
    {
        const int BORDER = 5; // FIXME: hardcoded value

        *borderTop = GetLabel().empty() ? BORDER : GetCharHeight();
        *borderOther = BORDER;
    }

private:
    DECLARE_NO_COPY_CLASS(wxStaticBoxBase)
};

#if defined(__WXUNIVERSAL__)
    #include <wxCtrls/univ/statbox.h>
#elif defined(__WXMSW__)
    #include <wxCtrls/msw/statbox.h>
#elif defined(__WXMOTIF__)
    #include <wxCtrls/motif/statbox.h>
#elif defined(__WXGTK20__)
    #include <wxCtrls/gtk/statbox.h>
#elif defined(__WXGTK__)
    #include <wxCtrls/gtk1/statbox.h>
#elif defined(__WXMAC__)
    #include <wxCtrls/mac/statbox.h>
#elif defined(__WXCOCOA__)
    #include <wxCtrls/cocoa/statbox.h>
#elif defined(__WXPM__)
    #include <wxCtrls/os2/statbox.h>
#endif

#endif // wxUSE_STATBOX

#endif
    // _WX_STATBOX_H_BASE_
