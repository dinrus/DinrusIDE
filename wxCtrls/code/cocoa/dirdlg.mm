/////////////////////////////////////////////////////////////////////////////
// Name:        src/cocoa/dirdlg.mm
// Purpose:     wxDirDialog for wxCocoa
// Author:      Ryan Norton
// Modified by: Hiroyuki Nakamura(maloninc)
// Created:     2006-01-10
// RCS-ID:      $Id: dirdlg.mm 40007 2006-07-05 13:10:46Z SC $
// Copyright:   (c) Ryan Norton
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

#if wxUSE_DIRDLG

#include <wxCtrls/dirdlg.h>

#ifndef WX_PRECOMP
    #include <wxCtrls/msgdlg.h>
    #include <wxCtrls/filedlg.h>
    #include <wxCtrls/app.h>
#endif

#include <wxCtrls/filename.h>

#include <wxCtrls/cocoa/autorelease.h>
#include <wxCtrls/cocoa/string.h>

#import <AppKit/NSOpenPanel.h>
#import <AppKit/NSSavePanel.h>

#import <Foundation/NSArray.h>
// ============================================================================
// implementation
// ============================================================================

IMPLEMENT_CLASS(wxCocoaDirDialog, wxDialog)

// ----------------------------------------------------------------------------
// wxDirDialog
// ----------------------------------------------------------------------------

wxDirDialog::wxDirDialog(wxWindow *parent, const wxString& message,
        const wxString& defaultPath, long style, const wxPoint& pos,
        const wxSize& size, const wxString& name)
{
    wxTopLevelWindows.Append(this);

    m_message = message;

    SetWindowStyle(style);
    m_parent = parent;
    m_path = defaultPath;

    wxASSERT(CreateBase(parent,wxID_ANY,pos,wxDefaultSize,style,wxDefaultValidator,wxDialogNameStr));

    if ( parent )
        parent->AddChild(this);

    m_cocoaNSWindow = nil;
    m_cocoaNSView = nil;

    //If the user requests to save - use a NSSavePanel
    //else use a NSOpenPanel
    if (HasFlag(wxFD_SAVE))
    {
        SetNSPanel([NSSavePanel savePanel]);

        [GetNSSavePanel() setTitle:wxNSStringWithWxString(message)];

        [GetNSSavePanel() setPrompt:@"Save"];
        [GetNSSavePanel() setTreatsFilePackagesAsDirectories:YES];
        [GetNSSavePanel() setCanSelectHiddenExtension:YES];
    }
    else //m_dialogStyle & wxFD_OPEN
    {
        SetNSPanel([NSOpenPanel openPanel]);
        [m_cocoaNSWindow setTitle:wxNSStringWithWxString(message)];

        [(NSOpenPanel*)m_cocoaNSWindow setResolvesAliases:YES];
        [(NSOpenPanel*)m_cocoaNSWindow setCanChooseFiles:NO];
        [(NSOpenPanel*)m_cocoaNSWindow setCanChooseDirectories:YES];
        [GetNSSavePanel() setPrompt:@"Open"];
    }

    if (HasFlag(wxDD_NEW_DIR_BUTTON)) //m_dialogStyle & wxDD_NEW_DIR_BUTTON
    {
        [(NSOpenPanel*)m_cocoaNSWindow setCanCreateDirectories:YES];
    }
}

wxDirDialog::~wxDirDialog()
{
}

int wxDirDialog::ShowModal()
{
    wxAutoNSAutoreleasePool thePool;

    m_fileNames.Empty();

    int nResult;

    if (HasFlag(wxFD_SAVE))
    {
        nResult = [GetNSSavePanel()
                    runModalForDirectory:wxNSStringWithWxString(m_dir)
                    file:wxNSStringWithWxString(m_fileName)];

        if (nResult == NSOKButton)
        {
            m_fileNames.Add(wxStringWithNSString([GetNSSavePanel() filename]));
            m_path = m_fileNames[0];
        }
    }
    else //m_dialogStyle & wxFD_OPEN
    {
        nResult = [(NSOpenPanel*)m_cocoaNSWindow
                    runModalForDirectory:wxNSStringWithWxString(m_dir)
                    file:wxNSStringWithWxString(m_fileName)
                    types:NULL];

        if (nResult == NSOKButton)
        {
            for(unsigned i = 0; i < [[(NSOpenPanel*)m_cocoaNSWindow filenames] count]; ++i)
            {
                m_fileNames.Add(wxStringWithNSString([[(NSOpenPanel*)m_cocoaNSWindow filenames] objectAtIndex:(i)]));
            }

            m_path = m_fileNames[0];
        }
    }

    return nResult == NSOKButton ? wxID_OK : wxID_CANCEL;
}

#endif // wxUSE_DIRDLG
