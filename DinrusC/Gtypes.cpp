#include "Core.h"

namespace РНЦПДинрус {

//template <>
//void Прямоугольник_<double>::союз(const Прямоугольник_<double>& r) {
void прям_дво_союз(Прямоугольник_<double>& self, const Прямоугольник_<double>& r) {
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

//bool Rect_double_Contains(const Прямоугольник_<double>& self, const Точка_<double>& p) {
//	return p.x >= self.left && p.x <= self.right && p.y >= self.top && p.y <= self.bottom;
//}

//template <>
//bool Прямоугольник_<double>::пересекается(const Прямоугольник_<double>& r) const {
bool прям_дво_пересекается(const Прямоугольник_<double>& self, const Прямоугольник_<double>& r) {
	if(пусто_ли(self) || пусто_ли(r)) return false;
	return r.right >= self.left && r.bottom >= self.top && r.left <= self.right && r.top <= self.bottom;
}

//template <>
//Точка_<double> Прямоугольник_<double>::свяжи(Точка_<double> pt) const
Точка_<double> прям_дво_свяжи(const Прямоугольник_<double>& self, Точка_<double> pt) {
	return Точка_<double>(pt.x < self.left ? self.left : pt.x > self.right ? self.right : pt.x,
		pt.y < self.top ? self.top : pt.y > self.bottom ? self.bottom : pt.y);
}

Размер iscale(Размер a, int b, int c)
{
	return Размер(iscale(a.cx, b, c), iscale(a.cy, b, c));
}

Размер iscalefloor(Размер a, int b, int c)
{
	return Размер(iscalefloor(a.cx, b, c), iscalefloor(a.cy, b, c));
}

Размер iscaleceil(Размер a, int b, int c)
{
	return Размер(iscaleceil(a.cx, b, c), iscaleceil(a.cy, b, c));
}

Размер idivfloor(Размер a, int b)
{
	return Размер(idivfloor(a.cx, b), idivfloor(a.cy, b));
}

Размер idivceil(Размер a, int b)
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

Размер  дайСоотношение(Размер sz, int cx, int cy)
{
	return cx == 0 ? cy == 0 ? sz : sz.cy ? Размер(sz.cx * cy / sz.cy, cy) : Размер(0, 0)
		       : cy == 0 ? sz.cx ? Размер(cx, sz.cy * cx / sz.cx) : Размер(0, 0)
				         : Размер(cx, cy);
}

Размер  дайРазмСхождения(Размер sz, int cx, int cy)
{
	if(cx <= 0 || cy <= 0 || sz.cx <= 0 || sz.cy <= 0)
		return Размер(0, 0);
	if(cx * sz.cy >= cy * sz.cx) // too high
		return iscale(sz, cy, sz.cy);
	else
		return iscale(sz, cx, sz.cx);
}

РазмерПЗ дайРазмСхождения(РазмерПЗ sz, double cx, double cy)
{
	if(cx <= 0 || cy <= 0 || sz.cx <= 0 || sz.cy <= 0)
		return Размер(0, 0);
	if(cx * sz.cy >= cy * sz.cx) // too high
		return sz * cy / sz.cy;
	else
		return sz * cx / sz.cx;
}

double квадратДист(const ТочкаПЗ& p1, const ТочкаПЗ& p2)
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
	double l = длина(p);
	return l < 1e-150 ? ТочкаПЗ(0, 0) : p / l;
}

double вКвадрате(const ТочкаПЗ& p)
{
	return p.x * p.x + p.y * p.y;
}

double длина(const ТочкаПЗ& p)
{
	return sqrt(вКвадрате(p));
}

double расстояние(const ТочкаПЗ& p1, const ТочкаПЗ& p2)
{
	return длина(p1 - p2);
}

double растояниеВКвадрате(const ТочкаПЗ& p1, const ТочкаПЗ& p2)
{
	return вКвадрате(p1 - p2);
}

ТочкаПЗ поляр(double a)
{
	return ТочкаПЗ(cos(a), sin(a));
}

ТочкаПЗ поляр(const ТочкаПЗ& p, double r, double a)
{
	return p + r * поляр(a);
}

double направление(const ТочкаПЗ& p)
{
	return atan2(p.y, p.x);
}

}
