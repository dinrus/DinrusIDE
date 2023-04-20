/////////////////////////////////////////////////////////////////////////////
// Name:        src/palmos/printpalm.cpp
// Purpose:     wxPalmPrinter framework
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: printpalm.cpp 42522 2006-10-27 13:07:40Z JS $
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

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

// Don't use the Windows printer if we're in wxUniv mode and using
// the PostScript architecture
#if wxUSE_PRINTING_ARCHITECTURE && (!defined(__WXUNIVERSAL__) || !wxUSE_POSTSCRIPT_ARCHITECTURE_IN_MSW)

#ifndef WX_PRECOMP
    #include <wxCtrls/window.h>
    #include <wxCtrls/palmos/private.h>
    #include <wxCtrls/utils.h>
    #include <wxCtrls/dc.h>
    #include <wxCtrls/app.h>
    #include <wxCtrls/msgdlg.h>
    #include <wxCtrls/intl.h>
    #include <wxCtrls/log.h>
    #include <wxCtrls/dcprint.h>
#endif

#include <wxCtrls/palmos/printpalm.h>
#include <wxCtrls/printdlg.h>
#include <wxCtrls/palmos/private.h>

// ---------------------------------------------------------------------------
// private functions
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// wxWin macros
// ---------------------------------------------------------------------------

    IMPLEMENT_DYNAMIC_CLASS(wxPalmPrinter, wxPrinterBase)
    IMPLEMENT_CLASS(wxPalmPrintPreview, wxPrintPreviewBase)

// ===========================================================================
// implementation
// ===========================================================================

// ---------------------------------------------------------------------------
// Printer
// ---------------------------------------------------------------------------

wxPalmPrinter::wxPalmPrinter(wxPrintDialogData *data)
                : wxPrinterBase(data)
{
}

wxPalmPrinter::~wxPalmPrinter()
{
}

bool wxPalmPrinter::Print(wxWindow *parent, wxPrintout *printout, bool prompt)
{
    return false;
}

wxDC* wxPalmPrinter::PrintDialog(wxWindow *parent)
{
    wxDC* dc = (wxDC*) NULL;

    return dc;
}

bool wxPalmPrinter::Setup(wxWindow *parent)
{
    return false;
}

/*
* Print preview
*/

wxPalmPrintPreview::wxPalmPrintPreview(wxPrintout *printout,
                                             wxPrintout *printoutForPrinting,
                                             wxPrintDialogData *data)
                     : wxPrintPreviewBase(printout, printoutForPrinting, data)
{
}

wxPalmPrintPreview::wxPalmPrintPreview(wxPrintout *printout,
                                             wxPrintout *printoutForPrinting,
                                             wxPrintData *data)
                     : wxPrintPreviewBase(printout, printoutForPrinting, data)
{
}

wxPalmPrintPreview::~wxPalmPrintPreview()
{
}

bool wxPalmPrintPreview::Print(bool interactive)
{
    return false;
}

void wxPalmPrintPreview::DetermineScaling()
{
    // TODO
}

#endif
    // wxUSE_PRINTING_ARCHITECTURE
