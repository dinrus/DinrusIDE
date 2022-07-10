#include "Draw.h"

namespace РНЦП {

DDARasterizer::DDARasterizer()
{
	cy = 0;
	p0 = p1 = Точка(0, 0);
	j1 = j2 = Null;
	b1 = b2 = Null;
	close = false;
	width = 1;
}

void DDARasterizer::AHorz(int x, int y, int cx)
{
	if(cx) {
		if(cx < 0)
			PutHorz(x + cx + 1, y, -cx);
		else
			PutHorz(x, y, cx);
	}
}

void DDARasterizer::AVert(int x, int y, int cy)
{
	if(cy) {
		if(cy < 0)
			PutVert(x, y + cy + 1, -cy);
		else
			PutVert(x, y, cy);
	}
}

void DDARasterizer::DoLine(Точка p1, Точка p2, bool last)
{
	dirx = зн(p2.x - p1.x);
	diry = зн(p2.y - p1.y);
	int dx = абс(p2.x - p1.x);
	int dy = абс(p2.y - p1.y);
	int x = p1.x;
	int y = p1.y;
	int x0 = x;
	int y0 = y;
	if(dx < dy) {
		int dda = dy >> 1;
		int n = dy + last;
		for(;;) {
			if(x != x0) {
				AVert(x0, y0, y - y0);
				x0 = x;
				y0 = y;
			}
			if(n-- <= 0)
				break;
			y += diry;
			dda -= dx;
			if(dda < 0) {
				dda += dy;
				x += dirx;
			}
		}
		AVert(x0, y0, y - y0);
	}
	else {
		int dda = dx >> 1;
		int n = dx + last;
		for(;;) {
			if(y != y0) {
				AHorz(x0, y0, x - x0);
				x0 = x;
				y0 = y;
			}
			if(n-- <= 0)
				break;
			x += dirx;
			dda -= dy;
			if(dda < 0) {
				dda += dx;
				y += diry;
			}
		}
		AHorz(x0, y0, x - x0);
	}
}

struct DDARasterizer::Segments : DDARasterizer {
	int         miny;
	int         maxy;
	bool        flag;
	Вектор< Вектор< Segment > > segment;
	
	virtual void PutHorz(int x, int y, int cx) {
		if(y >= 0 && y < cy) {
			Segment& m = segment.по(y).добавь();
			m.l = x;
			m.h = x + cx;
			m.flag = flag;
			miny = min(y, miny);
			maxy = max(y, maxy);
		}
		flag = true;
	}
	virtual void PutVert(int x, int y, int cy) {
		for(int i = 0; i < cy; i++)
			PutHorz(x, y + i, 1);
	}
	
	Segments() { miny = INT_MAX; maxy = 0; }
};

void DDARasterizer::FatLine(Точка p2)
{
	if(p1 == p2)
		return;
	Точка pp = p0;
	p0 = p1; // avoid закрой
	Точка v = p2 - p1;
	Точка shift = (width - 0.5) * ортогональ(ТочкаПЗ(v) / длина((ТочкаПЗ(v))));
	Точка p = p1 + shift / 2;
	Точка bj2 = j2;
	Polygon();
	if(!пусто_ли(j1)) {
		Move(j1);
		Строка(p);
	}
	else
		Move(p);
	if(пусто_ли(b1))
		b1 = p;
	Строка(p += v);
	j1 = p;
	if(close && !пусто_ли(b1)) {
		Строка(b1);
		Строка(b2);
	}
	Строка(p -= shift);
	j2 = p;
	Строка(p -= v);
	if(!пусто_ли(bj2))
		Строка(bj2);
	if(пусто_ли(b2))
		b2 = p;
	Fill();
	p0 = pp;
}

DDARasterizer& DDARasterizer::Move(Точка p)
{
	if(pseg)
		закрой();
	p0 = p1 = p;
	j1 = j2 = Null;
	return *this;
}

DDARasterizer& DDARasterizer::Строка(Точка p)
{
	if(pseg) {
		Точка a = p1;
		Точка b = p;
		if(a.y > b.y)
			разверни(a, b);
		pseg->flag = false;
		pseg->DoLine(a, b, true);
		p1 = p;
	}
	else {
		if(width <= 1)
			DoLine(p1, p, false);
		else
			FatLine(p);
		p1 = p;
	}
	return *this;
}

DDARasterizer& DDARasterizer::закрой()
{
	if(p1 != p0) {
		close = true;
		Строка(p0);
		close = false;
		if(!pseg)
			b1 = b2 = Null;
	}
	return *this;
}

DDARasterizer& DDARasterizer::Polygon()
{
	pseg.создай();
	pseg->Cy(cy);
	return *this;
}

DDARasterizer& DDARasterizer::Fill()
{
	ПРОВЕРЬ(pseg);
	закрой();
	for(int y = pseg->miny; y <= pseg->maxy; y++) {
		Вектор<Segment>& gg = pseg->segment[y];
		сортируй(gg);
		int i = 0;
		bool flag = false;
		while(i < gg.дайСчёт()) {
			int l = gg[i].l;
			int h = gg[i].h;
			flag ^= gg[i++].flag;
			while(i < gg.дайСчёт() && flag) {
				h = gg[i].h;
				flag ^= gg[i++].flag;
			}
			PutHorz(l, y, h - l);
		}
	}
	pseg.очисть();
	return *this;
}

DDARasterizer& DDARasterizer::Ellipse(const Прям& rect)
{
	Размер size = rect.дайРазм() / 2;
	int n = 16;
	Буфер<Точка> p(n);
	for(int i = 0; i < n; i++) {
		double angle = M_PI * i / (n - 1) / 2;
		p[i].x = min(size.cx, int(sin(angle) * size.cx + 0.5));
		p[i].y = min(size.cy, int(cos(angle) * size.cy + 0.5));
	}
	Точка center = rect.верхЛево() + size;
	Move(Точка(center.x - p[0].x, center.y - p[0].y));
	for(int i = 1; i < n; i++)
		Строка(Точка(center.x - p[i].x, center.y - p[i].y));
	center.y = rect.bottom - 1 - size.cy;
	for(int i = n - 1; i >= 0; i--)
		Строка(Точка(center.x - p[i].x, center.y + p[i].y));
	center.x = rect.right - 1 - size.cx;
	for(int i = 0; i < n; i++)
		Строка(Точка(center.x + p[i].x, center.y + p[i].y));
	center.y = rect.top + size.cy;
	for(int i = n - 1; i >= 0; i--)
		Строка(Точка(center.x + p[i].x, center.y - p[i].y));
	закрой();
	return *this;
}

DDARasterizer::~DDARasterizer() {}

}
