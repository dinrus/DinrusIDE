/////////////////////////////////////////////////////////////////////////////
// Name:        src/cococa/colour.mm
// Purpose:     wxColour class
// Author:      David Elliott
// Modified by:
// Created:     2003/06/17
// RCS-ID:      $Id: colour.mm 51585 2008-02-08 00:35:39Z DE $
// Copyright:   (c) 2003 David Elliott
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wxCtrls/wxprec.h>

#include <wxCtrls/colour.h>

#ifndef WX_PRECOMP
    #include <wxCtrls/gdicmn.h>
#endif //WX_PRECOMP

#include <wxCtrls/cocoa/autorelease.h>
#include <wxCtrls/cocoa/ObjcRef.h>

#import <AppKit/NSColor.h>

IMPLEMENT_DYNAMIC_CLASS(wxColour, wxObject)

void wxColour::Init()
{
    m_cocoaNSColor = NULL;
    m_red =
    m_blue =
    m_green = 0;
}

wxColour::wxColour (const wxColour& col)
:   m_cocoaNSColor(col.m_cocoaNSColor)
,   m_red(col.m_red)
,   m_green(col.m_green)
,   m_blue(col.m_blue)
,   m_alpha(col.m_alpha)
{
    wxGCSafeRetain(m_cocoaNSColor);
}

wxColour::wxColour( WX_NSColor aColor )
:   m_cocoaNSColor(nil)
{
    Set(aColor);
}

wxColour& wxColour::operator =(const wxColour& col)
{
    m_cocoaNSColor = wxGCSafeRetain(col.m_cocoaNSColor);
    m_red = col.m_red;
    m_green = col.m_green;
    m_blue = col.m_blue;
    m_alpha = col.m_alpha;
    return *this;
}

wxColour::~wxColour ()
{
    wxGCSafeRelease(m_cocoaNSColor);
}

void wxColour::InitRGBA(unsigned char r,
                        unsigned char g,
                        unsigned char b,
                        unsigned char a)
{
    wxAutoNSAutoreleasePool pool;
    wxGCSafeRelease(m_cocoaNSColor);
    m_cocoaNSColor = wxGCSafeRetain([NSColor colorWithCalibratedRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:a/255.0]);
    m_red = r;
    m_green = g;
    m_blue = b;
    m_alpha = a;
}

void wxColour::Set( WX_NSColor aColor )
{
    wxGCSafeRetain(aColor);
    wxGCSafeRelease(m_cocoaNSColor);
    m_cocoaNSColor = aColor;

    /* Make a temporary color in RGB format and get the values.  Note that
       unless the color was actually RGB to begin with it's likely that
       these will be fairly bogus. Particulary if the color is a pattern. */
    NSColor *rgbColor = [m_cocoaNSColor colorUsingColorSpaceName:NSCalibratedRGBColorSpace];
    m_red   = (wxUint8) ([rgbColor redComponent]   * 255.0);
    m_green = (wxUint8) ([rgbColor greenComponent] * 255.0);
    m_blue  = (wxUint8) ([rgbColor blueComponent]  * 255.0);
    m_alpha  = (wxUint8) ([rgbColor alphaComponent]  * 255.0);
}
