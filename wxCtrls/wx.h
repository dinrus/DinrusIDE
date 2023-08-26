/////////////////////////////////////////////////////////////////////////////
// Name:        wx/wx.h
// Purpose:     wxWidgets central header including the most often used ones
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id: wx.h 40943 2006-08-31 19:31:43Z ABX $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WX_H_
#define _WX_WX_H_

#include <wxCtrls/defs.h>
#include <wxCtrls/object.h>
#include <wxCtrls/dynarray.h>
#include <wxCtrls/list.h>
#include <wxCtrls/hash.h>
#include <wxCtrls/string.h>
#include <wxCtrls/hashmap.h>
#include <wxCtrls/arrstr.h>
#include <wxCtrls/intl.h>
#include <wxCtrls/log.h>
#include <wxCtrls/event.h>
#include <wxCtrls/app.h>
#include <wxCtrls/utils.h>
#include <wxCtrls/stream.h>
#include <wxCtrls/memory.h>
#include <wxCtrls/math.h>
#include <wxCtrls/stopwatch.h>
#include <wxCtrls/module.h>

#if wxUSE_GUI

#include <wxCtrls/window.h>
#include <wxCtrls/containr.h>
#include <wxCtrls/panel.h>
#include <wxCtrls/toplevel.h>
#include <wxCtrls/frame.h>
#include <wxCtrls/gdicmn.h>
#include <wxCtrls/gdiobj.h>
#include <wxCtrls/region.h>
#include <wxCtrls/bitmap.h>
#include <wxCtrls/image.h>
#include <wxCtrls/colour.h>
#include <wxCtrls/font.h>
#include <wxCtrls/dc.h>
#include <wxCtrls/dcclient.h>
#include <wxCtrls/dcmemory.h>
#include <wxCtrls/dcprint.h>
#include <wxCtrls/dcscreen.h>
#include <wxCtrls/button.h>
#include <wxCtrls/menuitem.h>
#include <wxCtrls/menu.h>
#include <wxCtrls/pen.h>
#include <wxCtrls/brush.h>
#include <wxCtrls/palette.h>
#include <wxCtrls/icon.h>
#include <wxCtrls/cursor.h>
#include <wxCtrls/dialog.h>
#include <wxCtrls/timer.h>
#include <wxCtrls/settings.h>
#include <wxCtrls/msgdlg.h>
#include <wxCtrls/cmndata.h>
#include <wxCtrls/dataobj.h>

#include <wxCtrls/control.h>
#include <wxCtrls/ctrlsub.h>
#include <wxCtrls/bmpbuttn.h>
#include <wxCtrls/checkbox.h>
#include <wxCtrls/checklst.h>
#include <wxCtrls/choice.h>
#include <wxCtrls/scrolbar.h>
#include <wxCtrls/stattext.h>
#include <wxCtrls/statbmp.h>
#include <wxCtrls/statbox.h>
#include <wxCtrls/listbox.h>
#include <wxCtrls/radiobox.h>
#include <wxCtrls/radiobut.h>
#include <wxCtrls/textctrl.h>
#include <wxCtrls/slider.h>
#include <wxCtrls/gauge.h>
#include <wxCtrls/scrolwin.h>
#include <wxCtrls/dirdlg.h>
#include <wxCtrls/toolbar.h>
#include <wxCtrls/combobox.h>
#include <wxCtrls/layout.h>
#include <wxCtrls/sizer.h>
#include <wxCtrls/mdi.h>
#include <wxCtrls/statusbr.h>
#include <wxCtrls/choicdlg.h>
#include <wxCtrls/textdlg.h>
#include <wxCtrls/filedlg.h>

// always include, even if !wxUSE_VALIDATORS because we need wxDefaultValidator
#include <wxCtrls/validate.h>

#if wxUSE_VALIDATORS
    #include <wxCtrls/valtext.h>
#endif // wxUSE_VALIDATORS

#endif // wxUSE_GUI

#endif // _WX_WX_H_
