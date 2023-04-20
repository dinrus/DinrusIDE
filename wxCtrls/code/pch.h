// pch.h: это предварительно скомпилированный заголовочный файл.
// Перечисленные ниже файлы компилируются только один раз, что ускоряет последующие сборки.
// Это также влияет на работу IntelliSense, включая многие функции просмотра и завершения кода.
// Однако изменение любого из приведенных здесь файлов между операциями сборки приведет к повторной компиляции всех(!) этих файлов.
// Не добавляйте сюда файлы, которые планируете часто изменять, так как в этом случае выигрыша в производительности не будет.

#ifndef PCH_H
#define PCH_H

// Добавьте сюда заголовочные файлы для предварительной компиляции
#include "framework.h"


// stdafx.h: включаемый файл для стандартных системных включаемых файлов
// или включаемых файлов для конкретного проекта, которые часто используются, но
// не часто изменяются
//

#pragma once

#include "targetver.h"

#define _CRT_SECURE_NO_WARNINGS
#define WXUSINGUNIV 0

#pragma warning( push )
#pragma warning (disable : 4996)


// TODO: Установите здесь ссылки на дополнительные заголовки, требующиеся для программы
#include <wxCtrls/config.h>
#include <wxCtrls/string.h>
#include <wxCtrls/event.h>
#include <wxCtrls/wx.h>
#include <wxCtrls/button.h>
#include <wxCtrls/bmpbuttn.h>
#include <wxCtrls/bitmap.h>
#include <wxCtrls/image.h>
#include <wxCtrls/artprov.h>
#include <wxCtrls/control.h>
#include <wxCtrls/combobox.h>
#include <wxCtrls/colordlg.h>
#include <wxCtrls/clipbrd.h>
#include <wxCtrls/textctrl.h>
#include <wxCtrls/choicdlg.h>
#include <wxCtrls/choice.h>
#include <wxCtrls/caret.h>
#include <wxCtrls/calctrl.h>
//#include <wxCtrls/generic/calctrlg.h>
#include <wxCtrls/tglbtn.h>
#include <wxCtrls/calctrl.h>
#include <wxCtrls/fdrepdlg.h>
#include <wxCtrls/treectrl.h>
#include <wxCtrls/listctrl.h>
#include <wxCtrls/notebook.h>
#include <wxCtrls/listbook.h>
#include <wxCtrls/grid.h>
#include <wxCtrls/sashwin.h>
#include <wxCtrls/laywin.h>
#include <wxCtrls/display.h>
#include <wxCtrls/dirdlg.h>
#include <wxCtrls/dataobj.h>
#include <wxCtrls/msw/dc.h>
#include <wxCtrls/msw/dcclient.h>
#include <wxCtrls/dcclient.h>
#include <wxCtrls/docview.h>
#include <wxCtrls/dnd.h>
#include <wxCtrls/filedlg.h>
#include <wxCtrls/fdrepdlg.h>
#include <wxCtrls/sizer.h>
#include <wxCtrls/font.h>
#include <wxCtrls/fontdlg.h>
#include <wxCtrls/fontmap.h>
#include <wxCtrls/encconv.h>
#include <wxCtrls/fontenum.h>
#include <wxCtrls/gauge.h>
#include <wxCtrls/gdicmn.h>
#include <wxCtrls/glcanvas.h>
#include <wxCtrls/window.h>
#include <wxCtrls/busyinfo.h>
#include <wxCtrls/grid.h>
#include <wxCtrls/generic/gridctrl.h>
#include <wxCtrls/gbsizer.h>
#include <wxCtrls/grid.h>
#include <wxCtrls/generic/gridctrl.h>
#include <wxCtrls/sizer.h>
#include <wxCtrls/html/htmlwin.h>
#include <wxCtrls/html/htmlproc.h>
#include <wxCtrls/html/htmprint.h>
#include <wxCtrls/html/helpctrl.h>
#include <wxCtrls/html/htmlcell.h>
#include <wxCtrls/htmllbox.h>
#include <wxCtrls/image.h>
#include <wxCtrls/mstream.h>
#include <wxCtrls/imaglist.h>
#include <wxCtrls/laywin.h>
#include <wxCtrls/listbox.h>
#include <wxCtrls/checklst.h>
//#include <wxCtrls/univ/listbox.h>
//#include <wxCtrls/univ/checklst.h>
#include <wxCtrls/listctrl.h>
#include <wxCtrls/log.h>
#include <wxCtrls/minifram.h>
#include <wxCtrls/mdi.h>
#include <wxCtrls/dcbuffer.h>
#include <wxCtrls/dcmemory.h>
#include <wxCtrls/minifram.h>
#include <wxCtrls/notebook.h>
#include <wxCtrls/sizer.h>
#include <wxCtrls/numdlg.h>
#include <wxCtrls/checklst.h>
#include <wxCtrls/palette.h>
#include <wxCtrls/panel.h>
#include <wxCtrls/cmndata.h>
#include <wxCtrls/generic/prntdlgg.h>
#include <wxCtrls/printdlg.h>
#include <wxCtrls/print.h>
#include <wxCtrls/progdlg.h>
#include <wxCtrls/radiobut.h>
#include <wxCtrls/region.h>
#include <wxCtrls/sashwin.h>
#include <wxCtrls/scrolbar.h>
#include <wxCtrls/scrolwin.h>
#include <wxCtrls/slider.h>
#include <wxCtrls/sound.h>
#include <wxCtrls/event.h>
#include <wxCtrls/spinbutt.h>
#include <wxCtrls/spinctrl.h>
#include <wxCtrls/splash.h>
#include <wxCtrls/splitter.h>
#include <wxCtrls/statbmp.h>
#include <wxCtrls/statbox.h>
#include <wxCtrls/statline.h>
//#include <wxCtrls/stc/stc.h>
#include <wxCtrls/settings.h>
//#include <wxCtrls/tabctrl.h>
#include <wxCtrls/taskbar.h>
#include <wxCtrls/textctrl.h>
#include <wxCtrls/textdlg.h>
#include <wxCtrls/thread.h>
#include <wxCtrls/timer.h>
#include <wxCtrls/tipdlg.h>
#include <wxCtrls/tipwin.h>
#include <wxCtrls/tglbtn.h>
#include <wxCtrls/toolbar.h>
#include <wxCtrls/tooltip.h>
#include <wxCtrls/treectrl.h>
#include <wxCtrls/validate.h>
#include <wxCtrls/vlbox.h>
#include <wxCtrls/vscroll.h>
#include <wxCtrls/wizard.h>
#include <wxCtrls/datetime.h>
#include <wxCtrls/platform.h>
#include <wxCtrls/utils.h>
#include <wxCtrls/version.h>
#include <wxCtrls/xrc/xmlres.h>
#include <wxCtrls/fontenum.h>
#include <wxCtrls/fs_mem.h>
#include <wxCtrls/aui/aui.h>
#include <wxCtrls/imagbmp.h>
#include <wxCtrls/wx.h>
#include "common.h"
#include "local_events.h"
#include <stdio.h>
#include <wxCtrls/menuitem.h>
#include <wxCtrls/renderer.h>
#include <wxCtrls/checklst.h>
#include <wxCtrls/listbox.h>

#define wxUSE_OWNER_DRAWN 1
#define  wxUSE_CHECKLISTBOX 1
//included from wx/msw
#include <wxCtrls/msw/checklst.h>
#include <wxCtrls/msw/listbox.h>

#endif //PCH_H
