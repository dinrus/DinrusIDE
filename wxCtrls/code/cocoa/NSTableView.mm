/////////////////////////////////////////////////////////////////////////////
// Name:        cocoa/NSTableView.mm
// Purpose:     wxCocoaNSTableView / wxCocoaNSTableDataSource
// Author:      David Elliott
// Modified by:
// Created:     2003/08/05
// RCS-ID:      $Id: NSTableView.mm 48107 2007-08-15 16:12:45Z DE $
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

#include <wxCtrls/cocoa/NSTableDataSource.h>
#include <wxCtrls/cocoa/NSTableView.h>
#import <AppKit/NSTableView.h>

// ----------------------------------------------------------------------------
// globals
// ----------------------------------------------------------------------------
WX_IMPLEMENT_OBJC_INTERFACE_HASHMAP(NSTableView)

// ============================================================================
// @class wxCocoaNSTableDataSource
// ============================================================================
@implementation wxCocoaNSTableDataSource : NSObject

- (int)numberOfRowsInTableView: (NSTableView *)tableView
{
    wxCocoaNSTableView *wxView = wxCocoaNSTableView::GetFromCocoa(tableView);
    wxCHECK_MSG(wxView, 0, wxT("No associated wx object"));
    return wxView->CocoaDataSource_numberOfRows();
}

- (id)tableView:(NSTableView *)tableView
    objectValueForTableColumn: (NSTableColumn *)tableColumn
    row: (int)rowIndex
{
    wxCocoaNSTableView *wxView = wxCocoaNSTableView::GetFromCocoa(tableView);
    wxCHECK_MSG(wxView, nil, wxT("No associated wx object"));
    return wxView->CocoaDataSource_objectForTableColumn(tableColumn,rowIndex);
}

@end // implementation wxCocoaNSTableDataSource
WX_IMPLEMENT_GET_OBJC_CLASS(wxCocoaNSTableDataSource,NSObject)

