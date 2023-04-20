/////////////////////////////////////////////////////////////////////////////
// Name:        src/palmos/brush.cpp
// Purpose:     wxBrush
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: brush.cpp 41194 2006-09-13 16:16:37Z ABX $
// Copyright:   (c) William Osborne
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

#ifndef WX_PRECOMP
    #include <wxCtrls/list.h>
    #include <wxCtrls/utils.h>
    #include <wxCtrls/app.h>
    #include <wxCtrls/brush.h>
    #include <wxCtrls/colour.h>
#endif // WX_PRECOMP

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// ============================================================================
// wxBrush implementation
// ============================================================================

IMPLEMENT_DYNAMIC_CLASS(wxBrush, wxGDIObject)

// ----------------------------------------------------------------------------
// wxBrush ctors/dtor
// ----------------------------------------------------------------------------

wxBrush::wxBrush()
{
}

wxBrush::wxBrush(const wxColour& col, int style)
{
}

wxBrush::wxBrush(const wxBitmap& stipple)
{
}

wxBrush::~wxBrush()
{
}

// ----------------------------------------------------------------------------
// wxBrush house keeping stuff
// ----------------------------------------------------------------------------

bool wxBrush::operator==(const wxBrush& brush) const
{
    return false;
}

wxObjectRefData *wxBrush::CreateRefData() const
{
    return NULL;
}

wxObjectRefData *wxBrush::CloneRefData(const wxObjectRefData *data) const
{
    return NULL;
}

// ----------------------------------------------------------------------------
// wxBrush accessors
// ----------------------------------------------------------------------------

wxColour wxBrush::GetColour() const
{
    return wxNullColour;
}

int wxBrush::GetStyle() const
{
    return -1;
}

wxBitmap *wxBrush::GetStipple() const
{
    return NULL;
}

WXHANDLE wxBrush::GetResourceHandle() const
{
    return (WXHANDLE)0;
}

// ----------------------------------------------------------------------------
// wxBrush setters
// ----------------------------------------------------------------------------

void wxBrush::SetColour(const wxColour& WXUNUSED(col))
{
}

void wxBrush::SetColour(unsigned char WXUNUSED(r), unsigned char WXUNUSED(g), unsigned char WXUNUSED(b))
{
}

void wxBrush::SetStyle(int WXUNUSED(style))
{
}

void wxBrush::SetStipple(const wxBitmap& WXUNUSED(stipple))
{
}
