/////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/NSSlider.h
// Purpose:     wxCocoaNSSlider class
// Author:      Mark Oxenham
// Modified by: David Elliott
// Created:     2007/08/10
// RCS-ID:      $Id: NSSlider.h 60554 2009-05-08 23:10:00Z VZ $
// Copyright:   (c) 2007 Software 2000 Ltd. All rights reserved.
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXNSSLIDER_H_
#define _WXNSSLIDER_H_

#include <wxCtrls/hashmap.h>
#include <wxCtrls/cocoa/ObjcAssociate.h>
#include <wxCtrls/cocoa/ObjcRef.h>

DECLARE_WXCOCOA_OBJC_CLASS(NSSlider);

WX_DECLARE_OBJC_HASHMAP(NSSlider);

// For when we're not in Objective-C mode:
typedef struct objc_selector    *SEL;    

class wxCocoaNSSliderLastSelectorChanger;

class wxCocoaNSSlider
{
    friend class wxCocoaNSSliderLastSelectorChanger;
    WX_DECLARE_OBJC_INTERFACE_HASHMAP(NSSlider);
public:
    void AssociateNSSlider(WX_NSSlider cocoaNSSlider);
    void DisassociateNSSlider(WX_NSSlider cocoaNSSlider);

    virtual void CocoaNotification_startTracking(WX_NSNotification notification) = 0;
    virtual void CocoaNotification_continueTracking(WX_NSNotification notification) = 0;
    virtual void CocoaNotification_stopTracking(WX_NSNotification notification) = 0;

    static SEL GetLastResponderSelector()
    {   return sm_lastResponderSelector; }
protected:
    virtual ~wxCocoaNSSlider() { }
    static SEL sm_lastResponderSelector;
};

#endif
