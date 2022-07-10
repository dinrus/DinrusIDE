#include "Esc.h"

namespace РНЦП {

#define LTIMING(x) // RTIMING(x)

Вектор<EscValue>& EscValue::CloneArray()
{
	LTIMING("CloneArray");
	ПРОВЕРЬ(IsArray());
	if(array->refcount != 1) {
		EscArray *c = new EscArray;
		c->array <<= array->array;
		array->отпусти();
		array = c;
	}
	hash = 0;
	return array->array;
}

const Вектор<EscValue>& EscValue::дайМассив() const
{
	ПРОВЕРЬ(IsArray());
	return array->array;
}

EscValue EscValue::ArrayGet(int i) const
{
	return дайМассив()[i];
}

EscValue EscValue::ArrayGet(int i, int n) const
{
	LTIMING("ArrayGet");
	const Вектор<EscValue>& sa = дайМассив();
	ПРОВЕРЬ(i >= 0 && i + n <= sa.дайСчёт());
	EscValue t;
	t.SetEmptyArray();
	t.array->array.приставь(sa, i, n);
	return t;
}

void EscValue::SetEmptyArray()
{
	освободи();
	тип = ESC_ARRAY;
	hash = 0;
	array = new EscArray;
}

bool EscValue::ArraySet(int i, EscValue val)
{
	LTIMING("ArraySet");
	Вектор<EscValue>& ta = CloneArray();
	if(i > max_total || i - ta.дайСчёт() + total > max_total)
		return false;
	CloneArray().по(i) = val;
	return true;
}

void EscValue::ArrayAdd(EscValue val)
{
	LTIMING("ArrayAdd");
	if(проц_ли())
		SetEmptyArray();
	CloneArray().добавь(val);
}

bool EscValue::замени(int i, int n, EscValue a)
{
	LTIMING("Массив замени");
	ПРОВЕРЬ(i >= 0);
	Вектор<EscValue>& ta = CloneArray();
	if(i > max_total || i + n > max_total || i + n - ta.дайСчёт() + total > max_total)
		return false;
	if(i > 0)
		ta.по(i - 1);
	const Вектор<EscValue>& sa = a.дайМассив();
	int q = sa.дайСчёт() - n;
	if(q > 0)
		ta.вставьН(i, q);
	else
		ta.удали(i, -q);
	for(q = 0; q < sa.дайСчёт(); q++)
		ta[q + i] = sa[q];
	return true;
}

bool EscValue::приставь(EscValue a)
{
	if(проц_ли())
		SetEmptyArray();
	return замени(дайСчёт(), 0, a);
}

EscValue::operator ШТкст() const
{
	LTIMING("operator Ткст");
	ШТкстБуф s;
	if(IsArray()) {
		const Вектор<EscValue>& a = дайМассив();
		for(int i = 0; i < a.дайСчёт(); i++)
			if(a[i].цел_ли()) {
				int c = a[i].дайЦел();
				if(c >= 0 && c < 65536)
					s.конкат(c);
			}
	}
	return ШТкст(s);
}

void EscValue::InitString(const ШТкст& s)
{
	тип = ESC_ARRAY;
	array = new EscArray();
	hash = 0;
	Вектор<EscValue>& a = array->array;
	a.устСчёт(s.дайСчёт());
	for(int i = 0; i < s.дайСчёт(); i++)
		a[i] = (int64)s[i];
	total++;
}

int   EscValue::GetTotalCount()
{
	return total;
}

void  EscValue::SetMaxTotalCount(int n)
{
	max_total = n;
}

int   EscValue::GetMaxTotalCount()
{
	return max_total;
}

EscValue::EscValue()
{
	тип = ESC_VOID; hash = 0; total++;
}

EscValue::EscValue(double n)
{
	number = n;
	тип = ESC_DOUBLE;
	hash = 0;
	total++;
}

EscValue::EscValue(int64 n)
{
	i64 = n;
	тип = ESC_INT64;
	hash = 0;
	total++;
}

EscValue::EscValue(int n)
{
	i64 = n;
	тип = ESC_INT64;
	hash = 0;
	total++;
}

}
