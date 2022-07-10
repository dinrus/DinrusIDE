#include "CtrlCore.h"

#ifdef GUI_WIN

namespace РНЦП {

#ifndef PLATFORM_WINCE

void ВинМетаФайл::иниц() {
	hemf = NULL;
}

void ВинМетаФайл::рисуй(Draw& w, const Прям& r) const {
	w.DrawRect(r, белый());
	if(!hemf)
		return;
	SystemDraw *h = dynamic_cast<SystemDraw *>(&w);
	if(h)
		PlayEnhMetaFile(h->дайУк(), hemf, r);
	else {
		Размер sz = r.дайРазм();
		SystemImageDraw iw(sz);
		рисуй(iw, sz);
		w.DrawImage(r.left, r.top, iw);
	}
}

void ВинМетаФайл::рисуй(Draw& w, int x, int y, int cx, int cy) const {
	рисуй(w, RectC(x, y, cx, cy));
}

void ВинМетаФайл::ReadClipboard() {
	очисть();
	if(::IsClipboardFormatAvailable(CF_ENHMETAFILE) && ::OpenClipboard(NULL)) {
		HENHMETAFILE hemf = (HENHMETAFILE) ::GetClipboardData(CF_ENHMETAFILE);
		if(hemf) прикрепи(hemf);
		::CloseClipboard();
	}
}

void ВинМетаФайл::WriteClipboard() const {
	if(hemf && ::OpenClipboard(NULL)) {
		::EmptyClipboard();
		::SetClipboardData(CF_ENHMETAFILE, CopyEnhMetaFile(hemf, NULL));
		::CloseClipboard();
	}
}

void ВинМетаФайл::очисть() {
	if(hemf)
		::DeleteEnhMetaFile(hemf);
	hemf = NULL;
}

/* TODO: удали picks
void ВинМетаФайл::подбери(pick_ ВинМетаФайл& src) {
	hemf = src.hemf;
	size = src.size;
	src.hemf = (HENHMETAFILE)(intptr_t) 0xffffffff;
}

void ВинМетаФайл::копируй(const ВинМетаФайл& src) {
	hemf = ::CopyEnhMetaFile(src.hemf, NULL);
	size = src.size;
}
*/
void ВинМетаФайл::прикрепи(HENHMETAFILE _hemf) {
	очисть();
	ENHMETAHEADER info;
	memset(&info, 0, sizeof(info));
	info.iType = EMR_HEADER;
	info.nSize = sizeof(info);
	info.dSignature = ENHMETA_SIGNATURE;
	if(_hemf && ::GetEnhMetaFileHeader(_hemf, sizeof(info), &info)
	   && info.rclFrame.left < info.rclFrame.right
	   && info.rclFrame.top < info.rclFrame.bottom) {
		size.cx = info.rclFrame.right - info.rclFrame.left;
		size.cy = info.rclFrame.bottom - info.rclFrame.top;
		size = 600 * size / 2540;
		hemf = _hemf;
	}
}

HENHMETAFILE ВинМетаФайл::открепи()
{
	size = Размер(0, 0);
	HENHMETAFILE out = hemf;
	hemf = NULL;
	return out;
}

#pragma pack(push, 1)
struct PLACEABLE_METAFILEHEADER
{
    DWORD   ключ;
    WORD    hmf;
    short   left, top, right, bottom;
    WORD    inch;
    DWORD   reserved;
    WORD    checksum;
};
#pragma pack(pop)

void ВинМетаФайл::уст(const void *data, dword len)
{
	очисть();

	if(len <= sizeof(ENHMETAHEADER))
		return;

	int first = Peek32le(data);

	HENHMETAFILE hemf;
	if((hemf = ::SetEnhMetaFileBits(len, (const BYTE *)data)) != NULL)
		прикрепи(hemf);
	else
	if(first == (int)0x9AC6CDD7) {
		if ( len <= 22 ) return;
		const PLACEABLE_METAFILEHEADER *mfh = (const PLACEABLE_METAFILEHEADER *)data;
		прикрепи(::SetWinMetaFileBits(len - 22, (const BYTE *)data + 22, NULL, NULL));
		size = 600 * Размер(mfh->right - mfh->left, mfh->bottom - mfh->top) / 2540;
		return;
	}
	else
		прикрепи(::SetWinMetaFileBits(len, (const BYTE *)data, NULL, NULL));
}

Ткст ВинМетаФайл::дай() const
{
	int size = ::GetEnhMetaFileBits(hemf, 0, 0);
	ТкстБуф b(size);
	::GetEnhMetaFileBits(hemf, size, (BYTE *)~b);
	return Ткст(b);
}

void ВинМетаФайл::сериализуй(Поток& s) {
	dword size = 0;
	if(s.сохраняется()) {
		if(hemf) {
			size = ::GetEnhMetaFileBits(hemf, 0, 0);
			s % size;
			Буфер<byte> буфер(size);
			::GetEnhMetaFileBits(hemf, size, буфер);
			s.SerializeRaw(буфер, size);
		}
		else
			s % size;
	}
	else {
		очисть();
		s % size;
		if(size) {
			Буфер<byte> буфер(size);
			s.SerializeRaw(буфер, size);
			HENHMETAFILE hemf = ::SetEnhMetaFileBits(size, буфер);
			прикрепи(hemf);
		}
	}
}

ВинМетаФайл::ВинМетаФайл(void *data, int len)
{
	иниц();
	уст(data, len);
}

ВинМетаФайл::ВинМетаФайл(const Ткст& data)
{
	иниц();
	уст(data);
}

ВинМетаФайл::ВинМетаФайл(HENHMETAFILE hemf) {
	иниц();
	прикрепи(hemf);
}

ВинМетаФайл::ВинМетаФайл(HENHMETAFILE hemf, Размер sz) {
	иниц();
	прикрепи(hemf);
	size = sz;
}

ВинМетаФайл::ВинМетаФайл(const char *file) {
	иниц();
	грузи(file);
}

struct cDrawWMF : DataDrawer {
	int  y;
	Размер sz;
	ВинМетаФайл wmf;

	virtual void открой(const Ткст& data, int cx, int cy);
	virtual void Render(ImageBuffer& ib);
};

void cDrawWMF::открой(const Ткст& data, int cx, int cy)
{
	y = 0;
	wmf.уст(data);
	sz = Размер(cx, cy);
}

void cDrawWMF::Render(ImageBuffer& ib)
{
	if(wmf) {
		ImageDraw iw(ib.дайРазм());
		wmf.рисуй(iw, 0, -y, sz.cx, sz.cy);
		y += ib.дайВысоту();
		Рисунок img(iw);
		ib = img;
	}
	else
		Fill(~ib, обнулиКЗСА(), ib.дайДлину());
}

ИНИЦБЛОК
{
	DataDrawer::регистрируй<cDrawWMF>("wmf");
};

void DrawWMF(Draw& w, int x, int y, int cx, int cy, const Ткст& wmf)
{
	w.DrawData(x, y, cx, cy, wmf, "wmf");
}

void DrawWMF(Draw& w, int x, int y, const Ткст& wmf)
{
	ВинМетаФайл h(wmf);
	Размер sz = h.дайРазм();
	DrawWMF(w, x, y, sz.cx, sz.cy, wmf);
}

Чертёж LoadWMF(const char *path, int cx, int cy)
{
	DrawingDraw iw(cx, cy);
	DrawWMF(iw, 0, 0, cx, cy, загрузиФайл(path));
	return iw;
}

Чертёж LoadWMF(const char *path)
{
	Ткст wmf = загрузиФайл(path);
	ВинМетаФайл h(wmf);
	if(h) {
		Размер sz = h.дайРазм();
		DrawingDraw iw(sz.cx, sz.cy);
		DrawWMF(iw, 0, 0, sz.cx, sz.cy, wmf);
		return iw;
	}
	return Null;
}

bool WinMetaFileDraw::создай(HDC hdc, int cx, int cy, const char *app, const char *имя, const char *file) {
	if(handle) закрой();

	Ткст s;
	if(app) s.конкат(app);
	s.конкат('\0');
	if(имя) s.конкат(имя);
	s.конкат('\0');

	bool del = false;
	if(!hdc) {
		hdc = ::GetWindowDC((HWND) NULL);
		del = true;
	}
	size = Размер(iscale(cx, 2540, 600), iscale(cy, 2540, 600));

	Прям r = size;

	HDC mfdc = ::CreateEnhMetaFile(hdc, file, r, имя || app ? (const char *)s : NULL);
	if(!mfdc)
		return false;
	Размер px(max(1, ::GetDeviceCaps(mfdc, HORZRES)),  max(1, ::GetDeviceCaps(mfdc, VERTRES)));
	Размер mm(max(1, ::GetDeviceCaps(mfdc, HORZSIZE)), max(1, ::GetDeviceCaps(mfdc, VERTSIZE)));
	прикрепи(mfdc);

	иниц();

	style = DOTS;

	::SetMapMode(handle, MM_ANISOTROPIC);
	::SetWindowOrgEx(handle, 0, 0, 0);
	::SetWindowExtEx(handle, 600, 600, 0);
	::SetViewportOrgEx(handle, 0, 0, 0);
	::SetViewportExtEx(handle, px.cx * 254 / (10 * mm.cx), px.cy * 254 / (10 * mm.cy), 0);
	::SelectClipRgn(mfdc, NULL);
	::IntersectClipRect(mfdc, 0, 0, cx, cy);

	if(del) {
		::ReleaseDC((HWND) NULL, hdc);
		hdc = NULL;
	}

	actual_offset = Точка(0, 0);

	return true;
}

bool WinMetaFileDraw::создай(int cx, int cy, const char *app, const char *имя, const char *file) {
	return создай(NULL, cx, cy, app, имя, file);
}

ВинМетаФайл WinMetaFileDraw::закрой() {
	HDC hdc = открепи();
	ПРОВЕРЬ(hdc);
	return ВинМетаФайл(CloseEnhMetaFile(hdc), size);
}

WinMetaFileDraw::~WinMetaFileDraw() {
	if(handle) закрой();
}

WinMetaFileDraw::WinMetaFileDraw(HDC hdc, int cx, int cy, const char *app, const char *имя, const char *file) {
	создай(hdc, cx, cy, app, имя, file);
}

WinMetaFileDraw::WinMetaFileDraw(int cx, int cy, const char *app, const char *имя, const char *file) {
	создай(cx, cy, app, имя, file);
}

Ткст AsWMF(const Чертёж& iw)
{
	Размер sz = iw.дайРазм();
	WinMetaFileDraw wd(sz.cx, sz.cy);
	wd.DrawDrawing(0, 0, sz.cx, sz.cy, iw);
	return wd.закрой().дай();
}

#endif

}

#endif
