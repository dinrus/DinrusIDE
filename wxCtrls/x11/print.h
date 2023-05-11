/////////////////////////////////////////////////////////////////////////////
// Name:        wx/x11/print.h
// Purpose:     wxPrinter, wxPrintPreview classes
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: print.h 42077 2006-10-17 14:44:52Z ABX $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PRINT_H_
#define _WX_PRINT_H_

#include <wxCtrls/prntbase.h>

/*
* Represents the printer: manages printing a wxPrintout object
*/

class WXDLLEXPORT wxPrinter: public wxPrinterBase
{
    DECLARE_DYNAMIC_CLASS(wxPrinter)

public:
    wxPrinter(wxPrintData *data = NULL);
    virtual ~wxPrinter();

    virtual bool Print(wxWindow *parent, wxPrintout *printout, bool prompt = TRUE);
    virtual bool PrintDialog(wxWindow *parent);
    virtual bool Setup(wxWindow *parent);
};

/*
* wxPrintPreview
* Programmer creates an object of this class to preview a wxPrintout.
*/

class WXDLLEXPORT wxPrintPreview: public wxPrintPreviewBase
{
    DECLARE_CLASS(wxPrintPreview)

public:
    wxPrintPreview(wxPrintout *printout, wxPrintout *printoutForPrinting = NULL, wxPrintData *data = NULL);
    virtual ~wxPrintPreview();

    virtual bool Print(bool interactive);
    virtual void DetermineScaling();
};

#endif
    // _WX_PRINT_H_