#include "Draw.h"

namespace РНЦП {
	
void  SetHotSpots(Рисунок& m, Точка hotspot, Точка hotspot2)
{
	ImageBuffer b(m);
	b.SetHotSpot(hotspot);
	b.Set2ndSpot(hotspot2);
	m = b;
}

Рисунок WithHotSpots(const Рисунок& m, Точка hotspot, Точка hotspot2)
{
	Рисунок h = m;
	SetHotSpots(h, hotspot, hotspot2);
	return h;
}

Рисунок WithHotSpots(const Рисунок& m, int x1, int y1, int x2, int y2)
{
	return WithHotSpots(m, Точка(x1, y1), Точка(x2, y2));
}

Рисунок WithHotSpot(const Рисунок& m, int x1, int y1)
{
	Рисунок h = m;
	ImageBuffer b(h);
	b.SetHotSpot(Точка(x1, y1));
	return b;
}

void  SetResolution(Рисунок& m, int res)
{
	ImageBuffer b(m);
	b.SetResolution(res);
	m = b;
}

Рисунок WithResolution(const Рисунок& m, int res)
{
	Рисунок h = m;
	SetResolution(h, res);
	return h;
}

Рисунок WithResolution(const Рисунок& m, const Рисунок& res)
{
	return WithResolution(m, res.GetResolution());
}

Рисунок CreateImage(Размер sz, const КЗСА& rgba)
{
	ImageBuffer ib(sz);
	Fill(~ib, rgba, ib.дайДлину());
	return ib;
}

Рисунок CreateImage(Размер sz, Цвет color)
{
	return CreateImage(sz, (КЗСА)color);
}

void ScanOpaque(Рисунок& m)
{
	ImageBuffer ib(m);
	ib.SetKind(ib.ScanKind());
	m = ib;
}

force_inline Размер DstSrc(ImageBuffer& dest, Точка& p, const Рисунок& src, Прям& sr)
{
	if(p.x < 0) {
		sr.left += -p.x;
		p.x = 0;
	}
	if(p.y < 0) {
		sr.top += -p.y;
		p.y = 0;
	}
	sr = sr & src.дайРазм();
	Размер sz = dest.дайРазм() - p;
	sz.cx = min(sz.cx, sr.дайШирину());
	sz.cy = min(sz.cy, sr.дайВысоту());
	return sz;
}

void DstSrcOp(ImageBuffer& dest, Точка p, const Рисунок& src, const Прям& srect,
                           void (*op)(КЗСА *t, const КЗСА *s, int n))
{
	dest.SetResolution(src.GetResolution());
	Прям sr = srect;
	Размер sz = DstSrc(dest, p, src, sr);
	if(sz.cx > 0)
		while(--sz.cy >= 0)
			(*op)(dest[p.y++] + p.x, src[sr.top++] + sr.left, sz.cx);
}

void копируй(ImageBuffer& dest, Точка p, const Рисунок& src, const Прям& srect)
{
	DstSrcOp(dest, p, src, srect, копируй);
}

void Over(ImageBuffer& dest, Точка p, const Рисунок& src, const Прям& srect)
{
	DstSrcOp(dest, p, src, srect, AlphaBlend);
}

Рисунок GetOver(const Рисунок& dest, const Рисунок& src)
{
	Рисунок r = dest;
	Over(r, src);
	return r;
}

Рисунок копируй(const Рисунок& src, const Прям& srect)
{
	ImageBuffer ib(srect.дайРазм());
	копируй(ib, Точка(0, 0), src, srect);
	return ib;
}

void Fill(ImageBuffer& dest, const Прям& rect, КЗСА color)
{
	Прям r = dest.дайРазм() & rect;
	int cx = r.дайШирину();
	if(cx)
		for(int y = r.top; y < r.bottom; y++)
			Fill(dest[y] + r.left, color, cx);
}

void OverStraightOpaque(ImageBuffer& dest, Точка p, const Рисунок& src, const Прям& srect)
{
	DstSrcOp(dest, p, src, srect, AlphaBlendStraightOpaque);
}

void  копируй(Рисунок& dest, Точка p, const Рисунок& _src, const Прям& srect)
{
	Рисунок src = _src;
	ImageBuffer b(dest);
	копируй(b, p, src, srect);
	dest = b;
}

void  Over(Рисунок& dest, Точка p, const Рисунок& _src, const Прям& srect)
{
	Рисунок src = _src;
	ImageBuffer b(dest);
	Over(b, p, src, srect);
	dest = b;
}

void  Over(Рисунок& dest, const Рисунок& _src)
{
	Over(dest, Точка(0, 0), _src, _src.дайРазм());
}

void Fill(Рисунок& dest, const Прям& rect, КЗСА color)
{
	ImageBuffer b(dest);
	Fill(b, rect, color);
	dest = b;
}

void  OverStraightOpaque(Рисунок& dest, Точка p, const Рисунок& _src, const Прям& srect)
{
	Рисунок src = _src;
	ImageBuffer b(dest);
	OverStraightOpaque(b, p, src, srect);
	dest = b;
}

void Crop(RasterEncoder& tgt, Raster& img, const Прям& rc)
{
	Прям r = rc & img.дайРазм();
	tgt.создай(r.размер(), img);
	for(int y = r.top; y < r.bottom; y++)
		tgt.WriteLine(~img[y] + r.left);
}

Рисунок Crop(const Рисунок& img, const Прям& rc)
{
	if(rc.left == 0 && rc.top == 0 && rc.размер() == img.дайРазм())
		return img;
	if((rc & img.дайРазм()).пустой())
		return Рисунок();
	ImageRaster  src(img);
	ImageEncoder tgt;
	Crop(tgt, src, rc);
	return WithResolution(tgt, img);
}

Рисунок AddMargins(const Рисунок& img, int left, int top, int right, int bottom, КЗСА color)
{
	Размер sz = img.дайРазм();
	ImageBuffer ib(sz.cx + left + right, sz.cy + top + bottom);
	Fill(ib, color, ib.дайДлину());
	копируй(ib, Точка(left, top), img, img.дайРазм());
	return ib;
}

Рисунок Crop(const Рисунок& img, int x, int y, int cx, int cy)
{
	return Crop(img, RectC(x, y, cx, cy));
}

bool IsUniform(const КЗСА *s, КЗСА c, int add, int n)
{
	while(n-- > 0) {
		if(*s != c)
			return false;
		s += add;
	}
	return true;
}

Прям FindBounds(const Рисунок& m, КЗСА bg)
{
	Размер isz = m.дайРазм();
	Прям r = isz;
	for(r.top = 0; r.top < isz.cy && IsUniform(m[r.top], bg, 1, isz.cx); r.top++)
		;
	for(r.bottom = isz.cy; r.bottom > r.top && IsUniform(m[r.bottom - 1], bg, 1, isz.cx); r.bottom--)
		;
	if(r.bottom <= r.top)
		return Null;
	int h = r.дайВысоту();
	const КЗСА *p = m[r.top];
	for(r.left = 0; r.left < isz.cy && IsUniform(p + r.left, bg, isz.cx, h); r.left++)
		;
	for(r.right = isz.cx; r.right > r.left && IsUniform(p + r.right - 1, bg, isz.cx, h); r.right--)
		;
	return r;
}

void AutoCrop(Рисунок *m, int count, КЗСА bg)
{
	if(!count)
		return;
	Прям r = FindBounds(m[0]);
	for(int i = 1; i < count; i++)
		r.союз(FindBounds(m[i], bg));
	for(int i = 0; i < count; i++) {
		Точка p1 = m[i].GetHotSpot() - r.верхЛево();
		Точка p2 = m[i].Get2ndSpot() - r.верхЛево();
		m[i] = WithHotSpots(Crop(m[i], r), p1.x, p1.y, p2.x, p2.y);
	}
}

Рисунок AutoCrop(const Рисунок& m, КЗСА bg)
{
	Рисунок mm = m;
	AutoCrop(&mm, 1, bg);
	return mm;
}

void ClampHotSpots(Рисунок& m)
{
	Точка p1 = m.GetHotSpot();
	Точка p2 = m.Get2ndSpot();
	Прям clamp = m.дайРазм();
	Точка p1a = clamp.свяжи(p1);
	Точка p2a = clamp.свяжи(p2);
	if(p1 != p1a || p2 != p2a)
		SetHotSpots(m, p1a, p2a);
}

Рисунок ColorMask(const Рисунок& src, Цвет ключ)
{
	ImageBuffer ib(src.дайРазм());
	ib.SetResolution(src.GetResolution());
	const КЗСА *s = src;
	const КЗСА *e = src + src.дайДлину();
	КЗСА *t = ~ib;
	byte kr = ключ.дайК();
	byte kg = ключ.дайЗ();
	byte kb = ключ.дайС();
	while(s < e) {
		if(s->r == kr && s->g == kg && s->b == kb)
			*t++ = обнулиКЗСА();
		else
			*t++ = *s;
		s++;
	}
	ib.SetHotSpots(src);
	return ib;
}

void CanvasSize(RasterEncoder& tgt, Raster& img, int cx, int cy)
{
	tgt.создай(cx, cy, img);
	int ccx = min(img.дайШирину(), cx);
	int ccy = min(img.дайВысоту(), cy);
	for(int y = 0; y < ccy; y++) {
		memcpy(~tgt, img[y], ccx * sizeof(КЗСА));
		memset(~tgt + ccx, 0, (cx - ccx) * sizeof(КЗСА));
		tgt.WriteLine();
	}
	for(int y = cy - ccy; --y >= 0;) {
		memset(~tgt, 0, cx * sizeof(КЗСА));
		tgt.WriteLine();
	}
}

Рисунок CanvasSize(const Рисунок& img, int cx, int cy)
{
	ImageRaster  src(img);
	ImageEncoder tgt;
	CanvasSize(tgt, src, cx, cy);
	return WithResolution(tgt, img);
}

Рисунок AssignAlpha(const Рисунок& img, const Рисунок& alpha)
{
	Размер sz = Размер(min(img.дайШирину(), alpha.дайШирину()),
	               min(img.дайВысоту(), alpha.дайВысоту()));
	if(sz.cx == 0 || sz.cy == 0)
		return Рисунок();
	ImageBuffer ib(sz);
	for(int y = 0; y < sz.cy; y++) {
		const КЗСА *s = img[y];
		const КЗСА *e = s + sz.cx;
		const КЗСА *a = alpha[y];
		КЗСА *t = ib[y];
		while(s < e) {
			*t = *s++;
			(t++)->a = (a++)->a;
		}
	}
	ib.SetHotSpots(img);
	ib.SetResolution(img.GetResolution());
	return ib;
}

int   EqualightCh(int c, int l, int h)
{
	return Saturate255((c - l) * 255 / (h - l) + l);
}

Рисунок Equalight(const Рисунок& img, int thold)
{
	int histogram[256];
	ZeroArray(histogram);
	const КЗСА *s = ~img;
	const КЗСА *e = s + img.дайДлину();
	while(s < e) {
		histogram[Grayscale(*s)]++;
		s++;
	}
	int n = (thold * img.дайДлину()) >> 8;
	int h = 255;
	int l = 0;
	while(l < h) {
		if(n < 0)
			break;
		n -= histogram[l++];
		if(n < 0)
			break;
		n -= histogram[h--];
	}
	if(l >= h)
		return img;
	ImageBuffer w(img.дайРазм());
	КЗСА *t = w;
	s = ~img;
	while(s < e) {
		t->r = EqualightCh(s->r, l, h);
		t->g = EqualightCh(s->g, l, h);
		t->b = EqualightCh(s->b, l, h);
		t->a = s->a;
		s++;
		t++;
	}
	w.SetHotSpots(img);
	w.SetResolution(img.GetResolution());
	return w;
}

Рисунок Grayscale(const Рисунок& img)
{
	const КЗСА *s = ~img;
	const КЗСА *e = s + img.дайДлину();
	ImageBuffer w(img.дайРазм());
	КЗСА *t = w;
	while(s < e) {
		int q = Grayscale(*s);
		t->r = q;
		t->g = q;
		t->b = q;
		t->a = s->a;
		t++;
		s++;
	}
	w.SetHotSpots(img);
	w.SetResolution(img.GetResolution());
	return w;
}

Рисунок Grayscale(const Рисунок& img, int amount)
{
	const КЗСА *s = ~img;
	const КЗСА *e = s + img.дайДлину();
	ImageBuffer w(img.дайРазм());
	КЗСА *t = w;
	int na = 256 - amount;
	while(s < e) {
		int q = Grayscale(*s);
		t->r = Saturate255((amount * q + na * s->r) >> 8);
		t->g = Saturate255((amount * q + na * s->g) >> 8);
		t->b = Saturate255((amount * q + na * s->b) >> 8);
		t->a = s->a;
		t++;
		s++;
	}
	w.SetHotSpots(img);
	w.SetResolution(img.GetResolution());
	return w;
}

Рисунок Colorize(const Рисунок& img, Цвет color, int alpha)
{
	const КЗСА *s = ~img;
	const КЗСА *e = s + img.дайДлину();
	ImageBuffer w(img.дайРазм());
	Unmultiply(w);
	КЗСА *t = w;
	byte r = color.дайК();
	byte g = color.дайЗ();
	byte b = color.дайС();
	alpha = alpha + (alpha >> 7);
	while(s < e) {
		int ga = Grayscale(*s);
		ga = ga + (ga >> 7);
		t->r = (alpha * (((ga * r) >> 8) - s->r) >> 8) + s->r;
		t->g = (alpha * (((ga * g) >> 8) - s->g) >> 8) + s->g;
		t->b = (alpha * (((ga * b) >> 8) - s->b) >> 8) + s->b;
		t->a = s->a;
		t++;
		s++;
	}
	Premultiply(w);
	w.SetHotSpots(img);
	w.SetResolution(img.GetResolution());
	return w;
}

Рисунок тёмнаяТема(const Рисунок& img)
{
	if(пусто_ли(img))
		return img;

	Рисунок simg = Unmultiply(img);
	const КЗСА *s = simg.begin();
	const КЗСА *e = simg.end();

	ImageBuffer ib(img.дайРазм());
	КЗСА *t = ~ib;
	
	while(s < e) {
		КЗСА h = *s++;
		byte a = h.a;
		h.a = 255;
		h = тёмнаяТема(Цвет(h));
		h.a = a;
		*t++ = h;
	}
	
	Premultiply(ib);

	ib.SetHotSpots(img);
	ib.SetResolution(img.GetResolution());

	return ib;
}

inline
byte ContrastCh(int amount, int ch)
{
	return Saturate255(128 + (amount * (ch - 128) >> 8));
}

Рисунок Contrast(const Рисунок& img, int amount)
{
	const КЗСА *s = ~img;
	const КЗСА *e = s + img.дайДлину();
	ImageBuffer w(img.дайРазм());
	Unmultiply(w);
	КЗСА *t = w;
	while(s < e) {
		t->r = ContrastCh(amount, s->r);
		t->g = ContrastCh(amount, s->g);
		t->b = ContrastCh(amount, s->b);
		t->a = s->a;
		t++;
		s++;
	}
	Premultiply(w);
	w.SetHotSpots(img);
	w.SetResolution(img.GetResolution());
	return w;
}

void sLine(КЗСА *t, int cx, const RasterLine l[3], ImageFilter9& filter)
{
	КЗСА h[3][3];
	const КЗСА *x[3];
	x[0] = h[0];
	x[1] = h[1];
	x[2] = h[2];
	if(cx == 1) {
		h[0][0] = l[0][0]; h[0][1] = l[0][0]; h[0][2] = l[0][0];
		h[1][0] = l[1][0]; h[1][1] = l[1][0]; h[1][2] = l[1][0];
		h[2][0] = l[2][0]; h[2][1] = l[2][0]; h[2][2] = l[2][0];
		*t = filter(x);
		return;
	}
	h[0][0] = l[0][0]; h[0][1] = l[0][0]; h[0][2] = l[0][1];
	h[1][0] = l[1][0]; h[1][1] = l[1][0]; h[1][2] = l[1][1];
	h[2][0] = l[2][0]; h[2][1] = l[2][0]; h[2][2] = l[2][1];
	*t++ = filter(x);
	for(int i = 1; i < cx - 1; i++) {
		x[0] = ~l[0] + i - 1;
		x[1] = ~l[1] + i - 1;
		x[2] = ~l[2] + i - 1;
		*t++ = filter(x);
	}
	h[0][0] = l[0][cx - 2]; h[0][1] = l[0][cx - 1]; h[0][2] = l[0][cx - 1];
	h[1][0] = l[1][cx - 2]; h[1][1] = l[1][cx - 1]; h[1][2] = l[1][cx - 1];
	h[2][0] = l[2][cx - 2]; h[2][1] = l[2][cx - 1]; h[2][2] = l[2][cx - 1];
	x[0] = h[0];
	x[1] = h[1];
	x[2] = h[2];
	*t++ = filter(x);
}

void  фильтруй(RasterEncoder& target, Raster& src, ImageFilter9& filter)
{
	Размер sz = src.дайРазм();
	target.создай(sz, src);
	if(sz.cy < 1)
		return;
	RasterLine l[3];
	if(sz.cy == 1) {
		l[0] = src[0];
		l[1] = src[0];
		l[2] = src[0];
		sLine(target, sz.cx, l, filter);
		return;
	}
	l[0] = src[0];
	l[1] = src[0];
	l[2] = src[1];
	sLine(target, sz.cx, l, filter);
	target.WriteLine();
	for(int y = 1; y < sz.cy - 1; y++) {
		l[0] = pick(l[1]);
		l[1] = pick(l[2]);
		l[2] = src[y + 1];
		sLine(target, sz.cx, l, filter);
		target.WriteLine();
	}
	l[0] = pick(l[1]);
	l[1] = pick(l[2]);
	l[2] = src[sz.cy - 1];
	sLine(target, sz.cx, l, filter);
	target.WriteLine();
}

Рисунок фильтруй(const Рисунок& img, ImageFilter9& filter)
{
	ImageEncoder tgt;
	ImageRaster src(img);
	фильтруй(tgt, src, filter);
	return WithResolution(tgt, img);
}

struct RGBAI {
	int r, g, b, a;

	RGBAI() { r = g = b = a= 0; }
};

static void sGetS(КЗСА q, RGBAI& p, int mul)
{
	p.r += mul * q.r;
	p.g += mul * q.g;
	p.b += mul * q.b;
	p.a += mul * q.a;
}

struct sSharpenFilter : ImageFilter9 {
	int amount;

	virtual КЗСА operator()(const КЗСА **mx);
};

КЗСА sSharpenFilter::operator()(const КЗСА **mx)
{
	RGBAI q;
	sGetS(mx[0][0], q, 7);
	sGetS(mx[0][1], q, 9);
	sGetS(mx[0][2], q, 7);
	sGetS(mx[1][0], q, 9);
	sGetS(mx[1][2], q, 9);
	sGetS(mx[2][0], q, 7);
	sGetS(mx[2][1], q, 9);
	sGetS(mx[2][2], q, 7);
	const КЗСА& s = mx[1][1];
	КЗСА t;
	int na = 256 + amount;
	t.b = Saturate255((na * (s.b << 6) - amount * q.b) >> 14);
	t.g = Saturate255((na * (s.g << 6) - amount * q.g) >> 14);
	t.r = Saturate255((na * (s.r << 6) - amount * q.r) >> 14);
	t.a = Saturate255((na * (s.a << 6) - amount * q.a) >> 14);
	return t;
}

void Sharpen(RasterEncoder& target, Raster& src, int amount)
{
	Размер sz = src.дайРазм();
	target.создай(sz, src);
	sSharpenFilter f;
	f.amount = amount;
	фильтруй(target, src, f);
}

Рисунок Sharpen(const Рисунок& img, int amount)
{
	ImageEncoder tgt;
	ImageRaster src(img);
	Sharpen(tgt, src, amount);
	return WithResolution(tgt, img);
}

struct sEtchFilter : ImageFilter9 {
	virtual КЗСА operator()(const КЗСА **mx);
};

КЗСА sEtchFilter::operator()(const КЗСА **mx)
{
	КЗСА t;
	КЗСА s = mx[1][1];
	if(s.a > 0x80 && s.r + s.g + s.b < 500) {
		t.r = t.g = t.b = 128;
		t.a = s.a;
		return t;
	}
	s = mx[0][0];
	if(s.a > 0x80 && s.r + s.g + s.b < 500) {
		t.r = t.g = t.b = 255;
		t.a = s.a;
		return t;
	}
	обнули(t);
	return t;
}

Рисунок Etched(const Рисунок& img)
{
	sEtchFilter ef;	
	return Premultiply(фильтруй(Unmultiply(img), ef));
}

Рисунок SetColorKeepAlpha(const Рисунок& img, Цвет c)
{
	КЗСА rgba = c;
	const КЗСА *s = ~img;
	const КЗСА *e = s + img.дайДлину();
	ImageBuffer w(img.дайРазм());
	КЗСА *t = w;
	while(s < e) {
		*t = rgba;
		(t++)->a = (s++)->a;
	}
	Premultiply(w);
	w.SetHotSpots(img);
	w.SetResolution(img.GetResolution());
	return w;
}

Рисунок CreateHorzFadeOut(Размер sz, Цвет color)
{
	ImageBuffer ib(sz);
	КЗСА c = color;
	for(int q = 0; q < sz.cx; q++) {
		c.a = q * 255 / sz.cx;
		КЗСА *t = ~ib + q;
		for(int n = sz.cy; n > 0; n--) {
			*t = c;
			t += sz.cx;
		}
	}
	Premultiply(ib);
	return ib;
}

struct FadeOutMaker : ImageMaker {
	Размер  sz;
	Цвет color;

	virtual Ткст Ключ() const {
		char h[sizeof(Размер) + sizeof(Цвет)];
		memcpy(h, &sz, sizeof(sz));
		memcpy(h + sizeof(Размер), &color, sizeof(Цвет));
		return Ткст(h, sizeof(h));
	}

	virtual Рисунок  сделай() const {
		return CreateHorzFadeOut(sz, color);
	}
};

Рисунок  HorzFadeOut(Размер sz, Цвет color)
{
	FadeOutMaker m;
	m.sz = sz;
	m.color = color;
	return MakeImage(m);
}

Рисунок HorzFadeOut(int cx, int cy, Цвет color)
{
	return HorzFadeOut(Размер(cx, cy), color);
}

void SetNormalizedHotSpots(ImageBuffer& ib, int x1, int y1, int x2, int y2)
{
	Прям r(x1, y1, x2, y2);
	r.нормализуй();
	ib.SetHotSpot(r.верхЛево());
	ib.Set2ndSpot(r.низПраво());
}

Рисунок  RotateClockwise(const Рисунок& img)
{
	Размер sz = img.дайРазм();
	ImageBuffer ib(sz.cy, sz.cx);
	for(int x = 0; x < sz.cx; x++)
		for(int y = 0; y < sz.cy; y++)
			ib[x][y] = img[sz.cy - y - 1][x];
	Точка p1 = img.GetHotSpot();
	Точка p2 = img.Get2ndSpot();
	SetNormalizedHotSpots(ib, sz.cy - p1.y - 1, p1.x, sz.cy - p2.y - 1, p2.x);
	ib.SetResolution(img.GetResolution());
	return ib;
}

Рисунок  RotateAntiClockwise(const Рисунок& img)
{
	Размер sz = img.дайРазм();
	ImageBuffer ib(sz.cy, sz.cx);
	for(int x = 0; x < sz.cx; x++)
		for(int y = 0; y < sz.cy; y++)
			ib[x][y] = img[y][sz.cx - x - 1];
	
	Точка p1 = img.GetHotSpot();
	Точка p2 = img.Get2ndSpot();
	SetNormalizedHotSpots(ib, p1.y, sz.cx - p1.x - 1, p2.y, sz.cx - p2.x - 1);
	ib.SetResolution(img.GetResolution());
	return ib;
}

Рисунок Rotate180(const Рисунок& orig)
{
	Размер sz = orig.дайРазм();
	ImageBuffer dest(sz);
	for(int rw = 0; rw < sz.cy; rw++)
		for(int cl = 0; cl < sz.cx; cl++)
			dest[rw][cl] = orig[sz.cy - rw - 1][sz.cx - cl - 1];
	Точка p1 = orig.GetHotSpot();
	Точка p2 = orig.Get2ndSpot();
	SetNormalizedHotSpots(dest, sz.cy - p1.y - 1, sz.cx - p1.x - 1, sz.cy - p2.y - 1, sz.cx - p2.x - 1);
	dest.SetResolution(dest.GetResolution());
	return dest;
}

Рисунок Transpose(const Рисунок& img)
{
	Размер sz = img.дайРазм();
	ImageBuffer ib(sz.cy, sz.cx);
	for(int x = 0; x < sz.cx; x++)
		for(int y = 0; y < sz.cy; y++)
			ib[x][y] = img[y][x];
	Точка p1 = img.GetHotSpot();
	Точка p2 = img.Get2ndSpot();
	SetNormalizedHotSpots(ib, p1.y, p1.x, p2.y, p2.x);
	ib.SetResolution(img.GetResolution());
	return ib;
}

Рисунок Transverse(const Рисунок& img)
{
	Размер sz = img.дайРазм();
	ImageBuffer ib(sz.cy, sz.cx);
	for(int x = 0; x < sz.cx; x++)
		for(int y = 0; y < sz.cy; y++)
			ib[x][y] = img[sz.cy - y - 1][sz.cx - x - 1];
	Точка p1 = img.GetHotSpot();
	Точка p2 = img.Get2ndSpot();
	SetNormalizedHotSpots(ib, sz.cy - p1.y - 1, sz.cx - p1.x - 1, sz.cy - p2.y - 1, sz.cx - p2.x - 1);
	ib.SetResolution(img.GetResolution());
	return ib;
}

Рисунок MirrorHorz(const Рисунок& img)
{
	Размер sz = img.дайРазм();
	Рисунок h = img;
	ImageBuffer ib(h);
	for(int y = 0; y < sz.cy; y++) {
		КЗСА *b = ib[y] + 0;
		КЗСА *e = b + sz.cx - 1;
		while(b < e) {
			разверни(*b, *e);
			b++;
			e--;
		}
	}
	Точка p1 = img.GetHotSpot();
	Точка p2 = img.Get2ndSpot();
	SetNormalizedHotSpots(ib, sz.cx - p1.x - 1, p1.y, sz.cx - p2.x - 1, p2.y);
	ib.SetResolution(img.GetResolution());
	return ib;
}

Рисунок MirrorVert(const Рисунок& img)
{
	Размер sz = img.дайРазм();
	Рисунок h = img;
	ImageBuffer ib(h);

	for(int y = 0; y < sz.cy / 2; y++) {
		КЗСА *b = ib[y];
		КЗСА *e = ib[sz.cy - y - 1];
		for(int x = 0; x < sz.cx; x++) {
			разверни(*b, *e);
			b++;
			e++;
		}
	}
	Точка p1 = img.GetHotSpot();
	Точка p2 = img.Get2ndSpot();
	SetNormalizedHotSpots(ib, p1.x, sz.cy - p1.y - 1, p2.x, sz.cy - p2.y - 1);
	ib.SetResolution(img.GetResolution());
	return ib;
}

Рисунок FlipImage(const Рисунок& m, int mode)
{
	return decode(mode,
	              FLIP_MIRROR_HORZ, MirrorHorz(m),
	              FLIP_ROTATE_180, Rotate180(m),
	              FLIP_MIRROR_VERT, MirrorVert(m),
	              FLIP_TRANSPOSE, Transpose(m),
	              FLIP_ROTATE_CLOCKWISE, RotateClockwise(m),
	              FLIP_TRANSVERSE, Transverse(m),
	              FLIP_ROTATE_ANTICLOCKWISE, RotateAntiClockwise(m),
	              m);
}

static ТочкаПЗ Cvp(double x, double y, double sina, double cosa)
{
	return ТочкаПЗ(x * cosa + y * sina, -x * sina + y * cosa);
}

Рисунок Rotate(const Рисунок& m, int angle)
{
	Размер isz = m.дайРазм();
	ТочкаПЗ centerf = ТочкаПЗ(Точка(isz)) / 2.0;
	double sina, cosa;
	Draw::SinCos(-angle, sina, cosa);
	ТочкаПЗ p1 = Cvp(-centerf.x, -centerf.y, sina, cosa);
	ТочкаПЗ p2 = Cvp(centerf.x, -centerf.y, sina, cosa);
	Размер sz2 = Размер(2 * (int)max(tabs(p1.x), tabs(p2.x)),
	                2 * (int)max(tabs(p1.y), tabs(p2.y)));
	ImageBuffer ib(sz2);
	Fill(~ib, обнулиКЗСА(), ib.дайДлину());
	КЗСА *t = ~ib;
	Draw::SinCos(angle, sina, cosa);
	int sini = int(sina * 128);
	int cosi = int(cosa * 128);
	Буфер<int> xmx(sz2.cx);
	Буфер<int> xmy(sz2.cx);
	for(int x = 0; x < sz2.cx; x++) {
		int xx = x + x - sz2.cx;
		xmx[x] = int(xx * cosi);
		xmy[x] = -int(xx * sini);
	}
	for(int y = 0; y < sz2.cy; y++) {
		int yy = y + y - sz2.cy;
		int ymx = int(yy * sini) + (isz.cx << 7);
		int ymy = int(yy * cosi) + (isz.cy << 7);
		for(int x = 0; x < sz2.cx; x++) {
			int xs = (xmx[x] + ymx) >> 8;
			int ys = (xmy[x] + ymy) >> 8;
			*t++ = xs >= 0 && xs < isz.cx && ys >= 0 && ys < isz.cy ? m[ys][xs] : обнулиКЗСА();
		}
	}
	ib.SetResolution(m.GetResolution());
	return ib;
}

Рисунок Dither(const Рисунок& m, int dival)
{
	static byte dither[8][8]= {
		{ 1, 49, 13, 61, 4, 52, 16, 64 },
        { 33, 17, 45, 29, 36, 20, 48, 32 },
        { 9, 57, 5, 53, 12, 60, 8, 56 },
        { 41, 25, 37, 21, 44, 28, 40, 24 },
        { 3, 51, 15, 63, 2, 50, 14, 62 },
        { 35, 19, 47, 31, 34, 18, 46, 30 },
        { 11, 59, 7, 55, 10, 58, 6, 54 },
        { 43, 27, 39, 23, 42, 26, 38, 22 },
	};
	
	Размер isz = m.дайРазм();
	ImageBuffer ib(m.дайРазм());
	const КЗСА *s = ~m;
	КЗСА *t = ~ib;
	for(int y = 0; y < isz.cy; y++)
		for(int x = 0; x < isz.cx; x++) {
			int g = Grayscale(*s++) * 100 / dival;
			*t++ = g > dither[y & 7][x & 7] ? белый() : чёрный();
		}
	ib.SetResolution(m.GetResolution());
	return ib;
}

Рисунок GaussianBlur(const Рисунок& img, int radius, bool co)
{
	// This code is adapted from Ivan Kutskir's fast blur implementation, published under MIT license.
	// See: http://blog.ivank.net/fastest-gaussian-blur.html
	
	// An implementation of well known fast box and gaussian blur
	// approximation algorithms by Wojciech Jarosz and Peter Kovesi.
	// See: https://elynxsdk.free.fr/ext-docs/Blur/Fast_box_blur.pdf
	// See: https://www.peterkovesi.com/papers/FastGaussianSmoothing.pdf
	
	auto ApplyBoxBlur = [&](const Рисунок& src, int r) -> Рисунок
	{
		double avg = 1.0 / (r + r + 1);
	
		Размер sz = src.дайРазм();
	
		ImageBuffer tmp(sz);
		ImageBuffer out(sz);

		auto DoLine = [&](int i) {
			int ti = 0;
			int li = ti;
			int ri = r;
			const КЗСА& fv = src[i][0];
			const КЗСА& lv = src[i][sz.cx - 1];
			dword rsum = fv.r * (r + 1);
			dword gsum = fv.g * (r + 1);
			dword bsum = fv.b * (r + 1);
			dword asum = fv.a * (r + 1);
			for(int j = 0; j < r; j++) {
				const КЗСА& p = src[i][j];
				rsum += p.r;
				gsum += p.g;
				bsum += p.b;
				asum += p.a;
			}
			for(int j = 0; j <= r; j++) {
				const КЗСА& p = src[i][ri++];
				КЗСА& q       = tmp[i][ti++];
				q.r = byte((rsum += p.r - fv.r) * avg);
				q.g = byte((gsum += p.g - fv.g) * avg);
				q.b = byte((bsum += p.b - fv.b) * avg);
				q.a = byte((asum += p.a - fv.a) * avg);
			}
			for(int j = r + 1; j < sz.cx - r; j++) {
				const КЗСА& p = src[i][ri++];
				const КЗСА& q = src[i][li++];
				КЗСА& t       = tmp[i][ti++];
				t.r = byte((rsum += p.r - q.r) * avg);
				t.g = byte((gsum += p.g - q.g) * avg);
				t.b = byte((bsum += p.b - q.b) * avg);
				t.a = byte((asum += p.a - q.a) * avg);
			}
			for(int j = sz.cx - r; j < sz.cx ; j++) {
				const КЗСА& p = src[i][li++];
				КЗСА& q       = tmp[i][ti++];
				q.r = byte((rsum += lv.r - p.r) * avg);
				q.g = byte((gsum += lv.g - p.g) * avg);
				q.b = byte((bsum += lv.b - p.b) * avg);
				q.a = byte((asum += lv.a - p.a) * avg);
			}
		};


		if(co)
			соФор(sz.cy, [&](int i) { DoLine(i); });
		else
			for(int i = 0; i < sz.cy; i++)
				DoLine(i);

		auto DoColumn = [&](int i) {
			int ti = 0;
			int li = ti;
			int ri = r;
			const КЗСА& fv = tmp[ti][i];
			const КЗСА& lv = tmp[sz.cy - 1][i];
			dword rsum = fv.r * (r + 1);
			dword gsum = fv.g * (r + 1);
			dword bsum = fv.b * (r + 1);
			dword asum = fv.a * (r + 1);
			for(int j = 0; j < r; j++) {
				const КЗСА& p = tmp[j][i];
				rsum += p.r;
				gsum += p.g;
				bsum += p.b;
				asum += p.a;
			}
			for(int j = 0; j <= r; j++) {
				const КЗСА& p = tmp[ri++][i];
				КЗСА& q       = out[ti++][i];
				q.r = byte((rsum += p.r - fv.r) * avg);
				q.g = byte((gsum += p.g - fv.g) * avg);
				q.b = byte((bsum += p.b - fv.b) * avg);
				q.a = byte((asum += p.a - fv.a) * avg);
			}
			for(int j = r + 1; j < sz.cy - r; j++) {
				const КЗСА& p = tmp[ri++][i];
				const КЗСА& q = tmp[li++][i];
				КЗСА&       t = out[ti++][i];
				t.r = byte((rsum += p.r - q.r) * avg);
				t.g = byte((gsum += p.g - q.g) * avg);
				t.b = byte((bsum += p.b - q.b) * avg);
				t.a = byte((asum += p.a - q.a) * avg);
			}
			for(int j = sz.cy - r; j < sz.cy; j++) {
				const КЗСА& p = tmp[li++][i];
				КЗСА& q       = out[ti++][i];
				q.r = byte((rsum += lv.r - p.r) * avg);
				q.g = byte((gsum += lv.g - p.g) * avg);
				q.b = byte((bsum += lv.b - p.b) * avg);
				q.a = byte((asum += lv.a - p.a) * avg);
			}
		};

		if(co) {
			std::atomic<int> ii(0);
			соДелай([&] {
				for(int i = 16 * ii++; i < sz.cx; i = 16 * ii++) { // go by cacheline
					int e = min(i + 16, sz.cx);
					for(int j = i; j < e; j++)
						DoColumn(j);
				}
			});
		}
		else
			for(int i = 0; i < sz.cx; i++)
				DoColumn(i);

		out.SetHotSpots(src);
		out.SetResolution(src.GetResolution());
		return out;
	};

	if(radius < 1 || пусто_ли(img))
		return img;
	
	double wl = ffloor(sqrt((12 * sqr(radius) / 3) + 1));
	if(fmod(wl, 2) == 0) wl--;
	double wu = wl + 2;
	double m = fround((12 * sqr(radius) - 3 * sqr(wl) - 4 * 3 * wl - 3 * 3) / (-4 * wl - 4));
	Вектор<int> sizes;
	Рисунок t = img;
	for (int i = 0; i < 3; i++)
		t = ApplyBoxBlur(t, int(((i < m ? wl : wu) - 1) / 2));
	return pick(t);
}

}
