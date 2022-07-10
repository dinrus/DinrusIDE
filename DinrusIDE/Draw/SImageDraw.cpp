#include "Draw.h"

namespace РНЦП {

void SImageDraw1::PutImage(Точка p, const Рисунок& m, const Прям& src)
{
	Over(ib, p, m, src);
}

void SImageDraw1::PutRect(const Прям& r, Цвет color)
{
	РНЦП::Fill(ib, r, color);
}

void SImageDraw1::создай(Размер sz)
{
	ib.создай(sz);
	иниц(sz);
}

Draw& SImageDraw::Alpha()
{
	if(!has_alpha) {
		Размер sz = ib.дайРазм();
		alpha.создай(sz);
		alpha.DrawRect(sz, серыйЦвет(0));
		has_alpha = true;
	}
	return alpha;
}

SImageDraw::operator Рисунок() const
{
	ImageBuffer b(ib.дайРазм());
	memcpy(b, ib.старт(), sizeof(КЗСА) * ib.дайДлину());
	КЗСА *t = b;
	const КЗСА *e = b.стоп();;
	if(has_alpha) {
		const КЗСА *s = alpha.ib.старт();
		while(t < e) {
			t->a = s->r;
			t++;
			s++;
		}
		Premultiply(b);
		b.SetKind(IMAGE_ALPHA);
	}
	else {
		while(t < e) {
			t->a = 255;
			t++;
		}
		b.SetKind(IMAGE_OPAQUE);
	}
	return b;
}

SImageDraw::SImageDraw(Размер sz)
{
	has_alpha = false;
	SImageDraw1::создай(sz);
}

SImageDraw::SImageDraw(int cx, int cy)
{
	has_alpha = false;
	SImageDraw1::создай(Размер(cx, cy));
}

}
