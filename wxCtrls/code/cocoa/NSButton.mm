/////////////////////////////////////////////////////////////////////////////
// Name:        cocoa/NSButton.cpp
// Purpose:     wxCocoaNSButton
// Author:      David Elliott
// Modified by:
// Created:     2003/01/31
// RCS-ID:      $Id: NSButton.mm 47445 2007-07-13 21:49:41Z DE $
// Copyright:   (c) 2003-2004 David Elliott
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

#include <wxCtrls/cocoa/objc/objc_uniquifying.h>
#include <wxCtrls/cocoa/NSButton.h>
#import <AppKit/NSButton.h>

// ----------------------------------------------------------------------------
// globals
// ----------------------------------------------------------------------------
WX_IMPLEMENT_OBJC_INTERFACE_HASHMAP(NSButton)

// ============================================================================
// @class wxNSButtonTarget
// ============================================================================
@interface wxNSButtonTarget : NSObject
{
}

- (void)wxNSButtonAction: (id)sender;
@end // wxNSButtonTarget
WX_DECLARE_GET_OBJC_CLASS(wxNSButtonTarget,NSObject)

@implementation wxNSButtonTarget :  NSObject
- (void)wxNSButtonAction: (id)sender
{
    wxCocoaNSButton *button = wxCocoaNSButton::GetFromCocoa(sender);
    wxCHECK_RET(button,wxT("wxNSButtonAction received without associated wx object"));
    button->Cocoa_wxNSButtonAction();
}

@end // implementation wxNSButtonTarget
WX_IMPLEMENT_GET_OBJC_CLASS(wxNSButtonTarget,NSObject)

// ============================================================================
// class wxCocoaNSButton
// ============================================================================
const wxObjcAutoRefFromAlloc<struct objc_object*> wxCocoaNSButton::sm_cocoaTarget = [[WX_GET_OBJC_CLASS(wxNSButtonTarget) alloc] init];

void wxCocoaNSButton::AssociateNSButton(WX_NSButton cocoaNSButton)
{
    if(cocoaNSButton)
    {
        sm_cocoaHash.insert(wxCocoaNSButtonHash::value_type(cocoaNSButton,this));
        [cocoaNSButton setTarget: sm_cocoaTarget];
        [cocoaNSButton setAction: @selector(wxNSButtonAction:)];
    }
}

