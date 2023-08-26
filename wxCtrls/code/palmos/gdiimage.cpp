///////////////////////////////////////////////////////////////////////////////
// Name:        src/palmos/gdiimage.cpp
// Purpose:     wxGDIImage implementation
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: gdiimage.cpp 39710 2006-06-14 10:02:19Z ABX $
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

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
    #include <wxCtrls/string.h>
    #include <wxCtrls/log.h>
    #include <wxCtrls/app.h>
    #include <wxCtrls/bitmap.h>
#endif // WX_PRECOMP

#include <wxCtrls/palmos/gdiimage.h>

#if wxUSE_WXDIB
#include <wxCtrls/palmos/dib.h>
#endif

#include <wxCtrls/file.h>

#include <wxCtrls/listimpl.cpp>
WX_DEFINE_LIST(wxGDIImageHandlerList)

// ----------------------------------------------------------------------------
// auxiliary functions
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// ============================================================================
// implementation
// ============================================================================

wxGDIImageHandlerList wxGDIImage::ms_handlers;

// ----------------------------------------------------------------------------
// wxGDIImage functions forwarded to wxGDIImageRefData
// ----------------------------------------------------------------------------

bool wxGDIImage::FreeResource(bool WXUNUSED(force))
{
    return false;
}

WXHANDLE wxGDIImage::GetResourceHandle() const
{
    return NULL;
}

// ----------------------------------------------------------------------------
// wxGDIImage handler stuff
// ----------------------------------------------------------------------------

void wxGDIImage::AddHandler(wxGDIImageHandler *handler)
{
}

void wxGDIImage::InsertHandler(wxGDIImageHandler *handler)
{
}

bool wxGDIImage::RemoveHandler(const wxString& name)
{
    return false;
}

wxGDIImageHandler *wxGDIImage::FindHandler(const wxString& name)
{
    return NULL;
}

wxGDIImageHandler *wxGDIImage::FindHandler(const wxString& extension,
                                           long type)
{
    return NULL;
}

wxGDIImageHandler *wxGDIImage::FindHandler(long type)
{
    return NULL;
}

void wxGDIImage::CleanUpHandlers()
{
}

void wxGDIImage::InitStandardHandlers()
{
}
