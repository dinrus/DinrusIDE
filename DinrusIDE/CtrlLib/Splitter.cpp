#include "CtrlLib.h"

namespace РНЦП {

CH_STYLE(Сплиттер, Стиль, дефСтиль)
{
	width = Ктрл::HorzLayoutZoom(4);
	vert[0] = horz[0] = SColorFace();
	vert[1] = horz[1] = GUI_GlobalStyle() >= GUISTYLE_XP ? смешай(SColorHighlight, SColorFace)
	                                                     : SColorShadow();
	dots = true;
}

int Сплиттер::ClientToPos(Точка p) const
{
	return minmax(vert ? 10000 * p.y / дайРазм().cy : 10000 * p.x / дайРазм().cx, 0, 9999);
}

int Сплиттер::PosToClient(int pos) const
{
	return (vert ? дайРазм().cy : дайРазм().cx) * pos / 10000;
}

void Сплиттер::Выкладка() {
	Размер sz = дайРазм();
	int count = GetViewChildCount();
	if(count == 0)
		return;
	mins.устСчёт(count, 0);
	minpx.устСчёт(count, 0);
	count--;
	if(pos.дайСчёт() < count) {
		pos.устСчёт(count, 0);
		for(int i = 0; i < count; i++)
			pos[i] = (i + 1) * 10000 / (count + 1);
	}
	if(style >= 0) {
		int i = 0;
		for(Ктрл *q = дайПервОтпрыск(); q; q = q->дайСледщ()) {
			if(!q->InFrame()) {
				if(style == i)
					q->SizePos().покажи();
				else
					q->скрой();
				i++;
			}
		}
		return;
	}

	int lw = chstyle->width >> 1;
	int rw = chstyle->width - lw;
	
	int i = 0;
	for(Ктрл *q = дайПервОтпрыск(); q; q = q->дайСледщ()) {
		if(!q->InFrame()) {
			int lo = i > 0 ? PosToClient(pos[i - 1]) + rw : 0;
			int hi = i < count ? PosToClient(pos[i]) - lw : vert ? sz.cy : sz.cx;
			q->покажи();
			if(vert)
				q->устПрям(0, lo, sz.cx, hi - lo);
			else
				q->устПрям(lo, 0, hi - lo, sz.cy);
			i++;
		}
	}
}

int Сплиттер::GetSplitWidth() const
{
	return chstyle->width;
}

void Сплиттер::PaintDots(Draw& w, const Прям& r, bool vert)
{
	int x = r.left + r.дайШирину() / 2;
	int y = r.top + r.дайВысоту() / 2;
	(vert ? x : y) -= 8 * 4;
	for(int i = 0; i < 16; i++) {
		w.DrawRect(x - 1, y - 1, 1, 1, SColorShadow());
		w.DrawRect(x, y, 1, 1, SColorShadow());
		(vert ? x : y) += 4;
	}
}

void Сплиттер::рисуй(Draw& w) {
	Размер sz = дайРазм();
	if(!IsTransparent())
		w.DrawRect(sz, SColorFace);
	const Значение *ch = vert ? chstyle->vert : chstyle->horz;
	if(style < 0)
		for(int i = 0; i < pos.дайСчёт(); i++) {
			int p = PosToClient(pos[i]) - (chstyle->width >> 1);
			Прям r = vert ? RectC(0, p, sz.cx, chstyle->width) : RectC(p, 0, chstyle->width, sz.cy);
			if(HasCapture() && i == mouseindex)
				ChPaint(w, r, ch[1]);
			else
			if(!IsTransparent())
				ChPaint(w, r, ch[0]);
			if(chstyle->dots)
				PaintDots(w, r, vert);
		}
}

void   Сплиттер::двигМыши(Точка p, dword) {
	if(HasCapture() && mouseindex >= 0 && mouseindex < pos.дайСчёт()) {
		устПоз(ClientToPos(p), mouseindex);
		освежи();
		WhenAction();
	}
}

void   Сплиттер::леваяВнизу(Точка p, dword) {
	SetCapture();
	освежи();
	mouseindex = найдиИндекс(p);
}

int Сплиттер::найдиИндекс(Точка client) const {
	int best = -1;
	int maxdist = chstyle->width;
	for(int i = 0; i < pos.дайСчёт(); i++) {
		int dist = абс((vert ? client.y : client.x) - PosToClient(pos[i]));
		if(dist <= maxdist) {
			best = i;
			maxdist = абс(dist);
		}
	}
	return best;
}

void   Сплиттер::леваяВверху(Точка p, dword keyflags) {
	if(HasCapture())
		WhenSplitFinish();
	ReleaseCapture();
	освежи();
}

Рисунок Сплиттер::рисКурсора(Точка p, dword) {
	return найдиИндекс(p) < 0 ? Рисунок::Arrow() : vert ? Рисунок::SizeVert() : Рисунок::SizeHorz();
}

int Сплиттер::GetMins(int i) const
{
	int min1 = (i < mins.дайСчёт() ? mins[i] : 0);
	int min2 = 0;
	int cx = вертикален() ? дайРазм().cy : дайРазм().cx;
	if(cx)
		min2 = (i < minpx.дайСчёт() ? minpx[i] : 0) * 10000 / cx;
	return max(min1, min2);
}

Сплиттер& Сплиттер::устПоз(int p, int i) {
	int l = (i > 0 && i - 1 < pos.дайСчёт() ? pos[i - 1] : 0) + GetMins(i);
	int h = (i + 1 < pos.дайСчёт() ? pos[i + 1] : 10000) - GetMins(i + 1);
	pos.по(i) = minmax(p, l, h);
	Выкладка();
	return *this;
}

void   Сплиттер::SyncMin()
{
	for(int i = 0; i < pos.дайСчёт(); i++)
		устПоз(pos[i], i);
}

void   Сплиттер::Zoom(int i)
{
	style = i;
	Выкладка();
}

Сплиттер& Сплиттер::верт(Ктрл& top, Ктрл& bottom)
{
	vert = true;
	уст(top, bottom);
	return *this;
}

Сплиттер& Сплиттер::гориз(Ктрл& left, Ктрл& right)
{
	vert = false;
	уст(left, right);
	return *this;
}

void Сплиттер::уст(Ктрл& l, Ктрл& r)
{
	очисть();
	*this << l << r;
	Выкладка();
}

void Сплиттер::добавь(Ктрл& pane)
{
	Ктрл::добавь(pane);
	Выкладка();
}

void Сплиттер::вставь(int ii, Ктрл& pane)
{
	if(ii >= дайСчёт())
		добавь(pane);
	else {	
		Ктрл::добавьОтпрыскПеред(&pane, GetIndexChild(ii));
		pos.очисть();
		Выкладка();
	}
}

void Сплиттер::разверни(Ктрл& child, Ктрл& newctrl)
{
	newctrl.устПрям(child.дайПрям());
	Ктрл::добавьОтпрыскПеред(&newctrl, &child);
	Ктрл::удалиОтпрыск(&child);
}

void Сплиттер::сериализуй(Поток& s) {
	int version = 0x02;
	s / version;
	if(version <= 1) {
		int p = дайПоз(0);
		s % p;
		устПоз(p, 0);
	}
	else
		s % pos;
	if(version >= 0x01) {
		s / style;
		if(version == 0x01 && style == 3)
			style = -1;
	}
	if(s.грузится()) {
		for(int i = 0; i < pos.дайСчёт(); i++)
			if(pos[i] < 0 || pos[i] > 10000) {
				pos.очисть();
				s.загрузиОш();
			}
		if(style >= GetViewChildCount()) {
			style = -1;
			s.загрузиОш();
		}
		Выкладка();
	}
}

void Сплиттер::удали(Ктрл& ctrl)
{
	int n = 0;
	Ктрл *c = дайПервОтпрыск();
	while(c) {
		if(c == &ctrl){
			if(c->дайСледщ())
				pos.удали(n);
			else
			if(n >= 1)
				pos.удали(n-1);
			mins.удали(n);
			minpx.удали(n);
			удалиОтпрыск(c);
			break;
		}
		c = c->дайСледщ();
		n++;
	}
}

void Сплиттер::очисть() {
	while(дайПервОтпрыск())
		удалиОтпрыск(дайПервОтпрыск());
	pos.очисть();
	mins.очисть();
	minpx.очисть();
}

void Сплиттер::переустанов() {
	очисть();
	style = -1;
	vert = false;
}

Сплиттер& Сплиттер::устСтиль(const Стиль& s)
{
	if(chstyle != &s) {
		chstyle = &s;
		освежиВыкладку();
		освежи();
	}
	return *this;
}

Сплиттер::Сплиттер() {
	chstyle = NULL;
	style = -1;
	vert = false;
	mouseindex = -1;
	pos.добавь(5000);
	устФрейм(фреймПусто());
	NoWantFocus();
	VSizePos(0, 0).HSizePos(0, 0);
	устСтиль(дефСтиль());
}

Сплиттер::~Сплиттер() {}

}
