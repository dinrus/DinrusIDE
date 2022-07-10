#include "CtrlLib.h"

#ifdef GUI_WIN
#ifndef PLATFORM_WINCE

#include <commdlg.h>
#include <cderr.h>

#define Ук Ук_
#define byte byte_
#define CY win32_CY_

#include <winnls.h>
#include <winnetwk.h>

#include <wincon.h>

#ifdef COMPILER_MINGW
#undef CY
#endif

#include <shlobj.h>

#undef Ук
#undef byte
#undef CY

#endif
#endif


namespace РНЦП {

#ifdef GUI_WIN

#ifndef PLATFORM_WINCE

#define LLOG(x)

FileSelNative::FileSelNative() {
	activetype = 0;
	readonly = rdonly = multi = false;
	asking = true;
}

FileSelNative& FileSelNative::Type(const char *имя, const char *ext) {
	FileType& t = тип.добавь();
	t.имя = имя;
	t.ext = ext;
	return *this;
}

FileSelNative& FileSelNative::AllFilesType() {
	return Type(t_("All files"), "*.*");
}

void FileSelNative::сериализуй(Поток& s) {
	int version = 2;
	s / version;
	s / activetype % activedir;
	if(version < 2) {
		Ткст dummy;
		s % dummy;
	}
}

Ткст FileSelNative::дай() const {
	return filename.дайСчёт() ? filename[0] : Ткст::дайПроц();
}

#ifdef _ОТЛАДКА
static Ткст Dump(Ктрл *ctrl)
{
	if(!ctrl)
		return "NULL";
	ТкстПоток out;
	ctrl -> Dump(out);
	return out;
}
#endif

Прям MonitorRectForHWND(HWND hwnd);

static UINT_PTR CALLBACK sCenterHook(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LLOG("msg = " << (int)msg << ", wParam = " << фмт("%08x", (int)wParam)
	    << ", lParam = " << фмт("%08x", (int)lParam)
		<< ", focus = " << Dump(Ктрл::GetFocusCtrl()));

	if(msg == WM_NOTIFY && ((LPNMHDR)lParam) -> code == CDN_INITDONE) {
		//&& !(GetWindowLong(hdlg, GWL_STYLE) & WS_CHILD)) {
		while(GetWindowLong(hdlg, GWL_STYLE) & WS_CHILD) {
			HWND p = GetParent(hdlg);
			if(!p) break;
			hdlg = p;
		}
		Прям dr, pr;
		::GetWindowRect(hdlg, dr);
		Прям wa = Ктрл::GetPrimaryWorkArea();
		if(HWND owner = ::GetWindow(hdlg, GW_OWNER)) {
			::GetWindowRect(owner, pr);
			wa = MonitorRectForHWND(owner);
		}
		else
			pr = wa;
		pr.пересек(wa);
		Точка p = pr.позЦентра(dr.размер());
		::SetWindowPos(hdlg, NULL, p.x, p.y, 0, 0,
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
//		::MoveWindow(hdlg, p.x, p.y, dr.устШирину(), dr.устВысоту(), false);
	}
	return 0;
}

#ifndef OFN_ENABLESIZING
#define OFN_ENABLESIZING             0x00800000
#endif

INT CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
	if (uMsg==BFFM_INITIALIZED) {
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);
		SendMessage(hwnd, BFFM_SETEXPANDED, TRUE, pData);
	}
	return 0;
}

bool FileSelNative::ExecuteSelectDir(const char *title)
{
	Вектор<Вектор<char16>> s16;
	auto W32 = [&](const Ткст& s) -> char16* {
		auto& h = s16.добавь();
		h = вСисНабсимШ(s);
		return h;
	};

	Ткст ret;

	BROWSEINFOW br;
	memset(&br, 0, sizeof(BROWSEINFO));
	Ктрл *q = Ктрл::GetActiveWindow();
	if(q) br.hwndOwner = q->дайОУК();
	br.lpfn = BrowseCallbackProc;
	br.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	br.lpszTitle = W32(title);
	br.lParam = (LPARAM)W32(activedir);

	LPITEMIDLIST pidl = NULL;
	if((pidl = SHBrowseForFolderW(&br)) != NULL) {
		char16 буфер[MAX_PATH];
		if(SHGetPathFromIDListW(pidl, буфер))
			filename << изСисНабсимаШ(буфер);
		else
			filename << Null;
        IMalloc *pMalloc;
        if(SHGetMalloc(&pMalloc) == NOERROR) {
            pMalloc->Free(pidl);
            pMalloc->Release();
        }
		return true;
	}

	return false;
}

bool FileSelNative::выполни(bool open, const char *dlgtitle) {
	Вектор<Вектор<char16>> s16;
	auto W32 = [&](const Ткст& s) -> char16* {
		auto& h = s16.добавь();
		h = вСисНабсимШ(s);
		return h;
	};
	Ткст filter;
	for(int i = 0; i < тип.дайСчёт(); i++) {
		filter.конкат(тип[i].имя);
		filter.конкат('\0');
		filter.конкат(тип[i].ext);
		filter.конкат('\0');
	}
	OPENFILENAMEW ofn;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	Ктрл *q = Ктрл::GetActiveWindow();
	if(q) ofn.hwndOwner = q->дайОУК();
	ofn.Flags = OFN_ENABLESIZING|OFN_ENABLEHOOK|OFN_EXPLORER;
	if(asking) ofn.Flags |= (open ? OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST : OFN_OVERWRITEPROMPT);
	if(!rdonly) ofn.Flags |= OFN_HIDEREADONLY;
	if(multi) ofn.Flags |= OFN_ALLOWMULTISELECT;
	ofn.lpstrFilter = W32(filter);
	ofn.nFilterIndex = activetype;
	ofn.lpstrInitialDir = W32(activedir);
	ofn.lpfnHook = sCenterHook;
	int bufsize = ofn.nMaxFile = (multi ? 32000 : _MAX_PATH);
	Буфер<char16> буфер(bufsize);
	*(ofn.lpstrFile = буфер) = 0;
	if(!filename.пустой())
	{
		Ткст out;
		for(int i = 0; i < filename.дайСчёт(); i++)
		{
			if(*ofn.lpstrInitialDir == 0 && ФайлПоиск().ищи(приставьИмяф(дайДиректориюФайла(filename[i]), "*")))
				ofn.lpstrInitialDir = W32(дайДиректориюФайла(filename[i]));
			if(!open || файлЕсть(filename[i]))
			{
				Ткст фн = дайИмяф(filename[i]);
				if(!пусто_ли(фн))
				{
					if(multi && фн.найди(' ') >= 0)
						out << W32(Ткст() << '\"' << фн << '\"');
					else
						out << W32(фн);
					out.конкат(0);
				}
			}
		}
		int l = min(out.дайДлину(), bufsize - 1);
		memcpy(буфер, out, l + 1);
	}

	if(dlgtitle)
		ofn.lpstrTitle = W32(dlgtitle);
	else if(open)
		ofn.lpstrTitle = W32(t_("открой.."));
	else
		ofn.lpstrTitle = W32(t_("сохрани as"));
	if(!defext.пустой())
		ofn.lpstrDefExt = W32(defext);
	bool res = !!(open ? GetOpenFileNameW : GetSaveFileNameW)(&ofn);
	if(!res && CommDlgExtendedError() == FNERR_INVALIDFILENAME)
	{
		*буфер = 0;
		res = !!(open ? GetOpenFileNameW : GetSaveFileNameW)(&ofn);
	}
	if(!res && CommDlgExtendedError() == FNERR_INVALIDFILENAME)
	{
		ofn.lpstrInitialDir = W32("");
		res = !!(open ? GetOpenFileNameW : GetSaveFileNameW)(&ofn);
	}
	if(!res)
		return false;
	filename.очисть();
	activetype = ofn.nFilterIndex;
	if(multi) {
		const char16 *s = ofn.lpstrFile;
		activedir = изСисНабсимаШ(s);
		s += strlen16(s);
		if(s[1] == 0)
			filename.добавь() = activedir;
		else
			do
				filename.добавь() = приставьИмяф(activedir, изСисНабсимаШ(++s));
			while((s += strlen16(s))[1]);
	}
	else {
		filename.добавь(изСисНабсимаШ(ofn.lpstrFile));
		activedir = дайДиректориюФайла(filename[0]);
	}
	readonly = ofn.Flags & OFN_READONLY ? TRUE : FALSE;
	return true;
}

#endif
#endif

}
