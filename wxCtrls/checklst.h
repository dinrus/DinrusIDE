///////////////////////////////////////////////////////////////////////////////
// Name:        wx/checklst.h
// Purpose:     wxCheckListBox class interface
// Author:      Vadim Zeitlin
// Modified by:
// Created:     12.09.00
// RCS-ID:      $Id: checklst.h 38319 2006-03-23 22:05:23Z VZ $
// Copyright:   (c) Vadim Zeitlin
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CHECKLST_H_BASE_
#define _WX_CHECKLST_H_BASE_

#if wxUSE_CHECKLISTBOX

#include <wxCtrls/listbox.h>

// ----------------------------------------------------------------------------
// wxCheckListBox: a listbox whose items may be checked
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxCheckListBoxBase : public
                                              #ifdef __WXWINCE__
                                                  // keep virtuals synchronised
                                                  wxListBoxBase
                                              #else
                                                  wxListBox
                                              #endif
{
public:
    wxCheckListBoxBase() { }

    // check list box specific methods
    virtual bool IsChecked(unsigned int item) const = 0;
    virtual void Check(unsigned int item, bool check = true) = 0;

    DECLARE_NO_COPY_CLASS(wxCheckListBoxBase)
};

#if defined(__WXUNIVERSAL__)
    #include <wxCtrls/univ/checklst.h>
#elif defined(__WXWINCE__)
    #include <wxCtrls/msw/wince/checklst.h>
#elif defined(__WXMSW__)
    #include <wxCtrls/msw/checklst.h>
#elif defined(__WXMOTIF__)
    #include <wxCtrls/motif/checklst.h>
#elif defined(__WXGTK20__)
    #include <wxCtrls/gtk/checklst.h>
#elif defined(__WXGTK__)
    #include <wxCtrls/gtk1/checklst.h>
#elif defined(__WXMAC__)
    #include <wxCtrls/mac/checklst.h>
#elif defined(__WXCOCOA__)
    #include <wxCtrls/cocoa/checklst.h>
#elif defined(__WXPM__)
    #include <wxCtrls/os2/checklst.h>
#endif

#endif // wxUSE_CHECKLISTBOX

#endif
    // _WX_CHECKLST_H_BASE_
