/////////////////////////////////////////////////////////////////////////////
// Name:        src/palmos/imaglist.cpp
// Purpose:     wxImageList implementation for Palm OS
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: imaglist.cpp 62205 2009-09-30 06:58:50Z JS $
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
    #include <wxCtrls/window.h>
    #include <wxCtrls/icon.h>
    #include <wxCtrls/dc.h>
    #include <wxCtrls/string.h>
    #include <wxCtrls/dcmemory.h>
    #include <wxCtrls/intl.h>
    #include <wxCtrls/log.h>
#endif

#include <wxCtrls/palmos/imaglist.h>
#include <wxCtrls/palmos/private.h>


// ----------------------------------------------------------------------------
// wxWin macros
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxImageList, wxObject)

#define GetHImageList()     ((HIMAGELIST)m_hImageList)

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxImageList creation/destruction
// ----------------------------------------------------------------------------

wxImageList::wxImageList()
{
}

// Creates an image list
bool wxImageList::Create(int width, int height, bool mask, int initial)
{
    return false;
}

wxImageList::~wxImageList()
{
}

// ----------------------------------------------------------------------------
// wxImageList attributes
// ----------------------------------------------------------------------------

int wxImageList::GetImageCount() const
{
    return 0;
}

bool wxImageList::GetSize(int WXUNUSED(index), int &width, int &height) const
{
    return false;
}

// ----------------------------------------------------------------------------
// wxImageList operations
// ----------------------------------------------------------------------------

int wxImageList::Add(const wxBitmap& bitmap, const wxBitmap& mask)
{
    return 0;
}

int wxImageList::Add(const wxBitmap& bitmap, const wxColour& maskColour)
{
    return 0;
}

int wxImageList::Add(const wxIcon& icon)
{
    return 0;
}

bool wxImageList::Replace(int index,
                          const wxBitmap& bitmap, const wxBitmap& mask)
{
    return false;
}

bool wxImageList::Replace(int i, const wxIcon& icon)
{
    return false;
}

bool wxImageList::Remove(int index)
{
    return false;
}

bool wxImageList::RemoveAll()
{
    return false;
}

bool wxImageList::Draw(int index,
                       wxDC& dc,
                       int x, int y,
                       int flags,
                       bool solidBackground)
{
    return false;
}
