#include "CtrlLib.h"

namespace РНЦП {

HeaderCtrl::Колонка::Колонка()
{
	ratio = 1;
	visible = true;
	min = 0;
	max = INT_MAX;
	margin = 4;
	header = NULL;
	устЛин(ALIGN_LEFT);
	paper = Null;
	Индекс = Null;
	NoWrap();
}

HeaderCtrl::Колонка&  HeaderCtrl::Колонка::SetMargin(int m)
{
	if(m != margin) {
		margin = m;
		обновиНадпись();
	}
	return *this;
}

HeaderCtrl::Колонка&  HeaderCtrl::Колонка::SetRatio(double wd)
{
	ratio = wd ? wd : 1;
	обновиНадпись();
	return *this;
}

void  HeaderCtrl::Колонка::обновиНадпись()
{
	if(header) {
		header->освежиВыкладкуРодителя();
		header->освежи();
		header->WhenLayout();
		header->SbTotal();
	}
}

CH_STYLE(HeaderCtrl, Стиль, дефСтиль)
{
	CtrlsImageLook(look, CtrlsImg::I_HTB);
	gridadjustment = 0;
	Точка p = Кнопка::StyleNormal().pressoffset;
	pressoffset = p.x || p.y;
}

void HeaderCtrl::Колонка::рисуй(bool& first, Draw& w,
                               int x, int y, int cx, int cy, bool disabled, bool push, bool hl)
{
	const HeaderCtrl::Стиль *style = header ? header->style : &HeaderCtrl::дефСтиль();;
	if(first) {
		if(cx >= -style->gridadjustment) {
			cx -= style->gridadjustment;
			if(cx < 0)
				cx = 0;
			first = false;
		}
	}
	else
		x -= style->gridadjustment;
	bool p = push && style->pressoffset;
	int q = CTRL_NORMAL;
	if(hl)
		q = CTRL_HOT;
	if(push)
		q = CTRL_PRESSED;
	if(disabled)
		q = CTRL_DISABLED;
	ChPaint(w, x, y, cx, cy, пусто_ли(paper) ? style->look[q] : paper.operator Значение());
	x += margin;
	cx -= 2 * margin;
	w.Clip(x + 2, y, cx - 4, cy);
	PaintLabel(w, x + 2, y + 1, cx - 4, cy - 4, disabled, p);
	w.стоп();
}

HeaderCtrl::Колонка& HeaderCtrl::Вкладка(int i) {
	if(i >= col.дайСчёт())
		for(int j = col.дайСчёт(); j <= i; j++) {
			Колонка& c = col.добавь();
			c.header = this;
			c.Индекс = i;
		}
	return col[i];
}

void HeaderCtrl::SetHeight(int cy)
{
	height = cy;
	освежиВыкладкуРодителя();
}

int  HeaderCtrl::дайВысоту() const
{
	int cy = 0;
	for(int i = 0; i < col.дайСчёт(); i++)
		cy = max(cy, col[i].GetLabelSize().cy);
	return max(height, cy + 4);
}

HeaderCtrl::Колонка& HeaderCtrl::добавь()
{
	oszcx = -1;
	return Вкладка(col.дайСчёт());
}

HeaderCtrl::Колонка& HeaderCtrl::добавь(const char *text, double ratio)
{
	HeaderCtrl::Колонка& c = добавь();
	c.ratio = ratio ? Zxf(ratio) : 1;
	c.устТекст(text);
	SbTotal();
	return c;
}

void HeaderCtrl::SetTabRatio(int i, double ratio)
{
	col[i].ratio = Zxf(ratio);
	oszcx = -1;
	SbTotal();
	освежи();
	WhenLayout();
}

void HeaderCtrl::SwapTabs(int first, int second)
{
	col.разверни(first, second);
	oszcx = -1;
	освежи();
	WhenLayout();
}

void HeaderCtrl::MoveTab(int from, int to)
{
	col.перемести(from, to);
	oszcx = -1;
	освежи();
	WhenLayout();
}

double HeaderCtrl::Denominator() const {
	double rs = 0;
	for(int i = 0; i < col.дайСчёт(); i++)
		if(col[i].visible)
			rs += col[i].ratio;
	return rs;
}

void HeaderCtrl::DoSbTotal()
{
	if(mode == SCROLL) {
		int cx = 0;
		for(int i = 0; i < col.дайСчёт(); i++)
			if(col[i].visible)
				cx += (int)col[i].ratio;
		sb.автоСкрой(autohidesb);
		sb.устВсего(cx);
	}
	else {
		sb.автоСкрой();
		sb.устВсего(0);
	}
}

void HeaderCtrl::SbTotal()
{
	if(HasCapture())
		return;
	DoSbTotal();
}

HeaderCtrl& HeaderCtrl::Proportional() { mode = PROPORTIONAL; SbTotal(); return *this; }
HeaderCtrl& HeaderCtrl::ReduceNext()   { mode = REDUCENEXT; SbTotal(); return *this; }
HeaderCtrl& HeaderCtrl::ReduceLast()   { mode = REDUCELAST; SbTotal(); return *this; }
HeaderCtrl& HeaderCtrl::Absolute()     { mode = SCROLL; SbTotal(); return *this; }
HeaderCtrl& HeaderCtrl::фиксирован()        { mode = FIXED; SbTotal(); return *this; }

int  HeaderCtrl::суммаMin(int from)
{
	int mincx = 0;
	for(int i = from; i < col.дайСчёт(); i++)
		if(col[i].visible)
			mincx += col[i].min;
	return mincx;
}

int  HeaderCtrl::суммаMax(int from)
{
	int maxcx = 0;
	for(int i = from; i < col.дайСчёт(); i++)
		if(col[i].visible) {
			if(col[i].max == INT_MAX)
				return INT_MAX;
			maxcx += col[i].max;
		}
	return maxcx;
}

void HeaderCtrl::ReCompute()
{
	int szcx = дайРазм().cx;
	tabrect.очисть();
	Прям r;
	r.top = 0;
	r.bottom = дайРазм().cy;
	r.right = 0;
	if(mode == SCROLL)
		for(int i = 0; i < col.дайСчёт(); i++) {
			r.left = r.right;
			if(col[i].visible)
				r.right += (int)col[i].ratio;
			tabrect.добавь(r);
		}
	else {
		double rs = Denominator();
		double rr = 0;
		double eps = rs / 2.0e5;
		Вектор<int> ci = GetVisibleCi(0);
		for(int i = 0; i < ci.дайСчёт() - 1; i++) {
			int cii = ci[i];
			r.left = r.right;
			while(tabrect.дайСчёт() < cii)
				tabrect.добавь(r);
			rr += rs ? col[cii].ratio : 1;
			r.right = int(rr * szcx / (rs ? rs : ci.дайСчёт()) + eps);
			tabrect.добавь(r);
		}
		r.left = r.right;
		if(!ci.пустой()) {
			int cii = ci.верх();
			while(tabrect.дайСчёт() < cii)
				tabrect.добавь(r);
			r.right = szcx;
			tabrect.добавь(r);
		}
		r.left = r.right;
		tabrect.устСчёт(col.дайСчёт(), r);
	}
}

void HeaderCtrl::Distribute(const Вектор<int>& sci, double delta)
{
	Вектор<int> ci(sci, 1);
	int szcx = дайРазм().cx;
	if(szcx == 0)
		return;
	double rs = Denominator();
	double eps = max(1.0e-6, rs / 1.0e6);
	double epsr = max(1.0e-4, rs / 1.0e4);
	bool checkmin = суммаMin(0) < szcx;
	for(;;) {
		double psm = 0;
		for(int i = 0; i < ci.дайСчёт(); i++)
			psm += col[ci[i]].ratio;
		if(fabs(psm) < eps)
			return;
		double q = 1 + delta / psm;
		double x = 0;
		Вектор<int> nci;
		for(int i = 0; i < ci.дайСчёт(); i++) {
			Колонка& c = col[ci[i]];
			c.ratio *= q;
			if(c.ratio < epsr)
				c.ratio = epsr;
			else
			if(c.ratio < c.min * rs / szcx && checkmin) {
				c.ratio = c.min * rs / szcx;
				if(delta > 0)
					nci.добавь(ci[i]);
			}
			else
			if(c.ratio > c.max * rs / szcx) {
				c.ratio = c.max * rs / szcx;
				if(delta < 0)
					nci.добавь(ci[i]);
			}
			else
				nci.добавь(ci[i]);
			x += c.ratio;
		}

		delta = (psm + delta) - x;

		if(fabs(delta) < eps)
			break;
		ci = pick(nci);
		if(ci.дайСчёт() == 0) {
			double psm = 0;
			for(int i = 0; i < sci.дайСчёт(); i++)
				psm += col[sci[i]].ratio;
			if(fabs(psm) < eps)
				return;
			double q = 1 + delta / psm;
			for(int i = 0; i < sci.дайСчёт(); i++)
				col[sci[i]].ratio *= q;
			return;
		}
	}
}

Вектор<int> HeaderCtrl::GetVisibleCi(int from)
{
	Вектор<int> sci;
	for(int i = from; i < col.дайСчёт(); i++)
		if(col[i].visible)
			sci.добавь(i);
	return sci;
}

void HeaderCtrl::RefreshDistribution()
{
	int szcx = дайРазм().cx;
	if(oszcx != szcx) {
		if(mode == SCROLL)
			for(int i = 0; i < col.дайСчёт(); i++) {
				Колонка& c = col[i];
				c.ratio = minmax((int)c.ratio, c.min, c.max);
			}
		else {
			Distribute(GetVisibleCi(0), 0);
		}
		oszcx = szcx;
		ReCompute();
	}
}

Прям HeaderCtrl::GetTabRect(int q)
{
	RefreshDistribution();
	return tabrect[q];
}

int  HeaderCtrl::GetTabWidth(int ci)
{
	return GetTabRect(ci).устШирину();
}

void HeaderCtrl::SetTabWidth0(int i, int cx)
{
	Колонка& c = col[i];
	int szcx = дайРазм().cx;
	Прям ir = GetTabRect(i);
	bool checkmin = суммаMin(0) < szcx;
	cx = checkmin ? minmax(cx, c.min, c.max) : min(cx, c.max);
	if(mode != SCROLL) {
		if(checkmin)
			cx = min(cx, szcx - суммаMin(i + 1) - ir.left);
		cx = max(cx, szcx - суммаMax(i + 1) - ir.left);
	}
	if(cx < 0)
		cx = 0;

	double rs = Denominator();
	int ocx = ir.устШирину();
	if(szcx == 0) return;

	double delta = rs * (cx - ocx) / szcx;
	if(ocx == cx) return;
	col[i].ratio += delta;

	switch(mode) {
	case PROPORTIONAL:
		Distribute(GetVisibleCi(i + 1), -delta);
		break;
	case REDUCELAST:
		for(int q = col.дайСчёт() - 1; q >= i; q--)
			Reduce(q, delta, rs, szcx, checkmin);
		break;
	case REDUCENEXT:
		for(int q = i + 1; q < col.дайСчёт(); q++)
			Reduce(q, delta, rs, szcx, checkmin);
		if(delta > 0)
			Reduce(i, delta, rs, szcx, checkmin);
		break;
	case SCROLL:
		col[i].ratio = cx;
		SbTotal();
		break;
	}

	ReCompute();
	освежи();
}

void HeaderCtrl::Reduce(int i, double& delta, double rs, int szcx, bool checkmin)
{
	if(col[i].visible) {
		Колонка& c = col[i];
		double q = minmax(c.ratio - delta, checkmin ? c.min * rs / szcx : 0, c.max * rs / szcx);
		if(q < 0)
			q = 0;
		delta -= c.ratio - q;
		c.ratio = q;
	}
}

void HeaderCtrl::SetTabWidth(int i, int cx) {
	SetTabWidth0(i, cx);
	WhenLayout();
}

void HeaderCtrl::рисуй(Draw& w) {
	RefreshDistribution();
	Размер sz = дайРазм();
	w.DrawRect(sz, SColorFace());
	bool ds = !IsShowEnabled();
	double rs = Denominator();
	double rr = 0;
	int x = -sb;
	light = -1;
	bool first = true;
	int dx = Null;
	for(int i = 0; i < col.дайСчёт(); i++) {
		if(col[i].visible) {
			Прям r;
			if(mode == SCROLL) {
				int cx = (int)col[i].ratio;
				r = RectC(x, 0, cx, sz.cy);
				x += cx;
			}
			else {
				rr += rs ? col[i].ratio : 1;
				int xx = int(rr * sz.cx / (rs ? rs : col.дайСчёт()));
				r = RectC(x, 0, i == col.дайСчёт() - 1 ? sz.cx - x : xx - x, sz.cy);
				x = xx;
			}
			bool mousein = HasMouseIn(r.дефлят(1, 0)) && col[i].WhenAction && pushi < 0 &&
			               !isdrag;
			if(mousein)
				light = i;
			col[i].рисуй(first, w,
			             r.left, r.top, r.устШирину(), r.устВысоту(), ds, push && i == pushi, mousein);
			if(isdrag && ti == i)
				dx = r.left;
		}
		if(x >= sz.cx) break;
	}
	Колонка h;
	h.header = this;
	h.рисуй(first, w, x, 0, sz.cx - x + 5, sz.cy, false, false, false);
	if(isdrag) {
		w.DrawImage(dragx + dragd, 0, dragtab);
		DrawVertDrop(w, пусто_ли(dx) ? sz.cx - 2 : dx - (dx > 0), 0, sz.cy);
	}
}

void HeaderCtrl::Выкладка()
{
	sb.устСтраницу(дайРазм().cx);
}

int HeaderCtrl::GetNextTabWidth(int i) {
	while(++i < col.дайСчёт())
		if(col[i].visible)
			return GetTabWidth(i);
	return 0;
}

int HeaderCtrl::GetLastVisibleTab() {
	int i = col.дайСчёт();
	while(--i >= 0)
		if(col[i].visible) return i;
	return -1;
}

int HeaderCtrl::GetSplit(int px) {
	if(!включен_ли())
		return Null;
	RefreshDistribution();
	px += sb;
	int cx = дайРазм().cx;
	double rs = Denominator();
	int n = col.дайСчёт();
	int l = GetLastVisibleTab();
	int D3 = DPI(3);
	int D4 = DPI(4);
	if(mode != SCROLL && абс(px - cx) <= D4 && n > 0 && l >= 0 && GetTabWidth(l) < D4)
		while(--n >= 0)
			if(GetTabWidth(n) >= D4)
				return n;
	double rr = 0;
	int x = 0;
	int first_move = 0;
	while(first_move < n && (!col[first_move].visible || col[first_move].min == col[first_move].max))
		first_move++;
	int last_move = n - 1;
	while(last_move >= 0 && (!col[last_move].visible || col[last_move].min == col[last_move].max))
		last_move--;
	if(mode == PROPORTIONAL)
		last_move--;
	for(int i = 0; i < n; i++) {
		Колонка& cl = col[i];
		if(cl.visible) {
			bool canmove = (i >= first_move && i <= last_move);
			if(cl.min == cl.max) {
				int j = i;
				while(++j < n && !col[j].visible)
					;
				if(j >= n || col[j].min == col[j].max)
					canmove = false;
			}
			if(mode == SCROLL) {
				x += (int)col[i].ratio;
				if(canmove && px >= x - D3 && px < x + D3 && (i >= n - 1 || GetNextTabWidth(i) >= D4 || px < x))
					return i;
			}
			else {
				rr += rs ? col[i].ratio : 1;
				x = int(rr * cx / (rs ? rs : col.дайСчёт()));
				if(canmove && px >= x - D3 && px < x + D3 && i < n - 1 && (i >= n - 1 || GetNextTabWidth(i) >= D4 || px < x))
					return i;
			}
			if(px < x)
				return -1 - i;
		}
	}
	return Null;
}

Рисунок HeaderCtrl::рисКурсора(Точка p, dword) {
	if(mode == FIXED)
		return Рисунок::Arrow();
	if(HasCapture())
		return split >= 0 ? CtrlImg::HorzPos() : Рисунок::Arrow();
	int q = GetSplit(p.x);
	return q < 0 ? Рисунок::Arrow()
	             : GetTabWidth(q) < 4 ? CtrlImg::HorzSplit()
	                                  : CtrlImg::HorzPos();
}

void HeaderCtrl::леваяВнизу(Точка p, dword keyflags) {
#ifdef _ОТЛАДКА
	if(keyflags & K_CTRL) {
		Ткст text;
		for(int i = 0; i < col.дайСчёт(); i++)
			text += фмт(i ? " %d" : "%d", InvZx(GetTabWidth(i)));
		WriteClipboardText(".ColumnWidths(\"" + text + "\");");
		бипВосклицание();
	}
#endif
	split = GetSplit(p.x);
	if(пусто_ли(split)) return;
	SetCapture();
	if(split >= 0) {
		colRect = GetTabRect(split);
		return;
	}
	li = pushi = -1 - split;
	col[pushi].ПриЛевКлике();
#ifdef _ОТЛАДКА
	if((keyflags & K_ALT) && pushi >= 0)
		WriteClipboardText(какТкст(GetTabWidth(pushi)));
#endif
	if(pushi >= 0) {
		if(!col[pushi].WhenAction) {
			pushi = -1;
			return;
		}
		colRect = GetTabRect(pushi);
		push = true;
	}
	освежи();
}

void HeaderCtrl::леваяДКлик(Точка p, dword keyflags)
{
	int q = GetSplit(p.x);
	if(q >= 0 || пусто_ли(q))
		return;
	col[-1 - q].WhenLeftDouble();
}

void HeaderCtrl::праваяВнизу(Точка p, dword)
{
	int q = GetSplit(p.x);
	if(q >= 0 || пусто_ли(q))
		return;
	q = -1 - q;
	if(col[q].WhenBar)
		БарМеню::выполни(col[q].WhenBar);
}

void HeaderCtrl::StartSplitDrag(int s)
{
	split = s;
	colRect = GetTabRect(split);
	SetCapture();
}

void HeaderCtrl::двигМыши(Точка p, dword keyflags) {
	if(isdrag) {
		ti = GetLastVisibleTab() + 1;
		for(int i = 0; i < дайСчёт(); i++)
			if(col[i].visible) {
				Прям r = GetTabRect(i).горизСмещенец(-sb);
				if(p.x < r.left + r.устШирину() / 2) {
					ti = i;
					break;
				}
			}
		dragx = p.x;
		освежи();
		return;
	}
	int q = GetSplit(p.x);
	int cx = ~q;
	if(cx >= 0 && cx < col.дайСчёт() && !пусто_ли(col[cx].tip))
		Подсказка(col[cx].tip);
	if(mode == FIXED)
		return;
	q = пусто_ли(q) || q >= 0 ? -1 : -1 - q;
	if(q != light)
		освежи();
	if(!HasCapture())
		return;
	Размер sz = дайРазм();
	int x = mode == SCROLL ? p.x + sb : min(sz.cx, p.x);
	if(split >= 0) {
		int w = x - colRect.left;
		if(w < 0) w = 0;
		if(w != GetTabWidth(split)) {
			SetTabWidth0(split, w);
			освежи();
			if(track) {
				синх();
				Action();
				WhenLayout();
			}
		}
	}
}

void HeaderCtrl::леваяТяг(Точка p, dword keyflags)
{
	if(li < 0 || !moving) return;
	int n = 0;
	for(int i = 0; i < col.дайСчёт(); i++)
		if(col[i].visible)
			n++;
	if(n < 2)
		return;
	push = false;
	ti = li;
	pushi = -1;
	освежи();
	Прям r = GetTabRect(li).горизСмещенец(-sb);
	Размер sz = r.дайРазм();
	ImageDraw iw(sz.cx, sz.cy);
	bool first = true;
	col[li].рисуй(first, iw, 0, 0, sz.cx, sz.cy, false, false, false);
	DrawFrame(iw, sz, SColorText());
	dragtab = iw;
	dragx = p.x;
	dragd = r.left - p.x;
	ImageBuffer ib(dragtab);
	Unmultiply(ib);
	КЗСА *s = ~ib;
	КЗСА *e = s + ib.дайДлину();
	while(s < e) {
		s->a >>= 1;
		s++;
	}
	Premultiply(ib);
	dragtab = ib;
	isdrag = true;
}

void HeaderCtrl::выходМыши()
{
	освежи();
}

void HeaderCtrl::леваяВверху(Точка, dword) {
	if(isdrag) {
		if(li >= 0 && ti >= 0)
			MoveTab(li, ti);
		li = ti = -1;
		освежи();
	}
	else
	if(pushi >= 0 && push)
		col[pushi].WhenAction();
	push = false;
	ti = li = pushi = -1;
	isdrag = false;
	освежи();
	if(split >= 0 && !track) {
		Action();
		WhenLayout();
	}
	DoSbTotal();
}

void HeaderCtrl::режимОтмены() {
	ti = li = split = pushi = -1;
	isdrag = push = false;
}

void HeaderCtrl::ShowTab(int i, bool show) {
	Колонка& cm = Вкладка(i);
	if(cm.visible == show) return;
	cm.visible = show;
	if(mode == PROPORTIONAL)
		InvalidateDistribution();
	ReCompute();
	освежи();
	WhenLayout();
	SbTotal();
}

void HeaderCtrl::Колонка::покажи(bool b)
{
	if(!header)
		return;
	int q = header->найдиИндекс(дайИндекс());
	if(q >= 0)
		header->ShowTab(q, b);
}

int HeaderCtrl::найдиИндекс(int ndx) const
{
	if(ndx >= 0 && ndx < col.дайСчёт() && col[ndx].Индекс == ndx) return ndx;
	for(int i = 0; i < col.дайСчёт(); i++)
		if(col[i].Индекс == ndx)
			return i;
	return -1;
}

#ifdef COMPILER_MSC
#pragma warning(push)
#pragma warning(disable: 4700) // MSVC6 complaint about n having not been initialized
#endif

void HeaderCtrl::сериализуй(Поток& s) {
	int version = 0x04;
	s / version;
	if(version >= 0x04) {
		int n = col.дайСчёт();
		s / n;
		Массив<Колонка> col2 = clone(col);
		if(s.грузится())
			col2.вставьН(0, n);
		for(int i = 0; i < n; i++) {
			int ndx = col2[i].Индекс;
			s % ndx;
			if(s.грузится())
				for(int j = n; j < col2.дайСчёт(); j++)
					if(col2[j].Индекс == ndx) {
						col2.разверни(i, j);
						break;
					}
			col2[i].Индекс = ndx;
			s % col2[i].ratio;
			s % col2[i].visible;			
		}
		if(s.грузится() && n == col.дайСчёт()) {
			col2.обрежь(n);
			col = pick(col2);
		}
	}
	else
	if(version < 0x01) {
		int n = col.дайСчёт();
		s / n;
		for(int i = 0; i < n; i++)
			if(i < col.дайСчёт()) {
				int n = 1;
				s / n;
				col[i].ratio = n;
			}
			else {
				int dummy = 0;
				s / dummy;
			}
	}
	else {
		int n = col.дайСчёт();
		s / n;
		if(version < 0x02)
			for(int i = 0; i < n; i++)
				if(i < col.дайСчёт())
					s % col[i].ratio;
				else {
					int dummy = 0;
					s % dummy;
				}
		else {
			int t = 0;
			for(int i = 0; i < n; i++) {
				if(n == col.дайСчёт()) {
					int ndx = col[i].Индекс;
					double r = col[i].ratio;
					s % ndx;
					s % r;
					int q = найдиИндекс(ndx);
					if(q >= 0) {
						col[q].ratio = r;
						col.разверни(t++, q);
					}
					if(version >= 0x03) {
						bool visible = IsTabVisible(i);
						s % visible;
						if(i < дайСчёт())
							ShowTab(i, visible);
					}
				}
				else {
					int dummy = 0;
					double dummy2 = 1.0;
					bool dummy3 = false;
					s % dummy;
					s % dummy2;
					if(version >= 0x03)
						s % dummy3;
				}
			}
		}
	}
	if(s.грузится()) {
		ReCompute();
		освежи();
		WhenLayout();
	}
}

#ifdef COMPILER_MSC
#pragma warning(pop)
#endif

void HeaderCtrl::добавьКФрейму(Ктрл& parent)
{
	parent.добавь(*this);
	parent.добавь(sb);
}

void HeaderCtrl::удалиФрейм()
{
	удали();
	sb.удали();
}

void HeaderCtrl::выложиФрейм(Прям& r)
{
	выложиФреймСверху(r, this, invisible ? 0 : дайВысоту());
	выложиФреймСнизу(r, &sb, sb.показан_ли() ? размПромотБара() : 0);
}

void HeaderCtrl::добавьРазмФрейма(Размер& sz)
{
	if(!invisible)
		sz.cy += дайВысоту();
	if(sb.видим_ли())
		sz.cy += размПромотБара();
}

HeaderCtrl& HeaderCtrl::Invisible(bool inv)
{
	invisible = inv;
	if(InFrame() && дайРодителя())
		освежиВыкладкуРодителя();
	return *this;
}

void HeaderCtrl::переустанов()
{
	col.очисть();
	track = true;
	mode = PROPORTIONAL;
	oszcx = -1;
	invisible = false;
	height = 0;
	style = &дефСтиль();
	освежи();
	moving = false;
}

void HeaderCtrl::WScroll()
{
	WhenLayout();
}

void HeaderCtrl::промотай()
{
	освежи();
	ПриПромоте();
}

void HeaderCtrl::ScrollVisibility()
{
	WhenScrollVisibility();
}

HeaderCtrl::HeaderCtrl() {
	переустанов();
	NoWantFocus();
	sb.автоСкрой();
	autohidesb = true;
	sb.ПриПромоте = THISBACK(промотай);
	ПриПромоте = THISBACK(WScroll);
	sb.ПриВидимости = THISBACK(ScrollVisibility);
	BackPaintHint();
}

HeaderCtrl::~HeaderCtrl() {}

}
