#include "Painter.h"

namespace РНЦП {

struct GlyphPainter : NilPainter, LinearPathConsumer {
	Вектор<float>       glyph;
	double              tolerance;
	ТочкаПЗ              pos, move;
	
	virtual void линияОп(const ТочкаПЗ& p, bool);
	virtual void двигОп(const ТочкаПЗ& p, bool);
	virtual void квадрОп(const ТочкаПЗ& p1, const ТочкаПЗ& p, bool);
	virtual void кубОп(const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p, bool);
	virtual void закройОп();

	virtual void Строка(const ТочкаПЗ& p);
	virtual void Move(const ТочкаПЗ& p);
};

void GlyphPainter::Move(const ТочкаПЗ& p)
{
	glyph.добавь((float)1e31);
	Строка(p);
	move = pos;
}

void GlyphPainter::Строка(const ТочкаПЗ& p)
{
	glyph.добавь((float)p.x);
	glyph.добавь((float)p.y);
	pos = p;
}

void GlyphPainter::двигОп(const ТочкаПЗ& p, bool)
{
	Move(p);
}

void GlyphPainter::линияОп(const ТочкаПЗ& p, bool)
{
	Строка(p);
}

void GlyphPainter::квадрОп(const ТочкаПЗ& p1, const ТочкаПЗ& p, bool)
{
	ApproximateQuadratic(*this, pos, p1, p, tolerance);
	pos = p;
}

void GlyphPainter::кубОп(const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p, bool)
{
	ApproximateCubic(*this, pos, p1, p2, p, tolerance);
	pos = p;
}

void GlyphPainter::закройОп()
{
	if(move != pos && !пусто_ли(move))
		Строка(move);
}

struct GlyphKey {
	Шрифт   fnt;
	int    chr;
	double tolerance;
	
	bool operator==(const GlyphKey& b) const {
		return fnt == b.fnt && chr == b.chr && tolerance == b.tolerance;
	}
	hash_t дайХэшЗнач() const {
		return комбинируйХэш(fnt, chr, tolerance);
	}
};

struct sMakeGlyph : LRUCache<Значение, GlyphKey>::Делец {
	GlyphKey gk;

	GlyphKey Ключ() const     { return gk; }
	int      сделай(Значение& v) const {
		GlyphPainter gp;
		gp.move = gp.pos = Null;
		gp.tolerance = gk.tolerance;
		рисуйСимвол(gp, ТочкаПЗ(0, 0), gk.chr, gk.fnt);
		int sz = gp.glyph.дайСчёт() * 4;
		v = RawPickToValue(pick(gp.glyph));
		return sz;
	}
};

void ApproximateChar(LinearPathConsumer& t, ТочкаПЗ at, int ch, Шрифт fnt, double tolerance)
{
	PAINTER_TIMING("ApproximateChar");
	Значение v;
	INTERLOCKED {
		PAINTER_TIMING("ApproximateChar::Fetch");
		static LRUCache<Значение, GlyphKey> cache;
		cache.сожми(500000);
		sMakeGlyph h;
		h.gk.fnt = fnt;
		h.gk.chr = ch;
		h.gk.tolerance = tolerance;
		v = cache.дай(h);
#ifdef _DEBUG0
		DLOG("==== ApproximateChar " << ch << " " << (char)ch << " " << fnt << ", tolerance: " << tolerance);
		DDUMP(ValueTo< Вектор<float> >(v));
		GlyphPainter chp;
		chp.move = chp.pos = Null;
		chp.tolerance = tolerance;
		рисуйСимвол(chp, ТочкаПЗ(0, 0), ch, fnt);
		DDUMP(chp.glyph);
		ПРОВЕРЬ(ValueTo< Вектор<float> >(v) == chp.glyph);
#endif
	}
	const Вектор<float>& g = ValueTo< Вектор<float> >(v);
	int i = 0;
	while(i < g.дайСчёт()) {
		ТочкаПЗ p;
		p.x = g[i++];
		if(p.x > 1e30) {
			p.x = g[i++];
			p.y = g[i++];
			t.Move(p + at);
		}
		else {
			PAINTER_TIMING("ApproximateChar::Строка");
			p.y = g[i++];
			t.Строка(p + at);
		}
	}
}

}
