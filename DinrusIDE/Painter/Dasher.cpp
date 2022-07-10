#include "Painter.h"

namespace РНЦП {

void Dasher::помести(const ТочкаПЗ& p)
{
	if(flag)
		PutLine(p);
	else
		PutMove(p);
}

void Dasher::Move(const ТочкаПЗ& p)
{
	PutMove(p);
	p0 = p;
}

void Dasher::Строка(const ТочкаПЗ& p)
{
	if(sum == 0) {
		PutLine(p);
		return;
	}
	ТочкаПЗ v = p - p0;
	double len = длина(v);
	double pos = 0;
	while(pos + rem < len) {
		pos += rem;
		помести(pos / len * v + p0);
		flag = !flag;
		rem = (*pattern)[patterni];
		patterni = (patterni + 1) % pattern->дайСчёт();
	}
	rem -= len - pos;
	помести(p);
	p0 = p;
}

void Dasher::иниц(const Вектор<double>& p, double distance)
{
	pattern = &p;
	sum = сумма(p);
	if(sum == 0)
		return;
	distance -= int(distance / sum) * sum;
	patterni = 0;
	flag = false;
	for(;;) {
		rem = (*pattern)[patterni];
		patterni = (patterni + 1) % pattern->дайСчёт();
		flag = !flag;
		if(rem > distance) {
			rem -= distance;
			break;
		}
		distance -= rem;
	}
	p0 = ТочкаПЗ(0, 0);
}

}
