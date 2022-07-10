#include "RichEdit.h"

namespace РНЦП {

double UnitMultiplier(int unit) {
	static double m[] =
	{
		1,
		72.0 / 600,
		1.0 / 600,
		25.4 / 600,
		2.54 / 600,
	};
	ПРОВЕРЬ(unit >= UNIT_DOT && unit <= UNIT_CM);
	return m[unit];
}

const char *UnitText(int unit) {
	static const char *txt[] =
	{
		"dt",
		"pt",
		"\"",
		"mm",
		"cm"
	};
	return txt[unit];
}

void UnitEdit::читай(double& d, int& u) const
{
	Ткст txt = дайТекст().вТкст();
	const char *s = txt;
	u = unit;
	d = Null;
	if(s && *s) {
		const char *e;
		int sign = 1;
		for(;;) {
			if(*s == '-' && зн)
				sign = -1;
			else
			if(*s != ' ')
				break;
			s++;
		}
		d = сканДво(s, &e);
		if(пусто_ли(d))
			return;
		d *= sign;
		if(e == s) {
			d = Null;
			return;
		}
		while(*e == ' ') e++;
		if(*e == '\"' || *e == 'i')
			u = UNIT_INCH;
		if(*e == 'm')
			u = UNIT_MM;
		if(*e == 'p' || *e == 'b')
			u = UNIT_POINT;
		if(*e == 'c')
			u = UNIT_CM;
		if(*e == 'd')
			u = UNIT_DOT;
	}
}

Значение UnitEdit::дайДанные() const
{
	double q;
	int u;
	читай(q, u);
	return пусто_ли(q) ? Null : int(q / UnitMultiplier(u) + 0.5);
}

Ткст UnitEdit::AsText(double d, int unit)
{
	if(пусто_ли(d))
		return Null;
	Ткст utxt = UnitText(unit);
	if(unit == UNIT_POINT)
		d = floor(10 * d + 0.5) / 10;
	return какТкст(d, unit == UNIT_DOT ? 0 : unit == UNIT_MM ? 1 : 2) + ' ' + utxt;
}

Ткст UnitEdit::DotAsText(int dot, int unit)
{
	if(пусто_ли(dot)) return Null;
	return AsText(dot * UnitMultiplier(unit), unit);
}

Значение UnitEdit::фмт(const Значение& v) const
{
	return DotAsText(v, unit);
}

void UnitEdit::устДанные(const Значение& v)
{
	устТекст(DotAsText(v, unit).вШТкст());
}

bool UnitEdit::Ключ(dword ключ, int repcnt)
{
	if(ключ == K_UP)   { Spin(+1); return true; }
	if(ключ == K_DOWN) { Spin(-1); return true; }
	return EditField::Ключ(ключ, repcnt);
}

void UnitEdit::колесоМыши(Точка p, int zdelta, dword keyflags)
{
	Spin(zdelta < 0 ? -1 : 1);
}

void UnitEdit::Spin(int delta)
{
	double q;
	int u;
	читай(q, u);
	if(пусто_ли(q))
		q = 0;
	else {
		double h = 10;
		switch(u) {
		case UNIT_DOT:   h = 10; break;
		case UNIT_POINT: h = 0.5; break;
		case UNIT_MM:    h = 0.5; break;
		case UNIT_CM:
		case UNIT_INCH:  h = 0.05; break;
		default:         NEVER();
		}
		h *= delta;
		q = ceil(q / h + 1e-2) * h;
		if(!зн && q < 0)
			q = 0;
	}
	устТекст(AsText(q, unit).вШТкст());
	UpdateAction();
}

int CharFilterUnitEdit(int c)
{
	return IsAlpha(c) ? впроп(c) : цифра_ли(c) || c == ' ' || c == '\"' || c == '.' ? c : 0;
}

int CharFilterUnitEditSgn(int c)
{
	return c == '-' ? c : CharFilterUnitEdit(c);
}

UnitEdit& UnitEdit::WithSgn(bool b)
{
	зн = b;
	устФильтр(b ? CharFilterUnitEditSgn : CharFilterUnitEdit);
	return *this;
}

UnitEdit::UnitEdit()
{
	добавьФрейм(spin);
	spin.inc.WhenRepeat = spin.inc.WhenPush = THISBACK1(Spin, 1);
	spin.dec.WhenRepeat = spin.dec.WhenPush = THISBACK1(Spin, -1);
	unit = UNIT_DOT;
	WithSgn(false);
}

}
