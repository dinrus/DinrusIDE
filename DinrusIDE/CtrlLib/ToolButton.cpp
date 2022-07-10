#include "CtrlLib.h"

namespace РНЦП {

#define LTIMING(x)  // TIMING(x)

CH_STYLE(ToolButton, Стиль, дефСтиль)
{
	CtrlsImageLook(look, CtrlsImg::I_TB, 6);
	font = StdFont();
	for(int i = 0; i < 4; i++)
		textcolor[i] = Кнопка::StyleNormal().textcolor[i];
	textcolor[CTRL_CHECKED] = textcolor[CTRL_NORMAL];
	textcolor[CTRL_HOTCHECKED] = textcolor[CTRL_HOT];
	for(int i = 0; i < 6; i++) {
		light[i] = false;
		contrast[i] = 0;
	}
	light[CTRL_PRESSED] = light[CTRL_HOT] = light[CTRL_HOTCHECKED] = true;
	overpaint = 0;
}

CH_STYLE(ToolButton, Стиль, StyleSolid)
{
	const Кнопка::Стиль& bs = Кнопка::StyleNormal();
	look[0] = bs.look[0];
	look[1] = bs.look[1];
	look[2] = bs.look[2];
	look[3] = bs.look[3];
	look[4] = bs.look[2];
	look[5] = bs.look[1];
	font = StdFont();
	for(int i = 0; i < 4; i++)
		textcolor[i] = Кнопка::StyleNormal().textcolor[i];
	textcolor[CTRL_CHECKED] = textcolor[CTRL_NORMAL];
	textcolor[CTRL_HOTCHECKED] = textcolor[CTRL_HOT];
	for(int i = 0; i < 6; i++) {
		light[i] = false;
		contrast[i] = 0;
	}
	light[CTRL_PRESSED] = light[CTRL_HOT] = light[CTRL_HOTCHECKED] = true;
}

ToolButton::ToolButton()
{
	переустанов();
	checked = false;
	paint_checked = false;
	nodarkadjust = false;
	Transparent();
	kind = NOLABEL;
}

ToolButton::~ToolButton() {}

void ToolButton::ResetKeepStyle()
{
	repeat = false;
	accel = 0;
	checked = false;
	NoWantFocus();
	minsize = Размер(0, 0);
	maxiconsize = Размер(INT_MAX, INT_MAX);
	Подсказка("");
	Help("");
	Topic("");
	Description("");
}

void ToolButton::переустанов()
{
	ResetKeepStyle();
	style = &дефСтиль();
}

void ToolButton::UpdateTip()
{
	LTIMING("UpdateTip");
	Ткст s = tiptext;
	if(пусто_ли(s) && kind == NOLABEL)
		s = text;
	if(accel) {
		if(s.дайСчёт())
			s << ' ';
		s << '(' << GetKeyDesc(accel) << ')';
	}
	Ктрл::Подсказка(s);
}

Бар::Элемент& ToolButton::устТекст(const char *txt)
{
	Ткст newtext;
	ExtractAccessKey(txt, newtext);
	if(newtext != text) {
		text = newtext;
		UpdateTip();
		освежи();
	}
	return *this;
}

ToolButton& ToolButton::устСтиль(const Стиль& s)
{
	if(style != &s) {
		style = &s;
		освежи();
	}
	return *this;
}

Бар::Элемент& ToolButton::Check(bool check)
{
	if(checked != check) {
		checked = check;
		освежи();
	}
	return *this;
}

Бар::Элемент& ToolButton::Radio(bool check)
{
	return Check(check);
}

Бар::Элемент& ToolButton::Ключ(dword ключ)
{
	if(ключ) {
		accel = ключ;
		UpdateTip();
	}
	return *this;
}

Бар::Элемент& ToolButton::повтори(bool r)
{
	repeat = r;
	return *this;
}

ToolButton& ToolButton::Надпись(const char *text, int _kind)
{
	if(kind != _kind) {
		kind = _kind;
		освежи();
	}
	устТекст(text);
	return *this;
}

ToolButton& ToolButton::Надпись(const char *text)
{
	Надпись(text, kind == NOLABEL ? RIGHTLABEL : kind);
	return *this;
}

ToolButton& ToolButton::Kind(int _kind)
{
	if(kind != _kind) {
		kind = _kind;
		освежи();
	}
	return *this;
}

Рисунок ToolButton::GetImage() const
{
	return CachedRescale(img, min(img.дайРазм(), maxiconsize));
}

Бар::Элемент& ToolButton::Рисунок(const РНЦП::Рисунок& img_)
{
	РНЦП::Рисунок m = img_;
	if(!m.одинаково(img)) {
		img = m;
		освежи();
	}
	return *this;
}

Бар::Элемент& ToolButton::вкл(bool enable)
{
	Ктрл::вкл(enable);
	return *this;
}

Бар::Элемент& ToolButton::Подсказка(const char *tip)
{
	if(tiptext != tip) {
		tiptext = tip;
		UpdateTip();
	}
	return *this;
}

Бар::Элемент& ToolButton::Help(const char *help)
{
	HelpLine(help);
	return *this;
}

Бар::Элемент& ToolButton::Topic(const char *help)
{
	HelpTopic(help);
	return *this;
}

Бар::Элемент& ToolButton::Description(const char *desc)
{
	Ктрл::Description(desc);
	return *this;
}

struct sCachedContrast : public ImageMaker
{
	int   d;
	Рисунок img;

	virtual Ткст Ключ() const {
		Ткст s;
		RawCat(s, d);
		RawCat(s, img.GetSerialId());
		return s;
	}

	virtual Рисунок сделай() const {
		return Contrast(img, 256 + d);
	}
};

Рисунок CachedContrast(const Рисунок& m, int d)
{
	if(d == 0)
		return m;
	sCachedContrast cr;
	cr.d = d;
	cr.img = m;
	return MakeImage(cr);
}

void  ToolButton::рисуй(Draw& w)
{
	LTIMING("ToolButton::рисуй");
	paint_checked = checked;
	Размер sz = дайРазм();
	РНЦП::Рисунок image = GetImage();
	Размер isz = image.дайРазм();
//	Ктрл *q = дайРодителя()->дайРодителя();
//	if(!q || !q->IsTransparent())
//		w.DrawRect(sz, checked && !естьМышь() ? смешай(SColorFace, SColorLight) : SColorFace);
	int li = включен_ли() ? естьМышь() ? дайЛевуюМыши() ? CTRL_PRESSED
						                               : checked ? CTRL_HOTCHECKED : CTRL_HOT
				                       : checked ? CTRL_CHECKED : CTRL_NORMAL
					     : CTRL_DISABLED;
	ChPaint(w, sz, style->look[li]);
	Точка off = style->offset[li];
	Точка ip = (sz - isz) / 2 + off;
	Размер tsz(0, 0);
	if(kind != NOLABEL)
		tsz = дайРазмТекста(text, style->font);
	if(kind == BOTTOMLABEL) {
		ip.y  -= tsz.cy / 2 + 1;
		w.DrawText((sz.cx - tsz.cx) / 2 + off.x, ip.y + isz.cy + 2 + off.y, text, style->font, style->textcolor[li]);
	}
	if(kind == RIGHTLABEL) {
		ip.x -= tsz.cx / 2 + 2;
		w.DrawText(ip.x + isz.cx + 3 + off.x, (sz.cy - tsz.cy) / 2 + off.y, text, style->font, style->textcolor[li]);
	}
	РНЦП::Рисунок img = CachedContrast(image, style->contrast[li]);
	if(!включен_ли())
		img = DisabledImage(img);
	if(включен_ли() && style->light[li])
		DrawHighlightImage(w, ip.x, ip.y, img, true);
	else
		w.DrawImage(ip.x, ip.y, img);
}

void  ToolButton::входМыши(Точка, dword)
{
	КтрлБар::SendHelpLine(this);
	освежи();
}

void  ToolButton::выходМыши()
{
	КтрлБар::ClearHelpLine(this);
	освежи();
}

Размер  ToolButton::дайМинРазм() const
{
	РНЦП::Рисунок image = GetImage();
	Размер sz = image.дайРазм();
	if(sz.пустой())
		sz = Размер(16, 16);
	sz += DPI(6);
	if(text.дайСчёт()) {
		Размер tsz = дайРазмТекста(text, style->font);
		if(kind == BOTTOMLABEL) {
			sz.cy += tsz.cy + DPI(3);
			sz.cx = max(sz.cx, tsz.cx + DPI(9));
		}
		if(kind == RIGHTLABEL) {
			sz.cx += tsz.cx + DPI(6);
			sz.cy = max(sz.cy, tsz.cy + DPI(6));
		}
	}
	return max(sz, minsize);
}

void  ToolButton::леваяВнизу(Точка, dword)
{
	освежи();
	if(repeat)
		WhenAction();
}

void  ToolButton::леваяПовтори(Точка, dword)
{
	освежи();
	if(repeat)
		WhenAction();
}

void  ToolButton::леваяВверху(Точка, dword)
{
	освежи();
	if(!repeat)
		WhenAction();
}

bool  ToolButton::горячаяКлав(dword ключ)
{
	if(ключ == accel) {
		WhenAction();
		return true;
	}
	return false;
}

void ToolButton::FinalSync()
{
	if(checked != paint_checked)
		освежи();
}

Ткст ToolButton::дайОпис() const
{
	return tiptext;
}

int ToolButton::рисуйПоверх() const
{
	return style->overpaint;
}

}
