/////////////////////////////////////////////////////////////////////////////
// Name:        src/mac/classic/statline.cpp
// Purpose:     wxStaticLine class
// Author:      Vadim Zeitlin
// Created:     28.06.99
// Version:     $Id: statline.cpp 39487 2006-05-31 18:27:51Z ABX $
// Copyright:   (c) 1998 Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h>.
#include <wxCtrls/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include <wxCtrls/statline.h>

#ifndef WX_PRECOMP
    #include <wxCtrls/statbox.h>
#endif

// ============================================================================
// implementation
// ============================================================================

IMPLEMENT_DYNAMIC_CLASS(wxStaticLine, wxControl)

// ----------------------------------------------------------------------------
// wxStaticLine
// ----------------------------------------------------------------------------

bool wxStaticLine::Create( wxWindow *parent,
                           wxWindowID id,
                           const wxPoint &pos,
                           const wxSize &size,
                           long style,
                           const wxString &name)
{
    if ( !CreateBase(parent, id, pos, size, style, wxDefaultValidator, name) )
        return false;

    // ok, this is ugly but it's better than nothing: use a thin static box to
    // emulate static line

    wxSize sizeReal = AdjustSize(size);

//    m_statbox = new wxStaticBox(parent, id, wxT(""), pos, sizeReal, style, name);

    return true;
}
