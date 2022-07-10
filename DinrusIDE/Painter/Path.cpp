#include "Painter.h"

#define LLOG(x) // DLOG(x)

namespace РНЦП {

void БуфРисовало::очистьПуть()
{
	current = move = Null;
	ccontrol = qcontrol = ТочкаПЗ(0, 0);
	path_info->сим_ли = false;
	path_info->path_min = ТочкаПЗ(1e200, 1e200);
	path_info->path_max = -ТочкаПЗ(1e200, 1e200);
	path_info->path.устСчёт(1);
	path_info->path.верх().очисть();
	path_info->path.верх().резервируй(128);
}

void БуфРисовало::делайПуть0()
{
	path_index = (path_index + co) & (BATCH_SIZE - 1);
	path_info = &paths[path_index];
	очистьПуть();
	pathattr = attr;
	current = ТочкаПЗ(0, 0);
}

ТочкаПЗ БуфРисовало::путьТчк(const ТочкаПЗ& p, bool rel)
{
	делайПуть();
	ТочкаПЗ r;
	r.x = пусто_ли(p.x) ? current.x : rel ? p.x + current.x : p.x;
	r.y = пусто_ли(p.y) ? current.y : rel ? p.y + current.y : p.y;
	if(dopreclip) {
		path_info->path_min = min(r, path_info->path_min);
		path_info->path_max = max(r, path_info->path_max);
	}
	return r;
}

ТочкаПЗ БуфРисовало::конТчк(const ТочкаПЗ& p, bool rel)
{
	return current = путьТчк(p, rel);
}

template <class T> T& БуфРисовало::добавьПуть(int тип)
{
	Вектор<byte>& p = path_info->path.верх();
	int l = p.дайСчёт();
	p.устСчёт(l + sizeof(T));
	T *e = (T *)(p.begin() + l);
	e->тип = тип;
	return *e;
}

void БуфРисовало::двигОп(const ТочкаПЗ& p, bool rel)
{
	LLOG("@ двигОп " << p << ", " << rel);
	move = ccontrol = qcontrol = конТчк(p, rel);
	добавьПуть<ЛинейнДанные>(MOVE).p = move;
}

void БуфРисовало::двигай0()
{
	if(пусто_ли(move))
		двигОп(ТочкаПЗ(0, 0), false);
}

void БуфРисовало::линияОп(const ТочкаПЗ& p, bool rel)
{
	двигай0();
	ЛинейнДанные h;
	добавьПуть<ЛинейнДанные>(LINE).p = ccontrol = qcontrol = конТчк(p, rel);
}

void БуфРисовало::квадрОп(const ТочкаПЗ& p1, const ТочкаПЗ& p, bool rel)
{
	LLOG("@ квадрОп " << p1 << ", " << p << ", " << rel);
	двигай0();
	auto& m = добавьПуть<КвадратнДанные>(QUADRATIC);
	qcontrol = m.p1 = путьТчк(p1, rel);
	m.p = конТчк(p, rel);
}

void БуфРисовало::квадрОп(const ТочкаПЗ& p, bool rel)
{
	квадрОп(2.0 * current - qcontrol, p, rel);
}

void БуфРисовало::кубОп(const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p, bool rel)
{
	LLOG("@ кубОп " << p1 << ", " << p1 << ", " << p << ", " << rel);
	двигай0();
	auto& m = добавьПуть<КубичДанные>(CUBIC);
	m.p1 = путьТчк(p1, rel);
	ccontrol = m.p2 = путьТчк(p2, rel);
	m.p = конТчк(p, rel);
}

void БуфРисовало::кубОп(const ТочкаПЗ& p2, const ТочкаПЗ& p, bool rel)
{
	кубОп(2.0 * current - ccontrol, p2, p, rel);
}

void БуфРисовало::дугОп(const ТочкаПЗ& c, const ТочкаПЗ& r, double angle, double sweep, bool rel)
{
	LLOG("@ дугОп " << c << ", " << r << ", " << angle << ", " << sweep << ", " << rel);
	двигай0();
	делайДугу(путьТчк(c, rel), r, angle, sweep, 0);
}

void БуфРисовало::SvgArcOp(const ТочкаПЗ& r, double xangle, bool large, bool sweep,
                             const ТочкаПЗ& p, bool rel)
{
	LLOG("@ SvgArcOp " << r << ", " << xangle << ", " << large << ", " << sweep << ", " << p << ", " << rel);
	двигай0();
	ТочкаПЗ c = current;
	DoSvgArc(r, xangle, large, sweep, конТчк(p, rel), c);
}

void БуфРисовало::закройОп()
{
	LLOG("@ закройОп");
	if(!пусто_ли(current) && !пусто_ли(move) && current != move) {
		Строка(move);
		move = Null;
	}
}

void БуфРисовало::делиОп()
{
	LLOG("@ делиОп");
	закройОп();
	path_info->path.добавь();
}

void БуфРисовало::симвОп(const ТочкаПЗ& p, int ch, Шрифт fnt)
{
	LLOG("@ симвОп " << p << ", " << ch << ", " << fnt);
#if 0
	двигай0();
	рисуйСимвол(*this, p, ch, fnt);
#else
	move = current = конТчк(p, false);
	auto& m = добавьПуть<СимДанные>(CHAR);
	m.p = конТчк(p, false);
	m.ch = ch;
	m.fnt = fnt;
	path_info->сим_ли = true;
	EvenOdd();
#endif
}

}
