/////////////////////////////////////////////////////////////////////////////
// Name:        clipbrd.h
// Purpose:     Clipboard functionality.
//              Note: this functionality is under review, and
//              is derived from wxWidgets 1.xx code. Please contact
//              the wxWidgets developers for further information.
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: clipbrd.h 41020 2006-09-05 20:47:48Z VZ $
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CLIPBRD_H_
#define _WX_CLIPBRD_H_

#if wxUSE_CLIPBOARD

#include <wxCtrls/list.h>
#include <wxCtrls/module.h>
#include <wxCtrls/dataobj.h>     // for wxDataFormat

//-----------------------------------------------------------------------------
// wxClipboard
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxDataObject;
class WXDLLEXPORT wxClipboard : public wxClipboardBase
{
    DECLARE_DYNAMIC_CLASS(wxClipboard)

public:
    wxClipboard();
    virtual ~wxClipboard();

    // open the clipboard before SetData() and GetData()
    virtual bool Open();

    // close the clipboard after SetData() and GetData()
    virtual void Close();

    // query whether the clipboard is opened
    virtual bool IsOpened() const;

    // set the clipboard data. all other formats will be deleted.
    virtual bool SetData( wxDataObject *data );

    // add to the clipboard data.
    virtual bool AddData( wxDataObject *data );

    // ask if data in correct format is available
    virtual bool IsSupported( const wxDataFormat& format );

    // fill data with data on the clipboard (if available)
    virtual bool GetData( wxDataObject& data );

    // clears wxTheClipboard and the system's clipboard if possible
    virtual void Clear();

    // flushes the clipboard: this means that the data which is currently on
    // clipboard will stay available even after the application exits (possibly
    // eating memory), otherwise the clipboard will be emptied on exit
    virtual bool Flush();

    // X11 has two clipboards which get selected by this call. Empty on MSW.
    void UsePrimarySelection( bool WXUNUSED(primary) = FALSE ) { }

private:
    wxDataObject     *m_data;
    bool              m_open;
};

#endif // wxUSE_CLIPBOARD

#endif
    // _WX_CLIPBRD_H_
