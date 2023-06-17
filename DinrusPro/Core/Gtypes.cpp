#include <DinrusPro/DinrusCore.h>

//template <>
//проц Прямоугольник_<дво>::союз(const Прямоугольник_<дво>& r) {
проц прям_дво_союз(Прямоугольник_<дво>& self, const Прямоугольник_<дво>& r) {
	if(пусто_ли(r)) return;
	if(пусто_ли(self)) {
		self.уст(r);
		return;
	}
	if(r.left   < self.left)   self.left   = r.left;
	if(r.top    < self.top)    self.top    = r.top;
	if(r.right  > self.right)  self.right  = r.right;
	if(r.bottom > self.bottom) self.bottom = r.bottom;
}

//бул Rect_double_Contains(const Прямоугольник_<дво>& self, const Точка_<дво>& p) {
//	return p.x >= self.left && p.x <= self.right && p.y >= self.top && p.y <= self.bottom;
//}

//template <>
//бул Прямоугольник_<дво>::пересекается(const Прямоугольник_<дво>& r) const {
бул прям_дво_пересекается(const Прямоугольник_<дво>& self, const Прямоугольник_<дво>& r) {
	if(пусто_ли(self) || пусто_ли(r)) return false;
	return r.right >= self.left && r.bottom >= self.top && r.left <= self.right && r.top <= self.bottom;
}

//template <>
//Точка_<дво> Прямоугольник_<дво>::свяжи(Точка_<дво> pt) const
Точка_<дво> прям_дво_свяжи(const Прямоугольник_<дво>& self, Точка_<дво> pt) {
	return Точка_<дво>(pt.x < self.left ? self.left : pt.x > self.right ? self.right : pt.x,
		pt.y < self.top ? self.top : pt.y > self.bottom ? self.bottom : pt.y);
}

Размер iscale(Размер a, цел b, цел c)
{
	return Размер(iscale(a.cx, b, c), iscale(a.cy, b, c));
}

Размер iscalefloor(Размер a, цел b, цел c)
{
	return Размер(iscalefloor(a.cx, b, c), iscalefloor(a.cy, b, c));
}

Размер iscaleceil(Размер a, цел b, цел c)
{
	return Размер(iscaleceil(a.cx, b, c), iscaleceil(a.cy, b, c));
}

Размер idivfloor(Размер a, цел b)
{
	return Размер(idivfloor(a.cx, b), idivfloor(a.cy, b));
}

Размер idivceil(Размер a, цел b)
{
	return Размер(idivceil(a.cx, b), idivceil(a.cy, b));
}

Размер iscale(Размер a, Размер b, Размер c)
{
	return Размер(iscale(a.cx, b.cx, c.cx), iscale(a.cy, b.cy, c.cy));
}

Размер iscalefloor(Размер a, Размер b, Размер c)
{
	return Размер(iscalefloor(a.cx, b.cx, c.cx), iscalefloor(a.cy, b.cy, c.cy));
}

Размер iscaleceil(Размер a, Размер b, Размер c)
{
	return Размер(iscaleceil(a.cx, b.cx, c.cx), iscaleceil(a.cy, b.cy, c.cy));
}

Размер idivfloor(Размер a, Размер b)
{
	return Размер(idivfloor(a.cx, b.cx), idivfloor(a.cy, b.cy));
}

Размер idivceil(Размер a, Размер b)
{
	return Размер(idivceil(a.cx, b.cx), idivceil(a.cy, b.cy));
}

Размер  дайСоотношение(Размер sz, цел cx, цел cy)
{
	return cx == 0 ? cy == 0 ? sz : sz.cy ? Размер(sz.cx * cy / sz.cy, cy) : Размер(0, 0)
		       : cy == 0 ? sz.cx ? Размер(cx, sz.cy * cx / sz.cx) : Размер(0, 0)
				         : Размер(cx, cy);
}

Размер  дайРазмСхождения(Размер sz, цел cx, цел cy)
{
	if(cx <= 0 || cy <= 0 || sz.cx <= 0 || sz.cy <= 0)
		return Размер(0, 0);
	if(cx * sz.cy >= cy * sz.cx) // too high
		return iscale(sz, cy, sz.cy);
	else
		return iscale(sz, cx, sz.cx);
}

РазмерПЗ дайРазмСхождения(РазмерПЗ sz, дво cx, дво cy)
{
	if(cx <= 0 || cy <= 0 || sz.cx <= 0 || sz.cy <= 0)
		return Размер(0, 0);
	if(cx * sz.cy >= cy * sz.cx) // too high
		return sz * cy / sz.cy;
	else
		return sz * cx / sz.cx;
}

дво квадратДист(const ТочкаПЗ& p1, const ТочкаПЗ& p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

ТочкаПЗ середина(const ТочкаПЗ& a, const ТочкаПЗ& b)
{
	return 0.5 * (a + b);
}

ТочкаПЗ ортогональ(const ТочкаПЗ& p)
{
	return ТочкаПЗ(-p.y, p.x);
}

ТочкаПЗ нормализуй(const ТочкаПЗ& p)
{
	дво l = длина(p);
	return l < 1e-150 ? ТочкаПЗ(0, 0) : p / l;
}

дво вКвадрате(const ТочкаПЗ& p)
{
	return p.x * p.x + p.y * p.y;
}

дво длина(const ТочкаПЗ& p)
{
	return sqrt(вКвадрате(p));
}

дво расстояние(const ТочкаПЗ& p1, const ТочкаПЗ& p2)
{
	return длина(p1 - p2);
}

дво растояниеВКвадрате(const ТочкаПЗ& p1, const ТочкаПЗ& p2)
{
	return вКвадрате(p1 - p2);
}

ТочкаПЗ поляр(дво a)
{
	return ТочкаПЗ(cos(a), sin(a));
}

ТочкаПЗ поляр(const ТочкаПЗ& p, дво r, дво a)
{
	return p + r * поляр(a);
}

дво направление(const ТочкаПЗ& p)
{
	return atan2(p.y, p.x);
}

