#include "CtrlLib.h"

namespace РНЦП {

void СтатичТекст::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	if(!IsTransparent())
		w.DrawRect(0, 0, sz.cx, sz.cy, SColorFace);
	PaintLabel(this, w, 0, 0, sz.cx, sz.cy, !IsShowEnabled(), false, false, VisibleAccessKeys());
}

Размер СтатичТекст::дайМинРазм() const
{
	return GetLabelSize();
}

void СтатичТекст::обновиНадпись() {
	освежи();
}

СтатичТекст::СтатичТекст()
{
	NoWantFocus();
	IgnoreMouse();
	Transparent();
	устЛин(ALIGN_LEFT);
}

Надпись& Надпись::устТекст(const char *text)
{
	НадписьОснова::устТекст(text);
	lbl.accesskey = 0;
	noac = false;
	return *this;
}

Надпись& Надпись::устНадпись(const char *_text)
{
	Ткст text;
	int accesskey = ExtractAccessKey(_text, text);
	НадписьОснова::устТекст(text);
	lbl.accesskey = accesskey;
	return *this;
}

bool Надпись::горячаяКлав(dword ключ) {
	if(CompareAccessKey(lbl.accesskey, ключ)) {
		IterateFocusForward(this, дайРодителя());
		return true;
	}
	return false;
}

dword Надпись::дайКлючиДоступа() const
{
	return AccessKeyBit(lbl.accesskey);
}

void  Надпись::присвойКлючиДоступа(dword used)
{
	if(noac)
		return;
	Ктрл *next = дайСледщ();
	if(!lbl.accesskey && next && next->IsInitFocus()) {
		next->присвойКлючиДоступа(used);
		if(!next->GetAccessKeysDeep()) {
			lbl.accesskey = ChooseAccessKey(дайТекст(), used);
			if(lbl.accesskey) освежи();
			used |= AccessKeyBit(lbl.accesskey);
		}
	}
	Ктрл::присвойКлючиДоступа(used);
}

Надпись::Надпись() {
	noac = false;
}

Надпись::~Надпись() {}

CH_COLOR(цветБоксНадписи, SColorShadow());

CH_COLOR(цветТекстаБоксНадписи, SColorText());
CH_COLOR(LabelBoxDisabledTextColor, SColorDisabled());

БоксНадпись::БоксНадпись()
{
	color = Null;
	НадписьОснова::устЧернила(цветТекстаБоксНадписи(), LabelBoxDisabledTextColor());
	SetVAlign(ALIGN_TOP);
}

void  БоксНадпись::присвойКлючиДоступа(dword used)
{
	Ктрл::присвойКлючиДоступа(used);
}

БоксНадпись::~БоксНадпись() {}

Прям БоксНадпись::дайПроцПрям() const
{
	Прям r = дайРазм();
	r.left += 2;
	r.top += max(GetLabelSize().cy, 2);
	r.bottom -= 2;
	r.right -= 2;
	return r;
}

Значение БоксНадпись::LabelBoxLook;

void рисуйБоксНадпись(Draw& w, Размер sz, Цвет color, int d)
{
	bool hline = sz.cy < 2 * Draw::GetStdFontCy();
	bool vline = sz.cx < 2 * Draw::GetStdFontCy();
	if(GUI_GlobalStyle() >= GUISTYLE_XP || !пусто_ли(color)) {
		if(hline) {
			d = sz.cy / 2;
			w.DrawRect(0, d - 1, sz.cx, 1, SColorLight);
			w.DrawRect(0, d, sz.cx, 1, SColorShadow);
			w.DrawRect(0, d + 1, sz.cx, 1, SColorLight);
		}
		else
		if(vline) {
			d = sz.cx / 2;
			w.DrawRect(d - 1, 0, 1, sz.cy, SColorLight);
			w.DrawRect(d, 0, 1, sz.cy, SColorShadow);
			w.DrawRect(d + 1, 0, 1, sz.cy, SColorLight);
		}
		else {
			Значение look = БоксНадпись::GetLook();
			if(пусто_ли(look) || !пусто_ли(color)) {
				Цвет c = Nvl(color, цветБоксНадписи);
				w.DrawRect(0, d + 2, 1, sz.cy - d - 4, c);
				w.DrawRect(sz.cx - 1, d + 2, 1, sz.cy - d - 4, c);
				w.DrawRect(2, sz.cy - 1, sz.cx - 4, 1, c);
				w.DrawRect(2, d, sz.cx - 4, 1, c);
	
				w.DrawRect(1, d + 1, 2, 1, c);
				w.DrawRect(1, d + 2, 1, 1, c);
	
				w.DrawRect(sz.cx - 3, d + 1, 2, 1, c);
				w.DrawRect(sz.cx - 2, d + 2, 1, 1, c);
	
				w.DrawRect(1, sz.cy - 2, 2, 1, c);
				w.DrawRect(1, sz.cy - 3, 1, 1, c);
	
				w.DrawRect(sz.cx - 3, sz.cy - 2, 2, 1, c);
				w.DrawRect(sz.cx - 2, sz.cy - 3, 1, 1, c);
			}
			else {
				Прям r = sz;
				r.top = d;
				ChPaintEdge(w, r, look);
			}
		}
	}
	else {
		if(hline) {
			d = sz.cy / 2;
			w.DrawRect(0, d, sz.cx, 1, SColorShadow);
			w.DrawRect(0, d + 1, sz.cx, 1, SColorLight);
		}
		else
		if(vline) {
			d = sz.cx / 2;
			w.DrawRect(d, 0, 1, sz.cy, SColorShadow);
			w.DrawRect(d - 1, 1, 0, sz.cy, SColorLight);
		}
		else {
			w.DrawRect(1, d, sz.cx - 2, 1, SColorShadow);
			w.DrawRect(1, d + 1, sz.cx - 2, 1, SColorLight);

			w.DrawRect(0, d, 1, sz.cy - d - 1, SColorShadow);
			w.DrawRect(1, d + 1, 1, sz.cy - d - 2, SColorLight);

			w.DrawRect(sz.cx - 2, d, 1, sz.cy - d, SColorShadow);
			w.DrawRect(sz.cx - 1, d, 1, sz.cy - d, SColorLight);

			w.DrawRect(1, sz.cy - 2, sz.cx - 2, 1, SColorShadow);
			w.DrawRect(1, sz.cy - 1, sz.cx - 2, 1, SColorLight);
		}
	}
}

void БоксНадпись::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	if(!IsTransparent())
		w.DrawRect(sz, SColorFace);
	Размер lsz = GetLabelSize();
	int d = lsz.cy >> 1;
	int ty = sz.cy < 2 * Draw::GetStdFontCy() ? (sz.cy - lsz.cy) / 2 : 0;
	Размер ts = PaintLabel(w, d + DPI(2), ty, sz.cx, lsz.cy, !IsShowEnabled(), false, false, VisibleAccessKeys());
	w.старт();
	w.ExcludeClip(d, ty, ts.cx + DPI(4), ts.cy);
	рисуйБоксНадпись(w, sz, color, d);
	w.стоп();
}

Прям КтрлРодитель::дайПроцПрям() const
{
	return дайРазм();
}

Размер КтрлРодитель::дайСтдРазм() const
{
	return дайМинРазм();
}

Размер КтрлРодитель::дайМинРазм() const
{
	return пусто_ли(minsize) ? Ктрл::дайМинРазм() : minsize;
}

КтрлРодитель::КтрлРодитель()
{
	NoWantFocus();
	Transparent();
	minsize = Null;
}

СтатичПрям& СтатичПрям::фон(const Значение& chvalue)
{
	if(chvalue != bg) {
		bg = chvalue;
		освежи();
	}
	return *this;
}

void СтатичПрям::рисуй(Draw& w)
{
	ChPaint(w, дайРазм(), bg);
}

СтатичПрям::СтатичПрям() {
	bg = SColorFace();
	NoWantFocus();
}

СтатичПрям::~СтатичПрям() {}

void КтрлРисунок::рисуй(Draw& w)
{
	if(!img) return;
	Размер sz = дайРазм();
	Размер bsz = дайСтдРазм();
	w.DrawImage((sz.cx - bsz.cx) / 2, (sz.cy - bsz.cy) / 2, img);
}

Размер КтрлРисунок::дайСтдРазм() const
{
	return img.дайРазм();
}

Размер КтрлРисунок::дайМинРазм() const
{
	return img.дайРазм();
}


КтрлЧертёж& КтрлЧертёж::фон(Цвет color)
{
	background = color;
	Transparent(пусто_ли(color));
	освежи();
	return *this;
}

void КтрлЧертёж::рисуй(Draw& w) {
	Размер sz = дайРазм();
	w.DrawRect(0, 0, sz.cx, sz.cy, background);
	if(!picture) return;
	int dx = 0;
	int dy = 0;
	Размер rz = sz;
	if(ratio) {
		Размер sr = picture.дайРазм();
		if(sr.cy * sz.cx < sz.cy * sr.cx) {
			if(sr.cx) {
				rz.cy = sr.cy * sz.cx / sr.cx;
				dy = (sz.cy - rz.cy) / 2;
			}
		}
		else {
			if(sr.cy) {
				rz.cx = sr.cx * sz.cy / sr.cy;
				dx = (sz.cx - rz.cx) / 2;
			}
		}
	}
	w.Clipoff(dx, dy, rz.cx, rz.cy);
	w.DrawDrawing(0, 0, rz.cx, rz.cy, picture);
	w.стоп();
}

КтрлЧертёж::КтрлЧертёж() {
	ratio = true;
	background = белый;
}

Размер КтрлСепаратор::дайМинРазм() const {
	return Размер(size, size);
}

CH_STYLE(КтрлСепаратор, Стиль, дефСтиль)
{
	l1 = SColorShadow();
	l2 = Null;
}

void КтрлСепаратор::рисуй(Draw& w) {
	Размер sz = дайРазм();
	if(sz.cx > sz.cy) {
		int q = sz.cy / 2;
		ChPaint(w, lmargin, q - 1, sz.cx - (lmargin + rmargin), 1, style->l1);
		ChPaint(w, lmargin, q, sz.cx - (lmargin + rmargin), 1, style->l2);
	}
	else {
		int q = sz.cx / 2;
		ChPaint(w, q - 1, lmargin, 1, sz.cy - (lmargin + rmargin), style->l1);
		ChPaint(w, q, lmargin, 1, sz.cy - (lmargin + rmargin), style->l2);
	}
};

КтрлСепаратор& КтрлСепаратор::Margin(int l, int r)
{
	if(l != lmargin || r != rmargin) {
		lmargin = l;
		rmargin = r;
		освежи();
	}
	return *this;
}

КтрлСепаратор& КтрлСепаратор::устРазм(int w)
{
	if(w != size) {
		size = w;
		освежи();
	}
	return *this;
}

КтрлСепаратор& КтрлСепаратор::устСтиль(const Стиль& s)
{
	if(&s != style) {
		style = &s;
		освежи();
	}
	return *this;
}

КтрлСепаратор::КтрлСепаратор()
{
	lmargin = rmargin = 2;
	size = 7;
	NoWantFocus();
	Transparent();
	откл();
	style = &дефСтиль();
}

void КтрлДисплей::рисуй(Draw& w)
{
	pr.рисуй(w, дайРазм());
}

Размер КтрлДисплей::дайМинРазм() const
{
	return pr.дайСтдРазм();
}

void КтрлДисплей::устДанные(const Значение& v)
{
	pr.устЗначение(v);
	освежи();
}

Значение КтрлДисплей::дайДанные() const
{
	return pr.дайЗначение();
}

void КтрлДисплей::устДисплей(const Дисплей& d)
{
	pr.устДисплей(d);
}

}
