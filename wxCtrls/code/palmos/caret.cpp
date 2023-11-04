///////////////////////////////////////////////////////////////////////////////
// Name:        src/palmos/caret.cpp
// Purpose:     Palm OS implementation of wxCaret
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10.13.04
// RCS-ID:      $Id: caret.cpp 35650 2005-09-23 12:56:45Z MR $
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h>.
#include <wxCtrls/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wxCtrls/window.h>
    #include <wxCtrls/log.h>
#endif // WX_PRECOMP

#include <wxCtrls/caret.h>

#if wxUSE_CARET

#include <wxCtrls/palmos/private.h>

// ---------------------------------------------------------------------------
// macros
// ---------------------------------------------------------------------------

#define CALL_CARET_API(api, args)   \
        if ( !api args )                \
            wxLogLastError(_T(#api))

// ===========================================================================
// implementation
// ===========================================================================

// ---------------------------------------------------------------------------
// blink time
// ---------------------------------------------------------------------------

//static
int wxCaretBase::GetBlinkTime()
{
    return 0;
}

//static
void wxCaretBase::SetBlinkTime(int milliseconds)
{
}

// ---------------------------------------------------------------------------
// creating/destroying the caret
// ---------------------------------------------------------------------------

bool wxCaret::PalmOSCreateCaret()
{
    return false;
}

void wxCaret::OnSetFocus()
{
}

void wxCaret::OnKillFocus()
{
}

// ---------------------------------------------------------------------------
// showing/hiding the caret
// ---------------------------------------------------------------------------

void wxCaret::DoShow()
{
}

void wxCaret::DoHide()
{
}

// ---------------------------------------------------------------------------
// moving the caret
// ---------------------------------------------------------------------------

void wxCaret::DoMove()
{
}


// ---------------------------------------------------------------------------
// resizing the caret
// ---------------------------------------------------------------------------

void wxCaret::DoSize()
{
}

#endif
