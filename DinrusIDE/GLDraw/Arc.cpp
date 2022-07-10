#include "GLDraw.h"

namespace РНЦП {

void GLArc(Вектор<Вектор<ТочкаПЗ>>& line, const ПрямПЗ& rc, ТочкаПЗ start, ТочкаПЗ end)
{
	if(rc.устШирину() <= 0 || rc.устВысоту() <= 0)
		return;
	РазмерПЗ radius = РазмерПЗ(rc.размер()) / 2.0;
	ТочкаПЗ center = ТочкаПЗ(rc.верхЛево()) + radius;
	double ang2 = направление((ТочкаПЗ(start) - center) / radius);
	double ang1 = направление((ТочкаПЗ(end) - center) / radius);
	if(ang1 > ang2)
		ang2 += M_2PI;
	if(ang1 == ang2)
		ang2 += M_2PI;
	
	line.добавь();
	for(double a = ang1; a <= ang2 + M_PI / 200; a += M_PI / 200)
		line.верх().добавь(radius * поляр(min(a, ang2)) + center);
}

};