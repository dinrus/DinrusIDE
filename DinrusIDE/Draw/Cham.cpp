#include "Draw.h"

#if 0

_DBG_
#include <plugin/png/png.h>

namespace РНЦП {
inline void LOGPNG(const char *имя, const Рисунок& m)
{
	PNGEncoder().сохраниФайл(дайФайлИзДомПапки(имя) + ".png", m);
}
};

#else

#define LOGPNG(a, b)

#endif

namespace РНЦП {

#define LTIMING(x) //  RTIMING(x)

void  ChDraw(Draw& w, int x, int y, int cx, int cy, const Рисунок& img, const Прям& src, Цвет ink)
{
	LTIMING("ChDraw");
	if(cx > 0 && cy > 0) {
		Рисунок m = CachedRescale(img, Размер(cx, cy), src);
		if(пусто_ли(ink))
			w.DrawImage(x, y, m);
		else
			w.DrawImage(x, y, m, ink);
	}
}

void  ChDraw(Draw& w, int x, int y, const Рисунок& img, const Прям& src, Цвет ink)
{
	ChDraw(w, x, y, src.дайШирину(), src.дайВысоту(), img, src, ink);
}

void  ChDraw(Draw& w, const Прям& r, const Рисунок& img, const Прям& src, Цвет ink)
{
	ChDraw(w, r.left, r.top, r.дайШирину(), r.дайВысоту(), img, src, ink);
}

struct sChLookWith {
	Значение look;
	Рисунок img;
	Цвет (*colorfn)(int i);
	int   ii;
	Цвет color;
	Точка offset;
};

Значение ChLookWith(const Значение& look, const Рисунок& img, Точка offset)
{
	sChLookWith x;
	x.look = look;
	x.img = img;
	x.colorfn = NULL;
	x.color = Null;
	x.offset = offset;
	return RawToValue(x);
}

Значение ChLookWith(const Значение& look, const Рисунок& img, Цвет color, Точка offset)
{
	sChLookWith x;
	x.look = look;
	x.img = img;
	x.colorfn = NULL;
	x.color = color;
	x.offset = offset;
	return RawToValue(x);
}

Значение ChLookWith(const Значение& look, const Рисунок& img, Цвет (*color)(int i), int i, Точка offset)
{
	sChLookWith x;
	x.look = look;
	x.img = img;
	x.colorfn = color;
	x.ii = i;
	x.offset = offset;
	return RawToValue(x);
}

void ChLookWith(Значение *look, const Рисунок& image, const Цвет *color, int n)
{
	for(int i = 0; i < n; i++)
		look[i] = ChLookWith(look[i], image, color[i]);
}

Значение sChOp(Draw& w, const Прям& r, const Значение& v, int op, Цвет ink = Null);

struct sChBorder {
	const ColorF *border;
	Значение face;
};

Значение ChBorder(const ColorF *colors, const Значение& face)
{ // adds border to face значение
	sChBorder b;
	b.border = colors;
	b.face = face;
	return RawToValue(b);
}

static void sDrawScrollbarThumb(Draw& w, int x, int y, int cx, int cy, const Рисунок& m, bool vert)
{
	if(cx <= 0)
		return;
	Размер isz = m.дайРазм();
	if(vert) {
		разверни(isz.cx, isz.cy);
		разверни(cx, cy);
	}
	int ecx = isz.cy / 2; // size of 'ending' (can be circular)
	int ccx = isz.cy / 2; // size of central part, there is sometimes some sort of 'handle'
	int sl = (isz.cx - 2 * ecx - ccx) / 2; // size of left stretched part source
	int sr = isz.cx - 2 * ecx - ccx - sl; // size of right stretched part source

	int tel = ecx; // left ending target size
	int l = (cx - 2 * ecx - ccx) / 2; // size of left stretched part
	int tccx = ccx; // target center size
	int r = cx - 2 * ecx - ccx - l; // size of right stretched part
	int ter = ecx; // size of right stretched part

	if(l < 0 || r < 0) { // if not enough space, remove left stretch and center
		l = 0;
		tccx = 0;
		r = cx - 2 * ecx - tccx - l;
	}
	
	if(r < 0) { // still not enough space, need to resize endings too
		r = 0;
		tel = cx / 2;
		ter = cx = tel;
	}

	int sx = 0;
	auto DrawPart = [&](int tcx, int scx) {
		if(scx) {
			if(vert) {
				w.DrawImage(x, y, CachedRescale(m, Размер(cy, tcx), RectC(0, sx, isz.cy, scx)));
				y += tcx;
			}
			else {
				w.DrawImage(x, y, CachedRescale(m, Размер(tcx, cy), RectC(sx, 0, scx, isz.cy)));
				x += tcx;
			}
		}
		sx += scx;
	};
	DrawPart(tel, ecx); // left ending
	DrawPart(l, sl); // left stretch
	DrawPart(tccx, ccx); // central part
	DrawPart(r, sr); // right stretch
	DrawPart(ter, ecx); // right ending
}

Значение StdChLookFn(Draw& w, const Прям& r, const Значение& v, int op, Цвет ink)
{
	if(IsType<sChLookWith>(v)) {
		const sChLookWith& x = ValueTo<sChLookWith>(v);
		if(op == LOOK_PAINT) {
			LOGPNG(какТкст(x.img.GetSerialId()), x.img);
			ChPaint(w, r, x.look);
			Точка p = r.позЦентра(x.img.дайРазм()) + x.offset;
			if(x.colorfn)
				w.DrawImage(p.x, p.y, x.img, (*x.colorfn)(x.ii));
			else
			if(!пусто_ли(x.color))
				w.DrawImage(p.x, p.y, x.img, x.color);
			else
				w.DrawImage(p.x, p.y, x.img);
			return 1;
		}
		return sChOp(w, r, x.look, op);
	}
	if(IsType<sChBorder>(v)) {
		sChBorder b = ValueTo<sChBorder>(v);
		int n = (int)(intptr_t)*b.border;
		switch(op) {
		case LOOK_PAINT:
			ChPaint(w, r.дефлят(n), b.face);
			// fall through - need to paint border now
		case LOOK_PAINTEDGE:
			DrawBorder(w, r, b.border);
			return 0;
		case LOOK_MARGINS:
			return Прям(n, n, n, n);
		}
	}
	if(IsType<Цвет>(v)) {
		Цвет c = v;
		switch(op) {
		case LOOK_PAINT:
			w.DrawRect(r, c);
			return 0;
		case LOOK_PAINTEDGE:
			DrawFrame(w, r, c);
			return 0;
		case LOOK_MARGINS:
			return Прям(1, 1, 1, 1);
		}
	}
	if(IsType<Рисунок>(v)) {
		Рисунок img = v;
		Размер isz = img.дайРазм();
		Размер sz = r.дайРазм();
		Точка p = img.GetHotSpot();
		if(p.x == CH_SCROLLBAR_IMAGE) {
			if(op == LOOK_MARGINS)
				return Прям(0, 0, 0, 0);
			if(op == LOOK_PAINT)
				sDrawScrollbarThumb(w, r.left, r.top, r.дайШирину(), r.дайВысоту(), img, isz.cx < isz.cy);
			return true;
		}
		Точка p2 = img.Get2ndSpot();
		int tile = 0;
		if(p2.x == CH_EDITFIELD_IMAGE) {
			if(op != LOOK_MARGINS)
				p.x = p.y = p2.y;
			p2.x = p2.y = 0;
		}
		if(p2.x || p2.y) {
			if(p2.x < p.x) {
				разверни(p2.x, p.x);
				tile = 1;
			}
			if(p2.y < p.y) {
				разверни(p2.y, p.y);
				tile += 2;
			}
			p2.x++;
			p2.y++;
		}
		else {
			p2.x = isz.cx - p.x;
			p2.y = isz.cy - p.y;
			if(p.x > isz.cx / 2) {
				tile = 1;
				p2.x = p.x;
				p.x = isz.cx - p.x - 1;
			}
			if(p.y > isz.cy / 2) {
				tile += 2;
				p2.y = p.y;
				p.y = isz.cy - p.y - 1;
			}
		}
		if(op == LOOK_MARGINS)
			return Прям(p.x, p.y, isz.cx - p2.x, isz.cy - p2.y);
		if(пусто_ли(img))
			return 1;
		LTIMING("ChPaint Рисунок");
		w.Clipoff(r);
		Прям sr(p, p2);
		Размер sz2(isz.cx - sr.right, isz.cy - sr.bottom);
		Прям r = RectC(p.x, p.y, sz.cx - sr.left - sz2.cx, sz.cy - sr.top - sz2.cy);
		int top = minmax(sz.cy / 2, 0, isz.cy);
		int bottom = minmax(sz.cy - top, 0, isz.cy);
		int yy = isz.cy - bottom;
		int left = minmax(sz.cx / 2, 0, isz.cx);
		int right = minmax(sz.cx - left, 0, isz.cx);
		int xx = isz.cx - right;
		if(!r.пустой()) {
			ChDraw(w, 0, 0, img, RectC(0, 0, p.x, p.y), ink);
			ChDraw(w, 0, r.bottom, img, RectC(0, sr.bottom, p.x, sz2.cy), ink);
			ChDraw(w, r.right, 0, img, RectC(sr.right, 0, sz2.cx, p.y), ink);
			ChDraw(w, r.right, r.bottom, img, RectC(sr.right, sr.bottom, sz2.cx, sz2.cy), ink);
			ChDraw(w, p.x, 0, r.устШирину(), p.y, img, RectC(p.x, 0, sr.устШирину(), p.y), ink);
			ChDraw(w, p.x, r.bottom, r.устШирину(), sz2.cy, img, RectC(p.x, sr.bottom, sr.устШирину(), sz2.cy), ink);
			ChDraw(w, 0, p.y, p.x, r.устВысоту(), img, RectC(0, p.y, p.x, sr.устВысоту()), ink);
			ChDraw(w, r.right, p.y, sz2.cx, r.устВысоту(), img, RectC(sr.right, p.y, sz2.cx, sr.устВысоту()), ink);
			if(op == LOOK_PAINT) {
				if(пусто_ли(r) || пусто_ли(sr)) {
					w.стоп();
					return 1;
				}
				if(tile) {
					LTIMING("Ch-Tiles");
					LTIMING("Ch-Tiles");
					Размер sz;
					sz.cx = (tile & 1 ? sr : r).дайШирину();
					sz.cy = (tile & 2 ? sr : r).дайВысоту();
					img = Rescale(img, sz, sr);
					DrawTiles(w, r, img);
				}
				else {
					static ВекторМап<int64, bool> single_color_body_cache;
					int64 ключ = img.GetSerialId();
					bool single_color_body;
					int q = single_color_body_cache.найди(ключ);
					if(q < 0) {
						LTIMING("ClassifyContent");
						single_color_body = IsSingleColor(img, sr);
						if(single_color_body_cache.дайСчёт() > 1000)
							single_color_body_cache.очисть();
						single_color_body_cache.добавь(ключ, single_color_body);
					}
					else
						single_color_body = single_color_body_cache[q];
					КЗСА c = img[sr.top][sr.left];
					if(single_color_body && c.a == 255) {
						LTIMING("Ch-singlecolor");
						w.DrawRect(r, c);
					}
					else
						ChDraw(w, r, img, sr, ink);
				}
			}
		}
		else
		if(r.left < r.right) {
			ChDraw(w, 0, 0, img, RectC(0, 0, p.x, top), ink);
			ChDraw(w, 0, sz.cy - bottom, img, RectC(0, yy, p.x, bottom), ink);
			ChDraw(w, r.right, 0, img, RectC(sr.right, 0, sz2.cx, top), ink);
			ChDraw(w, r.right, sz.cy - bottom, img, RectC(sr.right, yy, sz2.cx, bottom), ink);
			ChDraw(w, p.x, 0, r.устШирину(), top, img, RectC(p.x, 0, sr.устШирину(), top), ink);
			ChDraw(w, p.x, sz.cy - bottom, r.устШирину(), bottom, img, RectC(p.x, yy, sr.устШирину(), bottom), ink);
		}
		else
		if(r.top < r.bottom) {
			ChDraw(w, 0, 0, img, RectC(0, 0, left, p.y), ink);
			ChDraw(w, 0, r.bottom, img, RectC(0, sr.bottom, left, sz2.cy), ink);
			ChDraw(w, sz.cx - right, 0, img, RectC(xx, 0, right, p.y), ink);
			ChDraw(w, sz.cx - right, r.bottom, img, RectC(xx, sr.bottom, right, sz2.cy), ink);
			ChDraw(w, 0, p.y, left, r.устВысоту(), img, RectC(0, p.y, left, sr.устВысоту()), ink);
			ChDraw(w, sz.cx - right, p.y, right, r.устВысоту(), img, RectC(xx, p.y, right, sr.устВысоту()), ink);
		}
		else {
			ChDraw(w, 0, 0, img, RectC(0, 0, left, top), ink);
			ChDraw(w, 0, sz.cy - bottom, img, RectC(0, yy, left, top), ink);
			ChDraw(w, sz.cx - right, 0, img, RectC(xx, 0, right, top), ink);
			ChDraw(w, sz.cx - right, sz.cy - bottom, img, RectC(xx, yy, right, bottom), ink);
		}
		w.стоп();
		return 1;
	}
	return Null;
}

typedef Значение (*ChPainterFn)(Draw& w, const Прям& r, const Значение& v, int op, Цвет ink);

Вектор<ChPainterFn>& sChps()
{
	static Вектор<ChPainterFn> x;
	return x;
}

void ChLookFn(Значение (*фн)(Draw& w, const Прям& r, const Значение& v, int op, Цвет ink))
{
	if(найдиИндекс(sChps(), фн) < 0)
		sChps().добавь(фн);
}

struct sStyleCh : Движимое<sStyleCh> {
	byte *status;
	void (*init)();
};

Вектор<sStyleCh>& sChStyle()
{
	static Вектор<sStyleCh> x;
	return x;
}

void ChRegisterStyle__(byte& state, byte& registered, void (*init)())
{
	if(!registered) {
		sStyleCh& s = sChStyle().добавь();
		s.status = &state;
		s.init = init;
		registered = true;
	}
}

static bool sChInvalid = true;

void ChInvalidate()
{
	sChInvalid = true;
}

bool ChIsInvalidated()
{
	return sChInvalid;
}

static bool sLabelTextColorMismatch;

bool IsLabelTextColorMismatch()
{
	return sLabelTextColorMismatch;
}

static bool sIsDarkColorFace;

bool IsDarkColorFace()
{
	return sIsDarkColorFace;
}

void ChReset()
{
	dark_theme__ = false;
	for(int i = 0; i < sChStyle().дайСчёт(); i++)
		*sChStyle()[i].status = 0;
	ChLookFn(StdChLookFn);
}

void ChFinish()
{
	sChInvalid = false;
	for(int i = 0; i < sChStyle().дайСчёт(); i++)
		sChStyle()[i].init();
	sIsDarkColorFace = тёмен(SColorFace());
	sLabelTextColorMismatch = тёмен(SColorText()) != тёмен(SColorLabel());
}

Значение sChOp(Draw& w, const Прям& r, const Значение& v, int op, Цвет ink)
{
	if(r.right < r.left || r.bottom < r.top)
		return Прям(0, 0, 0, 0);
	Значение q;
	if(!пусто_ли(v))
		for(int i = sChps().дайСчёт() - 1; i >= 0; i--) {
			q = (*sChps()[i])(w, r, v, op, ink);
			if(!пусто_ли(q))
				break;
		}
	return q;
}

void ChPaint(Draw& w, const Прям& r, const Значение& look, Цвет ink)
{
	sChOp(w, r, look, LOOK_PAINT, ink);
}

void ChPaint(Draw& w, int x, int y, int cx, int cy, const Значение& look, Цвет ink)
{
	sChOp(w, RectC(x, y, cx, cy), look, LOOK_PAINT, ink);
}

void ChPaintEdge(Draw& w, const Прям& r, const Значение& look, Цвет ink)
{
	sChOp(w, r, look, LOOK_PAINTEDGE, ink);
}

void ChPaintEdge(Draw& w, int x, int y, int cx, int cy, const Значение& look, Цвет ink)
{
	sChOp(w, RectC(x, y, cx, cy), look, LOOK_PAINTEDGE, ink);
}

void ChPaintBody(Draw& w, const Прям& r, const Значение& look, Цвет ink)
{
	Прям m = ChMargins(look);
	w.Clip(r);
	ChPaint(w, Прям(r.left - m.left, r.top - m.top, r.right + m.right, r.bottom + m.bottom),
	        look, ink);
	w.стоп();
}

void ChPaintBody(Draw& w, int x, int y, int cx, int cy, const Значение& look, Цвет ink)
{
	ChPaintBody(w, RectC(x, y, cx, cy), look, ink);
}

Прям ChMargins(const Значение& look)
{
	NilDraw w;
	return sChOp(w, Null, look, LOOK_MARGINS);
}

void   DeflateMargins(Прям& r, const Прям& m)
{
	r = Прям(r.left + m.left, r.top + m.top, r.right - m.right, r.bottom - m.bottom);
}

void   ChDeflateMargins(Прям& r, const Значение& look)
{
	return DeflateMargins(r, ChMargins(look));
}

void DeflateMargins(Размер& sz, const Прям& m)
{
	sz = Размер(sz.cx + m.left + m.right, sz.cy + m.top + m.bottom);
}

void ChDeflateMargins(Размер& sz, const Значение& look)
{
	DeflateMargins(sz, ChMargins(look));
}

void   InflateMargins(Прям& r, const Прям& m)
{
	r = Прям(r.left - m.left, r.top - m.top, r.right + m.right, r.bottom + m.bottom);
}

void   ChInflateMargins(Прям& r, const Значение& look)
{
	return InflateMargins(r, ChMargins(look));
}

void InflateMargins(Размер& sz, const Прям& m)
{
	sz = Размер(sz.cx + m.left + m.right, sz.cy + m.top + m.bottom);
}

void ChInflateMargins(Размер& sz, const Значение& look)
{
	InflateMargins(sz, ChMargins(look));
}

Рисунок AdjustColors(const Рисунок& simg) {
	Рисунок h = simg;
	ImageBuffer img(h);
	Unmultiply(img);
	const КЗСА *s = ~img;
	const КЗСА *e = s + img.дайДлину();
	ImageBuffer w(img.дайРазм());
	КЗСА *t = w;
	Цвет black = SColorText();
	Цвет shadow = SColorShadow();
	Цвет face = SColorFace();
	Цвет light = SColorLight();
	Цвет highlight = SColorHighlight();
	Цвет hot = highlight;
	Цвет paper = SColorPaper();
	while(s < e) {
		*t = *s;
		if(s->r == s->g && s->g == s->b)
			if(s->r < 128)
				*t = смешай(black, shadow, 2 * s->r);
			else
			if(s->r >= 128 && s->r < 192)
				*t = смешай(shadow, face, 4 * (s->r - 128));
			else
				*t = смешай(face, light, 4 * (s->r - 192));
		else
		if(s->r == 0 && s->g == 0)
			*t = смешай(black, highlight, 2 * (s->b - 128));
		else
		if(s->b == 0 && s->g == 0)
			*t = смешай(black, hot, 2 * (s->r - 128));
		else
		if(s->r == s->g && s->b == 0)
			*t = смешай(black, paper, s->r);
		t->a = s->a;
		t++;
		s++;
	}
	w.SetHotSpot(img.GetHotSpot());
	w.Set2ndSpot(img.Get2ndSpot());
	Premultiply(w);
	return w;
}

void  Override(Iml& target, Iml& source, bool colored)
{
	for(int i = 0; i < target.дайСчёт(); i++) {
		int q = source.найди(target.дайИд(i));
		if(q >= 0) {
			Рисунок m = source.дай(q);
			if(colored)
				m = AdjustColors(m);
			target.уст(i, m);
		}
	}
}

void ColoredOverride(Iml& target, Iml& source)
{
	Override(target, source, true);
}

}
