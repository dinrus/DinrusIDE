#include "CtrlLib.h"

namespace РНЦП {

int СписокБроса::найдиКлюч(const Значение& k) const
{
	return ключ.найди(k);
}

void СписокБроса::синх() {
	Значение v;
	if(displayall)
		v = значение;
	int i = ключ.найди(значение);
	const Дисплей& d = valuedisplay ? *valuedisplay : i >= 0 ? list.дайДисплей(i, 0)
	                                                         : list.дайДисплей(0);
	if(i >= 0) v = list.дай(i, 0);
	MultiButton::устДисплей(d);
	MultiButton::SetValueCy(list.GetLineCy());
	v = valueconvert->фмт(v);
	уст(v, false);
}

void СписокБроса::Change(int q) {
	if(ключ.дайСчёт() == 0) return;
	int i = ключ.найди(значение);
	for(int l = 0; l < list.дайСчёт(); l++) {
		i += q;
		if(i >= ключ.дайСчёт()) i = ключ.дайСчёт() - 1;
		if(i < 0) i = 0;
		if(list.строкаАктивирована(i) && list.GetLineCy(i) > 0) {
			if(значение != ключ[i]) {
				значение = ключ[i];
				синх();
				UpdateAction();
			}
			return;
		}
	}
}

bool СписокБроса::Ключ(dword k, int) {
	if(толькочтен_ли()) return false;
	switch(k) {
	case K_ALT_DOWN:
		сбрось();
		break;
	case K_DOWN:
	case K_RIGHT:
		Change(1);
		break;
	case K_UP:
	case K_LEFT:
		Change(-1);
		break;
	default:
		if(k >= 32 && k < K_CHAR_LIM) {
			bool b = list.Ключ(k, 1);
			if(list.дайКурсор() >= 0 && list.дайКурсор() < ключ.дайСчёт() && ключ[list.дайКурсор()] != значение)
				выдели();
			return b;
		}
		return false;
	}
	return true;
}

void СписокБроса::колесоМыши(Точка, int zdelta, dword)
{
	if(usewheel)
		Change(zdelta < 0 ? 1 : -1);
}

void СписокБроса::сбрось() {
	if(толькочтен_ли()) return;
	if(dropfocus)
		устФокус();
	WhenDrop();
	list.устКурсор(ключ.найди(значение));
	list.PopUp(this, dropwidth);
}

void СписокБроса::выдели() {
	int c = list.дайКурсор();
	if(c >= 0)
		значение = ключ[c];
	if(dropfocus)
		устФокус();
	синх();
	UpdateAction();
}

void СписокБроса::Cancel() {
	if(dropfocus)
		устФокус();
	синх();
}

void СписокБроса::удали(int i)
{
	ключ.удали(i);
	list.удали(i);
	Adjust();
}

void СписокБроса::очистьСписок() {
	ключ.очисть();
	list.очисть();
	синх();
	list.освежи();
	вклБрос(false);
}

void СписокБроса::очисть() {
	очистьСписок();
	значение = Значение();
	синх();
	Update();
}

СписокБроса& СписокБроса::добавь(const Значение& _key, const Значение& text, bool enable)
{
	ключ.добавь(_key);
	list.добавь(text);
	if(!enable)
		list.SetLineCy(list.дайСчёт() - 1, 0);
	list.освежи();
	вклБрос();
	синх();
	return *this;
}

СписокБроса& СписокБроса::добавь(std::initializer_list<std::pair<Значение, Значение>> init)
{
	for(const auto& i : init)
		добавь(i.first, i.second);
	return *this;
}

struct DummyValue__ {};

СписокБроса& СписокБроса::добавьСепаратор()
{
	ключ.добавь(RawToValue(DummyValue__()));
	list.добавьСепаратор();
	list.освежи();
	вклБрос();
	синх();
	return *this;
}

void СписокБроса::обрежь(int n) {
	if(дайИндекс() >= n)
		SetIndex(0);
	ключ.обрежь(n);
	list.устСчёт(n);
	синх();
	list.освежи();
	вклБрос(n);
}

void СписокБроса::устДанные(const Значение& data) {
	if(значение != data) {
		значение = data;
		Update();
		синх();
	}
}

Значение СписокБроса::дайДанные() const {
	return notnull && пусто_ли(значение) ? ОшибкаНеПусто() : значение;
}

Значение СписокБроса::дайЗначение() const {
	int i = найдиКлюч(значение);
	return i < 0 ? Null : дайЗначение(i);
}

void  СписокБроса::устЗначение(int i, const Значение& v) {
	list.уст(i, 0, v);
	вклБрос();
	синх();
}

void  СписокБроса::устЗначение(const Значение& v) {
	int i = найдиКлюч(значение);
	if(i >= 0)
		устЗначение(i, v);
	синх();
}

СписокБроса& СписокБроса::SetValueConvert(const Преобр& cv)
{
	valueconvert = &cv;
	синх();
	return *this;
}

СписокБроса& СписокБроса::SetConvert(const Преобр& cv)
{
	list.колонкаПо(0).SetConvert(cv);
	return SetValueConvert(cv);
}

СписокБроса& СписокБроса::устДисплей(int i, const Дисплей& d)
{
	list.устДисплей(i, 0, d);
	синх();
	return *this;
}

СписокБроса& СписокБроса::устДисплей(const Дисплей& d)
{
	list.колонкаПо(0).устДисплей(d);
	синх();
	return *this;
}

СписокБроса& СписокБроса::устДисплей(const Дисплей& d, int lcy)
{
	устДисплей(d);
	SetLineCy(lcy);
	синх();
	return *this;
}

СписокБроса& СписокБроса::ValueDisplay(const Дисплей& d)
{
	valuedisplay = &d;
	синх();
	return *this;
}

void  СписокБроса::Adjust()
{
	int i = найдиКлюч(значение);
	if(i < 0) {
		if(дайСчёт())
			SetIndex(0);
		else
			устДанные(Null);
	}
}

void СписокБроса::Adjust(const Значение& k)
{
	int i = найдиКлюч(значение);
	if(i < 0 && дайСчёт())
		устДанные(k);
}

Значение СписокБроса::фмт(const Значение& q) const {
	int i = найдиКлюч(q);
	return valueconvert->фмт(i < 0 ? displayall ? q : Значение() : дайЗначение(i));
}

СписокБроса& СписокБроса::бросВсегда(bool e)
{
	alwaysdrop = e;
	if(e)
		вклБрос();
	return *this;
}

СписокБроса::СписокБроса()
{
	displayall = false;
	valueconvert = &NoConvert();
	valuedisplay = NULL;
	dropfocus = false;
	notnull = false;
	alwaysdrop = false;
	AddButton().Main().WhenPush = THISBACK(сбрось);
	NoInitFocus();
	вклБрос(false);
	list.Normal();
	list.WhenSelect = THISBACK(выдели);
	list.WhenCancel = THISBACK(Cancel);
	dropwidth = 0;
	usewheel = true;
}

СписокБроса::~СписокБроса() {}

void приставь(СписокБроса& list, const ВекторМап<Значение, Значение>& values)
{
	for(int i = 0; i < values.дайСчёт(); i++)
		list.добавь(values.дайКлюч(i), values[i]);
}

void приставь(МапПреобр& convert, const ВекторМап<Значение, Значение>& values)
{
	for(int i = 0; i < values.дайСчёт(); i++)
		convert.добавь(values.дайКлюч(i), values[i]);
}

void приставь(СписокБроса& list, const ВекторМап<int, Ткст>& values)
{
	for(int i = 0; i < values.дайСчёт(); i++)
		list.добавь(values.дайКлюч(i), values[i]);
}

void приставь(МапПреобр& convert, const ВекторМап<int, Ткст>& values)
{
	for(int i = 0; i < values.дайСчёт(); i++)
		convert.добавь(values.дайКлюч(i), values[i]);
}

void приставь(СписокБроса& list, const МапПреобр& convert)
{
	for(int i = 0; i < convert.дайСчёт(); i++)
		list.добавь(convert.дайКлюч(i), convert.дайЗначение(i));
}

void operator*=(СписокБроса& list, const ВекторМап<Значение, Значение>& values)
{
	list.очистьСписок();
	приставь(list, values);
}

void operator*=(МапПреобр& convert, const ВекторМап<Значение, Значение>& values)
{
	convert.очисть();
	приставь(convert, values);
}

void operator*=(СписокБроса& list, const ВекторМап<int, Ткст>& values)
{
	list.очистьСписок();
	приставь(list, values);
}

void operator*=(МапПреобр& convert, const ВекторМап<int, Ткст>& values)
{
	convert.очисть();
	приставь(convert, values);
}

void operator*=(СписокБроса& list, const МапПреобр& convert)
{
	list.очистьСписок();
	приставь(list, convert);
}

}
