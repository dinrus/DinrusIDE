/////////////////////////////////////////////////////////////////////////////
// Name:        src/cocoa/NSPanel.mm
// Purpose:     wxCocoaNSPanel
// Author:      David Elliott
// Modified by:
// Created:     2003/03/16
// RCS-ID:      $Id: NSPanel.mm 42046 2006-10-16 09:30:01Z ABX $
// Copyright:   (c) 2003 David Elliott
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include <wxCtrls/wxprec.h>
#ifndef WX_PRECOMP
    #include <wxCtrls/log.h>
#endif // WX_PRECOMP

#include <wxCtrls/cocoa/NSPanel.h>

#import <AppKit/NSPanel.h>

// ----------------------------------------------------------------------------
// globals
// ----------------------------------------------------------------------------
WX_IMPLEMENT_OBJC_INTERFACE_HASHMAP(NSPanel)
