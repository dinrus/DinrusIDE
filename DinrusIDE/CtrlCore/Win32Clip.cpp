#include "CtrlCore.h"
#include <plugin/bmp/bmp.h>

#ifdef GUI_WIN

namespace РНЦП {

#define LLOG(x)  // LOG(x)

ВекторМап<int, ClipData>& sClipMap()
{
	static ВекторМап<int, ClipData> x;
	return x;
}

extern HWND utilityHWND;

int  GetClipboardFormatCode(const char *format_id)
{
	ЗамкниГип ___;
	int x = (int)(intptr_t)format_id;
	if(x >= 0 && x < 65535)
		return x;
	Ткст fmt = format_id;
	if(fmt == "text")
		return CF_TEXT;
	if(fmt == "wtext")
		return CF_UNICODETEXT;
	if(fmt == "dib")
		return CF_DIB;
	if(fmt == "files")
		return CF_HDROP;
	static СтатическийСтопор m;
	Стопор::Замок __(m);
	static ВекторМап<Ткст, int> format_map;
	int f = format_map.найди(format_id);
	if(f < 0) {
		f = format_map.дайСчёт();
		format_map.добавь(format_id,
#ifdef PLATFORM_WINCE
			::RegisterClipboardFormat(вСисНабсим(format_id))
#else
			::RegisterClipboardFormat(format_id)
#endif
		);
	}
	return format_map[f];
}

bool DebugClipboard()
{
	static bool b = дайКлючИни("DebugClipboard") == "1";
	return b;
}

void ClipboardLog(const char *txt)
{
	if(!DebugClipboard())
		return;
	ФайлДоп f(дайФайлИзПапкиИсп("clip.log"));
	f << дайСисВремя() << ": " << txt << "\n";
}

void ClipboardError(const char *txt)
{
	if(!DebugClipboard())
		return;
	Ткст s = txt;
	s << "\n" << дайПоследнОшСооб();
	MessageBox(::GetActiveWindow(), s, "Clipboard Ошибка", MB_ICONSTOP | MB_OK | MB_APPLMODAL);
	ClipboardLog(Ткст().конкат() << s << " Ошибка");
}

Ткст FromWin32CF(int cf);

void ClipboardError(const char *txt, int формат)
{
	if(!DebugClipboard())
		return;
	ClipboardError(Ткст().конкат() << txt << ' ' << FromWin32CF(формат));
}

bool ClipboardOpen()
{
	// Win32 has serious race condition problem with clipboard; system or other apps open it
	// right after we close it thus blocking us to send more formats
	// So the solution is to wait and retry... (mirek, 2011-01-09)
	for(int i = 0; i < 200; i++) {
		if(OpenClipboard(utilityHWND)) {
			ClipboardLog("----- ClipboardOpen OK");
			return true;
		}
		Sleep(10);
	}
	ClipboardError("ClipboardOpen has failed!");
	return false;
}

void ClearClipboard()
{
	ЗамкниГип __;
	sClipMap().очисть();
	ClipboardLog("* ClearClipboard");
	if(ClipboardOpen()) {
		if(!EmptyClipboard())
			ClipboardError("EmptyClipboard Ошибка");
		if(!CloseClipboard())
			ClipboardError("CloseClipboard Ошибка");
	}
}

void SetClipboardRaw(int формат, const byte *data, int length)
{
	ЗамкниГип __;
	HANDLE handle = NULL;
	ClipboardLog("* SetClipboardRaw");
	if(data) {
		handle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, length + 2);
		byte *ptr;
		if(!handle) {
			ClipboardError("GlobalAlloc Ошибка");
			return;
		}
		if(!(ptr = (byte *)GlobalLock(handle))) {
			ClipboardError("GlobalLock Ошибка");
			GlobalFree(handle);
			return;
		}
		memcpy(ptr, data, length);
		ptr[length] = 0;
		ptr[length + 1] = 0;
		GlobalUnlock(handle);
	}
	if(SetClipboardData(формат, handle) != handle) {
		ClipboardError("SetCliboardData", формат);
		LLOG("SetClipboardData Ошибка: " << дайПоследнОшСооб());
		GlobalFree(handle);
	}
}

void AppendClipboard(int формат, const byte *data, int length)
{
	ЗамкниГип __;
	ClipboardLog("* AppendClipboard");
	if(ClipboardOpen()) {
		SetClipboardRaw(формат, data, length);
		if(!CloseClipboard())
			ClipboardError("CloseClipboard", формат);
	}
}

void AppendClipboard(const char *формат, const byte *data, int length)
{
	ЗамкниГип __;
	Вектор<Ткст> f = разбей(формат, ';');
	for(int i = 0; i < f.дайСчёт(); i++)
		AppendClipboard(GetClipboardFormatCode(f[i]), data, length);
}

void AppendClipboard(const char *формат, const Ткст& data)
{
	ЗамкниГип __;
	AppendClipboard(формат, data, data.дайДлину());
}

void AppendClipboard(const char *формат, const Значение& data, Ткст (*render)(const Значение&))
{
	ЗамкниГип __;
	Вектор<Ткст> f = разбей(формат, ';');
	for(int i = 0; i < f.дайСчёт(); i++) {
		int c = GetClipboardFormatCode(f[i]);
		sClipMap().дайДобавь(c) = ClipData(data, render);
		AppendClipboard(c, NULL, 0);
	}
}

void Ктрл::RenderFormat(int формат)
{
	ЗамкниГип __;
	int q = sClipMap().найди(формат);
	if(q >= 0) {
		Ткст s = sClipMap()[q].Render();
		SetClipboardRaw(формат, s, s.дайДлину());
	}
}

void Ктрл::RenderAllFormats()
{
	ЗамкниГип __;
	if(sClipMap().дайСчёт() && OpenClipboard(utilityHWND)) {
		for(int i = 0; i < sClipMap().дайСчёт(); i++)
			RenderFormat(sClipMap().дайКлюч(i));
		CloseClipboard();
	}
}

void Ктрл::DestroyClipboard()
{
	ЗамкниГип __;
	sClipMap().очисть();
}

Ткст ReadClipboard(const char *формат)
{
	ЗамкниГип __;
	if(!ClipboardOpen())
		return Null;
	HGLOBAL hmem = GetClipboardData(GetClipboardFormatCode(формат));
	if(hmem == 0) {
		ClipboardError("GetClipboardData failed");
		CloseClipboard();
		return Null;
	}
	const byte *src = (const byte *)GlobalLock(hmem);
	ПРОВЕРЬ(src);
	int length = (int)GlobalSize(hmem);
	if(length < 0) {
		ClipboardError("ReadCliboard length < 0");
		CloseClipboard();
		return Null;
	}
	Ткст out(src, length);
	GlobalUnlock(hmem);
	CloseClipboard();
	return out;
}

void AppendClipboardText(const Ткст& s)
{
	AppendClipboardUnicodeText(s.вШТкст());
}

void AppendClipboardUnicodeText(const ШТкст& s)
{
	Вектор<char16> ws = вУтф16(s);
	AppendClipboard("wtext", (const byte *)ws.begin(), 2 * ws.дайСчёт());
}

const char *ClipFmtsText()
{
	return "wtext;text";
}

Ткст дайТкст(PasteClip& clip)
{
	ЗамкниГип __;
	if(clip.прими("wtext")) {
		Ткст s = ~clip;
		return вУтф8((const char16 *)~s, strlen16((const char16 *)~s));
	}
	if(clip.прими("text"))
		return ~clip;
	return Null;
}

ШТкст дайШТкст(PasteClip& clip)
{
	ЗамкниГип __;
	if(clip.прими("wtext")) {
		Ткст s = ~clip;
		return вУтф32((const char16 *)~s, strlen16((const char16 *)~s));
	}
	if(clip.прими("text"))
		return (~clip).вШТкст();
	return Null;
}


bool AcceptText(PasteClip& clip)
{
	return clip.прими(ClipFmtsText());
}

static Ткст sText(const Значение& data)
{
	return data;
}

static Ткст sWText(const Значение& data)
{
	return Unicode__(ШТкст(data));
}

void приставь(ВекторМап<Ткст, ClipData>& data, const Ткст& text)
{
	data.дайДобавь("text", ClipData(text, sText));
	data.дайДобавь("wtext", ClipData(text, sWText));
}

void приставь(ВекторМап<Ткст, ClipData>& data, const ШТкст& text)
{
	data.дайДобавь("text", ClipData(text, sText));
	data.дайДобавь("wtext", ClipData(text, sWText));
}

Ткст GetTextClip(const ШТкст& text, const Ткст& fmt)
{
	if(fmt == "text")
		return text.вТкст();
	if(fmt == "wtext")
		return Unicode__(text);
	return Null;
}

Ткст GetTextClip(const Ткст& text, const Ткст& fmt)
{
	if(fmt == "text")
		return text;
	if(fmt == "wtext")
		return Unicode__(text.вШТкст());
	return Null;
}

Ткст ReadClipboardText()
{
#ifdef PLATFORM_WINCE
	return ReadClipboardUnicodeText().вТкст();
#else
	Ткст s = ReadClipboardUnicodeText().вТкст();
	if(s.дайСчёт())
		return s;
	s = ReadClipboard((const char *)CF_TEXT);
	return Ткст(s, (int)strlen(~s));
#endif
}

ШТкст ReadClipboardUnicodeText()
{
	Ткст s = ReadClipboard((const char *)CF_UNICODETEXT);
	return вУтф32((const char16 *)~s, strlen16((const char16 *)~s));
}

bool IsClipboardAvailable(const char *id)
{
	return ::IsClipboardFormatAvailable(GetClipboardFormatCode(id));
}

bool IsClipboardAvailableText()
{
	return IsClipboardAvailable((const char *)CF_TEXT);
}

const char *ClipFmtsImage()
{
	static const char *q;
	ONCELOCK {
		static Ткст s = "dib;" + ClipFmt<Рисунок>();
		q = s;
	}
	return q;
}

bool AcceptImage(PasteClip& clip)
{
	ЗамкниГип __;
	return clip.прими(ClipFmtsImage());
}

Рисунок GetImage(PasteClip& clip)
{
	ЗамкниГип __;
	Рисунок m;
	if(прими<Рисунок>(clip)) {
		грузиИзТкст(m, ~clip);
		if(!m.пустой())
			return m;
				}
	if(clip.прими("dib")) {
		Ткст data = ~clip;
		if((unsigned)data.дайСчёт() < sizeof(BITMAPINFO)) return Null;
		BITMAPINFO *lpBI = (BITMAPINFO *)~data;
		BITMAPINFOHEADER& hdr = lpBI->bmiHeader;
		byte *bits = (byte *)lpBI + hdr.biSize;
		if(hdr.biBitCount <= 8)
			bits += (hdr.biClrUsed ? hdr.biClrUsed : 1 << hdr.biBitCount) * sizeof(RGBQUAD);
		if(hdr.biBitCount >= 16 || hdr.biBitCount == 32) {
			if(hdr.biCompression == 3)
				bits += 12;
			if(hdr.biClrUsed != 0)
				bits += hdr.biClrUsed * sizeof(RGBQUAD);
		}
		int h = абс((int)hdr.biHeight);
		ImageDraw   iw(hdr.biWidth, h);
		::StretchDIBits(iw.дайУк(),
			0, 0, hdr.biWidth, h,
			0, 0, hdr.biWidth, h,
			bits, lpBI, DIB_RGB_COLORS, SRCCOPY);
		return iw;
	}
	return Null;
}

Рисунок ReadClipboardImage()
{
	ЗамкниГип __;
	PasteClip d = Ктрл::Clipboard();
	return GetImage(d);
}

Ткст sDib(const Значение& image)
{
	Рисунок img = image;
	BITMAPINFOHEADER header;
	обнули(header);
	header.biSize = sizeof(header);
	header.biWidth = img.дайШирину();
	header.biHeight = -img.дайВысоту();
	header.biBitCount = 32;
	header.biPlanes = 1;
	header.biCompression = BI_RGB;
	ТкстБуф b(sizeof(header) + 4 * img.дайДлину());
	byte *p = (byte *)~b;
	memcpy(p, &header, sizeof(header));
	memcpy(p + sizeof(header), ~img, 4 * img.дайДлину());
	return Ткст(b);
}

Ткст sImage(const Значение& image)
{
	Рисунок img = image;
	return сохраниКакТкст(const_cast<Рисунок&>(img));
}

Ткст GetImageClip(const Рисунок& img, const Ткст& fmt)
{
	ЗамкниГип __;
	if(img.пустой()) return Null;
	if(fmt == "dib")
		return sDib(img);
	if(fmt == ClipFmt<Рисунок>())
		return sImage(img);
	return Null;
}

void AppendClipboardImage(const Рисунок& img)
{
	ЗамкниГип __;
	if(img.пустой()) return;
	AppendClipboard(ClipFmt<Рисунок>(), img, sImage);
	AppendClipboard("dib", img, sDib);
}

void приставь(ВекторМап<Ткст, ClipData>& data, const Рисунок& img)
{
	data.добавь(ClipFmt<Рисунок>(), ClipData(img, sImage));
	data.добавь("dib", ClipData(img, sDib));
}

bool AcceptFiles(PasteClip& clip)
{
	if(clip.прими("files")) {
		clip.SetAction(DND_COPY);
		return true;
	}
	return false;
}

bool IsAvailableFiles(PasteClip& clip)
{
	return clip.IsAvailable("files");
}

struct sDROPFILES {
    DWORD offset;
    POINT pt;
    BOOL  nc;
    BOOL  unicode;
};

Вектор<Ткст> GetClipFiles(const Ткст& data)
{
	ЗамкниГип __;
	Вектор<Ткст> f;
	if((unsigned)data.дайСчёт() < sizeof(sDROPFILES) + 2)
		return f;
	const sDROPFILES *df = (const sDROPFILES *)~data;
	const char *s = ((const char *)df + df->offset);
	if(df->unicode) {
		const char16 *ws = (char16 *)s;
		while(*ws) {
			ШТкст фн;
			while(*ws)
				фн.конкат(*ws++);
			f.добавь(фн.вТкст());
			ws++;
		}
	}
	else
		while(*s) {
			Ткст фн;
			while(*s)
				фн.конкат(*s++);
			f.добавь(фн.вТкст());
			s++;
		}
	return f;
}

Вектор<Ткст> GetFiles(PasteClip& clip)
{
	ЗамкниГип __;
	Вектор<Ткст> f;
	return GetClipFiles(clip.дай("files"));
}

void AppendFiles(ВекторМап<Ткст, ClipData>& clip, const Вектор<Ткст>& files)
{
	ШТкст wfiles;
	for(int i = 0; i < files.дайСчёт(); i++)
		wfiles << files[i].вШТкст() << (wchar)0;
	sDROPFILES h;
	h.unicode = true;
	h.offset = sizeof(h);
    GetCursorPos(&h.pt);
    h.nc = TRUE;
    Ткст data;
	data.конкат((byte *)&h, sizeof(h));
	data.конкат((byte *)~wfiles, 2 * (wfiles.дайСчёт() + 1));
	clip.дайДобавь("files") = ClipData(data);
}

bool   Has(ЦельЮБроса *dt, const char *fmt);
Ткст дай(ЦельЮБроса *dt, const char *fmt);

bool PasteClip::IsAvailable(const char *fmt) const
{
	if(this == &Ктрл::Selection())
		return false;
	return dt ? РНЦП::Has(dt, fmt) : IsClipboardAvailable(fmt);
}

Ткст PasteClip::дай(const char *fmt) const
{
	if(this == &Ктрл::Selection())
		return Null;
	return dt ? РНЦП::дай(dt, fmt) : ReadClipboard(fmt);
}

void PasteClip::GuiPlatformConstruct()
{
	dt = NULL;
}

}

#endif
