#include "CtrlCore.h"

#ifdef GUI_X11

namespace РНЦП {

#define LLOG(x)  // LOG(x)

Индекс<Ткст> Ктрл::sel_formats;
Ук<Ктрл>     Ктрл::sel_ctrl;

void Ктрл::SetSelectionSource(const char *fmts)
{
	ЗамкниГип __;
	LLOG("SetSelectionSource " << РНЦП::Имя(this) << ": " << fmts);
	sel_formats = разбей(fmts, ';');
	sel_ctrl = this;
	XSetSelectionOwner(Xdisplay, XAtom("PRIMARY"), xclipboard().win, CurrentTime);
}

Ктрл::Xclipboard::Xclipboard()
{
	ЗамкниГип __;
	XSetWindowAttributes swa;
	win = XCreateWindow(Xdisplay, RootWindow(Xdisplay, Xscreenno),
	                    0, 0, 10, 10, 0, CopyFromParent, InputOnly, CopyFromParent,
	                    0, &swa);
	XSelectInput(Xdisplay, win, PropertyChangeMask);
}

Ктрл::Xclipboard::~Xclipboard()
{
	ЗамкниГип __;
	XDestroyWindow(Xdisplay, win);
}

void Ктрл::Xclipboard::пиши(int fmt, const ClipData& _data)
{
	ЗамкниГип __;
	LLOG("SetSelectionOwner " << XAtomName(fmt));
	data.дайДобавь(fmt) = _data;
	XSetSelectionOwner(Xdisplay, XAtom("CLIPBOARD"), win, CurrentTime);
}

void Ктрл::Xclipboard::Request(XSelectionRequestСобытие *se)
{
	ЗамкниГип __;
	LLOG("Request " << XAtomName(se->target));
	XСобытие e;
	e.xselection.тип      = SelectionNotify;
	e.xselection.дисплей   = Xdisplay;
	e.xselection.requestor = se->requestor;
	e.xselection.selection = se->selection;
	e.xselection.target    = se->target;
	e.xselection.time      = se->time;
	e.xselection.property  = se->property;
	if(se->target == XAtom("TARGETS")) {
		LLOG("Request targets:");
		if(se->selection == XAtom("PRIMARY")) {
			Буфер<Atom> x(sel_formats.дайСчёт());
			for(int i = 0; i < sel_formats.дайСчёт(); i++) {
				x[i] = XAtom(sel_formats[i]);
				LLOG('\t' << sel_formats[i]);
			}
			XChangeProperty(Xdisplay, se->requestor, se->property, XAtom("ATOM"),
			                32, 0, (unsigned char*)~x,
			                sel_formats.дайСчёт());
		}
		else {
			Буфер<Atom> x(data.дайСчёт());
			for(int i = 0; i < data.дайСчёт(); i++) {
				x[i] = data.дайКлюч(i);
				LLOG('\t' << XAtomName(x[i]));
			}
			XChangeProperty(Xdisplay, se->requestor, se->property, XAtom("ATOM"),
			                32, 0, (unsigned char*)~x,
			                data.дайСчёт());
		}
	}
	else {
		if(se->selection == XAtom("PRIMARY")) {
			LLOG("Request PRIMARY data " << XAtomName(se->target));
			Ткст fmt = XAtomName(se->target);
			int i = sel_formats.найди(fmt);
			if(i >= 0 && sel_ctrl) {
				Ткст d = sel_ctrl->GetSelectionData(fmt);
				XChangeProperty(Xdisplay, se->requestor, se->property, se->target, 8, PropModeReplace,
				                d, d.дайДлину());
			}
			else
			    e.xselection.property = None;
		}
		else {
			LLOG("Request CLIPBOARD data " << XAtomName(se->target));
			int i = data.найди(se->target);
			if(i >= 0) {
				Ткст d = data[i].Render();
				XChangeProperty(Xdisplay, se->requestor, se->property, se->target, 8, PropModeReplace,
				                d, d.дайДлину());
			}
			else
			    e.xselection.property = None;
		}
	}
	XSendСобытие(Xdisplay, se->requestor, XFalse, 0, &e);
}

Ткст Ктрл::Xclipboard::читай(int fmt, int selection, int property)
{
	ЗамкниГип __;
	if(data.дайСчёт() && (dword)selection == XAtom("CLIPBOARD")) {
		int q = data.найди(fmt);
		return q >= 0 ? data[q].Render() : Ткст();
	}
	if(sel_ctrl && (dword)selection == XAtom("PRIMARY"))
		return sel_ctrl->GetSelectionData(XAtomName(fmt));
	XConvertSelection(Xdisplay, selection, fmt, property, win, CurrentTime);
	XFlush(Xdisplay);
	XСобытие event;
	for(int i = 0; i < 20; i++) {
		if(XCheckTypedWindowСобытие(Xdisplay, win, SelectionNotify, &event)) {
			if(event.xselection.property != None) {
				XSync(Xdisplay, false);
				return ReadPropertyData(win, event.xselection.property);
			}
			return Null;
		}
		if(XCheckTypedWindowСобытие(Xdisplay, win, SelectionRequest, &event) &&
		   event.xselectionrequest.owner == win)
			Request(&event.xselectionrequest);
		if(XCheckTypedWindowСобытие(Xdisplay, win, SelectionClear, &event) &&
		   event.xselectionclear.window == win) {
			if(event.xselectionclear.selection == XAtom("CLIPBOARD"))
				очисть();
			if(event.xselectionclear.selection == XAtom("PRIMARY")) {
				sel_ctrl = NULL;
				sel_formats.очисть();
			}
		}
		спи(10);
	}
	return Null;
}

Ктрл::Xclipboard& Ктрл::xclipboard()
{
	static Xclipboard xc;
	return xc;
}

void ClearClipboard()
{
	ЗамкниГип __;
	Ктрл::xclipboard().очисть();
}

void AppendClipboard(const char *формат, const Значение& data, Ткст (*render)(const Значение& data))
{
	ЗамкниГип __;
	Вектор<Ткст> s = разбей(формат, ';');
	for(int i = 0; i < s.дайСчёт(); i++)
		Ктрл::xclipboard().пиши(XAtom(s[i]), ClipData(data, render));
}

Ткст sRawClipData(const Значение& data);

void AppendClipboard(const char *fmt, const Ткст& data)
{
	ЗамкниГип __;
	AppendClipboard(fmt, data, sRawClipData);
}

Ткст ReadClipboard(const char *fmt)
{
	ЗамкниГип __;
	return Ктрл::xclipboard().читай(XAtom(fmt), XAtom("CLIPBOARD"), XAtom("CLIPDATA"));
}

void AppendClipboardText(const Ткст& s)
{
	ЗамкниГип __;
	AppendClipboard("STRING", s);
}

Ткст ReadClipboardText()
{
	ЗамкниГип __;
	return ReadClipboard("STRING");
}

void AppendClipboardUnicodeText(const ШТкст& s)
{
	ЗамкниГип __;
	AppendClipboard("UTF8_STRING", вУтф8(s));
}

ШТкст ReadClipboardUnicodeText()
{
	ЗамкниГип __;
	return вУтф32(ReadClipboard("UTF8_STRING"));
}

bool Ктрл::Xclipboard::IsAvailable(int fmt, const char *тип)
{
	ЗамкниГип __;
	if(data.дайСчёт())
		return data.найди(fmt) >= 0;
	Ткст formats = читай(XAtom("TARGETS"), XAtom(тип), XAtom("CLIPDATA"));
	int c = formats.дайСчёт() / sizeof(Atom);
	const Atom *m = (Atom *) ~formats;
	for(int i = 0; i < c; i++) {
		if(m[i] == (dword)fmt)
			return true;
	}
	return false;
}

bool Ктрл::ClipHas(int тип, const char *fmt)
{
	ЗамкниГип __;
	LLOG("ClipHas " << тип << ": " << fmt);
	if(strcmp(fmt, "files") == 0)
		fmt = "text/uri-list";
	if(тип == 0)
		return Ктрл::xclipboard().IsAvailable(XAtom(fmt), "CLIPBOARD");
	if(тип == 2) {
		if(sel_ctrl)
			return sel_formats.найди(fmt) >= 0;
		return Ктрл::xclipboard().IsAvailable(XAtom(fmt), "PRIMARY");
	}
	return drop_formats.найди(fmt) >= 0;
}

Ткст DnDGetData(const Ткст& f);

Ткст Ктрл::ClipGet(int тип, const char *fmt)
{
	ЗамкниГип __;
	LLOG("ClipGet " << тип << ": " << fmt);
	if(strcmp(fmt, "files") == 0)
		fmt = "text/uri-list";
	if(тип && GetDragAndDropSource())
		return DnDGetData(fmt);
	return Ктрл::xclipboard().читай(
	           XAtom(fmt),
	           XAtom(тип == 2 ? "PRIMARY" : тип == 1 ? "XdndSelection" : "CLIPBOARD"),
	           тип == 1 ? XA_SECONDARY : XAtom("CLIPDATA")
	       );
}

const char *ClipFmtsText()
{
	return "STRING;UTF8_STRING;text/plain;text/unicode";
}

Ткст дайТкст(PasteClip& clip)
{
	ЗамкниГип __;
	if(clip.прими("STRING") || clip.прими("text/plain"))
		return ~clip;
	if(clip.прими("UTF8_STRING"))
		return вУтф32(~clip).вТкст();
	if(clip.прими("text/unicode"))
		return Unicode__(~clip).вТкст();
	return Null;
}

ШТкст дайШТкст(PasteClip& clip)
{
	ЗамкниГип __;
	if(clip.прими("UTF8_STRING"))
		return вУтф32(~clip);
	if(clip.прими("text/unicode"))
		return Unicode__(~clip);
	if(clip.прими("STRING") || clip.прими("text/plain"))
		return вЮникод(~clip, CHARSET_ISO8859_1);
	return Null;
}

Ткст GetTextClip(const ШТкст& text, const Ткст& fmt)
{
	ЗамкниГип __;
	if(fmt == "STRING" || fmt == "text/plain")
		return text.вТкст();
	if(fmt == "UTF8_STRING")
		return вУтф8(text);
	if(fmt == "text/unicode")
		return Unicode__(text);
	return Null;
}

Ткст GetTextClip(const Ткст& text, const Ткст& fmt)
{
	ЗамкниГип __;
	if(fmt == "STRING" || fmt == "text/plain")
		return text;
	if(fmt == "UTF8_STRING")
		return вУтф8(text.вШТкст());
	if(fmt == "text/unicode")
		return Unicode__(text.вШТкст());
	return Null;
}

bool AcceptText(PasteClip& clip)
{
	ЗамкниГип __;
	return clip.прими(ClipFmtsText());
}

void приставь(ВекторМап<Ткст, ClipData>& data, const Ткст& text) // optimize
{
	ЗамкниГип __;
	data.дайДобавь("STRING", text);
	data.дайДобавь("text/plain", text);
	data.дайДобавь("UTF8_STRING", вУтф8(text.вШТкст()));
	data.дайДобавь("text/unicode", Unicode__(text.вШТкст()));
}

void приставь(ВекторМап<Ткст, ClipData>& data, const ШТкст& text) // optimize
{
	ЗамкниГип __;
	data.дайДобавь("STRING", text.вТкст());
	data.дайДобавь("text/plain", text.вТкст());
	data.дайДобавь("UTF8_STRING", вУтф8(text));
	data.дайДобавь("text/unicode", Unicode__(text));
}

bool IsClipboardAvailable(const char *fmt)
{
	ЗамкниГип __;
	return Ктрл::xclipboard().IsAvailable(XAtom(fmt), "CLIPBOARD");
}

bool IsClipboardAvailableText()
{
	ЗамкниГип __;
	return IsClipboardAvailable("STRING") ||
	       IsClipboardAvailable("UTF8_STRING") ||
	       IsClipboardAvailable("text/plain") ||
	       IsClipboardAvailable("text/unicode");
}

bool AcceptFiles(PasteClip& clip)
{
	ЗамкниГип __;
	return clip.прими("text/uri-list");
}

bool IsAvailableFiles(PasteClip& clip)
{
	ЗамкниГип __;
	return clip.IsAvailable("text/uri-list");
}

int JustLf(int c)
{
	return (byte)c >= 32 || c == '\n' ? c : 0;
}

Вектор<Ткст> GetClipFiles(const Ткст& data)
{
	Вектор<Ткст> r;
	Вектор<Ткст> f = разбей(фильтруй(data, JustLf), '\n');
	for(int i = 0; i < f.дайСчёт(); i++)
		if(f[i].начинаетсяС("file://"))
			r.добавь(UrlDecode(f[i].середина(7)));
	return r;
}

Вектор<Ткст> GetFiles(PasteClip& clip) {
	ЗамкниГип __;
	Вектор<Ткст> r;
	if(clip.прими("text/uri-list")) {
		Ткст txt = clip;
		r = GetClipFiles(txt);
	}
	return r;
}

void AppendFiles(ВекторМап<Ткст, ClipData>& data, const Вектор<Ткст>& files)
{
	ЗамкниГип __;
	if(files.дайСчёт() == 0)
		return;
	Ткст h;
	for(int i = 0; i < files.дайСчёт(); i++)
		h << "file://" << UrlEncode(files[i]) << '\n';
	data.дайДобавь("text/uri-list") = h;
}

bool PasteClip::IsAvailable(const char *fmt) const
{
	return Ктрл::ClipHas(тип, fmt);
}

Ткст PasteClip::дай(const char *fmt) const
{
	return Ктрл::ClipGet(тип, fmt);
}

void PasteClip::GuiPlatformConstruct()
{
	тип = 0;
}

}

#endif
