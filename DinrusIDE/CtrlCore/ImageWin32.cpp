#include "CtrlCore.h"

#ifdef GUI_WIN

#include <shellapi.h>

namespace РНЦП {

#define LTIMING(x)  // RTIMING(x)
#define LLOG(x)     // DLOG(x)

bool ImageFallBack
// = true
;

class BitmapInfo32__ {
	Буфер<byte> data;

public:
	operator BITMAPINFO *()        { return (BITMAPINFO *)~data; }
	operator BITMAPINFOHEADER *()  { return (BITMAPINFOHEADER *)~data; }
	BITMAPINFOHEADER *operator->() { return (BITMAPINFOHEADER *)~data; }

	BitmapInfo32__(int cx, int cy);
};

BitmapInfo32__::BitmapInfo32__(int cx, int cy)
{
	data.размести(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256);
	BITMAPINFOHEADER *hi = (BITMAPINFOHEADER *) ~data;;
	memset8(hi, 0, sizeof(BITMAPINFOHEADER));
	hi->biSize = sizeof(BITMAPINFOHEADER);
	hi->biPlanes = 1;
#ifdef PLATFORM_WINCE
	hi->biBitCount = 32;
	hi->biCompression = BI_BITFIELDS;
	dword *x = (dword *)(~data + sizeof(BITMAPINFOHEADER));
	x[2] = 0xff;
	x[1] = 0xff00;
	x[0] = 0xff0000;
#else
	hi->biBitCount = 32;
	hi->biCompression = BI_RGB;
#endif
	hi->biSizeImage = 0;
	hi->biClrUsed = 0;
	hi->biClrImportant = 0;
	hi->biWidth = cx;
	hi->biHeight = -cy;
}

HBITMAP CreateBitMask(const КЗСА *data, Размер sz, Размер tsz, Размер csz, КЗСА *ct)
{
	LTIMING("CreateBitMask");
	ЗамкниГип __;
	Fill(ct, обнулиКЗСА(), tsz.cx * tsz.cy);
	int linelen = (tsz.cx + 15) >> 4 << 1;
	Буфер<byte>  mask(tsz.cy * linelen, 0xff);
	byte *m = mask;
	КЗСА *ty = ct;
	const КЗСА *sy = data;
	for(int y = 0; y < csz.cy; y++) {
		const КЗСА *s = sy;
		КЗСА *t = ty;
		for(int x = 0; x < csz.cx; x++) {
			if(s->a > 128) {
				*t = *s;
				m[x >> 3] &= ~(0x80 >> (x & 7));
			}
			else
				t->r = t->g = t->b = 0;
			t->a = 0;
			t++;
			s++;
		}
		m += linelen;
		sy += sz.cx;
		ty += tsz.cx;
	}
	return ::CreateBitmap(tsz.cx, tsz.cy, 1, 1, mask);
}

void устПоверхность(HDC dc, const Прям& dest, const КЗСА *pixels, Размер srcsz, Точка srcoff)
{
	LTIMING("устПоверхность");
	ЗамкниГип __;
	BitmapInfo32__ bi(srcsz.cx, srcsz.cy);
	::SetDIBitsToDevice(dc, dest.left, dest.top, dest.дайШирину(), dest.дайВысоту(),
	                    srcoff.x, -srcoff.y - dest.устВысоту() + srcsz.cy, 0, srcsz.cy, pixels, bi,
	                    DIB_RGB_COLORS);
}

void устПоверхность(HDC dc, int x, int y, int cx, int cy, const КЗСА *pixels)
{
	устПоверхность(dc, RectC(x, y, cx, cy), pixels, Размер(cx, cy), Точка(0, 0));
}

void устПоверхность(SystemDraw& w, int x, int y, int cx, int cy, const КЗСА *pixels)
{
	устПоверхность(w.дайУк(), x, y, cx, cy, pixels);
}

void устПоверхность(SystemDraw& w, const Прям& dest, const КЗСА *pixels, Размер psz, Точка poff)
{
	устПоверхность(w.дайУк(), dest, pixels, psz, poff);
}

class DrawSurface : БезКопий {
	int          x, y;
	Размер         size;
	КЗСА        *pixels;
	HDC          dc, dcMem;
	HBITMAP      hbmp, hbmpOld;

	void  иниц(SystemDraw& w, int x, int y, int cx, int cy);
	КЗСА* Строка(int i) const { ПРОВЕРЬ(i >= 0 && i < size.cy); return (КЗСА *)pixels + size.cx * (size.cy - i - 1); }

public:
	operator КЗСА *()                   { return pixels; }
	Размер        дайРазм() const         { return size; }
	КЗСА       *operator[](int i)       { return Строка(i); }
	const КЗСА *operator[](int i) const { return Строка(i); }
	int         GetLineDelta() const    { return -size.cx; }

	DrawSurface(SystemDraw& w, const Прям& r);
	DrawSurface(SystemDraw& w, int x, int y, int cx, int cy);
	~DrawSurface();
};

void DrawSurface::иниц(SystemDraw& w, int _x, int _y, int cx, int cy)
{
	LTIMING("DrawSurface");
	ЗамкниГип __;
	dc = w.дайУк();
	size = Размер(cx, cy);
	x = _x;
	y = _y;
	dcMem = ::CreateCompatibleDC(dc);
	BitmapInfo32__ bi(cx, cy);
	hbmp = CreateDIBSection(dc, bi, DIB_RGB_COLORS, (void **)&pixels, NULL, 0);
	hbmpOld = (HBITMAP) ::SelectObject(dcMem, hbmp);
	::BitBlt(dcMem, 0, 0, cx, cy, dc, x, y, SRCCOPY);
}

DrawSurface::DrawSurface(SystemDraw& w, const Прям& r)
{
	иниц(w, r.left, r.top, r.устШирину(), r.устВысоту());
}

DrawSurface::DrawSurface(SystemDraw& w, int x, int y, int cx, int cy)
{
	иниц(w, x, y, cx, cy);
}

DrawSurface::~DrawSurface()
{
	ЗамкниГип __;
	::BitBlt(dc, x, y, size.cx, size.cy, dcMem, 0, 0, SRCCOPY);
	::DeleteObject(::SelectObject(dcMem, hbmpOld));
	::DeleteDC(dcMem);
}

#ifndef PLATFORM_WINCE
typedef BOOL (WINAPI *tAlphaBlend)(HDC hdcDest, int nXOriginDest, int nYOriginDest,
                                   int nWidthDest, int nHeightDest,
                                   HDC hdcSrc, int nXOriginSrc, int nYOriginSrc,
                                   int nWidthSrc, int nHeightSrc, BLENDFUNCTION blendFunction);

static tAlphaBlend fnAlphaBlend()
{
	ЗамкниГип __;
	static tAlphaBlend pSet;
	static bool inited = false;
	if(!inited) {
		inited = true;
		if(HMODULE hDLL = LoadLibrary("msimg32.dll"))
			pSet = (tAlphaBlend) GetProcAddress(hDLL, "AlphaBlend");
	}
	return pSet;
}
#endif

struct ImageSysData {
	Рисунок       img;
	HBITMAP     hbmp;
	HBITMAP     hmask;
	HBITMAP     himg;
	КЗСА       *section;
	
	void иниц(const Рисунок& img);
	void CreateHBMP(HDC dc, const КЗСА *data);
	void рисуй(SystemDraw& w, int x, int y, const Прям& src, Цвет c);
	
	~ImageSysData();
};

void ImageSysData::иниц(const Рисунок& _img)
{
	img = _img;
	hbmp = hmask = himg = NULL;
	LLOG("ImageSysData::иниц " << img.GetSerialId() << " " << img.дайРазм());
}

ImageSysData::~ImageSysData()
{
	SysImageReleased(img);
	if(hbmp) {
		ЗамкниГип __;
		DeleteObject(hbmp);
	}
	if(hmask) {
		ЗамкниГип __;
		DeleteObject(hmask);
	}
	if(himg) {
		ЗамкниГип __;
		DeleteObject(himg);
	}
	hbmp = hmask = himg = NULL;
	img.очисть();
}

void ImageSysData::CreateHBMP(HDC dc, const КЗСА *data)
{
	LTIMING("CreateHBMP");
	LLOG("Creating BMP for " << img.GetSerialId() << ' ' << img.дайРазм());
	Размер sz = img.дайРазм();
	BitmapInfo32__ bi(sz.cx, sz.cy);
	HDC dcMem = ::CreateCompatibleDC(dc);
	КЗСА *pixels;
	HBITMAP hbmp32 = CreateDIBSection(dcMem, bi, DIB_RGB_COLORS, (void **)&pixels, NULL, 0);
	HDC hbmpOld = (HDC) ::SelectObject(dcMem, hbmp32);
	копируй(pixels, data, img.дайДлину());
	HDC dcMem2 = ::CreateCompatibleDC(dc);
	hbmp = ::CreateCompatibleBitmap(dc, sz.cx, sz.cy);
	HBITMAP o2 = (HBITMAP)::SelectObject(dcMem2, hbmp);
	::BitBlt(dcMem2, 0, 0, sz.cx, sz.cy, dcMem, 0, 0, SRCCOPY);
	::SelectObject(dcMem2, o2);
	::DeleteDC(dcMem2);
	::SelectObject(dcMem, hbmpOld);
	::DeleteObject(hbmp32);
	::DeleteDC(dcMem);
}

void ImageSysData::рисуй(SystemDraw& w, int x, int y, const Прям& src, Цвет c)
{
	HDC dc = w.дайУк();
	Размер sz = img.дайРазм();
	int  len = sz.cx * sz.cy;
	Прям sr = src & sz;
	Размер ssz = sr.размер();
	if(sr.пустой())
		return;

	if(fnAlphaBlend() && пусто_ли(c) && !ImageFallBack &&
	   !(w.IsPrinter() && (GetDeviceCaps(dc, SHADEBLENDCAPS) & (SB_PIXEL_ALPHA|SB_PREMULT_ALPHA)) !=
	                     (SB_PIXEL_ALPHA|SB_PREMULT_ALPHA))) {
		if(!himg) {
			LTIMING("Рисунок Alpha create");
			BitmapInfo32__ bi(sz.cx, sz.cy);
			himg = CreateDIBSection(ScreenHDC(), bi, DIB_RGB_COLORS, (void **)&section, NULL, 0);
			if(!himg) return; // Return on release.
			копируй(section, ~img, img.дайДлину());
		}
		LTIMING("Рисунок Alpha blit");
		BLENDFUNCTION bf;
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 255;
		bf.AlphaFormat = AC_SRC_ALPHA;
		HDC dcMem = w.GetCompatibleDC();
		HBITMAP o = (HBITMAP)::SelectObject(dcMem, himg);
		fnAlphaBlend()(dc, x, y, ssz.cx, ssz.cy, dcMem, sr.left, sr.top, ssz.cx, ssz.cy, bf);
		::SelectObject(dcMem, o);
		SysImageRealized(img);
	}
	else {
		LTIMING("Рисунок Alpha sw");
		DrawSurface sf(w, x, y, ssz.cx, ssz.cy);
		КЗСА *t = sf;
		if(w.IsPrinter()) // We have got here because printer does not support alpha blending
			Fill(t, белый(), ssz.cx * ssz.cy);
		for(int i = sr.top; i < sr.bottom; i++) {
			if(пусто_ли(c))
				AlphaBlendOpaque(t, img[i] + sr.left, ssz.cx);
			else
				AlphaBlendOpaque(t, img[i] + sr.left, ssz.cx, c);
			t += ssz.cx;
		}
	}
}

struct ImageSysDataMaker : LRUCache<ImageSysData, int64>::Делец {
	Рисунок img;

	virtual int64  Ключ() const                      { return img.GetSerialId(); }
	virtual int    сделай(ImageSysData& object) const { object.иниц(img); return img.дайДлину(); }
};

void SystemDraw::SysDrawImageOp(int x, int y, const Рисунок& img, const Прям& src, Цвет color)
{
	LLOG("SysDrawImageOp " << img.GetSerialId() << ' ' << img.дайРазм());

	ЗамкниГип __;
	if(img.дайДлину() == 0)
		return;

	Размер sz = img.дайРазм();
	int kind = img.GetKindNoScan();
	if(kind == IMAGE_OPAQUE && !пусто_ли(color)) {
		DrawRect(x, y, sz.cx, sz.cy, color);
		return;
	}
	if(kind == IMAGE_OPAQUE && (!IsPrinter() || (GetDeviceCaps(дайУк(), RASTERCAPS) & RC_DIBTODEV)) && src == img.дайРазм()) {
		LTIMING("Рисунок Opaque direct set");
		устПоверхность(*this, x, y, sz.cx, sz.cy, ~img);
		return;
	}

	ImageSysDataMaker m;
	static LRUCache<ImageSysData, int64> cache;
	static int Rsz;
	Rsz += img.дайДлину();
	if(Rsz > 200 * 200) { // we do not want to do this for each small image painted...
		Rsz = 0;
		cache.удали([](const ImageSysData& object) { return object.img.GetRefCount() == 1; });
	}
	LLOG("SysImage cache pixels " << cache.дайРазм() << ", count " << cache.дайСчёт());
	sz = Ктрл::GetVirtualScreenArea().дайРазм();
	m.img = IsPrinter() && GetDeviceCaps(дайУк(), NUMCOLORS) == 2 ? Dither(img, 360) : img; // If printer does not support color, dither
	cache.дай(m).рисуй(*this, x, y, src, color);
	cache.сожми(4 * sz.cx * sz.cy, 1000);
}

void ImageDraw::Section::иниц(int cx, int cy)
{
	ЗамкниГип __;
	dc = ::CreateCompatibleDC(ScreenHDC());
	BitmapInfo32__ bi(cx, cy);
	hbmp = CreateDIBSection(dc, bi, DIB_RGB_COLORS, (void **)&pixels, NULL, 0);
	hbmpOld = (HBITMAP) ::SelectObject(dc, hbmp);
}

ImageDraw::Section::~Section()
{
	ЗамкниГип __;
	::DeleteObject(::SelectObject(dc, hbmpOld));
	::DeleteDC(dc);
}

void ImageDraw::иниц()
{
	ЗамкниГип __;
	rgb.иниц(size.cx, size.cy);
	a.иниц(size.cx, size.cy);
	прикрепи(rgb.dc);
	InitClip(size);
	alpha.прикрепи(a.dc);
	alpha.InitClip(size);
	has_alpha = false;
}

Draw& ImageDraw::Alpha()
{
	if(!has_alpha) {
		alpha.DrawRect(size, серыйЦвет(0));
		has_alpha = true;
	}
	return alpha;
}

Рисунок ImageDraw::дай(bool pm) const
{
	ImageBuffer b(size);
	int n = size.cx * size.cy;
	memcpy(~b, rgb.pixels, n * sizeof(КЗСА));
	const КЗСА *s = a.pixels;
	const КЗСА *e = a.pixels + n;
	КЗСА *t = b;
	if(has_alpha) {
		while(s < e) {
			t->a = s->r;
			t++;
			s++;
		}
		if(pm)
			Premultiply(b);
		b.SetKind(IMAGE_ALPHA);
	}
	else {
		while(s < e) {
			t->a = 255;
			t++;
			s++;
		}
		b.SetKind(IMAGE_OPAQUE);
	}
	return b;
}

ImageDraw::operator Рисунок() const
{
	return дай(true);
}

Рисунок ImageDraw::GetStraight() const
{
	return дай(false);
}

ImageDraw::ImageDraw(Размер sz)
{
	size = sz;
	иниц();
}

ImageDraw::ImageDraw(int cx, int cy)
{
	size = Размер(cx, cy);
	иниц();
}

ImageDraw::~ImageDraw()
{
	открепи();
	alpha.открепи();
}

#ifdef PLATFORM_WINCE

Рисунок Рисунок::Arrow() { return Null; }
Рисунок Рисунок::жди() { return Null; }
Рисунок Рисунок::IBeam() { return Null; }
Рисунок Рисунок::No() { return Null; }
Рисунок Рисунок::SizeAll() { return Null; }
Рисунок Рисунок::SizeHorz() { return Null; }
Рисунок Рисунок::SizeVert() { return Null; }
Рисунок Рисунок::SizeTopLeft() { return Null; }
Рисунок Рисунок::SizeTop() { return Null; }
Рисунок Рисунок::SizeTopRight() { return Null; }
Рисунок Рисунок::SizeLeft() { return Null; }
Рисунок Рисунок::SizeRight() { return Null; }
Рисунок Рисунок::SizeBottomLeft() { return Null; }
Рисунок Рисунок::SizeBottom() { return Null; }
Рисунок Рисунок::SizeBottomRight() { return Null; }

#else

static Рисунок sWin32Icon(HICON icon, bool cursor)
{
	ЗамкниГип __;
	ICONINFO iconinfo;
	if(!icon || !GetIconInfo(icon, &iconinfo))
		return Рисунок();
	BITMAP bm;
	::GetObject((HGDIOBJ)iconinfo.hbmMask, sizeof(BITMAP), (LPVOID)&bm);
	HDC dcMem = ::CreateCompatibleDC(NULL);
	Размер sz(bm.bmWidth, bm.bmHeight);
	BitmapInfo32__ bi(sz.cx, sz.cy);
	int len = sz.cx * sz.cy;
	Буфер<КЗСА> mask(len);
	::SelectObject(dcMem, iconinfo.hbmColor);
	::GetDIBits(dcMem, iconinfo.hbmMask, 0, sz.cy, ~mask, bi, DIB_RGB_COLORS);
	ImageBuffer b(sz.cx, iconinfo.hbmColor ? sz.cy : sz.cy / 2);
	b.SetHotSpot(Точка(iconinfo.xHotspot, iconinfo.yHotspot));
	if(iconinfo.hbmColor) {
		::SelectObject(dcMem, iconinfo.hbmColor);
		::GetDIBits(dcMem, iconinfo.hbmColor, 0, sz.cy, ~b, bi, DIB_RGB_COLORS);
		КЗСА *s = ~b;
		КЗСА *e = s + len;
		КЗСА *m = mask;
		while(s < e) {
			if(s->a != 255 && s->a != 0) {
				Premultiply(b);
				goto alpha;
			}
			s++;
		}
		s = ~b;
		while(s < e) {
			s->a = m->r ? 0 : 255;
			s++;
			m++;
		}
	}
	else {
		len /= 2;
		КЗСА *s = ~b;
		КЗСА *e = s + len;
		КЗСА *c = mask + len;
		КЗСА *m = mask;
		while(s < e) {
			s->a = (m->r & ~c->r) ? 0 : 255;
			s->r = s->g = s->b = (c->r & ~m->r) ? 255 : 0;
			s++;
			m++;
			c++;
		}
	}
alpha:
	::DeleteDC(dcMem);
	::DeleteObject(iconinfo.hbmColor);
	::DeleteObject(iconinfo.hbmMask);
	Рисунок img(b);
	::DestroyIcon(icon);
	return img;
}

Рисунок SystemDraw::Win32IconCursor(LPCSTR id, int iconsize, bool cursor)
{
	HICON icon;
	if(cursor)
		icon = (HICON)LoadCursor(0, id);
	else
		if(iconsize)
			icon = (HICON)LoadImage(GetModuleHandle(NULL), id,
			                        IMAGE_ICON, iconsize, iconsize, LR_DEFAULTCOLOR);
		else
			icon = LoadIcon(0, id);
	Рисунок img = sWin32Icon(icon, cursor);
	if(cursor)
		img.SetAuxData(reinterpret_cast<uint64>(id));
	return img;
}

Рисунок Win32DllIcon(const char *dll, int ii, bool large)
{
	HICON icon;
	if(ExtractIconEx(dll, ii, large ? &icon : NULL, large ? NULL : &icon, 1) == 1)
		return sWin32Icon(icon, false);
	return Null;
}

Рисунок Win32Icon(LPCSTR id, int iconsize)
{
	return SystemDraw::Win32IconCursor(id, iconsize, false);
}

Рисунок Win32Icon(int id, int iconsize)
{
	return Win32Icon(MAKEINTRESOURCE(id), iconsize);
}

Рисунок Win32Cursor(LPCSTR id)
{
	return SystemDraw::Win32IconCursor(id, 0, true);
}

Рисунок Win32Cursor(int id)
{
	return Win32Cursor(MAKEINTRESOURCE(id));
}

HICON SystemDraw::IconWin32(const Рисунок& img, bool cursor)
{
	ЗамкниГип __;
	if(img.пустой())
		return NULL;
	if(cursor) {
		LPCSTR id = reinterpret_cast<LPCSTR>(img.GetAuxData());
		if(id)
			return (HICON)LoadCursor(0, id);
	}
	Размер sz = img.дайРазм();
	ICONINFO iconinfo;
	iconinfo.fIcon = !cursor;
	Точка p = img.GetHotSpot();
	iconinfo.xHotspot = p.x;
	iconinfo.yHotspot = p.y;
	Размер tsz = sz;
	Размер csz = Размер(min(tsz.cx, sz.cx), min(tsz.cy, sz.cy));
	if(!ImageFallBack) {
		КЗСА *pixels;
		BitmapInfo32__ bi(tsz.cx, tsz.cy);
		HDC dcMem = ::CreateCompatibleDC(NULL);
		iconinfo.hbmColor = ::CreateDIBSection(dcMem, bi, DIB_RGB_COLORS, (void **)&pixels, NULL, 0);
		iconinfo.hbmMask = ::CreateBitmap(tsz.cx, tsz.cy, 1, 1, NULL);
		Fill(pixels, обнулиКЗСА(), tsz.cx * tsz.cy);
		for(int y = 0; y < csz.cy; y++)
			memcpy(pixels + y * tsz.cx, img[y], csz.cx * sizeof(КЗСА));
		::DeleteDC(dcMem);
	}
	else {
		Буфер<КЗСА>  h(tsz.cx * tsz.cy);
		HDC dc = ::GetDC(NULL);
		BitmapInfo32__ bi(tsz.cx, tsz.cy);
		iconinfo.hbmMask = CreateBitMask(~img, sz, tsz, csz, h);
		iconinfo.hbmColor = ::CreateDIBitmap(dc, bi, CBM_INIT, h, bi, DIB_RGB_COLORS);
		::ReleaseDC(NULL, dc);
	}

	HICON icon = ::CreateIconIndirect(&iconinfo);
	::DeleteObject(iconinfo.hbmColor);
	::DeleteObject(iconinfo.hbmMask);
	return icon;
}

#define WCURSOR_(x)\
{ Рисунок m; INTERLOCKED { static Рисунок img = Win32Cursor(x); m = img; } return m; }

Рисунок Рисунок::Arrow() WCURSOR_(IDC_ARROW)
Рисунок Рисунок::жди() WCURSOR_(IDC_WAIT)
Рисунок Рисунок::IBeam() WCURSOR_(IDC_IBEAM)
Рисунок Рисунок::No() WCURSOR_(IDC_NO)
Рисунок Рисунок::SizeAll() WCURSOR_(IDC_SIZEALL)
Рисунок Рисунок::SizeHorz()	WCURSOR_(IDC_SIZEWE)
Рисунок Рисунок::SizeVert() WCURSOR_(IDC_SIZENS)
Рисунок Рисунок::SizeTopLeft() WCURSOR_(IDC_SIZENWSE)
Рисунок Рисунок::SizeTop() WCURSOR_(IDC_SIZENS)
Рисунок Рисунок::SizeTopRight() WCURSOR_(IDC_SIZENESW)
Рисунок Рисунок::SizeLeft() WCURSOR_(IDC_SIZEWE)
Рисунок Рисунок::SizeRight() WCURSOR_(IDC_SIZEWE)
Рисунок Рисунок::SizeBottomLeft() WCURSOR_(IDC_SIZENESW)
Рисунок Рисунок::SizeBottom() WCURSOR_(IDC_SIZENS)
Рисунок Рисунок::SizeBottomRight()  WCURSOR_(IDC_SIZENWSE)
Рисунок Рисунок::Cross() WCURSOR_(IDC_CROSS)
Рисунок Рисунок::Hand() WCURSOR_(IDC_HAND)

#endif

}

#endif
