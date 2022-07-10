#include "CtrlCore.h"

#ifdef GUI_WIN

namespace РНЦП {

#define LLOG(x) // LOG(x)
#define LTIMING(x) // RTIMING(x)

static COLORREF sLightGray;

Прям SystemDraw::GetVirtualScreenArea()
{
	ЗамкниГип __;
	return RectC(GetSystemMetrics(SM_XVIRTUALSCREEN),
		GetSystemMetrics(SM_YVIRTUALSCREEN),
		GetSystemMetrics(SM_CXVIRTUALSCREEN),
		GetSystemMetrics(SM_CYVIRTUALSCREEN));
}

dword SystemDraw::GetInfo() const
{
	return DATABANDS|(native || !(style & DOTS) ? style|NATIVE : style);
}

Размер SystemDraw::GetPageSize() const
{
	return native && Dots() ? nativeSize : pageSize;
}

Размер SystemDraw::GetNativeDpi() const
{
	return nativeDpi;
}

#ifndef PLATFORM_WINCE
void добавь(LOGPALETTE *pal, int r, int g, int b)
{
	pal->palPalEntry[pal->palNumEntries].peRed   = min(r, 255);
	pal->palPalEntry[pal->palNumEntries].peGreen = min(g, 255);
	pal->palPalEntry[pal->palNumEntries].peBlue  = min(b, 255);
	pal->palPalEntry[pal->palNumEntries++].peFlags = PC_NOCOLLAPSE;
}

HPALETTE GetQlibPalette()
{
	static HPALETTE hQlibPalette;
	if(hQlibPalette) return hQlibPalette;
	SystemDraw::InitColors();
	LOGPALETTE *pal = (LOGPALETTE *) new byte[sizeof(LOGPALETTE) + 256 * sizeof(PALETTEENTRY)];
	pal->palNumEntries = 0;
	pal->palVersion    = 0x300;
	for(int r = 0; r < 6; r++)
		for(int g = 0; g < 6; g++)
			for(int b = 0; b < 6; b++)
				добавь(pal, 255 * r / 5, 255 * g / 5, 255 * b / 5);
	for(int q = 0; q <= 16; q++)
		добавь(pal, 16 * q, 16 * q, 16 * q);
	добавь(pal, дайЗнК(sLightGray), дайЗнЗ(sLightGray), дайЗнС(sLightGray));
	hQlibPalette = CreatePalette(pal);
	delete[] pal;
	return hQlibPalette;
}
#endif

SystemDraw& ScreenInfo()
{
	static ScreenDraw sd(true);
	return sd;
}

HDC ScreenHDC()
{
	static HDC hdc;
	ONCELOCK {
		hdc = CreateDC("DISPLAY", NULL, NULL, NULL);
	}
	return hdc;
}

static bool _AutoPalette = true;
bool SystemDraw::AutoPalette() { return _AutoPalette; }
void SystemDraw::SetAutoPalette(bool ap) { _AutoPalette = ap; }

COLORREF SystemDraw::дайЦвет(Цвет c) const {
	COLORREF color = c;
#ifdef PLATFORM_WINCE
	return color;
#else
	if(!palette)
		return color;
	static Индекс<dword> *SColor;
	ONCELOCK {
		static Индекс<dword> StaticColor;
		StaticColor << дайКЗС(0x00, 0x00, 0x00) << дайКЗС(0x80, 0x00, 0x00) << дайКЗС(0x00, 0x80, 0x00)
					<< дайКЗС(0x80, 0x80, 0x00) << дайКЗС(0x00, 0x00, 0x80) << дайКЗС(0x80, 0x00, 0x80)
					<< дайКЗС(0x00, 0x80, 0x80) << дайКЗС(0xC0, 0xC0, 0xC0) << дайКЗС(0xC0, 0xDC, 0xC0)
					<< дайКЗС(0xA6, 0xCA, 0xF0) << дайКЗС(0xFF, 0xFB, 0xF0) << дайКЗС(0xA0, 0xA0, 0xA4)
					<< дайКЗС(0x80, 0x80, 0x80) << дайКЗС(0xFF, 0x00, 0x00) << дайКЗС(0x00, 0xFF, 0x00)
					<< дайКЗС(0xFF, 0xFF, 0x00) << дайКЗС(0x00, 0x00, 0xFF) << дайКЗС(0xFF, 0x00, 0xFF)
					<< дайКЗС(0x00, 0xFF, 0xFF) << дайКЗС(0xFF, 0xFF, 0xFF);
		SColor = &StaticColor;
	}
	if(color16 || !AutoPalette())
		return GetNearestColor(handle, color);
	if(SColor->найди(color) >= 0)
		return color;
	if(color == sLightGray)
		return PALETTEINDEX(216 + 17);
	int r = дайЗнК(color);
	int g = дайЗнЗ(color);
	int b = дайЗнС(color);
	return PALETTEINDEX(r == g && g == b ? (r + 8) / 16 + 216
		                                 : (r + 25) / 51 * 36 +
		                                   (g + 25) / 51 * 6 +
		                                   (b + 25) / 51);
#endif
}

void SystemDraw::InitColors()
{
}

void SystemDraw::устЦвет(Цвет color)
{
	ЗамкниГип __;
	LLOG("устЦвет " << color);
	if(color != lastColor) {
		LLOG("Setting, lastColor:" << фмтЦелГекс(lastColor.дайСырой())
			<< " color:" << фмтЦелГекс(color.дайСырой()) <<
		     " дайЦвет:" << фмтЦелГекс(дайЦвет(color)) << " palette:" << palette);
		HBRUSH oldBrush = actBrush;
		HBRUSH h;
		if(!пусто_ли(color))
			h = (HBRUSH) SelectObject(handle, actBrush = CreateSolidBrush(дайЦвет(color)));
		else {
			HGDIOBJ empty = GetStockObject(HOLLOW_BRUSH);
			h = (HBRUSH) SelectObject(handle, empty);
			actBrush = NULL;
		}
		ПРОВЕРЬ(h);
		if(!orgBrush) orgBrush = h;
		if(oldBrush) DeleteObject(oldBrush);
		lastColor = color;
	}
}

void SystemDraw::SetDrawPen(int width, Цвет color) {
	ЗамкниГип __;
	if(пусто_ли(width))
		width = PEN_NULL;
	if(width != lastPen || color != lastPenColor) {
		static int penstyle[] = {
			PS_NULL, PS_SOLID, PS_DASH,
		#ifndef PLATFORM_WINCE
			PS_DOT, PS_DASHDOT, PS_DASHDOTDOT
		#endif
		};
		HPEN oldPen = actPen;
		actPen = CreatePen(width < 0 ? penstyle[-width - 1] : PS_SOLID,
			               width < 0 ? 0 : width, дайЦвет(color));
		HPEN h = (HPEN) SelectObject(handle, actPen);
		if(!orgPen) orgPen = h;
		if(oldPen) DeleteObject(oldPen);
		lastPen = width;
		lastPenColor = color;
	}
}

void SystemDraw::SetOrg() {
	ЗамкниГип __;
#ifdef PLATFORM_WINCE
	::SetViewportOrgEx(handle, actual_offset.x, actual_offset.y, 0);
#else
	LLOG("SystemDraw::SetOrg: clip = " << GetClip() << ", offset = " << actual_offset);
	::SetWindowOrgEx(handle, -actual_offset.x, -actual_offset.y, 0);
	LLOG("//SystemDraw::SetOrg: clip = " << GetClip());
#endif
}

#ifndef PLATFORM_WINCE
Точка SystemDraw::LPtoDP(Точка p) const {
	ЗамкниГип __;
	::LPtoDP(handle, p, 1);
	return p;
}

Точка SystemDraw::DPtoLP(Точка p) const {
	ЗамкниГип __;
	::DPtoLP(handle, p, 1);
	return p;
}

Прям  SystemDraw::LPtoDP(const Прям& r) const {
	ЗамкниГип __;
	Прям w = r;
	::LPtoDP(handle, reinterpret_cast<POINT *>(&w), 2);
	return w;
}

Прям  SystemDraw::DPtoLP(const Прям& r) const {
	ЗамкниГип __;
	Прям w = r;
	::LPtoDP(handle, reinterpret_cast<POINT *>(&w), 2);
	return w;
}
#endif

Размер SystemDraw::GetSizeCaps(int i, int j) const {
	ЗамкниГип __;
	return Размер(GetDeviceCaps(handle, i), GetDeviceCaps(handle, j));
}

void SystemDraw::DotsMode()
{
	::SetMapMode(handle, MM_ANISOTROPIC);
	::SetViewportExtEx(handle, nativeDpi.cx, nativeDpi.cy, NULL);
	::SetViewportOrgEx(handle, 0, 0, NULL);
	::SetWindowExtEx(handle, 600, 600, NULL);
	::SetWindowOrgEx(handle, 0, 0, NULL);
}

void SystemDraw::BeginNative()
{
	if(GetPixelsPerInch() != nativeDpi && ++native == 1) {
		::SetMapMode(handle, MM_TEXT);
		actual_offset_bak = actual_offset;
		Native(actual_offset);
		SetOrg();
	}
}

void SystemDraw::EndNative()
{
	if(GetPixelsPerInch() == nativeDpi && --native == 0) {
		DotsMode();
		actual_offset = actual_offset_bak;
		SetOrg();
	}
}

int SystemDraw::GetCloffLevel() const
{
	return cloff.дайСчёт();
}

void SystemDraw::LoadCaps() {
	ЗамкниГип __;
	color16 = false;
	palette = (GetDeviceCaps(handle, RASTERCAPS) & RC_PALETTE);
	if(palette)
		color16 = GetDeviceCaps(handle, SIZEPALETTE) != 256;
	nativeSize = pageSize = GetSizeCaps(HORZRES, VERTRES);
	nativeDpi = GetSizeCaps(LOGPIXELSX, LOGPIXELSY);
	is_mono = GetDeviceCaps(handle, BITSPIXEL) == 1 && GetDeviceCaps(handle, PLANES) == 1;
}

void SystemDraw::Cinit() {
	ЗамкниГип __;
	lastColor = Цвет::FromCR(COLORREF(-5));
	lastPenColor = Цвет::FromCR(COLORREF(-5));
	lastTextColor = COLORREF(-1);
	lastPen = Null;
	actBrush = orgBrush = NULL;
	actPen = orgPen = NULL;
}

void SystemDraw::иниц() {
	ЗамкниГип __;
	drawingclip = Прям(-(INT_MAX >> 1), -(INT_MAX >> 1), +(INT_MAX >> 1), +(INT_MAX >> 1));
	Cinit();
	SetBkMode(handle, TRANSPARENT);
	::SetTextAlign(handle, TA_BASELINE);
#ifdef PLATFORM_WINCE
	actual_offset = Точка(0, 0);
#else
	::GetViewportOrgEx(handle, actual_offset);
#endif
	LoadCaps();
}

void SystemDraw::InitClip(const Прям& clip)
{
	drawingclip = clip;
}

void SystemDraw::переустанов() {
	ЗамкниГип __;
	style = 0;
}

SystemDraw::SystemDraw() {
	ЗамкниГип __;
	native = 0;
	InitColors();
	actual_offset = Точка(0, 0);
	переустанов();
	handle = NULL;
	dcMem = NULL;
}

SystemDraw::SystemDraw(HDC hdc) {
	ЗамкниГип __;
	native = 0;
	InitColors();
	переустанов();
	прикрепи(hdc);
}

void SystemDraw::Unselect0() {
	ЗамкниГип __;
	if(orgPen) SelectObject(handle, orgPen);
	if(orgBrush) SelectObject(handle, orgBrush);
	if(actPen) DeleteObject(actPen);
	if(actBrush) DeleteObject(actBrush);
	Cinit();
}

void SystemDraw::Unselect() {
	ЗамкниГип __;
	while(cloff.дайСчёт())
		стоп();
	Unselect0();
}

void  SystemDraw::прикрепи(HDC ahandle)
{
	handle = ahandle;
	dcMem = ::CreateCompatibleDC(handle);
	иниц();
}

HDC   SystemDraw::открепи()
{
	Unselect();
	HDC h = handle;
	handle = NULL;
	::DeleteDC(dcMem);
	dcMem = NULL;
	return h;
}

SystemDraw::~SystemDraw() {
	ЗамкниГип __;
	открепи();
}

HDC SystemDraw::BeginGdi() {
	ЗамкниГип __;
	старт();
	return handle;
}

void SystemDraw::EndGdi() {
	ЗамкниГип __;
	Unselect0();
	стоп();
}

void BackDraw::создай(SystemDraw& w, int cx, int cy) {
	ПРОВЕРЬ(w.дайУк());
	ЗамкниГип __;
	разрушь();
	size.cx = cx;
	size.cy = cy;
	hbmp = ::CreateCompatibleBitmap(w.дайУк(), cx, cy);
	handle = ::CreateCompatibleDC(w.дайУк());
	dcMem = ::CreateCompatibleDC(handle);
	ПРОВЕРЬ(hbmp);
	ПРОВЕРЬ(handle);
#ifndef PLATFORM_WINCE
	if(AutoPalette()) {
		::SelectPalette(handle, GetQlibPalette(), FALSE);
		::RealizePalette(handle);
	}
#endif
	hbmpold = (HBITMAP) ::SelectObject(handle, hbmp);
	иниц();
	InitClip(size);
}

void BackDraw::помести(SystemDraw& w, int x, int y) {
	ЗамкниГип __;
	ПРОВЕРЬ(handle);
	LTIMING("BackDraw::помести");
#ifdef PLATFORM_WINCE
	::SetViewportOrgEx(handle, 0, 0, 0);
#else
	::SetWindowOrgEx(handle, 0, 0, NULL);
#endif
	::BitBlt(w, x, y, size.cx, size.cy, *this, 0, 0, SRCCOPY);
}

void BackDraw::разрушь() {
	ЗамкниГип __;
	if(handle) {
		Unselect();
		::SelectObject(handle, hbmpold);
		::DeleteDC(handle);
		::DeleteObject(hbmp);
		handle = NULL;
		::DeleteDC(dcMem);
		dcMem = NULL;
	}
}

ScreenDraw::ScreenDraw(bool ic) {
	ЗамкниГип __;
#ifdef PLATFORM_WINCE
	прикрепи(CreateDC(NULL, NULL, NULL, NULL));
#else
	прикрепи(ic ? CreateIC("DISPLAY", NULL, NULL, NULL) : CreateDC("DISPLAY", NULL, NULL, NULL));
	InitClip(GetVirtualScreenArea());
	if(AutoPalette()) {
		SelectPalette(handle, GetQlibPalette(), TRUE);
		RealizePalette(handle);
	}
#endif
}

ScreenDraw::~ScreenDraw() {
	ЗамкниГип __;
	Unselect();
	DeleteDC(handle);
}

#ifndef PLATFORM_WINCE

void PrintDraw::InitPrinter()
{
	ЗамкниГип __;
	иниц();
	style = PRINTER|DOTS;
	DotsMode();
	native = 0;
	actual_offset = Точка(0, 0);
	aborted = false;
	pageSize.cx = 600 * nativeSize.cx / nativeDpi.cx; 
	pageSize.cy = 600 * nativeSize.cy / nativeDpi.cy;
	InitClip(pageSize);
}

void PrintDraw::StartPage()
{
	ЗамкниГип __;
	if(aborted) return;
	Unselect();
	if(::StartPage(handle) <= 0)
		aborted = true;
	else
		InitPrinter();
}

void PrintDraw::EndPage()
{
	ЗамкниГип __;
	if(aborted) return;
	Unselect();
	if(::EndPage(handle) <= 0)
		aborted = true;
}

PrintDraw::PrintDraw(HDC hdc, const char *docname)
   : SystemDraw(hdc)
{
	ЗамкниГип __;
	DOCINFOW di;
	memset(&di, 0, sizeof(di));
	di.cbSize = sizeof(di);
	Вектор<char16> sys_docname = вСисНабсимШ(docname);
	di.lpszDocName = sys_docname;
	if(::StartDocW(hdc, &di) <= 0)
		aborted = true;
	else
		InitPrinter();
}

PrintDraw::~PrintDraw() {
	ЗамкниГип __;
	if(aborted)
		::AbortDoc(handle);
	else
		::EndDoc(handle);
}
#endif

}

#endif
