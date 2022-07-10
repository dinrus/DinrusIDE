#include "IconDes.h"

namespace РНЦП {

struct sFloodFill {
	Прям         rc;
	Размер         sz;
	Буфер<byte> flag;
	ImageBuffer& ib;
	КЗСА         scolor;
	КЗСА         color;
	bool         done;
	int          tolerance;

	КЗСА& по(int x, int y)         { return ib[y + rc.top][x + rc.left]; }
	bool  Eq(int x, int y);
	byte& Flag(int x, int y) { return flag[y * sz.cx + x]; }
	void  Fill(КЗСА color, Точка pt, const Прям& rc, int tolerance_);
	void  Try(int x, int y);

	sFloodFill(ImageBuffer& ib) : ib(ib) { tolerance = 0; }
};

force_inline
bool sFloodFill::Eq(int x, int y)
{
	const КЗСА& q = по(x, y);
	if(tolerance < 0 && q.a)
		return q.a != scolor.a || q.r != scolor.r || q.g != scolor.g || q.b != scolor.b;
	if((q.a | scolor.a) == 0) return true;
	return абс(q.a - scolor.a) <= tolerance && абс(q.r - scolor.r) + абс(q.g - scolor.g) + абс(q.b - scolor.b) <= tolerance;
}

void sFloodFill::Try(int x, int y)
{
	if(x >= 0 && x < sz.cx && y >= 0 && y < sz.cy && Flag(x, y) == 0 && Eq(x, y)) {
		Flag(x, y) = 1;
		по(x, y) = color;
		done = false;
	}
}

void sFloodFill::Fill(КЗСА _color, Точка pt, const Прям& _rc, int tolerance_)
{
	tolerance = tolerance_;
	rc = _rc & ib.дайРазм();
	if(!rc.содержит(pt))
		return;
	scolor = tolerance < 0 ? _color : ib[pt.y][pt.x];
	color = tolerance < 0 ? обнулиКЗСА() : _color;
	sz = rc.дайРазм();
	flag.размести(sz.cx * sz.cy, 0);
	pt -= rc.верхЛево();
	Flag(pt.x, pt.y) = 1;
	по(pt.x, pt.y) = color;
	do {
		done = true;
		for(int y = 0; y < sz.cy; y++)
			for(int x = 0; x < sz.cx; x++) {
				if(Flag(x, y) == 1) {
					Flag(x, y) = 2;
					Try(x + 1, y);
					Try(x - 1, y);
					Try(x, y + 1);
					Try(x, y - 1);
				}
			}
	}
	while(!done);
}

void FloodFill(ImageBuffer& img, КЗСА color, Точка pt, const Прям& rc, int tolerance)
{
	sFloodFill(img).Fill(color, pt, rc, tolerance);
}

struct InterpolateFilter : ImageFilter9 {
	int todo;

	virtual КЗСА operator()(const КЗСА **mx);
};

КЗСА InterpolateFilter::operator()(const КЗСА **mx)
{
	КЗСА t = mx[1][1];
	int na = mx[0][1].a, wa = mx[1][0].a, ea = mx[1][2].a, sa = mx[2][1].a;
	int suma = na + wa + ea + sa, half = suma >> 1;
	if(suma > 0) {
		t.r = (mx[0][1].r * na + mx[1][0].r * wa + mx[1][2].r * ea + mx[2][1].r * sa + half) / suma;
		t.g = (mx[0][1].g * na + mx[1][0].g * wa + mx[1][2].g * ea + mx[2][1].g * sa + half) / suma;
		t.b = (mx[0][1].b * na + mx[1][0].b * wa + mx[1][2].b * ea + mx[2][1].b * sa + half) / suma;
		t.a = max(max(na, sa), max(wa, ea));
		if(t != mx[1][1])
			todo++;
	}
	return t;
}

void InterpolateImage(Рисунок& img, const Прям& _rc)
{
	Прям rc = _rc & img.дайРазм();
	Рисунок m = Crop(img, rc);
	ТаймСтоп tm;
	Рисунок imp = CreateImage(rc.дайРазм(), Null);
	Over(imp, Точка(0, 0), m, m.дайРазм());
	Progress pi("Interpolating...");
	for(int qq = 0; qq < 2000; qq++) {
		InterpolateFilter f;
		f.todo = 0;
		imp = фильтруй(imp, f);
		Over(imp, Точка(0, 0), m, m.дайРазм());
		if(f.todo == 0)
			break;
		if(pi.SetCanceled(qq, 2000))
			break;
	}
	копируй(img, rc.верхЛево(), imp, imp.дайРазм());
}

void MirrorHorz(Рисунок& img, const Прям& rect)
{
	ImageBuffer ib(img);
	Прям rc = rect & ib.дайРазм();
	for(int y = rc.top; y < rc.bottom; y++) {
		КЗСА *b = ib[y] + rc.left;
		КЗСА *e = ib[y] + rc.right - 1;
		while(b < e) {
			разверни(*b, *e);
			b++;
			e--;
		}
	}
	img = ib;
}

void MirrorVert(Рисунок& img, const Прям& rect)
{
	ImageBuffer ib(img);
	Прям rc = rect & ib.дайРазм();
	int b = rc.top;
	int e = rc.bottom - 1;
	int n = rc.right - rc.left;
	if(n > 0) {
		Буфер<КЗСА> h(n);
		n *= sizeof(КЗСА);
		while(b < e) {
			memcpy(h, ib[b] + rc.left, n);
			memcpy(ib[b] + rc.left, ib[e] + rc.left, n);
			memcpy(ib[e] + rc.left, h, n);
			b++;
			e--;
		}
	}
	img = ib;
}

Ткст PackImlData(const Вектор<ImageIml>& image)
{
	ТкстБуф block;
	for(const ImageIml& m : image) {
		const Рисунок& img = m.image;
		ТкстПоток ss;
		ss.помести(((dword)img.GetResolution() << 6) | m.flags);
		Размер sz = img.дайРазм();
		ss.Put16le(sz.cx);
		ss.Put16le(sz.cy);
		Точка p = img.GetHotSpot();
		ss.Put16le(p.x);
		ss.Put16le(p.y);
		p = img.Get2ndSpot();
		ss.Put16le(p.x);
		ss.Put16le(p.y);
		block.конкат(ss.дайРез());
		const КЗСА *s = img;
		const КЗСА *e = s + img.дайДлину();
		while(s < e) {
			block.конкат(s->r);
			block.конкат(s->g);
			block.конкат(s->b);
			block.конкат(s->a);
			s++;
		}
	}
	return ZCompress(block);
}

Рисунок DownSample3x(const Рисунок& src)
{
	Размер tsz = src.дайРазм() / 3;
	ImageBuffer ib(tsz);
	int w = src.дайРазм().cx;
	int w2 = 2 * w;
	for(int y = 0; y < tsz.cy; y++) {
		КЗСА *t = ib[y];
		КЗСА *e = t + tsz.cx;
		const КЗСА *s = src[3 * y];
		while(t < e) {
			int r, g, b, a;
			const КЗСА *q;
			r = g = b = a = 0;
#define S__SUM(delta) q = s + delta; r += q->r; g += q->g; b += q->b; a += q->a;
			S__SUM(0) S__SUM(1) S__SUM(2)
			S__SUM(w + 0) S__SUM(w + 1) S__SUM(w + 2)
			S__SUM(w2 + 0) S__SUM(w2 + 1) S__SUM(w2 + 2)
#undef  S__SUM
			t->a = a / 9;
			t->r = r / 9;
			t->g = g / 9;
			t->b = b / 9;
			t++;
			s += 3;
		}
	}
	ib.SetResolution(src.GetResolution());
	return ib;
}

Рисунок DownSample2x(const Рисунок& src)
{
	Размер tsz = src.дайРазм() / 2;
	ImageBuffer ib(tsz);
	int w = src.дайРазм().cx;
	for(int y = 0; y < tsz.cy; y++) {
		КЗСА *t = ib[y];
		КЗСА *e = t + tsz.cx;
		const КЗСА *s = src[2 * y];
		while(t < e) {
			int r, g, b, a;
			const КЗСА *q;
			r = g = b = a = 0;
#define S__SUM(delta) q = s + delta; r += q->r; g += q->g; b += q->b; a += q->a;
			S__SUM(0) S__SUM(1)
			S__SUM(w + 0) S__SUM(w + 1)
#undef  S__SUM
			t->a = a / 4;
			t->r = r / 4;
			t->g = g / 4;
			t->b = b / 4;
			t++;
			s += 2;
		}
	}
	ib.SetResolution(src.GetResolution());
	return ib;
}

}
