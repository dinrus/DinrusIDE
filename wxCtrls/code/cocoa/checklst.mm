/////////////////////////////////////////////////////////////////////////////
// Name:        src/cocoa/checklst.mm
// Purpose:     wxCheckListBox
// Author:      David Elliott
// Modified by:
// Created:     2003/03/18
// RCS-ID:      $Id: checklst.mm 39463 2006-05-29 21:26:35Z ABX $
// Copyright:   (c) 2003 David Elliott
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wxCtrls/wxprec.h>

#if wxUSE_CHECKLISTBOX

#include <wxCtrls/checklst.h>

#ifndef WX_PRECOMP
    #include <wxCtrls/log.h>
    #include <wxCtrls/app.h>
#endif //WX_PRECOMP

IMPLEMENT_DYNAMIC_CLASS(wxCheckListBox, wxListBox)
BEGIN_EVENT_TABLE(wxCheckListBox, wxCheckListBoxBase)
END_EVENT_TABLE()
// WX_IMPLEMENT_COCOA_OWNER(wxCheckListBox,NSButton,NSControl,NSView)

bool wxCheckListBox::Create(wxWindow *parent, wxWindowID winid,
            const wxPoint& pos,
            const wxSize& size,
            const wxArrayString& choices,
            long style,
            const wxValidator& validator,
            const wxString& name)
{
    wxCArrayString chs(choices);

    return Create(parent, winid, pos, size, chs.GetCount(), chs.GetStrings(),
                  style, validator, name);
}

bool wxCheckListBox::Create(wxWindow *parent, wxWindowID winid,
            const wxPoint& pos,
            const wxSize& size,
            int n, const wxString choices[],
            long style,
            const wxValidator& validator,
            const wxString& name)
{
    if(!CreateControl(parent,winid,pos,size,style,validator,name))
        return false;

    if(m_parent)
        m_parent->CocoaAddChild(this);
    return true;
}

wxCheckListBox::~wxCheckListBox()
{
}

bool wxCheckListBox::IsChecked(unsigned int item) const
{
    return false;
}


void wxCheckListBox::Check(unsigned int item, bool check)
{
}

#endif
