#include "Local.h"

#ifdef VIRTUALGUI

NAMESPACE_UPP

#define LLOG(x)  // LOG(x)

static ВекторМап<Ткст, ClipData> fbClipboard;

void ClearClipboard()
{
	ЗамкниГип __;
	fbClipboard.очисть();
}

void AppendClipboard(const char *формат, const Значение& data, Ткст (*render)(const Значение&))
{
	ЗамкниГип __;
	ClipData& cd = fbClipboard.дайДобавь(формат);
	cd.data = data;
	cd.render = render;
}

static Ткст sRawRender(const Значение& v)
{
	return v;
}

void AppendClipboard(const char *формат, const Ткст& data)
{
	ЗамкниГип __;
	AppendClipboard(формат, data, sRawRender);
}

void AppendClipboard(const char *формат, const byte *data, int length)
{
	ЗамкниГип __;
	AppendClipboard(формат, Ткст(data, length));
}

Ткст ReadClipboard(const char *формат)
{
	ЗамкниГип __;
	int q = fbClipboard.найди(формат);
	return q >= 0 ? (*fbClipboard[q].render)(fbClipboard[q].data) : Ткст();
}

void AppendClipboardText(const Ткст& s)
{
	AppendClipboard("text", вСисНабсим(s));
}

void AppendClipboardUnicodeText(const ШТкст& s)
{
	AppendClipboard("wtext", (byte *)~s, sizeof(wchar) * s.дайДлину());
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
		return ШТкст((const wchar *)~s, strlen__((const wchar *)~s)).вТкст();
	}
	if(clip.IsAvailable("text"))
		return ~clip;
	return Null;
}

ШТкст дайШТкст(PasteClip& clip)
{
	ЗамкниГип __;
	if(clip.прими("wtext")) {
		Ткст s = ~clip;
		return ШТкст((const wchar *)~s, strlen__((const wchar *)~s));
	}
	if(clip.IsAvailable("text"))
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
	Ткст w = ReadClipboard("text");
	return w.дайСчёт() ? w : ReadClipboardUnicodeText().вТкст();
}

ШТкст ReadClipboardUnicodeText()
{
	Ткст w = ReadClipboard("wtext");
	if(w.дайСчёт())
		return ШТкст((const wchar *)~w, w.дайДлину() / 2);
	return ReadClipboard("text").вШТкст();
}

bool IsClipboardAvailable(const char *id)
{
	return fbClipboard.найди(id) >= 0;
}

bool IsClipboardAvailableText()
{
	return IsClipboardAvailable("text") || IsClipboardAvailable("wtext");
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
	return Null;
}

Рисунок ReadClipboardImage()
{
	ЗамкниГип __;
	PasteClip d = Ктрл::Clipboard();
	return GetImage(d);
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
	if(fmt == ClipFmt<Рисунок>())
		return sImage(img);
	return Null;
}

void AppendClipboardImage(const Рисунок& img)
{
	ЗамкниГип __;
	if(img.пустой()) return;
	AppendClipboard(ClipFmt<Рисунок>(), img, sImage);
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

Вектор<Ткст> GetFiles(PasteClip& clip)
{
	ЗамкниГип __;
	Вектор<Ткст> f;
	return f;
}

END_UPP_NAMESPACE

#endif
