#include "CtrlCore.h"

namespace РНЦП {

Ткст sRawClipData(const Значение& data)
{
	return data;
}

ClipData::ClipData(const Значение& data, Ткст (*render)(const Значение& data))
:	data(data), render(render ? render : sRawClipData)
{}

ClipData::ClipData(const Ткст& data)
:	data(data), render(sRawClipData)
{}

ClipData::ClipData()
:	render(sRawClipData)
{}

void Ктрл::тягИБрос(Точка p, PasteClip& d)       {}
void Ктрл::FrameDragAndDrop(Точка p, PasteClip& d)  {}
void Ктрл::тягВойди()                              {}
void Ктрл::тягПовтори(Точка p) {}
void Ктрл::тягПокинь() {}

PasteClip& Ктрл::Clipboard()
{
	ЗамкниГип __;
	static PasteClip d;
	d.fmt.очисть();
	return d;
}

PasteClip& Ктрл::Selection()
{
	ЗамкниГип __;
	static PasteClip d;
	GuiPlatformSelection(d);
	return d;
}

Ткст Ктрл::GetDropData(const Ткст& fmt) const
{
	return GetSelectionData(fmt);
}

Ткст Ктрл::GetSelectionData(const Ткст& fmt) const
{
	return Null;
}

bool PasteClip::прими()
{
	accepted = true;
	return paste;
}

bool   PasteClip::прими(const char *_fmt)
{
	Вектор<Ткст> f = разбей(_fmt, ';');
	for(int i = 0; i < f.дайСчёт(); i++) {
		if(IsAccepted() && fmt == f[i])
			return paste;
		if(IsAvailable(f[i])) {
			accepted = true;
			if(paste) {
				fmt = f[i];
				data = дай(f[i]);
				return true;
			}
			break;
		}
	}
	return false;
}

PasteClip::PasteClip()
{
	paste = true;
	accepted = false;
	GuiPlatformConstruct();
}

int Ктрл::DoDragAndDrop(const char *fmts, const Рисунок& sample, dword actions)
{
	ВекторМап<Ткст, ClipData> dummy;
	return DoDragAndDrop(fmts, sample, actions, dummy);
}

int Ктрл::DoDragAndDrop(const ВекторМап<Ткст, ClipData>& data, const Рисунок& sample, dword actions)
{
	return DoDragAndDrop("", sample, actions, data);
}

Uuid        sDndUuid;
const void *sInternalPtr;

Ткст GetInternalDropId__(const char *тип, const char *id)
{
	return "U++ Internal clip:" + какТкст(sDndUuid) + '-' + тип + '-' + id;
}

void NewInternalDrop__(const void *ptr)
{
	sDndUuid = Uuid::создай();
	sInternalPtr = ptr;
}

const void *GetInternalDropPtr__()
{
	return sInternalPtr;
}

Ткст Unicode__(const ШТкст& w)
{
	return Ткст((const char *)~w, 2 * w.дайДлину());
}

ШТкст Unicode__(const Ткст& s)
{
	return ШТкст((const wchar *)~s, s.дайДлину() / 2);
}

void GuiPlatformAdjustDragImage(ImageBuffer& b);

Рисунок MakeDragImage(const Рисунок& arrow, Рисунок sample)
{
	ImageBuffer b;
	if(пусто_ли(sample)) {
		sample = CtrlCoreImg::DndData();
		b = sample;
		Over(b, Точка(0, 0), arrow, arrow.дайРазм());
	}
	else {
		b.создай(128, 128);
		memset(~b, 0, sizeof(КЗСА) * b.дайДлину());
		Over(b, Точка(2, 22), sample, sample.дайРазм());
		Unmultiply(b);
		for(int y = 20; y < 96; y++) {
			КЗСА *s = b[y];
			КЗСА *e = s + 96;
			while(s < e)
				(s++)->a >>= 1;
			e += 32;
			int q = 128;
			while(s < e) {
				s->a = (s->a * q) >> 8;
				q -= 4;
				s++;
			}
		}
		int qq = 128;
		for(int y = 96; y < 128; y++) {
			КЗСА *s = b[y];
			КЗСА *e = s + 96;
			while(s < e) {
				s->a = (s->a * qq) >> 8;
				s++;
			}
			e += 32;
			int q = 255;
			while(s < e) {
				s->a = (s->a * q * qq) >> 16;
				q -= 8;
				s++;
			}
			qq -= 4;
		}
		КЗСА *s = b[21] + 1;
		КЗСА c1 = синий();
		КЗСА c2 = белый();
		for(int a = 255; a > 0; a -= 3) {
			c1.a = c2.a = a;
			*s++ = c1;
			разверни(c1, c2);
		}
		s = b[21] + 1;
		c1 = чёрный();
		c2 = белый();
		for(int a = 255; a > 0; a -= 8) {
			c1.a = c2.a = a;
			*s = c1;
			s += b.дайШирину();
			разверни(c1, c2);
		}
		Premultiply(b);
		GuiPlatformAdjustDragImage(b);
		Over(b, Точка(0, 0), arrow, arrow.дайРазм());
	}

	return b;
}

Ук<Ктрл> Ктрл::dndctrl;
Точка     Ктрл::dndpos;
bool      Ктрл::dndframe;
PasteClip Ктрл::dndclip;

void Ктрл::DnDRepeat()
{
	ЗамкниГип __;
	if(dndctrl) {
		dndctrl->тягПовтори(dndpos);
		if(dndctrl) {
			PasteClip d = dndclip;
			if(dndframe)
				dndctrl->FrameDragAndDrop(dndpos, d);
			else
				dndctrl->тягИБрос(dndpos, d);
		}
	}
	else
		РНЦП::глушиОбрвызВремени(&dndpos);
}

void Ктрл::ТиБ(Точка p, PasteClip& clip)
{
	ЗамкниГип __;
	РНЦП::глушиОбрвызВремени(&dndpos);
	dndclip = clip;
	Точка hp = p - дайПрямЭкрана().верхЛево();
	Ук<Ктрл> ctrl = this;
	while(ctrl && ctrl->включен_ли()) {
		Прям view = ctrl->GetScreenView();
		if(ctrl->IsMouseActive()) {
			if(view.содержит(p)) {
				dndpos = p - view.верхЛево();
				dndframe = false;
				ctrl->тягИБрос(dndpos, clip);
				if(clip.IsAccepted())
					break;
			}
			else {
				dndpos = p - ctrl->дайПрямЭкрана().верхЛево();
				dndframe = true;
				ctrl->FrameDragAndDrop(dndpos, clip);
				if(clip.IsAccepted())
					break;
			}
		}
		ctrl = ctrl->отпрыскИзТочки(hp);
	}
	if(ctrl != dndctrl) {
		if(dndctrl)
			dndctrl->тягПокинь();
		dndctrl = ctrl;
		if(dndctrl)
			dndctrl->тягВойди();
	}
	if(dndctrl)
		РНЦП::устОбрвызВремени(-40, callback(DnDRepeat), &dndpos);
}

void Ктрл::DnDLeave()
{
	ЗамкниГип __;
	if(dndctrl) {
		dndctrl->тягПокинь();
		РНЦП::глушиОбрвызВремени(&dndpos);
		dndctrl = NULL;
	}
}

Ктрл *Ктрл::GetDragAndDropTarget()
{
	ЗамкниГип __;
	return dndctrl;
}

void AppendClipboard(const char *формат, const ClipData& data)
{
	AppendClipboard(формат, data.data, data.render);
}

void AppendClipboard(const ВекторМап<Ткст, ClipData>& data)
{
	for(int i = 0; i < data.дайСчёт(); i++)
		AppendClipboard(data.дайКлюч(i), data[i]);
}

void InitRichImage(Ткст      (*fGetImageClip)(const Рисунок& img, const Ткст& fmt),
                   bool        (*fAcceptImage)(PasteClip& clip),
                   Рисунок       (*fGetImage)(PasteClip& clip),
                   const char *(*fClipFmtsImage)());

ИНИЦБЛОК {
	InitRichImage(GetImageClip, AcceptImage, GetImage, ClipFmtsImage);
}

const char *sClipFmtsRTF = "Rich устТекст фмт;text/rtf;application/rtf";

const char *ClipFmtsRTF()
{
	return sClipFmtsRTF;
}

#ifdef PLATFORM_POSIX

void WriteClipboardHTML(const Ткст& html)
{
    WriteClipboard("text/html", html);
}

#else

void WriteClipboardHTML(const Ткст& html)
{
	Ткст data;
	
	data =
		"Version:0.9\n"
		"StartHTML:#000001#\n"
		"EndHTML:#000002#\n"
		"StartFragment:#000003#\n"
		"EndFragment:#000004#\n"
		"StartSelection:#000003#\n"
		"EndSelection:#000004#\n"
		"<HTML>\n"
		"<!--StartFragment -->"
	    "<!--EndFragment -->\n"
		"</HTML>\n"
	;
	
	data.замени("#000001#", спринтф("%08u", data.найди("<HTML>") + 6));
	int start = data.найди("<!--StartFragment -->") + 21;
	data.замени("#000003#", спринтф("%08u", start));
	data.замени("#000004#", спринтф("%08u", data.найди("<!--EndFragment -->") + html.дайСчёт()));
	data.замени("#000002#", спринтф("%08u", data.найди("</HTML>") + html.дайСчёт()));
	
	data.вставь(start, html);
	
	WriteClipboard("HTML фмт", data);
}

#endif

}
