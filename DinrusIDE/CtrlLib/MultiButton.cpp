#include "CtrlLib.h"

namespace РНЦП {

CH_STYLE(MultiButton, Стиль, дефСтиль)
{
	for(int i = 0; i < 4; i++) {
		simple[i] = left[i] = right[i] = lmiddle[i] = rmiddle[i]
			= Кнопка::StyleEdge().look[i];
		monocolor[i] = Кнопка::StyleEdge().monocolor[i];
		fmonocolor[i] = i == CTRL_DISABLED ? SColorDisabled() : SColorText();
		look[i] = trivial[i] = ChLookWith(simple[i], CtrlsImg::DA(), monocolor[i]);
		edge[i] = EditFieldEdge();
	}
	activeedge = false;
	trivialborder = DPI(1);
	border = DPI(1);
	pressoffset = Кнопка::StyleEdge().pressoffset;
	sep1 = SColorHighlight();
	sep2 = SColorLight();
	sepm = DPI(2);
	stdwidth = FrameButtonWidth();
	trivialsep = false;
	margin = Прям(DPI(2), 2, DPI(2), 2);
	usetrivial = false;
	overpaint = loff = roff = 0;
	Ошибка = смешай(светлоКрасный(), красный());
	paper = SColorPaper();
	coloredge = Null;
	clipedge = false;
}

CH_STYLE(MultiButton, Стиль, StyleFrame)
{
	присвой(MultiButton::дефСтиль());
}

MultiButton::SubButton::SubButton()
{
	cx = Null;
	left = false;
	monoimg = false;
	enabled = true;
	main = false;
	visible = true;
}

void MultiButton::SubButton::освежи()
{
	owner->освежи();
	if(owner->Фрейм() && owner->дайРодителя())
		owner->дайРодителя()->освежиВыкладку();
}

MultiButton::SubButton& MultiButton::SubButton::устРисунок(const Рисунок& m)
{
	img = m;
	monoimg = false;
	освежи();
	return *this;
}

MultiButton::SubButton& MultiButton::SubButton::SetMonoImage(const Рисунок& m)
{
	img = m;
	monoimg = true;
	освежи();
	return *this;
}

MultiButton::SubButton& MultiButton::SubButton::SetStdImage()
{
	img = Null;
	monoimg = true;
	освежи();
	return *this;
}

MultiButton::SubButton& MultiButton::SubButton::устНадпись(const char *text)
{
	label = text;
	освежи();
	return *this;
}

MultiButton::SubButton& MultiButton::SubButton::лево(bool b)
{
	left = b;
	освежи();
	return *this;
}

MultiButton::SubButton& MultiButton::SubButton::устШирину(int w)
{
	cx = w;
	освежи();
	return *this;
}

MultiButton::SubButton& MultiButton::SubButton::вкл(bool b)
{
	enabled = b;
	освежи();
	return *this;
}

MultiButton::SubButton& MultiButton::SubButton::покажи(bool b)
{
	visible = b;
	освежи();
	return *this;
}

MultiButton::SubButton& MultiButton::AddButton()
{
	SubButton& b = button.добавь();
	b.owner = this;
	return b;
}

MultiButton::SubButton& MultiButton::InsertButton(int i)
{
	SubButton& b = button.вставь(i);
	b.owner = this;
	return b;
}

void MultiButton::RemoveButton(int i)
{
	button.удали(i);
}

MultiButton::SubButton& MultiButton::SubButton::Main(bool b)
{
	if(b)
		for(int i = 0; i < owner->button.дайСчёт(); i++)
			owner->button[i].main = false;
	main = b;
	return *this;
}

Размер MultiButton::дайМинРазм() const
{
	return Размер(4, StdFont().Info().дайВысоту() + 6);
}

void MultiButton::сфокусирован()
{
	освежи();
}

void MultiButton::расфокусирован()
{
	освежи();
}

static КтрлФрейм& sNullFrame()
{
	static КлассФреймаПусто f;
	return f;
}

int MultiButton::рисуйПоверх() const
{
	return style->overpaint;
}

bool MultiButton::Фрейм()
{
	return false;
}

MultiButton& MultiButton::NoBackground(bool b)
{
	устФрейм(EditFieldFrame());
	nobg = b;
	Transparent();
	освежи();
	return *this;
}

bool MultiButton::ComplexFrame()
{
	return Фрейм() ? дайРодителя() && &дайРодителя()->дайФрейм() != &sNullFrame()
	                 : &дайФрейм() != &sNullFrame();
}

bool MultiButton::Metrics(int& border, int& lx, int &rx, const Прям& r)
{
	border = IsTrivial() ? style->trivialborder : style->border;
	lx = r.left;
	rx = r.right;
	if(ComplexFrame()) {
		border = 0;
		return false;
	}
	if(!пусто_ли(style->edge[0])) {
		lx += border;
		rx -= border;
		return true;
	}
	border = 0;
	return false;
}

bool MultiButton::Metrics(int& border, int& lx, int &rx)
{
	return Metrics(border, lx, rx, дайРазм());
}

int MultiButton::FindButton(int px)
{
	if(толькочтен_ли())
		return Null;
	if(IsTrivial() && !Фрейм())
		return button[0].enabled ? 0 : Null;
	int border, lx, rx;
	Metrics(border, lx, rx);
	for(int i = 0; i < button.дайСчёт(); i++) {
		SubButton& b = button[i];
		int x = 0, cx = 0;
		if(дайПоз(b, lx, rx, x, cx, px))
			return b.enabled ? i : Null;
	}
	if(WhenPush || WhenClick)
		return MAIN;
	if(дисплей)
		for(int i = 0; i < button.дайСчёт(); i++)
			if(button[i].main)
				return i;
	return Null;
}

bool MultiButton::дайПоз(SubButton& b, int& lx, int& rx, int& x, int& cx, int px)
{
	Размер tsz = дайРазмТекста(b.label, StdFont());
	if(b.visible) {
		cx = Nvl(b.cx, style->stdwidth + tsz.cx);
		if(пусто_ли(b.cx) && tsz.cx > 0 && !пусто_ли(b.img))
			cx += DPI(LB_IMAGE + LB_MARGIN);
	}
	else
		cx = 0;
	if(b.left) {
		x = lx;
		lx += cx;
		if(px >= 0 && px < lx)
			return true;
	}
	else {
		rx -= cx;
		x = rx;
		if(px >= 0 && px >= rx)
			return true;
	}
	return false;
}

void MultiButton::дайПоз(int ii, int& x, int& cx)
{
	int border, lx, rx;
	Metrics(border, lx, rx);
	x = cx = 0;
	for(int i = 0; i <= ii; i++) {
		SubButton& b = button[i];
		дайПоз(b, lx, rx, x, cx);
	}
}

void MultiButton::GetLR(int& lx, int& rx)
{
	int border;
	Metrics(border, lx, rx);
	int x = 0;
	int cx = 0;
	for(int i = 0; i < button.дайСчёт(); i++) {
		SubButton& b = button[i];
		дайПоз(b, lx, rx, x, cx);
	}
}

int MultiButton::ChState(int i)
{
	bool frm = Фрейм();
	Ктрл *p = дайРодителя();
	if(i == MAIN && frm && style->activeedge) {
		int q = 0;
		if(p)
			q = !p->включен_ли() || !включен_ли() || p->толькочтен_ли() || i >= 0 && !button[i].enabled ? CTRL_DISABLED
			    : p->естьФокус() || push ? CTRL_PRESSED
			    : p->естьМышь() || hl >= 0 ? CTRL_HOT
			    : CTRL_NORMAL;
		return q;
	}
	if(!IsShowEnabled() || толькочтен_ли() || frm && p && p->толькочтен_ли() || i >= 0 && !button[i].enabled)
		return CTRL_DISABLED;
	if(IsTrivial() && !frm)
		i = 0;
	return hl == i ? push ? CTRL_PRESSED
	                      : CTRL_HOT
	               : CTRL_NORMAL;
}

Прям MultiButton::GetMargin()
{
	Прям m = style->margin;
	int fcy = GetStdFontCy();
	int cy = дайРазм().cy;
	if(m.top + m.bottom + fcy > cy)
		m.top = m.bottom = max((cy - fcy) / 2, 0);
	return m;
}

void MultiButton::Lay(Прям& r, bool minsize)
{
	int border, lx, rx;
	bool frm = Metrics(border, lx, rx);
	bool left = false;
	bool right = false;
	for(int i = 0; i < button.дайСчёт(); i++) {
		SubButton& b = button[i];
		int cx = 0; int x = 0;
		дайПоз(b, lx, rx, x, cx);
		(b.left ? left : right) = true;
	}
	if(ComplexFrame()) {
		r.right = r.left + rx;
		r.left += lx;
	}
	else
	if(frm) {
		Прям m = minsize ? style->margin : GetMargin();
		r = Прям(r.left + max(lx, m.left), r.top + m.top, min(rx, r.right - m.right), r.bottom - m.bottom);
	}
	else {
		Прям m = style->margin;
		r = Прям(r.left + max(lx, m.left), r.top + m.top, min(rx, r.right - m.right), r.bottom - m.bottom);
		if(!IsTrivial() || style->trivialsep) {
			if(left)
				r.left++;
			if(right)
				r.right--;
		}
	}
	if(!пусто_ли(valuecy)) {
		r.top += (r.дайВысоту() - valuecy) / 2;
		r.bottom = r.top + valuecy;
	}
}

void MultiButton::рисуй(Draw& w)
{
	рисуй0(w, false);
}

Прям MultiButton::рисуй0(Draw& w, bool getcr)
{
	Размер sz = дайРазм();
	int border, lx, rx;
	bool frm = Metrics(border, lx, rx);
	int mst = ChState(MAIN);

	Цвет fpaper = Null;
	Цвет text = SColorLabel;
	bool hotpressed = false;
	if(!nobg && !ComplexFrame() && frm) {
		if(mst == CTRL_HOT && !IsTrivial()) {
			fpaper = смешай(SColorHighlight, SColorPaper, 235);
			hotpressed = true;
		}
		else
		if(mst == CTRL_PRESSED && !IsTrivial()) {
			fpaper = смешай(SColorHighlight, SColorFace, 235);
			hotpressed = true;
		}
		else
		if(естьФокус()) {
			fpaper = SColorHighlight();
			text = SColorHighlightText();
			hotpressed = true;
		}
		else
			fpaper = включен_ли() && редактируем_ли() ? style->paper : SColorFace();
	}

	if(frm && !nobg && !getcr) {
		if(style->clipedge) {
			int l, r;
			GetLR(l, r);
			w.Clip(l, 0, r - l, sz.cy);
		}
		ChPaint(w, sz, style->edge[style->activeedge ? mst : 0]);
		Цвет p = paper;
		if(frm && style->activeedge && естьФокус())
			p = SColorHighlight();
		if(hotpressed && (WhenPush || WhenClick))
			p = Nvl(fpaper, paper);
		if(включен_ли() && редактируем_ли())
			p = Nvl(p, style->paper);
		if(!пусто_ли(p) && !пусто_ли(style->coloredge))
			ChPaint(w, sz, style->coloredge, p);
		if(style->clipedge)
			w.стоп();
	}
	bool left = false;
	bool right = false;
	for(int i = 0; i < button.дайСчёт(); i++) {
		SubButton& b = button[i];
		int st = ChState(i);
		int x = 0, cx = 0;
		дайПоз(b, lx, rx, x, cx);
		if(getcr)
			continue;
		bool dopaint = true;
		Значение v = b.left ? left ? style->lmiddle[st] : style->left[st]
		                 : right ? style->rmiddle[st] : style->right[st];
		if(!nobg) {
			if(ComplexFrame())
				ChPaint(w, x, border, cx, sz.cy - 2 * border, style->simple[st]);
			else
			if(frm) {
				if(IsTrivial() && style->usetrivial)
					dopaint = false;
				ChPaint(w, x, border, cx, sz.cy - 2 * border,
				        dopaint ? v : style->trivial[st]);
			}
			else {
				w.Clip(x, 0, cx, sz.cy);
				ChPaint(w, sz, style->look[Фрейм() ? mst : st]);
				if(пусто_ли(v) || !Фрейм()) {
					if((!IsTrivial() || style->trivialsep) && включен_ли() && редактируем_ли()) {
						if(b.left) {
							if(left)
								ChPaint(w, x, style->sepm, 1, sz.cy - 2 * style->sepm, style->sep1);
							ChPaint(w, x + cx - 1, style->sepm, 1, sz.cy - 2 * style->sepm, style->sep2);
						}
						else {
							ChPaint(w, x, style->sepm, 1, sz.cy - 2 * style->sepm, style->sep1);
							if(right)
								ChPaint(w, x + cx - 1, style->sepm, 1, sz.cy - 2 * style->sepm, style->sep2);
						}
					}
				}
				else
					ChPaint(w, x, 0, cx, sz.cy, v);
				w.стоп();
			}
		}
		if(dopaint) {
			Размер tsz = дайРазмТекста(b.label, StdFont());
			Рисунок m = tsz.cx > 0 ? b.img : (Рисунок)Nvl(b.img, CtrlsImg::DA());
			Размер isz = m.дайРазм();
			Точка p = (st == CTRL_PRESSED) * style->pressoffset;
			p.x += x + (cx - isz.cx - tsz.cx - (tsz.cx > 0 && isz.cx > 0 ? DPI(LB_IMAGE) : 0)) / 2;
			p.y += (sz.cy - isz.cy) / 2;
			if(b.left) {
				if(!left) p.x += style->loff;
			}
			else
				if(!right) p.x += style->roff;
				
			Цвет ink = frm ? style->fmonocolor[st] : style->monocolor[st];
			if(b.monoimg || пусто_ли(b.img))
				w.DrawImage(p.x, p.y, m, ink);
			else
				w.DrawImage(p.x, p.y, m);

			if(tsz.cx > 0) {
				if(isz.cx > 0)
					p.x += isz.cx + DPI(LB_IMAGE);
				w.DrawText(p.x, (sz.cy - tsz.cy) / 2, b.label, StdFont(), ink);
			}
		}
		(b.left ? left : right) = true;
	}
	Прям r, cr;
	cr = дайРазм();
	cr.left = lx;
	cr.right = rx;
	Цвет paper = Null;
	if(!nobg) {
		if(ComplexFrame()) {
			r = cr;
			paper = естьФокус() ? SColorHighlight() : style->paper;
			if(естьФокус())
				text = SColorHighlightText();
			w.DrawRect(r, paper);
		}
		else
		if(frm) {
			Прям m = GetMargin();
			r = Прям(max(lx, m.left), m.top, min(rx, sz.cx - m.right), sz.cy - m.bottom);
			w.DrawRect(r, fpaper);
			cr = r;
		}
		else {
			w.Clip(lx, 0, rx - lx, sz.cy);
			ChPaint(w, sz, style->look[mst]);
			Прям m = style->margin;
			r = Прям(max(lx, m.left), m.top, min(rx, sz.cx - m.right), sz.cy - m.bottom);
			if(!IsTrivial() || style->trivialsep) {
				if(left) {
					r.left++;
					if(включен_ли() && редактируем_ли())
						ChPaint(w, lx, style->sepm, 1, sz.cy - 2 * style->sepm, style->sep1);
				}
				if(right) {
					if(включен_ли() && редактируем_ли())
						ChPaint(w, rx - 1, style->sepm, 1, sz.cy - 2 * style->sepm, style->sep2);
					r.right--;
				}
			}
			w.стоп();
			cr = r;
		}
	}
	cr.left += DPI(1);
	Прям clr = cr;
	if(!пусто_ли(valuecy) && cr.дайВысоту() > valuecy) {
		cr.top += (cr.дайВысоту() - valuecy) / 2;
		cr.bottom = cr.top + valuecy;
	}
	if(getcr)
		return cr;
	Значение v = convert->фмт(значение);
	bool f = естьФокус() && !push && frm;
	if(cr.left < cr.right && дисплей) {
		w.Clip(clr);
		if(!пусто_ли(Ошибка)) {
			v = Ошибка;
			text = style->Ошибка;
		}
		дисплей->рисуй(w, cr, v,
		               IsShowEnabled() ? text : SColorDisabled,
		               paper, f ? Дисплей::CURSOR : Дисплей::FOCUS|Дисплей::CURSOR);
		w.стоп();
	}
	if(!frm && естьФокус())
		DrawFocus(w, r);
	return cr;
}

void MultiButton::синхИнфо()
{
	if((естьМышь() || info.естьМышь()) && дисплей &&
	   (дайФлагиМыши() & (K_MOUSELEFT|K_MOUSERIGHT|K_MOUSEMIDDLE)) == 0) {
		Точка p = GetMouseViewPos();
		NilDraw nw;
		Прям r = рисуй0(nw, true);
		if(r.содержит(p)) {
			Значение v = convert->фмт(значение);
			int cm = DPI(2);
			r.left -= cm;
			r.right += cm;
			info.уст(this, r, значение, дисплей, SColorText, style->paper, 0, DPI(2));
			return;
		}
	}
	info.Cancel();
}

void MultiButton::двигМыши(Точка p, dword flags)
{
	int h = FindButton(p.x);
	Ктрл::Подсказка(h >= 0 && h < button.дайСчёт() ? Nvl(button[h].tip, tip) : tip);
	if(hl != h) {
		hl = h;
		освежи();
	}
	bool b = flags & K_MOUSELEFT;
	if(b != push) {
		push = b;
		освежи();
	}
	синхИнфо();
}

void MultiButton::леваяВнизу(Точка p, dword flags)
{
	push = true;
	освежи();
	if(пусто_ли(hl))
		pushrect = Null;
	else {
		if(hl == MAIN)
			pushrect = дайПрямЭкрана();
		else {
			int x, cx;
			int border = IsTrivial() ? style->trivialborder : style->border;
			дайПоз(hl, x, cx);
			pushrect = RectC(x, border, cx, дайРазм().cy - 2 * border)
			           .смещенец(GetScreenView().верхЛево());
		}
		синх();
		if(hl >= 0)
			button[hl].WhenPush();
		else
			WhenPush();
	}
	синхИнфо();
}

void MultiButton::леваяВверху(Точка p, dword flags)
{
	push = false;
	освежи();
	синх();
	if(!пусто_ли(hl)) {
		if(hl >= 0)
			button[hl].WhenClick();
		else
			WhenClick();
	}
	синхИнфо();
}

void MultiButton::выходМыши()
{
	if(!info.открыт()) {
		hl = Null;
		освежи();
		синхИнфо();
	}
}

void MultiButton::режимОтмены()
{
	hl = Null;
	push = false;
	освежи();
	info.Cancel();
}

bool MultiButton::IsTrivial() const
{
	return button.дайСчёт() == 1 && пусто_ли(button[0].img) && !WhenPush && !WhenClick;
}

MultiButton::SubButton& MultiButton::MainButton()
{
	for(int i = 0; i < button.дайСчёт(); i++)
		if(button[i].main)
			return button[i];
	NEVER();
	return button[0];
}

void MultiButton::переустанов()
{
	button.очисть();
}

MultiButton& MultiButton::устДисплей(const Дисплей& d)
{
	if(дисплей != &d) {
		дисплей = &d;
		освежи();
	}
	return *this;
}

MultiButton& MultiButton::NoDisplay()
{
	if(дисплей) {
		дисплей = NULL;
		освежи();
	}
	return *this;
}

MultiButton& MultiButton::SetConvert(const Преобр& c)
{
	if(convert != &c) {
		convert = &c;
		освежи();
	}
	return *this;
}

MultiButton& MultiButton::уст(const Значение& v, bool update)
{
	if(значение != v) {
		значение = v;
		if(update)
			обновиОсвежи();
		else
			освежи();
	}
	return *this;
}

MultiButton& MultiButton::SetValueCy(int cy)
{
	if(cy != valuecy) {
		valuecy = cy;
		освежи();
		if(Фрейм() && дайРодителя())
			дайРодителя()->освежиВыкладку();
	}
	return *this;
}

void MultiButton::устДанные(const Значение& v)
{
	if(значение != v) {
		значение = v;
		обновиОсвежи();
	}
	синхИнфо();
}

Значение MultiButton::дайДанные() const
{
	return значение;
}

void MultiButton::PseudoPush(int bi)
{
	hl = bi;
	push = true;
	button[bi].WhenPush();
	синх();
	Sleep(50);
	button[bi].WhenClick();
	hl = Null;
	push = false;
	синх();
}

void MultiButton::PseudoPush()
{
	hl = MAIN;
	WhenPush();
	синх();
	Sleep(50);
	WhenClick();
	hl = Null;
	push = false;
	синх();
}

MultiButton::MultiButton()
{
	Transparent();
	устСтиль(дефСтиль());
	дисплей = &StdDisplay();
	convert = &NoConvert();
	valuecy = Null;
	push = false;
	устФрейм(sNullFrame());
	nobg = false;
}

void MultiButtonFrame::добавьКФрейму(Ктрл& parent)
{
	parent.добавь(*this);
	NoWantFocus();
}

void MultiButtonFrame::удалиФрейм()
{
	удали();
	WantFocus();
}

void MultiButtonFrame::выложиФрейм(Прям& r)
{
	SetFrameRect(r);
	Lay(r);
}

void MultiButtonFrame::добавьРазмФрейма(Размер& sz)
{
	Прям r(0, 0, 100, 100);
	Lay(r, true);
	sz.cx += r.left + 100 - r.right;
	sz.cy += r.top + 100 - r.bottom;
}

bool MultiButtonFrame::Фрейм()
{
	return true;
}

void MultiButtonFrame::добавьК(Ктрл& w)
{
	w.устФрейм(sNullFrame());
	w.добавьФрейм(*this);
}

}
