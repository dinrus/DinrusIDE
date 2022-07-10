#include "CtrlLib.h"

namespace РНЦП {

void CtrlsImageLook(Значение *look, int i, int n)
{
	while(n--)
		*look++ = CtrlsImg::дай(i++);
}

void CtrlsImageLook(Значение *look, int i, const Рисунок& image, const Цвет *color, int n)
{
	for(int q = 0; q < n; q++)
		*look++ = ChLookWith(CtrlsImg::дай(i++), image, *color++);
}

void CtrlsImageLook(Значение *look, int i, const Рисунок& image, int n)
{
	for(int q = 0; q < n; q++)
		*look++ = ChLookWith(CtrlsImg::дай(i++), image);
}

Ткст DeAmp(const char *s)
{
	Ткст out;
	for(; *s; out.конкат(*s++))
		if(*s == '&')
			out.конкат('&');
	return out;
}

Размер GetSmartTextSize(const char *text, Шрифт font, int cx) {
	if(*text == '\1') {
		Размер sz;
		RichText txt = ParseQTF(text + 1);
		txt.ApplyZoom(GetRichTextStdScreenZoom());
		sz.cx = min(cx, txt.дайШирину());
		sz.cy = txt.дайВысоту(Zoom(1, 1), sz.cx);
		return sz;
	}
	return GetTLTextSize(вЮникод(text, CHARSET_DEFAULT), font);
}

int GetSmartTextHeight(const char *s, int cx, Шрифт font) {
	if(*s == '\1') {
		Размер sz;
		RichText txt = ParseQTF(s + 1);
		txt.ApplyZoom(GetRichTextStdScreenZoom());
		return txt.дайВысоту(Zoom(1, 1), cx);
	}
	int cy = font.Info().дайВысоту();
	int h = cy;
	while(*s) {
		if(*s == '\n')
			h += cy;
		s++;
	}
	return h;
}

void DrawSmartText(Draw& draw, int x, int y, int cx, const char *text, Шрифт font, Цвет ink, int accesskey, Цвет qtf_ink) {
	if(*text == '\1') {
		RichText txt = ParseQTF(text + 1, accesskey);
		txt.ApplyZoom(GetRichTextStdScreenZoom());
		PaintInfo pi;
		pi.darktheme = Grayscale(SColorPaper()) < 100;
		pi.textcolor = qtf_ink;
		txt.рисуй(draw, x, y, cx, pi);
		return;
	}
	DrawTLText(draw, x, y, cx, вЮникод(text, CHARSET_DEFAULT), font, ink, accesskey);
}

bool CompareAccessKey(int accesskey, dword ключ)
{
	return accesskey && dword(взаг(accesskey & 255) - 'A' + K_ALT_A) == ключ;
}

int  ExtractAccessKey(const char *s, Ткст& label)
{
	byte akey = 0;
	int  pos = 0;
	Ткст text;
	bool qtf = *s == '\1';
	while(*s)
		if((*s == '&' && !qtf || *s == '\b') && s[1] && s[1] != '&') {
			akey = вАски(взаг(s[1]));
			pos = text.дайДлину() + 1;
			s++;
		}
		else
			text.конкат(*s++);
	text.сожми();
	label = text;
	return MAKELONG(akey, pos);
}

int  ChooseAccessKey(const char *text, dword used)
{
	for(const char *s = text; *s; s++) {
		byte ac = *s;
		if(ac < 128 && ac >= 'A' && ac <= 'Z' && (Ктрл::AccessKeyBit(ac) & used) == 0)
			return MAKELONG(ac, s - text + 1);
	}
	for(const char *s = text; *s; s++) {
		dword ac = взаг(*s);
		if(ac < 128 && ac >= 'A' && ac <= 'Z' && ac != 'I' && ac != 'L' && (Ктрл::AccessKeyBit(ac) & used) == 0)
			return ac;
	}
	for(const char *s = text; *s; s++) {
		dword ac = взаг(*s);
		if(ac < 128 && ac >= 'A' && ac <= 'Z' && (Ктрл::AccessKeyBit(ac) & used) == 0)
			return ac;
	}
	return 0;
}

DrawLabel::DrawLabel()
{
	push = focus = disabled = false;
	lspc = rspc = 0;
	limg_never_hide = false;
	rimg_never_hide = false;
	ink = disabledink = Null;
	align = valign = ALIGN_CENTER;
	accesskey = 0;
	accesspos = -1;
	font = StdFont();
	nowrap = false;
}

Размер DrawLabel::дайРазм(int txtcx) const
{
	return дайРазм(txtcx, limg.дайРазм(), lspc, rimg.дайРазм(), rspc);
}

Размер DrawLabel::дайРазм(int txtcx, Размер sz1, int lspc, Размер sz2, int rspc) const
{
	Размер isz(0, 0);
	Размер txtsz = *text ? GetSmartTextSize(text, font, nowrap ? INT_MAX/2 : txtcx)
	                   : paintrect.дайСтдРазм();

	if(!пусто_ли(lspc)) {
		isz.cx = lspc;
		isz.cy = sz1.cy;
		isz.cx += sz1.cx;
	}

	if(!пусто_ли(rspc)) {
		isz.cx += rspc;
		if(sz2.cy > isz.cy) isz.cy = sz2.cy;
		isz.cx += sz2.cx;
	}

	isz.cy = max(txtsz.cy, max(sz1.cy, sz2.cy));
	isz.cx += txtsz.cx;

	return isz;
}

Рисунок DisImage(const Рисунок& m)
{
	Рисунок mm = Grayscale(m, 200);
	ImageBuffer ib(mm);
	КЗСА *s = ~ib;
	КЗСА *e = s + ib.дайДлину();
	while(s < e)
		(s++)->a /= 3;
	Premultiply(ib);
	return ib;
}

Рисунок DisabledImage(const Рисунок& img, bool dis)
{
	return dis ? MakeImage(img, GUI_GlobalStyle() == GUISTYLE_CLASSIC ? Etched : DisImage)
	           : img;
}

Цвет GetLabelTextColor(const Ктрл *ctrl)
{
	if(!IsLabelTextColorMismatch())
		return SColorLabel();
	while(ctrl) {
		if(!ctrl->IsTransparent()) {
			if(dynamic_cast<const ТопОкно *>(ctrl) || dynamic_cast<const КтрлВкладка *>(ctrl) ||
			   dynamic_cast<const ToolBar *>(ctrl) || dynamic_cast<const БарМеню *>(ctrl) ||
			   dynamic_cast<const СтатичПрям *>(ctrl) || dynamic_cast<const StaticBarArea *>(ctrl))
				break;
			return SColorText();
		}
		ctrl = ctrl->дайРодителя();
	}
	return SColorLabel();
}

Размер DrawLabel::рисуй(Ктрл *ctrl, Draw& w, const Прям& r, bool visibleaccesskey) const
{
	int lspc = this->lspc;
	int rspc = this->rspc;
	Размер sz1 = limg.дайРазм();
	Размер sz2 = rimg.дайРазм();
	int txtcx = r.дайШирину() - sz1.cx - Nvl(lspc, 0) - sz2.cx - Nvl(rspc, 0);
	bool donowrap = nowrap && *text != '\1';
	Размер txtsz = *text ? GetSmartTextSize(text, font, donowrap ? INT_MAX/2 : txtcx) : paintrect.дайСтдРазм();
	if(txtsz.cx) {
		if(!rimg_never_hide && txtsz.cx + sz1.cx + sz2.cx + Nvl(lspc, 0) + Nvl(rspc, 0) > r.дайШирину()) {
			sz2.cx = 0;
			rspc = 0;
		}
		if(!limg_never_hide && txtsz.cx + sz1.cx + sz2.cx + Nvl(lspc, 0) + Nvl(rspc, 0) > r.дайШирину()) {
			sz1.cx = 0;
			lspc = 0;
		}
	}
	Размер isz = дайРазм(txtcx, sz1, lspc, sz2, rspc);
	Точка p = r.верхЛево(), ip;
	if(align == ALIGN_LEFT)
		p.x = r.left;
	else
	if(align == ALIGN_RIGHT)
		p.x = r.right - isz.cx;
	else
	if(align == ALIGN_CENTER)
		p.x = (r.right + r.left - isz.cx) / 2;
	if(valign == ALIGN_TOP)
		p.y = r.top;
	else
	if(valign == ALIGN_BOTTOM)
		p.y = r.bottom - isz.cy;
	else
	if(valign == ALIGN_CENTER)
		p.y = (r.bottom + r.top - txtsz.cy) / 2;
	Цвет color = disabled && !пусто_ли(disabledink) ? disabledink : ink;
	if(пусто_ли(color))
		color = disabled ? SColorDisabled : GetLabelTextColor(ctrl);
	int ix;
	if(пусто_ли(lspc))
		ix = r.left + push;
	else {
		ix = p.x + push;
		p.x += sz1.cx;
		p.x += lspc;
	}
	int iy = push + (r.top + r.bottom - sz1.cy) / 2;

	if(sz1.cx) {
		if(пусто_ли(lcolor))
			w.DrawImage(ix, iy, DisabledImage(limg, disabled));
		else
			w.DrawImage(ix, iy, limg, lcolor);
	}
	iy = push + (r.top + r.bottom - sz2.cy) / 2;
	ix = (пусто_ли(rspc) ? r.right - sz2.cx : p.x + txtsz.cx + rspc) + push;
	if(sz2.cx) {
		if(пусто_ли(rcolor))
			w.DrawImage(ix, iy, DisabledImage(rimg, disabled));
		else
			w.DrawImage(ix, iy, rimg, rcolor);
	}
	paintrect.рисуй(w, p.x + push, p.y + push, txtsz.cx, isz.cy, color, Null);
	
	if(*text) {
		if(disabled && *text != '\1')
			DrawSmartText(w, p.x + push + 1, p.y + push + 1,
			              donowrap ? INT_MAX/2 : txtcx, text, font, SColorPaper);
		DrawSmartText(w, p.x + push, p.y + push, donowrap ? INT_MAX/2 : txtcx,
		              text, font, color, visibleaccesskey ? accesskey : 0);
		if(focus)
			DrawFocus(w, p.x - 2, p.y, txtsz.cx + 5, isz.cy);
	}

	return isz;
}

Размер DrawLabel::рисуй(Ктрл *ctrl, Draw& w, int x, int y, int cx, int cy, bool visibleaccesskey) const
{
	return рисуй(ctrl, w, RectC(x, y, cx, cy), visibleaccesskey);
}

Размер DrawLabel::рисуй(Draw& w, const Прям& r, bool visibleaccesskey) const
{
	return рисуй(NULL, w, r, visibleaccesskey);
}

Размер DrawLabel::рисуй(Draw& w, int x, int y, int cx, int cy, bool vak) const
{
	return рисуй(w, RectC(x, y, cx, cy), vak);
}

void НадписьОснова::обновиНадпись() {}

НадписьОснова& НадписьОснова::SetLeftImage(const Рисунок& img, int spc, bool never_hide) {
	lbl.limg = img;
	lbl.lspc = spc;
	lbl.limg_never_hide = never_hide;
	обновиНадпись();
	return *this;
}

НадписьОснова& НадписьОснова::SetRightImage(const Рисунок& img, int spc, bool never_hide) {
	lbl.rimg = img;
	lbl.rspc = spc;
	lbl.rimg_never_hide = never_hide;
	обновиНадпись();
	return *this;
}

НадписьОснова& НадписьОснова::SetPaintRect(const РисПрям& paintrect) {
	lbl.paintrect = paintrect;
	обновиНадпись();
	return *this;
}


НадписьОснова& НадписьОснова::устТекст(const char *text) {
	lbl.text = text;
	обновиНадпись();
	return *this;
}

НадписьОснова& НадписьОснова::устШрифт(Шрифт font) {
	if(lbl.font != font) {
		lbl.font = font;
		обновиНадпись();
	}
	return *this;
}

НадписьОснова& НадписьОснова::NoWrap(bool b)
{
	if(lbl.nowrap != b) {
		lbl.nowrap = b;
		обновиНадпись();
	}
	return *this;
}

НадписьОснова& НадписьОснова::устЧернила(Цвет ink, Цвет disabledink) {
	if(lbl.ink != ink || lbl.disabledink != disabledink) {
		lbl.ink = ink;
		lbl.disabledink = disabledink;
		обновиНадпись();
	}
	return *this;
}

НадписьОснова& НадписьОснова::устЛин(int align) {
	if(lbl.align != align) {
		lbl.align = align;
		обновиНадпись();
	}
	return *this;
}

НадписьОснова& НадписьОснова::SetVAlign(int valign) {
	if(lbl.valign != valign) {
		lbl.valign = valign;
		обновиНадпись();
	}
	return *this;
}

Размер НадписьОснова::PaintLabel(Ктрл *ctrl, Draw& w, const Прям& r, bool disabled, bool push, bool focus, bool vak)
{
	DrawLabel lbl1 = lbl;
	lbl1.disabled = disabled;
	lbl1.push = push;
	lbl1.focus = focus;
	return lbl1.рисуй(ctrl, w, r, vak);
}

Размер НадписьОснова::PaintLabel(Ктрл *ctrl, Draw& w, int x, int y, int cx, int cy, bool disabled, bool push, bool focus, bool vak)
{
	return PaintLabel(ctrl, w, RectC(x, y, cx, cy), disabled, push, focus, vak);
}

Размер НадписьОснова::PaintLabel(Draw& w, const Прям& r, bool disabled, bool push, bool focus, bool vak)
{
	return PaintLabel(NULL, w, r, disabled, push, focus, vak);
}


Размер НадписьОснова::PaintLabel(Draw& w, int x, int y, int cx, int cy, bool disabled, bool push, bool focus, bool vak)
{
	return PaintLabel(w, RectC(x, y, cx, cy), disabled, push, focus, vak);
}

Размер НадписьОснова::GetLabelSize() const
{
	return lbl.дайРазм();
}

void LinkToolTipIn__();

НадписьОснова::~НадписьОснова() {
	LinkToolTipIn__();
}

void DrawFocus(Draw& w, int x, int y, int cx, int cy, Цвет c) {
	w.Clipoff(x, y, cx, cy);
	for(int a = 0; a < cx; a += CtrlImg::focus_h().дайШирину()) {
		w.DrawImage(a, 0, CtrlImg::focus_h(), c);
		w.DrawImage(a, cy - DPI(1), CtrlImg::focus_h(), c);
	}
	for(int a = 0; a < cy; a += CtrlImg::focus_v().дайВысоту()) {
		w.DrawImage(0, a, CtrlImg::focus_v(), c);
		w.DrawImage(cx - DPI(1), a, CtrlImg::focus_v(), c);
	}
	w.стоп();
}

void DrawFocus(Draw& w, const Прям& r, Цвет c) {
	DrawFocus(w, r.left, r.top, r.устШирину(), r.устВысоту(), c);
}

void DrawHorzDrop(Draw& w, int x, int y, int cx)
{
	w.DrawRect(x, y, cx, 2, SColorHighlight);
	w.DrawRect(x, y - 2, 1, 6, SColorHighlight);
	w.DrawRect(x + cx - 1, y - 2, 1, 6, SColorHighlight);
	w.DrawRect(x + 1, y - 1, 1, 4, SColorHighlight);
	w.DrawRect(x + cx - 2, y - 1, 1, 4, SColorHighlight);
}

void DrawVertDrop(Draw& w, int x, int y, int cy)
{
	w.DrawRect(x, y, 2, cy, SColorHighlight);
	w.DrawRect(x - 2, y, 6, 1, SColorHighlight);
	w.DrawRect(x - 2, y + cy - 1, 6, 1, SColorHighlight);
	w.DrawRect(x - 1, y + 1, 4, 1, SColorHighlight);
	w.DrawRect(x - 1, y + cy - 2, 4, 1, SColorHighlight);
}

Точка GetDragScroll(Ктрл *ctrl, Точка p, Размер max)
{
	if(ctrl->толькочтен_ли())
		return Точка(0, 0);
	Размер sz = ctrl->дайРазм();
	Размер sd = min(sz / 6, Размер(16, 16));
	Точка d(0, 0);
	if(p.x < sd.cx)
		d.x = p.x - sd.cx;
	if(p.x > sz.cx - sd.cx)
		d.x = p.x - sz.cx + sd.cx;
	if(p.y < sd.cy)
		d.y = p.y - sd.cy;
	if(p.y > sz.cy - sd.cy)
		d.y = p.y - sz.cy + sd.cy;
	d.x = minmax(d.x, -max.cx, max.cx);
	d.y = minmax(d.y, -max.cy, max.cy);
	return d;
}

Точка GetDragScroll(Ктрл *ctrl, Точка p, int max)
{
	return GetDragScroll(ctrl, p, Размер(max, max));
}

Прям LookMargins(const Прям& r, const Значение& ch)
{
	Прям m = ChMargins(ch);
	int fcy = GetStdFontCy();
	if(m.top + m.bottom + fcy > r.дайВысоту())
		m.top = m.bottom = max((r.дайВысоту() - fcy) / 2, 0);
	return m;
}

void ActiveEdgeFrame::выложиФрейм(Прям& r)
{
	Прям m = LookMargins(r, edge[0]);
	r.left += m.left;
	r.right -= m.right;
	r.top += m.top;
	r.bottom -= m.bottom;
}

void ActiveEdgeFrame::рисуйФрейм(Draw& w, const Прям& r)
{
	int i = 0;
	if(ctrl) {
		i = !ctrl->включен_ли() || ctrl->толькочтен_ли() ? CTRL_DISABLED
		    : button ? push : ctrl->естьФокус() ? CTRL_PRESSED
		    : mousein ? CTRL_HOT
		    : CTRL_NORMAL;
	}
	ChPaintEdge(w, r, edge[i]);
	if(!пусто_ли(coloredge))
		ChPaintEdge(w, r, coloredge, color);
}

void ActiveEdgeFrame::добавьРазмФрейма(Размер& sz)
{
	Прям m = ChMargins(edge[0]);
	sz.cx += m.left + m.right;
	sz.cy += m.top + m.bottom;
}

void ActiveEdgeFrame::уст(const Ктрл *ctrl_, const Значение *edge_, bool active)
{
	ctrl = active ? ctrl_ : NULL;
	edge = edge_;
}

}
