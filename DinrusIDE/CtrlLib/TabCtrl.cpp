#include "CtrlLib.h"

namespace РНЦП {

CH_STYLE(КтрлВкладка, Стиль, дефСтиль)
{
	font = StdFont();
	tabheight = font.Info().дайВысоту() + 8;
	margin = 2;
	sel = Прям(2, 2, 2, 2);
	edge = Прям(6, 6, 6, 6);
	extendleft = 0;
	CtrlsImageLook(normal, CtrlsImg::I_TAB, 4);
	CtrlsImageLook(first, CtrlsImg::I_FTAB, 4);
	CtrlsImageLook(last, CtrlsImg::I_LTAB, 4);
	CtrlsImageLook(both, CtrlsImg::I_BTAB, 4);
	body = CtrlsImg::TABB();
	for(int i = 0; i < 4; i++)
		text_color[i] = SColorText();
}

КтрлВкладка::Элемент& КтрлВкладка::Элемент::устТекст(const Ткст& _text)
{
	text = _text;
	owner->Выкладка();
	return *this;
}

КтрлВкладка::Элемент& КтрлВкладка::Элемент::Picture(РисПрям d)
{
	pict = d;
	owner->Выкладка();
	return *this;
}

КтрлВкладка::Элемент& КтрлВкладка::Элемент::устКтрл(Ктрл *_ctrl)
{
	if(ctrl)
		ctrl->удали();
	ctrl = _ctrl;
	owner->Выкладка();
	return *this;
}

КтрлВкладка::Элемент& КтрлВкладка::Элемент::Slave(Ктрл *_slave)
{
	if(slave)
		slave->удали();
	slave = _slave;
	if(slave)
		owner->pane.добавь(*slave);
	int q = owner->sel;
	owner->sel = -1;
	owner->уст(q);
	return *this;
}

КтрлВкладка::Элемент& КтрлВкладка::Элемент::вкл(bool _en)
{
	enabled = _en;
	owner->освежи();
	return *this;
}

КтрлВкладка::Элемент::Элемент()
{
	ctrl = slave = NULL;
	enabled = true;
	ключ = 0;
}

void КтрлВкладка::Элемент::Выкладка(int xp, int y, int cy)
{
	Шрифт fnt = owner->style->font;
	Размер chsz = дайРазмТекста("M", fnt);
	x = xp;
	Размер sz = pict.дайСтдРазм();
	pictpos = Точка(0, 0);
	if(sz.cx) {
		xp += chsz.cx / 2;
		pictpos.x = xp;
		pictpos.y = y + (cy - sz.cy) / 2;
		xp += sz.cx + chsz.cx / 2;
	}
	else
		xp += chsz.cx;
	sz = GetSmartTextSize(text, fnt);
	if(sz.cx) {
		textpos.x = xp;
		textpos.y = y + (cy - sz.cy) / 2;
		xp += sz.cx;
	}
	if(ctrl) {
		xp += chsz.cx / 2;
		sz = ctrl->дайПрям().дайРазм();
		ctrl->устПрям(xp, y + (cy - sz.cy) / 2, sz.cx, sz.cy);
		xp += sz.cx + chsz.cx / 2;
	}
	else
		xp += chsz.cx;
	cx = xp - x;
}

void КтрлВкладка::Элемент::рисуй(Draw& w, int state)
{
	Размер sz = pict.дайСтдРазм();
	pict.рисуй(w, pictpos.x, pictpos.y, sz.cx, sz.cy, owner->style->text_color[state], Null);
	DrawSmartText(w, textpos.x, textpos.y, 99999, text, owner->style->font, owner->style->text_color[state]);
}

void КтрлВкладка::синхТабы()
{
	int x = style->margin - x0;
	for(int i = 0; i < tab.дайСчёт(); i++) {
		Элемент& t = tab[i];
		t.Выкладка(x, style->sel.top * (i != sel),
		            style->tabheight + style->sel.top * (i == sel));
		x += t.cx;
	}
	left.покажи(x0 > 0);
	right.покажи(tab.дайСчёт() && tab.верх().право() > tabs.дайРазм().cx);
}

void КтрлВкладка::Выкладка()
{
	for(int i = 0; i < tab.дайСчёт(); i++)
		if(tab[i].ctrl)
			tab[i].ctrl->удали();
	for(int i = 0; i < tab.дайСчёт(); i++)
		if(tab[i].ctrl)
			Ктрл::добавь(*tab[i].ctrl);
	int th = style->tabheight + style->sel.top;
	tabs.TopPos(0, th + style->sel.bottom)
	    .HSizePos(0, style->sel.left + style->sel.right);
	синхТабы();
	if(tab.дайСчёт() && tab.верх().право() < TabsRight())
		x0 = 0;
	SyncHot();
	if(sel < tab.дайСчёт())
		промотайДо(sel);
	pane.VSizePos(style->tabheight + style->edge.top, style->edge.bottom)
	    .HSizePos(style->edge.left, style->edge.right);
	int sx = DPI(16);
	left.LeftPos(0, sx).TopPos(th - sx, sx);
	right.RightPos(0, sx).TopPos(th - sx, sx);
	синхТабы();
	освежи();
}

Размер КтрлВкладка::вычислиРазм(Размер pane)
{
	return Размер(pane.cx + style->edge.left + style->edge.right,
	            pane.cy + style->tabheight + style->edge.top + style->edge.bottom);
}

Размер КтрлВкладка::вычислиРазм()
{
	Размер sz(0, 0);
	for(int i = 0; i < tab.дайСчёт(); i++) {
		Ктрл *q = tab[i].slave;
		if(q)
			sz = max(sz, q->дайМинРазм());
	}
	return вычислиРазм(sz);
}

int КтрлВкладка::TabsRight()
{
	return tabs.дайРазм().cx - style->sel.left - style->sel.right;
}

void КтрлВкладка::Вкладки::рисуй(Draw& w)
{
	static_cast<КтрлВкладка *>(дайРодителя())->рисуйТабы(w);
}

Прям КтрлВкладка::дайПлотныйПрям() const
{
	return IsTransparent() ? Прям(0, 0, 0, 0) : pane.дайПрям();
}

void КтрлВкладка::рисуйТабы(Draw& w)
{
	int tt = style->sel.top;
	int th = style->tabheight + tt;
	Размер sz = дайРазм();
	ChPaint(w, 0, th, sz.cx, sz.cy - th, style->body);
	for(int phase = 0; phase < 2; phase++) {
		for(int i = tab.дайСчёт() - 1; i >= 0; i--)
			if((sel == i) == phase) {
				Элемент& t = tab[i];
				Прям r = RectC(t.x, tt, t.cx, th - tt);
				if(i)
					r.left -= style->extendleft;
				if(phase) {
					r.left -= style->sel.left;
					r.right += style->sel.right;
					r.top -= tt;
					r.bottom += style->sel.bottom;
				}
				int ndx = !включен_ли() || !t.enabled ? CTRL_DISABLED :
					       phase ? CTRL_PRESSED :
					       i == hot ? CTRL_HOT : CTRL_NORMAL;
				ChPaint(w, r,
					(tab.дайСчёт() == 1 ? style->both : i == 0 ? style->first :
					 i == tab.дайСчёт() - 1 ? style->last : style->normal)
					[ndx]
				);
				t.рисуй(w, ndx);
			}
	}
}

void КтрлВкладка::рисуй(Draw& w)
{
	int th = style->tabheight + style->sel.top;
	Размер sz = дайРазм();
	ChPaint(w, 0, th, sz.cx, sz.cy - th, style->body);
}

int  КтрлВкладка::дайТаб(Точка p) const
{
	if(p.y >= 0 && p.y < style->tabheight)
		for(int i = 0; i < tab.дайСчёт(); i++)
			if(p.x < tab[i].право())
				return i;
	return -1;
}

void КтрлВкладка::режимОтмены()
{
	hot = -1;
}

void КтрлВкладка::SyncHot()
{
	Точка p = дайПозМыши() - дайПрямЭкрана().верхЛево();
	int h = дайТаб(p);
	if(h != hot) {
		hot = h;
		tabs.освежи();
	}
	if(толькочтен_ли())
		hot = -1;
}

void КтрлВкладка::двигМыши(Точка p, dword keyflags)
{
	SyncHot();
}

void КтрлВкладка::леваяВнизу(Точка p, dword keyflags)
{
	if(!редактируем_ли())
		return;
	int h = дайТаб(p);
	if(h >= 0 && tab[h].включен_ли()) {
		уст(h);
		Action();
	}
}

void КтрлВкладка::выходМыши()
{
	SyncHot();
}

void КтрлВкладка::промотайДо(int i)
{
	if(i < 0)
		return;
	Элемент& t = tab[i];
	int tr = TabsRight();
	if(t.право() > tr) {
		x0 += t.право() - tr;
		tabs.освежи();
		синхТабы();
	}
	if(t.x < style->margin) {
		x0 += t.x - style->margin;
		tabs.освежи();
		синхТабы();
	}
}

void КтрлВкладка::лево()
{
	if(x0 <= 0)
		return;
	for(int i = tab.дайСчёт() - 1; i >= 0; i--)
		if(tab[i].x < style->margin) {
			промотайДо(i);
			break;
		}
}

void КтрлВкладка::право()
{
	for(int i = 0; i < tab.дайСчёт(); i++)
		if(tab[i].право() > tabs.дайПрям().дайШирину() - style->sel.left - style->sel.right) {
			промотайДо(i);
			break;
		}
}

void КтрлВкладка::уст(int i)
{
	if(i != sel) {
		bool refocus = HasFocusDeep();
		sel = i;
		tabs.освежи();
		синхТабы();
		for(int i = 0; i < tab.дайСчёт(); i++)
			if(tab[i].slave)
				tab[i].slave->покажи(sel == i);
		if(sel >= 0 && refocus)
			IterateFocusForward(tab[sel].slave, дайТопКтрл(), false, true);
		КогдаУст();
	}
	промотайДо(sel);
}

int  КтрлВкладка::найди(const Ктрл& slave) const
{
	for(int i = 0; i < tab.дайСчёт(); i++)
		if(tab[i].slave == &slave)
			return i;
	return -1;
}

void КтрлВкладка::уст(Ктрл& slave)
{
	int i = найди(slave);
	if(i >= 0)
		уст(i);
}

void КтрлВкладка::удали(Ктрл& slave)
{
	int i = найди(slave);
	if(i >= 0)
		удали(i);
}

int КтрлВкладка::найдиВставь(Ктрл& slave)
{
	int i = найди(slave);
	return i < 0 ? дайСчёт() : i;
}

КтрлВкладка::Элемент& КтрлВкладка::вставь(Ктрл& before_slave)
{
	return вставь(найдиВставь(before_slave));
}

КтрлВкладка::Элемент& КтрлВкладка::вставь(Ктрл& before_slave, const char *text)
{
	return вставь(найдиВставь(before_slave), text);
}

КтрлВкладка::Элемент& КтрлВкладка::вставь(Ктрл& before_slave, const Рисунок& m, const char *text)
{
	return вставь(найдиВставь(before_slave), m, text);
}

КтрлВкладка::Элемент& КтрлВкладка::вставь(Ктрл& before_slave, Ктрл& slave, const char *text)
{
	return вставь(найдиВставь(before_slave), slave, text);
}

КтрлВкладка::Элемент& КтрлВкладка::вставь(Ктрл& before_slave, Ктрл& slave, const Рисунок& m, const char *text)
{
	return вставь(найдиВставь(before_slave), slave, m, text);
}


void КтрлВкладка::устДанные(const Значение& data)
{
	уст(data);
}

Значение КтрлВкладка::дайДанные() const
{
	return дай();
}

КтрлВкладка::Элемент& КтрлВкладка::добавь()
{
	режимОтмены();
	Элемент& t = tab.добавь();
	t.owner = this;
	if(sel < 0)
		уст(0);
	Выкладка();
	return t;
}

КтрлВкладка::Элемент& КтрлВкладка::добавь(const char *text)
{
	return добавь().устТекст(text);
}

КтрлВкладка::Элемент& КтрлВкладка::добавь(const Рисунок& m, const char *text)
{
	return добавь().устТекст(text).устРисунок(m);
}

КтрлВкладка::Элемент& КтрлВкладка::добавь(Ктрл& slave, const char *text)
{
	return добавь(text).Slave(&slave);
}

КтрлВкладка::Элемент& КтрлВкладка::добавь(Ктрл& slave, const Рисунок& m, const char *text)
{
	return добавь(slave, text).устРисунок(m);
}

КтрлВкладка::Элемент& КтрлВкладка::вставь(int i)
{
	режимОтмены();
	int c = i < sel ? sel + 1 : sel;
	КтрлВкладка::Элемент& m = tab.вставь(i);
	m.owner = this;
	Выкладка();
	sel = -1;
	уст(c);
	return m;
}

КтрлВкладка::Элемент& КтрлВкладка::вставь(int i, const char *text)
{
	return вставь(i).устТекст(text);
}

КтрлВкладка::Элемент& КтрлВкладка::вставь(int i, const Рисунок& m, const char *text)
{
	return вставь(i).устТекст(text).устРисунок(m);
}

КтрлВкладка::Элемент& КтрлВкладка::вставь(int i, Ктрл& slave, const char *text)
{
	return вставь(i, text).Slave(&slave);
}

КтрлВкладка::Элемент& КтрлВкладка::вставь(int i, Ктрл& slave, const Рисунок& m, const char *text)
{
	return вставь(i, slave, text).устРисунок(m);
}


void КтрлВкладка::удали(int i)
{
	режимОтмены();
	if(tab[i].ctrl)
		tab[i].ctrl->удали();
	if(tab[i].slave)
		tab[i].slave->удали();
	int c = i < sel ? sel - 1 : sel;
	tab.удали(i);
	Выкладка();
	sel = -1;
	if(tab.дайСчёт())
		уст(minmax(c, 0, tab.дайСчёт() - 1));
	else {
		x0 = 0;
		sel = -1;
		accept_current = false;
		КогдаУст();
	}
}

void КтрлВкладка::иди(int d)
{
	if(редактируем_ли() && tab.дайСчёт()) {
		int i = sel + d;
		while(i != sel) {
			if(i < 0)
				i = tab.дайСчёт() - 1;
			if(i >= tab.дайСчёт())
				i = 0;
			if(tab[i].включен_ли()) {
				уст(i);
				break;
			}
			i += d;
		}
	}
}

bool КтрлВкладка::Ключ(dword ключ, int repcnt)
{
	switch(ключ) {
#ifdef PLATFORM_COCOA
	case K_ALT|K_TAB:
	case K_OPTION|K_TAB:
#endif
	case K_CTRL_TAB:
		идиСледщ();
		Action();
		return true;
#ifdef PLATFORM_COCOA
	case K_SHIFT|K_ALT|K_TAB:
	case K_SHIFT|K_OPTION|K_TAB:
#endif
	case K_SHIFT_CTRL_TAB:
		идиПредш();
		Action();
		return true;
	}
	return Ктрл::Ключ(ключ, repcnt);
}

bool КтрлВкладка::горячаяКлав(dword ключ)
{
	switch(ключ) {
#ifdef PLATFORM_COCOA
	case K_ALT|K_TAB:
	case K_OPTION|K_TAB:
#endif
	case K_CTRL_TAB:
		идиСледщ();
		Action();
		return true;
#ifdef PLATFORM_COCOA
	case K_SHIFT|K_ALT|K_TAB:
	case K_SHIFT|K_OPTION|K_TAB:
#endif
	case K_SHIFT_CTRL_TAB:
		идиПредш();
		Action();
		return true;
	}
	return Ктрл::горячаяКлав(ключ);
}

bool КтрлВкладка::прими()
{
	if(tab.дайСчёт() == 0 || no_accept)
		return true;
	int ii = дай();
	if(accept_current)
		return !tab[ii].slave || tab[ii].slave -> прими();
	Ук<Ктрл> refocus = GetFocusChildDeep();
	for(int i = 0; i < tab.дайСчёт(); i++)
		if(tab[i].slave) {
			уст(i);
			if(!tab[i].slave->прими())
				return false;
		}
	уст(ii);
	if(refocus)
		refocus->устФокус();
	return true;
}

void КтрлВкладка::переустанов()
{
	for(int i = 0; i < tab.дайСчёт(); i++) {
		if(tab[i].ctrl)
			tab[i].ctrl->удали();
		if(tab[i].slave)
			tab[i].slave->удали();
	}
	tab.очисть();
	x0 = 0;
	режимОтмены();
	sel = -1;
	освежи();
	accept_current = false;
	КогдаУст();
}

КтрлВкладка::КтрлВкладка()
{
	hot = -1;
	sel = -1;
	x0 = 0;
	no_accept = accept_current = false;
	Ктрл::добавь(tabs);
	Ктрл::добавь(pane);
	tabs.BackPaint().IgnoreMouse();
	Ктрл::добавь(left.SetMonoImage(CtrlsImg::SLA()).ScrollStyle().NoWantFocus());
	left <<= THISBACK(лево);
	Ктрл::добавь(right.SetMonoImage(CtrlsImg::SRA()).ScrollStyle().NoWantFocus());
	right <<= THISBACK(право);
	Transparent().NoWantFocus();
	tabs.Transparent().NoWantFocus();
	устСтиль(дефСтиль());
}

// ----------------------------------------------------------------

void ВкладкаДлг::поместиКнопку(Кнопка& b, int& r)
{
	if(b.дайРодителя()) {
		b.удали();
		добавьОтпрыск(&b, &tabctrl);
		b.RightPosZ(r, 64).BottomPosZ(4, 24);
		r += 72;
	}
}

void ВкладкаДлг::Rearrange()
{
	int r = 4;
	поместиКнопку(apply, r);
	поместиКнопку(cancel, r);
	поместиКнопку(exit, r);
	поместиКнопку(ok, r);
	устПрям(Прям(дайПрям().верхЛево(), tabctrl.вычислиРазм(sz) + Ктрл::LayoutZoom(Размер(8, 40))));
}

КтрлВкладка::Элемент& ВкладкаДлг::добавь0(Ктрл& tab, const char *text)
{
	Размер tsz = max(tab.дайПрям().дайРазм(), sz);
	tab.SizePos();
	КтрлВкладка::Элемент& m = tabctrl.добавь(tab, text);
	if(tsz != sz) {
		sz = tsz;
		Rearrange();
	}
	return m;
}

ВкладкаДлг& ВкладкаДлг::AButton(Кнопка& b)
{
	if(binit) {
		exit.удали();
		binit = false;
	}
	Ктрл::добавь(b);
	Rearrange();
	return *this;
}

ВкладкаДлг::ВкладкаДлг()
{
	binit = true;
	Ктрл::добавь(tabctrl.HSizePosZ(4, 4).VSizePosZ(4, 36));
	sz = Размер(0, 0);
	ok.Ok().устНадпись(t_("OK"));
	Acceptor(ok, IDOK);
	cancel.Cancel().устНадпись(t_("Cancel"));
	Rejector(cancel, IDCANCEL);
	apply.устНадпись(t_("Apply"));
	Acceptor(apply, IDYES);
	exit.устНадпись(t_("закрой"));
	Acceptor(exit, IDEXIT);
	Ктрл::добавь(exit);
}

}
