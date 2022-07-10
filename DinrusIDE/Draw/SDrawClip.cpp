#include "Draw.h"

namespace РНЦП {

void SDraw::иниц(const Прям& r)
{
	Cy(r.дайВысоту());
	Cloff& c = cloff.добавь();
	c.clip.добавь(r);
	c.offset = r.верхЛево();
}

void SDraw::иниц(const Вектор<Прям>& rs, int height, Точка offset)
{
	Cy(height);
	Cloff& c = cloff.добавь();
	c.clip <<= rs;
	c.offset = offset;
}

void SDraw::BeginOp()
{
	Cloff& c = cloff.верх();
	cloff.добавь() <<= c;
}

bool SDraw::ClipOp(const Прям& r)
{
	Cloff& c = cloff.верх();
	Cloff& c1 = cloff.добавь();
	c1.clip = Intersection(c.clip, r + c.offset);
	c1.offset = c.offset;
	return c1.clip.дайСчёт();
}

bool SDraw::ClipoffOp(const Прям& r)
{
	Cloff& c = cloff.верх();
	Cloff& c1 = cloff.добавь();
	c1.clip = Intersection(c.clip, r + c.offset);
	c1.offset = c.offset + r.верхЛево();
	return c1.clip.дайСчёт();
}

bool SDraw::IntersectClipOp(const Прям& r)
{
	Cloff& c = cloff.верх();
	c.clip = Intersection(c.clip, r + c.offset);
	return c.clip.дайСчёт();
}

bool SDraw::ExcludeClipOp(const Прям& r)
{
	Cloff& c = cloff.верх();
	bool dummy;
	c.clip = Subtract(c.clip, r + c.offset, dummy);
	return c.clip.дайСчёт();
}

bool SDraw::IsPaintingOp(const Прям& r) const
{
	return true;
}

void SDraw::OffsetOp(Точка p)
{
	Cloff& c = cloff.верх();
	Cloff& c1 = cloff.добавь();
	c1.clip <<= c.clip;
	c1.offset = c.offset + p;
}

void SDraw::EndOp()
{
	cloff.сбрось();
}

}
