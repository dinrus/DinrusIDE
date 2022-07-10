#include "Debuggers.h"

#ifdef PLATFORM_WIN32

void Pdb::PrettyString(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	bool small = IntAt(val, "chr", 14) == 0;
	p.data_count = small ? IntAt(val, "chr", 15) : IntAt(val, "w", 2);
	adr_t a = (small ? дайАтр(val, "chr") : DeRef(дайАтр(val, "ptr"))).address;
	p.data_type << "char";
	for(int i = 0; i < count; i++)
		p.data_ptr.добавь(a + from + i);
	p.kind = TEXT;
}

void Pdb::PrettyWString(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	p.data_count = дайЦел64(дайАтр(val, "length"));
	Val q = DeRef(дайАтр(val, "ptr"));
	int sz = размТипа(q.тип);
	p.data_type.добавь(sz == 4 ? "int" : "short int");
	for(int i = 0; i < count; i++)
		p.data_ptr.добавь(q.address + from + sz * i);
	p.kind = TEXT;
}

void Pdb::PrettyVector(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	p.data_count = дайЦел64(дайАтр(val, "items"));
	Val элт = DeRef(дайАтр(val, "vector"));
	int sz = размТипа(tparam[0]);
	for(int i = 0; i < count; i++)
		p.data_ptr.добавь(элт.address + (i + from) * sz);
}

void Pdb::PrettyBiVector(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	p.data_count = дайЦел64(дайАтр(val, "items"));
	int start = дайЦел(дайАтр(val, "start"));
	int alloc = дайЦел(дайАтр(val, "alloc"));
	Val элт = DeRef(дайАтр(val, "vector"));
	int sz = размТипа(tparam[0]);
	for(int i = 0; i < count; i++) {
		int ii = i + start < alloc ? i + start : i + start - alloc;
		p.data_ptr.добавь(элт.address + (ii + from) * sz);
	}
}

void Pdb::PrettyIndex(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	return PrettyVector(дайАтр(val, "ключ"), tparam, from, count, p);
}

void Pdb::PrettyArray(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	PrettyVector(дайАтр(val, "vector"), { "int *" }, from, count, p);
	for(adr_t& a : p.data_ptr)
		a = подбериУк(a);
}

void Pdb::PrettyBiArray(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	PrettyBiVector(дайАтр(val, "bv"), { "int *" }, from, count, p);
	for(adr_t& a : p.data_ptr)
		a = подбериУк(a);
}

void Pdb::PrettyMap(Pdb::Pretty& p, Pdb::Pretty& ключ, Pdb::Pretty& значение)
{
	p.data_count = min(ключ.data_count, значение.data_count);
	int n = min(ключ.data_ptr.дайСчёт(), значение.data_ptr.дайСчёт());
	for(int i = 0; i < n; i++)
		p.data_ptr << ключ.data_ptr[i] << значение.data_ptr[i];
}

void Pdb::PrettyVectorMap(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	Pretty ключ, значение;
	PrettyIndex(дайАтр(val, "ключ"), { tparam[0] }, from, count, ключ);
	PrettyVector(дайАтр(val, "значение"), { tparam[1] }, from, count, значение);
	PrettyMap(p, ключ, значение);
}

void Pdb::PrettyArrayMap(Pdb::Val val, const Вектор<Ткст>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	Pretty ключ, значение;
	PrettyIndex(дайАтр(val, "ключ"), { tparam[0] }, from, count, ключ);
	PrettyArray(дайАтр(val, "значение"), { tparam[1] }, from, count, значение);
	PrettyMap(p, ключ, значение);
}

void Pdb::PrettyDate(Pdb::Val val, const Вектор<Ткст>&, int64 from, int count, Pdb::Pretty& p)
{
	int day = GetIntAttr(val, "day");
	int month = GetIntAttr(val, "month");
	int year = GetIntAttr(val, "year");
	p.kind = SINGLE_VALUE;
	if(year < -20000)
		p.устПусто();
	else
	if(day >= 0 && day <= 31 && month >= 1 && month <= 12 && year > 1900 && year < 3000)
		p.устТекст(фмт("%04d/%02d/%02d", year, month, day));
}

void Pdb::PrettyTime(Pdb::Val val, const Вектор<Ткст>&, int64 from, int count, Pdb::Pretty& p)
{
	int day = GetIntAttr(val, "day");
	int month = GetIntAttr(val, "month");
	int year = GetIntAttr(val, "year");
	int hour = GetIntAttr(val, "hour");
	int minute = GetIntAttr(val, "minute");
	int second = GetIntAttr(val, "second");
	p.kind = SINGLE_VALUE;
	if(year < -20000)
		p.устПусто();
	else
	if(day >= 1 && day <= 31 && month >= 1 && month <= 12 && year > 1900 && year < 3000 &&
	   hour >= 0 && hour <= 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60)
		p.устТекст(фмт("%04d/%02d/%02d %02d:%02d:%02d", year, month, day, hour, minute, second));
}

void Pdb::PrettyFont(Pdb::Val val, const Вектор<Ткст>&, int64 from, int count, Pdb::Pretty& p)
{
	int64 data = GetInt64Attr(val, "data");
	Шрифт fnt;
	memcpy(&fnt, &data, 8);
	p.устТекст(какТкст(fnt));
	p.kind = SINGLE_VALUE;
}

void Pdb::PrettyValueArray_(adr_t a, Pdb::Pretty& p)
{
	Val v = дайАтр(делайЗнач("РНЦП::МассивЗнач::Данные", подбериУк(a)), "data");
	p.data_type << "РНЦП::Вектор<РНЦП::Значение>";
	p.data_ptr << v.address;
	p.kind = SINGLE_VALUE;
}

void Pdb::PrettyValueArray(Pdb::Val val, const Вектор<Ткст>&, int64 from, int count, Pdb::Pretty& p)
{
	PrettyValueArray_(дайАтр(val, "data").address, p);
}

void Pdb::PrettyValueMap_(adr_t a, Pdb::Pretty& p, int64 from, int count)
{
	p.kind = CONTAINER;
	p.data_type << "РНЦП::Значение" << "РНЦП::Значение";

	Pretty ключ, значение;
	Val m = делайЗнач("РНЦП::МапЗнач::Данные", подбериУк(a));
	PrettyIndex(дайАтр(m, "ключ"), { "РНЦП::Значение" }, from, count, ключ);
	PrettyVector(дайАтр(DeRef(дайАтр(дайАтр(m, "значение"), "data")), "data"),
	             { "РНЦП::Значение" }, from, count, значение);
	PrettyMap(p, ключ, значение);
}

void Pdb::PrettyValueMap(Pdb::Val val, const Вектор<Ткст>&, int64 from, int count, Pdb::Pretty& p)
{
	PrettyValueMap_(дайАтр(val, "data").address, p, from, count);
}

void Pdb::PrettyValue(Pdb::Val val, const Вектор<Ткст>&, int64 from, int count, Pdb::Pretty& p)
{
	val = дайАтр(val, "data");
	adr_t a = val.address;
	p.kind = SINGLE_VALUE;
	if(PeekWord(a + 12) && !PeekWord(a + 14)) { // this is not Ткст
		int st = PeekByte(a + 13);
		if(st == 255) { // REF
			st = PeekDword(a + 8);
			if(st == 8) { // WSTRING_V
				a = подбериУк(a);
				Val v = дайАтр(делайЗнач("РНЦП::RawValueRep<РНЦП::ШТкст>", a), "v");
				p.data_type << "РНЦП::ШТкст";
				p.data_ptr << v.address;
			}
			if(st == 6) { // ERROR_V
				p.устТекст("значОш ", SLtRed());
				p.has_data = true;
				a = подбериУк(a);
				Val v = дайАтр(делайЗнач("РНЦП::RawValueRep<РНЦП::Ткст>", a), "v");
				p.data_type << "РНЦП::Ткст";
				p.data_ptr << v.address;
			}
			if(st == 9) // VALUEARRAY_V
				PrettyValueArray_(a, p);
			if(st == 12)
				PrettyValueMap_(a, p, from, count);

			static ВекторМап<int, Ткст> single {
				{ 20, "РНЦП::Complex" }, { 40, "РНЦП::Шрифт" },
				{ 48, "РНЦП::Painting" }, { 49, "РНЦП::Чертёж" }, { 150, "РНЦП::Рисунок" },
				{ POINT64_V, "РНЦП::Точка_<__int64>" }, { POINTF_V, "РНЦП::Точка_<double>" },
				{ SIZE64_V, "РНЦП::Размер_<__int64>" }, { SIZEF_V, "РНЦП::Размер_<double>" },
				{ RECT_V, "РНЦП::Прямоугольник_<int>" },
				{ RECT64_V, "РНЦП::Прямоугольник_<__int64>" }, { RECTF_V, "РНЦП::Прямоугольник_<double>" },
			};
			
			Ткст t = single.дай(st, Null);
			if(t.дайСчёт()) {
				p.data_type << t;
				p.data_ptr << подбериУк(a) + (win64 ? 16 : 8);
			}
			return;
		}
		p.data_ptr << a;
		if(st == 3) {
			p.устТекст("void", SCyan);
			return;
		}
		static ВекторМап<int, Ткст> single {
			{ 1, "int" }, { 2, "double" }, { 4, "РНЦП::Дата" }, { 5, "РНЦП::Время" }, { 10, "int64" },
			{ 11, "bool" }, {39, "РНЦП::Цвет" },
			{ POINT_V, "РНЦП::Точка_<int>" }, { SIZE_V, "РНЦП::Размер_<int>" }
		};
		Ткст t = single.дай(st, Null);
		if(t.дайСчёт())
			p.data_type << t;
	}
	else {
		p.data_ptr << a;
		p.data_type << "РНЦП::Ткст";
	}
}

void Pdb::PrettyColor(Pdb::Val val, const Вектор<Ткст>&, int64 from, int count, Pdb::Pretty& p)
{
	dword color = GetIntAttr(val, "color");
	Цвет c = Цвет::изСырого(color);
	if(пусто_ли(c))
		p.устПусто();
	else {
		p.устТекст("\1", c);
		p.устТекст(" ");
		p.устТекст(цветВГЯР(c));
	}
	p.kind = SINGLE_VALUE;
}

void Pdb::PrettyRGBA(Pdb::Val val, const Вектор<Ткст>&, int64 from, int count, Pdb::Pretty& p)
{
	КЗСА rc;
	rc.r = GetIntAttr(val, "r");
	rc.g = GetIntAttr(val, "g");
	rc.b = GetIntAttr(val, "b");
	rc.a = 255;
	
	Цвет c(rc);

	p.устТекст("\1", c);
	p.устТекст(" ");
	p.устТекст(цветВГЯР(c));
	p.устТекст(", a: " + какТкст(GetIntAttr(val, "a")));
	
	p.kind = SINGLE_VALUE;
}

void Pdb::PrettyImageBuffer(Pdb::Val val, const Вектор<Ткст>&, int64 from, int count, Pdb::Pretty& p)
{
	p.kind = SINGLE_VALUE;

	Val sz = дайАтр(val, "size");
	Ткст h;
	h.конкат("\2");
	PrettyImage img;
	img.size.cx = (int)GetIntAttr(sz, "cx");
	img.size.cy = (int)GetIntAttr(sz, "cy");
	img.pixels = DeRef(дайАтр(дайАтр(val, "pixels"), "ptr")).address;

	if(img.pixels && img.size.cx * img.size.cy) {
		RawCat(h, img);
		p.устТекст(h);
	}
	else
		p.устПусто();

	bool Ошибка = img.size.cx < 0 || img.size.cx > 10000 || img.size.cy < 0 || img.size.cy > 10000 || !img.pixels;
	Цвет c1 = Ошибка ? SLtRed() : SLtBlue();
	Цвет c2 = Ошибка ? SLtRed() : SRed();
	p.устТекст(" [", c1);
	p.устТекст(какТкст(img.size.cx), c2);
	p.устТекст("x", c1);
	p.устТекст(какТкст(img.size.cy), c2);
	p.устТекст("]", c1);
}

void Pdb::PrettyImg(Pdb::Val val, const Вектор<Ткст>& t, int64 from, int count, Pdb::Pretty& p)
{
	p.kind = SINGLE_VALUE;
	val = DeRef(дайАтр(val, "data"));
	if(val.address)
		PrettyImageBuffer(дайАтр(val, "буфер"), t, from, count, p);
	else
		p.устПусто();
}

#endif
