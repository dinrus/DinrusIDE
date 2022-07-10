#include "Draw.h"

namespace РНЦП {

int Diff(КЗСА a, КЗСА b)
{
	return max(абс(a.a - b.a), max(абс(a.r - b.r), max(абс(a.b - b.b), абс(a.g - b.g))));
}

struct ButtonDecomposer {
	Рисунок src;
	Рисунок dst;
	int   aa;
	int   maxdiff;
	КЗСА  color;
	int   gdiff;
	int   gcount;

	void Do() {
		Размер sz = src.дайРазм();
		int a = min(5, min(sz.cx, sz.cy) / 5);
		maxdiff = gdiff = gcount = 0;
		color = SColorDisabled();
		dst = src;
		ImageBuffer b(dst);
		for(int y = a; y < sz.cy - a; y++) {
			КЗСА *p = b[y];
			КЗСА c1 = p[a - 1];
			КЗСА c2 = p[sz.cx - a + 1];
			int n = sz.cx - 2 * a;
			p += a;
			for(int x = 0; x < n; x++) {
				КЗСА c;
				c.r = ((n - x) * c1.r + x * c2.r) / n;
				c.g = ((n - x) * c1.g + x * c2.g) / n;
				c.b = ((n - x) * c1.b + x * c2.b) / n;
				c.a = ((n - x) * c1.a + x * c2.a) / n;
				int d = Diff(c, *p);
				if(d > 30) {
					gcount++;
					gdiff += d;
					if(d >= maxdiff) {
						maxdiff = d;
						color = *p;
					}
				}
				*p++ = c;
			}
		}
		b.SetHotSpot(Точка(a, a));
		dst = b;
	}
};

Рисунок Unglyph(const Рисунок& m, Цвет& c, double& gfactor)
{
	ButtonDecomposer b;
	b.src = Unmultiply(m);
	b.Do();
	c = b.color;
	gfactor = (double)b.gdiff / b.gcount;
	return Premultiply(b.dst);
}

Рисунок Unglyph(const Рисунок& m, Цвет& c)
{
	double dummy;
	return Unglyph(m, c, dummy);
}

Рисунок Unglyph(const Рисунок& m)
{
	Цвет dummy;
	return Unglyph(m, dummy);
}

Рисунок VertBlend(Рисунок img1, Рисунок img2, int y0, int y1)
{
	Размер sz = img1.дайРазм();
	Размер sz2 = img2.дайРазм();
	sz.cx = min(sz.cx, sz2.cx);
	sz.cy = min(sz.cy, sz2.cy);
	ImageBuffer b(sz);
	for(int y = 0; y < sz.cy; y++)
		if(y >= y1)
			memcpy(b[y], img2[y], sz.cx * sizeof(КЗСА));
		else
		if(y >= y0 && y1 > y0) {
			int alpha = 256 * (y - y0) / (y1 - y0);
			КЗСА *t = b[y];
			const КЗСА *s1 = img1[y];
			const КЗСА *s2 = img2[y];
			const КЗСА *e = s1 + sz.cx;
			while(s1 < e) {
				t->r = s1->r + ((alpha * (s2->r - s1->r)) >> 8);
				t->g = s1->g + ((alpha * (s2->g - s1->g)) >> 8);
				t->b = s1->b + ((alpha * (s2->b - s1->b)) >> 8);
				t->a = s1->a + ((alpha * (s2->a - s1->a)) >> 8);
				s1++;
				s2++;
				t++;
			}
		}
		else
			memcpy(b[y], img1[y], sz.cx * sizeof(КЗСА));
	b.SetHotSpot(img1.GetHotSpot());
	b.Set2ndSpot(img1.Get2ndSpot());
	return b;
}

Рисунок HorzBlend(Рисунок img1, Рисунок img2, int x0, int x1)
{
	Рисунок m = RotateAntiClockwise(VertBlend(RotateClockwise(img1), RotateClockwise(img2), x0, x1));
	ImageBuffer b(m);
	b.SetHotSpot(img1.GetHotSpot());
	b.Set2ndSpot(img1.Get2ndSpot());
	return b;
}

Рисунок HorzSymm(Рисунок src) {
	ImageBuffer b(src);
	Размер sz = b.дайРазм();
	for(int y = 0; y < sz.cy; y++) {
		КЗСА *l = b[y];
		for(int x = 0; x < sz.cx / 2; x++)
			l[sz.cx - x - 1] = l[x];
	}
	b.SetHotSpot(src.GetHotSpot());
	b.Set2ndSpot(src.Get2ndSpot());
	return b;
}

bool EqLine(const Рисунок& m, int l1, int l2, int x, int width)
{
	return !memcmp(m[l1] + x, m[l2] + x, width * sizeof(КЗСА));
}

bool EqLine(const Рисунок& m, int l1, int l2)
{
	return EqLine(m, l1, l2, 0, m.дайШирину());
}

bool IsSingleColor(const Рисунок& m, const Прям& rect_)
{
	Прям rect = rect_;
	rect.пересек(m.дайРазм());
	if(пусто_ли(rect))
		return true;
	КЗСА c = m[rect.top][rect.left];
	for(int y = rect.top; y < rect.bottom; y++) {
		const КЗСА *line = m[y];
		for(int x = rect.left; x < rect.right; x++)
			if(line[x] != c)
				return false;
	}
	return true;
}

Рисунок RecreateAlpha(const Рисунок& overwhite, const Рисунок& overblack)
{
	Размер sz = overwhite.дайРазм();
	ПРОВЕРЬ(overblack.дайРазм() == sz);
	ImageBuffer r(sz);
	const КЗСА *ws = overwhite;
	const КЗСА *bs = overblack;
	КЗСА *t = r;
	КЗСА *e = t + r.дайДлину();
	while(t < e) {
		t->a = Saturate255(bs->r - ws->r + 255);
		if(t->a) {
			t->r = Saturate255(bs->r * 255 / t->a);
			t->g = Saturate255(bs->g * 255 / t->a);
			t->b = Saturate255(bs->b * 255 / t->a);
		}
		else
			t->r = t->g = t->b = 0;
		t++;
		bs++;
		ws++;
	}
	Premultiply(r);
	return r;
}

int ImageMargin(const Рисунок& _m, int p, int dist)
{
	Рисунок m = Unmultiply(_m);
	Цвет c = m[p][p];
	int d;
	Размер sz = m.дайРазм();
	for(d = p; d >= 0; d--)
		if(Diff(m[d][d], c) > dist || Diff(m[sz.cx - d - 1][sz.cy - d - 1], c) > dist)
			break;
	return d + 1;
}

int ImageMarginV(const Рисунок& _m, int p, int dist)
{
	Рисунок m = Unmultiply(_m);
	Размер sz = m.дайРазм();
	Цвет c = m[sz.cx / 2][p];
	int d;
	for(d = p; d >= 0; d--)
		if(Diff(m[sz.cx / 2][d], c) > dist || Diff(m[sz.cx / 2][sz.cy - d - 1], c) > dist)
			break;
	return d + 1;
}

Прям GetImageMargins(const Рисунок& m, КЗСА margin_color)
{
	Прям r;
	Размер isz = m.дайРазм();
	for(int pass = 0; pass < 2; pass++) {
		int& y = pass ? r.bottom : r.top;
		for(y = 0; y < isz.cy; y++) {
			const КЗСА *s = m[pass ? isz.cy - y - 1 : y];
			for(int x = 0; x < isz.cx; x++)
				if(*s++ != margin_color)
					goto foundy;
		}
	foundy:
		int& x = pass ? r.right : r.left;
		for(x = 0; x < isz.cx; x++) {
			const КЗСА *s = m[0] + (pass ? isz.cx - x - 1 : x);
			for(int y = 0; y < isz.cy; y++) {
				if(*s != margin_color)
					goto foundx;
				s += isz.cx;
			}
		}
	foundx:;
	}
	return r;
}

ChPartMaker::ChPartMaker(const Рисунок& m)
{
	image = m;
	border = SColorShadow();
	bg = Null;
	ResetShape();
}

void ChPartMaker::ResetShape()
{
	t = b = l = r = true;
	tl = tr = bl = br = 0;
}

Рисунок ChPartMaker::сделай() const
{
	Размер sz = image.дайРазм();
	ПРОВЕРЬ(sz.cx >= 6 && sz.cy >= 6);
	Рисунок h = image;
	ImageBuffer ib(h);
	for(int x = 0; x < sz.cx; x++) {
		if(t)
			ib[0][x] = x >= tl && x < sz.cx - tr ? border : bg;
		if(b)
			ib[sz.cy - 1][x] = x >= bl && x < sz.cx - br ? border : bg;
	}
	for(int y = 0; y < sz.cy; y++) {
		if(l)
			ib[y][0] = y >= tl && y < sz.cy - bl ? border : bg;
		if(r)
			ib[y][sz.cx - 1] = y >= tr && y < sz.cy - br ? border : bg;
	}
	if(tl == 2)
		ib[1][1] = border;
	if(tr == 2)
		ib[1][sz.cx - 2] = border;
	if(bl == 2)
		ib[sz.cy - 2][1] = border;
	if(br == 2)
		ib[sz.cy - 2][sz.cx - 2] = border;
	int q = max(max(tl, tr), max(br, bl));
	ib.SetHotSpot(Точка(q, q));
	ib.Set2ndSpot(Точка(sz.cx - q - 1, sz.cy - q - 1));
	return ib;
}

}
