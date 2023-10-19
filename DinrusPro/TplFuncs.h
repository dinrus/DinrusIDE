#ifndef _DinrusPro_TplFuncs_h_
#define _DinrusPro_TplFuncs_h_

#include <DinrusPro/DinrusPro.h>
#include <DinrusPro/Funcs.h>
#include <DinrusPro/Tpl.h>
#include <DinrusPro/NonTpl.h>
///////////////////////

т_хэш дайХэшЗначУкз(кук a) ;
ук разместиТини(цел size);
ук разместиПам(т_мера size);

struct Ууид;
class Ткст;
class Ткст0;
class ШТкст;
class Поток;
class РярВВ;
class ДжейсонВВ;
class Значение;
struct РефНаТипЗнач;
цел длинтекс__(кткст0 s) ;
цел длинтекс__(const шим *s);
/////////////////////////////
/////////////////////////////
//class КэшЛРУ
template <class T, class K>
проц КэшЛРУ<T, K>::линкуйГолову(цел i)
{
	Элемент& m = данные[i];
	if(head >= 0) {
		цел tail = данные[head].prev;
		m.next = head;
		m.prev = tail;
		данные[head].prev = i;
		данные[tail].next = i;
	}
	else
		m.prev = m.next = i;
	head = i;
	count++;
}


template <class T, class K>
проц КэшЛРУ<T, K>::отлинкуй(цел i)
{
	Элемент& m = данные[i];
	if(m.prev == i)
		head = -1;
	else {
		if(head == i)
			head = m.next;
		данные[m.next].prev = m.prev;
		данные[m.prev].next = m.next;
	}
	count--;
}

template <class T, class K>
T& КэшЛРУ<T, K>::дайЛРУ()
{
	цел tail = данные[head].prev;
	return *данные[tail].данные;
}

template <class T, class K>
const K& КэшЛРУ<T, K>::дайКлючЛРУ()
{
	цел tail = данные[head].prev;
	return ключ[tail].ключ;
}

template <class T, class K>
проц КэшЛРУ<T, K>::сбросьЛРУ()
{
	if(head >= 0) {
		цел tail = данные[head].prev;
		size -= данные[tail].size;
		данные[tail].данные.очисть();
		отлинкуй(tail);
		ключ.отлинкуй(tail);
	}
}

template <class T, class K>
template <class P>
проц КэшЛРУ<T, K>::настройРазм(P getsize)
{
	size = 0;
	count = 0;
	for(цел i = 0; i < данные.дайСчёт(); i++)
		if(!ключ.отлинкован(i)) {
			цел sz = getsize(*данные[i].данные);
			if(sz >= 0)
				данные[i].size = sz + InternalSize;
			size += данные[i].size;
			count++;
		}
}

template <class T, class K>
template <class P>
цел КэшЛРУ<T, K>::удали(P predicate)
{
	цел n = 0;
	цел i = 0;
	while(i < данные.дайСчёт())
		if(!ключ.отлинкован(i) && predicate(*данные[i].данные)) {
			size -= данные[i].size;
			отлинкуй(i);
			ключ.отлинкуй(i);
			n++;
		}
		else
			i++;
	return n;
}

template <class T, class K>
template <class P>
бул КэшЛРУ<T, K>::удалиОдин(P predicate)
{
	цел i = head;
	if(i >= 0)
		for(;;) {
			цел next = данные[i].next;
			if(predicate(*данные[i].данные)) {
				size -= данные[i].size;
				отлинкуй(i);
				ключ.отлинкуй(i);
				return true;
			}
			if(i == next || next == head || next < 0)
				break;
			i = next;
		}
	return false;
}

template <class T, class K>
проц КэшЛРУ<T, K>::сожми(цел maxsize, цел maxcount)
{
	if(maxsize >= 0 && maxcount >= 0)
		while(count > maxcount || size > maxsize)
			DropLRU();
}

template <class T, class K>
проц КэшЛРУ<T, K>::очисть()
{
	head = -1;
	size = 0;
	count = 0;
	newsize = foundsize = 0;
	ключ.очисть();
	данные.очисть();
}

template <class T, class K>
проц КэшЛРУ<T, K>::очистьСчётчики()
{
	flag = !flag;
	newsize = foundsize = 0;
}

template <class T, class K>
T& КэшЛРУ<T, K>::дай(const Делец& m)
{
	Ключ k;
	k.ключ = m.Ключ();
	k.тип = typeid(m).name();
	цел q = ключ.найди(k);
	if(q < 0) {
		q = ключ.помести(k);
		Элемент& t = данные.по(q);
		t.size = m.сделай(t.данные.создай()) + InternalSize;
		size += t.size;
		newsize += t.size;
		t.flag = flag;
	}
	else {
		Элемент& t = данные[q];
		отлинкуй(q);
		if(t.flag != flag) {
			t.flag = flag;
			foundsize += t.size;
		}
	}
	линкуйГолову(q);
	return *данные[q].данные;
}
/////////////////
//КэшЗначений
extern СтатическийСтопор СтопорКэшаЗначений;

template <class P>
цел кэшзначУдали(P what)
{
	Стопор::Замок __(СтопорКэшаЗначений);
	return дайКэшЗначений().удали(what);
}

template <class P>
цел кэшзначУдалиОдно(P what)
{
	Стопор::Замок __(СтопорКэшаЗначений);
	return дайКэшЗначений().удали(what);
}

template <class P>
проц кэшзначНастройРазмер(P getsize)
{
	Стопор::Замок __(СтопорКэшаЗначений);
	дайКэшЗначений().настройРазм(getsize);
}
/////////////
//
template <typename CHAR, typename BYTE>
форс_инлайн
проц пропустиПробелы__(const CHAR *&s)
{
	while(*s && (BYTE)*s <= ' ') s++;
}

template <typename CHAR, typename BYTE>
форс_инлайн
цел сканЗнак__(const CHAR *&s)
{
	цел sign = 1;
	if(*s == '+')
		s++;
	if(*s == '-') {
		sign = -1;
		s++;
	}
	пропустиПробелы__<CHAR, BYTE>(s);
	return sign;
}

template <typename CHAR, typename BYTE, typename UINT, цел base>
форс_инлайн
const CHAR *сканБцел0(UINT& result, const CHAR *s, бул& overflow)
{
	auto дай = [&] (цел i) -> цел {
		цел c = (BYTE)s[i];
		if(base > 10) {
			if(c <= '9')
				return c - '0';
			if(c <= 'Z')
				return c - 'A' + 10;
			return c - 'a' + 10;
		}
		return c - '0';
	};
	цел c1 = дай(0);
	if(c1 < 0 || c1 >= base)
		return NULL;
	UINT n = 0;
	const цел base2 = base * base;
	const цел base3 = base * base2;
	const цел base4 = base * base3;
	for(;;) {
		UINT n0 = n;
		auto CheckOverflow = [&](UINT n1) { if(n0 > n1) overflow = true; };
		if(c1 < 0 || c1 >= base) {
			result = n;
			return s;
		}
		цел c2 = дай(1);
		if(c2 < 0 || c2 >= base) {
			result = base * n + c1;
			CheckOverflow(result);
			return s + 1;
		}
		цел c3 = дай(2);
		if(c3 < 0 || c3 >= base) {
			result = base2 * n + base * c1 + c2;
			CheckOverflow(result);
			return s + 2;
		}
		цел c4 = дай(3);
		if(c4 < 0 || c4 >= base) {
			result = base3 * n + base2 * c1 + base * c2 + c3;
			CheckOverflow(result);
			return s + 3;
		}
		n = base4 * n + base3 * c1 + base2 * c2 + base * c3 + c4;
		CheckOverflow(n);
		s += 4;
		c1 = дай(0);
	}
}

template <typename CHAR, typename BYTE, typename UINT, цел base>
форс_инлайн
const CHAR *сканБцел(UINT& result, const CHAR *s, бул& overflow)
{
	пропустиПробелы__<CHAR, BYTE>(s);
	return сканБцел0<CHAR, BYTE, UINT, base>(result, s, overflow);
}

template <typename CHAR, typename BYTE, typename UINT, цел base>
форс_инлайн
const CHAR *сканБцел(UINT& result, кткст0 s)
{
	бул overflow = false;
	s = сканБцел<CHAR, BYTE, UINT, base>(result, s, overflow);
	return overflow ? NULL : s;
}

template <typename CHAR, typename BYTE, typename UINT, typename INT, цел base>
форс_инлайн
const CHAR *сканЦел(INT& result, const CHAR *s, бул& overflow)
{
	пропустиПробелы__<CHAR, BYTE>(s);
	цел зн = сканЗнак__<CHAR, BYTE>(s);
	UINT uresult;
	s = сканБцел0<CHAR, BYTE, UINT, base>(uresult, s, overflow);
	if(s) {
		if(зн < 0) {
			result = INT(зн * uresult);
			if(result > 0) overflow = true;
		}
		else {
			result = INT(uresult);
			if(result < 0) overflow = true;
		}
	}
	return s;
}

template <typename CHAR, typename BYTE, typename UINT, typename INT, цел base>
форс_инлайн
const CHAR *сканЦел(INT& result, const CHAR *s)
{
	бул overflow = false;
	s = сканЦел<CHAR, BYTE, UINT, INT, base>(result, s, overflow);
	return overflow ? NULL : s;
}
/////////////////////////
//Sort.h
template <class I, class Less>
форс_инлайн
проц OrderIter2__(I a, I b, const Less& less)
{
	if(less(*b, *a))
		IterSwap(a, b);
}

template <class I, class Less>
форс_инлайн // 2-3 compares, 0-2 swaps
проц OrderIter3__(I x, I y, I z, const Less& less)
{
    if(less(*y, *x)) {
	    if(less(*z, *y))
	        IterSwap(x, z);
	    else {
		    IterSwap(x, y);
		    if(less(*z, *y))
		        IterSwap(y, z);
	    }
    }
    else
    if(less(*z, *y)) {
        IterSwap(y, z);
        if(less(*y, *x))
            IterSwap(x, y);
    }
}

template <class I, class Less>
форс_инлайн // 3-6 compares, 0-5 swaps
проц OrderIter4__(I x, I y, I z, I u, const Less& less)
{
	OrderIter3__(x, y, z, less);
	if(less(*z, *u)) return;
	IterSwap(z, u);
	if(less(*y, *z)) return;
	IterSwap(y, z);
	if(less(*x, *y)) return;
	IterSwap(x, y);
}

template <class I, class Less>
форс_инлайн // 4-10 compares, 0-9 swaps
проц OrderIter5__(I x, I y, I z, I u, I v, const Less& less)
{
	OrderIter4__(x, y, z, u, less);
	if(less(*u, *v)) return;
	IterSwap(u, v);
	if(less(*z, *u)) return;
	IterSwap(z, u);
	if(less(*y, *z)) return;
	IterSwap(y, z);
	if(less(*x, *y)) return;
	IterSwap(x, y);
}

template <class I, class Less>
форс_инлайн // 9-15 compares, 0-14 swaps
проц OrderIter6__(I x, I y, I z, I u, I v, I w, const Less& less)
{
	OrderIter5__(x, y, z, u, v, less);
	if(less(*v, *w)) return;
	IterSwap(v, w);
	if(less(*u, *v)) return;
	IterSwap(u, v);
	if(less(*z, *u)) return;
	IterSwap(z, u);
	if(less(*y, *z)) return;
	IterSwap(y, z);
	if(less(*x, *y)) return;
	IterSwap(x, y);
}

template <class I, class Less>
форс_инлайн // 16-26 compares, 0-20 swaps
проц OrderIter7__(I x, I y, I z, I u, I v, I w, I q, const Less& less)
{
	OrderIter6__(x, y, z, u, v, w, less);
	if(less(*w, *q)) return;
	IterSwap(w, q);
	if(less(*v, *w)) return;
	IterSwap(v, w);
	if(less(*u, *v)) return;
	IterSwap(u, v);
	if(less(*z, *u)) return;
	IterSwap(z, u);
	if(less(*y, *z)) return;
	IterSwap(y, z);
	if(less(*x, *y)) return;
	IterSwap(x, y);
}

template <class I, class Less>
проц Sort__(I l, I h, const Less& less)
{
	цел count = цел(h - l);
	I middle = l + (count >> 1);        // get the middle element
	for(;;) {
		switch(count) {
		case 0:
		case 1: return;
		case 2: OrderIter2__(l, l + 1, less); return;
		case 3: OrderIter3__(l, l + 1, l + 2, less); return;
		case 4: OrderIter4__(l, l + 1, l + 2, l + 3, less); return;
		case 5: OrderIter5__(l, l + 1, l + 2, l + 3, l + 4, less); return;
		case 6: OrderIter6__(l, l + 1, l + 2, l + 3, l + 4, l + 5, less); return;
		case 7: OrderIter7__(l, l + 1, l + 2, l + 3, l + 4, l + 5, l + 6, less); return;
		}
		if(count > 1000) { // median of 5, 2 of them random elements
			middle = l + (count >> 1); // iterators cannot point to the same object!
			I q = l + 1 + (цел)случ((count >> 1) - 2);
			I w = middle + 1 + (цел)случ((count >> 1) - 2);
			OrderIter5__(l, q, middle, w, h - 1, less);
		}
		else // median of 3
			OrderIter3__(l, middle, h - 1, less);

		I pivot = h - 2;
		IterSwap(pivot, middle); // move median pivot to h - 2
		I i = l;
		I j = h - 2; // l, h - 2, h - 1 already sorted above
		for(;;) { // Hoare’s partition (modified):
			while(less(*++i, *pivot));
			do
				if(!(i < j)) goto done;
			while(!less(*--j, *pivot));
			IterSwap(i, j);
		}
	done:
		IterSwap(i, h - 2);                 // put pivot back in between partitions

		I ih = i;
		while(ih + 1 != h && !less(*i, *(ih + 1))) // найди middle range of elements equal to pivot
			++ih;

		цел count_l = цел(i - l);
		if(count_l == 1) // this happens if there are many elements equal to pivot, фильтр them out
			for(I q = ih + 1; q != h; ++q)
				if(!less(*i, *q))
					IterSwap(++ih, q);

		цел count_h = цел(h - ih) - 1;

		if(count_l < count_h) {       // recurse on smaller partition, tail on larger
			Sort__(l, i, less);
			l = ih + 1;
			count = count_h;
		}
		else {
			Sort__(ih + 1, h, less);
			h = i;
			count = count_l;
		}

		if(count > 8 && мин(count_l, count_h) < (макс(count_l, count_h) >> 2)) // If unbalanced, randomize the next step
			middle = l + 1 + (цел)случ(count - 2); // случ pivot selection
		else
			middle = l + (count >> 1); // the middle is probably still the best guess otherwise
	}
}

template <class Диапазон, class Less>
проц сортируй(Диапазон&& c, const Less& less)
{
	Sort__(c.begin(), c.end(), less);
}

template <class Диапазон>
проц сортируй(Диапазон&& c)
{
	Sort__(c.begin(), c.end(), std::less<ТипЗначУ<Диапазон>>());
}

template <class Диапазон, class Less>
проц стабСортируй(Диапазон&& r, const Less& less)
{
	auto begin = r.begin();
	auto end = r.end();
	typedef ТипЗначУ<Диапазон> VT;
	typedef decltype(begin) I;
	цел count = (цел)(uintptr_t)(end - begin);
	Буфер<цел> h(count);
	for(цел i = 0; i < count; i++)
		h[i] = i;
	Sort__(ОбходчикСтабСорта__<I, VT>(begin, ~h), ОбходчикСтабСорта__<I, VT>(end, ~h + count),
	       СтабСортМеньшее__<VT, Less>(less));
}

template <class Диапазон>
проц стабСортируй(Диапазон&& r)
{
	стабСортируй(r, std::less<ТипЗначУ<Диапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Less>
проц индексСортируй(МастерДиапазон&& r, Диапазон2&& r2, const Less& less)
{
	ПРОВЕРЬ(r.дайСчёт() == r2.дайСчёт());
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef ТипЗначУ<МастерДиапазон> VT;
	if(r.дайСчёт() == 0)
		return;
	Sort__(ИндексСортОбходчик__<I, I2, VT>(r.begin(), r2.begin()),
	       ИндексСортОбходчик__<I, I2, VT>(r.end(), r2.end()),
		   less);
}

template <class МастерДиапазон, class Диапазон2>
проц индексСортируй(МастерДиапазон&& r, Диапазон2&& r2)
{
	индексСортируй(r, r2, std::less<ТипЗначУ<МастерДиапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Less>
проц стабИндексСортируй(МастерДиапазон&& r, Диапазон2&& r2, const Less& less)
{
	ПРОВЕРЬ(r.дайСчёт() == r2.дайСчёт());
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef ТипЗначУ<МастерДиапазон> VT;
	if(r.дайСчёт() == 0)
		return;
	стабСортируй(СубДиапазон(ИндексСортОбходчик__<I, I2, VT>(r.begin(), r2.begin()),
		                ИндексСортОбходчик__<I, I2, VT>(r.end(), r2.end())).пиши(),
	           less);
}

template <class МастерДиапазон, class Диапазон2>
проц стабИндексСортируй(МастерДиапазон&& r, Диапазон2&& r2)
{
	стабИндексСортируй(r, r2, std::less<ТипЗначУ<МастерДиапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Less>
проц индексСортируй2(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, const Less& less)
{
	ПРОВЕРЬ(r.дайСчёт() == r2.дайСчёт());
	ПРОВЕРЬ(r.дайСчёт() == r3.дайСчёт());
	if(r.дайСчёт() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef ТипЗначУ<МастерДиапазон> VT;
	Sort__(ИндексСорт2Обходчик__<I, I2, I3, VT>(r.begin(), r2.begin(), r3.begin()),
	       ИндексСорт2Обходчик__<I, I2, I3, VT>(r.end(), r2.end(), r3.end()),
		   less);
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3>
проц индексСортируй2(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3)
{
	индексСортируй2(r, r2, r3, std::less<ТипЗначУ<МастерДиапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Less>
проц стабИндексСортируй2(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, const Less& less)
{
	ПРОВЕРЬ(r.дайСчёт() == r2.дайСчёт());
	ПРОВЕРЬ(r.дайСчёт() == r3.дайСчёт());
	if(r.дайСчёт() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef ТипЗначУ<МастерДиапазон> VT;
	стабСортируй(СубДиапазон(ИндексСорт2Обходчик__<I, I2, I3, VT>(r.begin(), r2.begin(), r3.begin()),
	                    ИндексСорт2Обходчик__<I, I2, I3, VT>(r.end(), r2.end(), r3.end())).пиши(),
		       less);
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3>
проц стабИндексСортируй2(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3)
{
	стабИндексСортируй2(r, r2, r3, std::less<ТипЗначУ<МастерДиапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Диапазон4, class Less>
проц индексСортируй3(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, Диапазон4&& r4, const Less& less)
{
	ПРОВЕРЬ(r.дайСчёт() == r2.дайСчёт());
	ПРОВЕРЬ(r.дайСчёт() == r3.дайСчёт());
	ПРОВЕРЬ(r.дайСчёт() == r4.дайСчёт());
	if(r.дайСчёт() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef decltype(r4.begin()) I4;
	typedef ТипЗначУ<МастерДиапазон> VT;
	Sort__(ИндексСорт3Обходчик__<I, I2, I3, I4, VT>(r.begin(), r2.begin(), r3.begin(), r4.begin()),
	       ИндексСорт3Обходчик__<I, I2, I3, I4, VT>(r.end(), r2.end(), r3.end(), r4.end()),
		   less);
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Диапазон4>
проц индексСортируй3(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, Диапазон4&& r4)
{
	индексСортируй3(r, r2, r3, r4, std::less<ТипЗначУ<МастерДиапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Диапазон4, class Less>
проц стабИндексСортируй3(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, Диапазон4&& r4, const Less& less)
{
	ПРОВЕРЬ(r.дайСчёт() == r2.дайСчёт());
	ПРОВЕРЬ(r.дайСчёт() == r3.дайСчёт());
	ПРОВЕРЬ(r.дайСчёт() == r4.дайСчёт());
	if(r.дайСчёт() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef decltype(r4.begin()) I4;
	typedef ТипЗначУ<МастерДиапазон> VT;
	стабСортируй(СубДиапазон(ИндексСорт3Обходчик__<I, I2, I3, I4, VT>(r.begin(), r2.begin(), r3.begin(), r4.begin()),
	                    ИндексСорт3Обходчик__<I, I2, I3, I4, VT>(r.end(), r2.end(), r3.end(), r4.end())).пиши(),
		       less);
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Диапазон4>
проц стабИндексСортируй3(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, Диапазон4&& r4)
{
	стабИндексСортируй3(r, r2, r3, r4, std::less<ТипЗначУ<МастерДиапазон>>());
}

template <class Диапазон, class Less>
Вектор<цел> дайСортПорядок(const Диапазон& r, const Less& less)
{
	auto begin = r.begin();
	Вектор<цел> индекс;
	индекс.устСчёт(r.дайСчёт());
	for(цел i = индекс.дайСчёт(); --i >= 0; индекс[i] = i)
		;
	typedef ОбходчикПорядкаСорта__<decltype(begin), ТипЗначУ<Диапазон>> It;
	Sort__(It(индекс.begin(), begin), It(индекс.end(), begin), less);
	return индекс;
}

template <class Диапазон>
Вектор<цел> дайСортПорядок(const Диапазон& r)
{
	return дайСортПорядок(r, std::less<ТипЗначУ<Диапазон>>());
}

template <class Диапазон, class Less>
Вектор<цел> дайПорядокСтабСорта(const Диапазон& r, const Less& less)
{
	Вектор<цел> индекс;
	индекс.устСчёт(r.дайСчёт());
	for(цел i = индекс.дайСчёт(); --i >= 0; индекс[i] = i)
		;
	auto begin = r.begin();
	typedef ТипЗначУ<Диапазон> VT;
	typedef ОбходчикПорядкаСтабСорта__<decltype(begin), VT> It;
	Sort__(It(индекс.begin(), begin), It(индекс.end(), begin), СтабСортМеньшее__<VT, Less>(less));
	return индекс;
}

template <class Диапазон>
Вектор<цел> дайПорядокСтабСорта(const Диапазон& r)
{
	return дайПорядокСтабСорта(r, std::less<ТипЗначУ<Диапазон>>());
}

template <class вКарту, class Less>
проц сортируйПоКлючу(вКарту& map, const Less& less)
{
	typename вКарту::КонтейнерКлючей k = map.подбериКлючи();
	typename вКарту::КонтейнерЗначений v = map.подбериЗначения();
	индексСортируй(k, v, less);
	map = вКарту(пикуй(k), пикуй(v));
}

template <class вКарту>
проц сортируйПоКлючу(вКарту& map)
{
	сортируйПоКлючу(map, std::less<typename вКарту::ТипКлюча>());
}

template <class вКарту, class Less>
проц сортируйПоЗнач(вКарту& map, const Less& less)
{
	typename вКарту::КонтейнерКлючей k = map.подбериКлючи();
	typename вКарту::КонтейнерЗначений v = map.подбериЗначения();
	индексСортируй(v, k, less);
	map = вКарту(пикуй(k), пикуй(v));
}

template <class вКарту>
проц сортируйПоЗнач(вКарту& map)
{
	сортируйПоЗнач(map, std::less<ТипЗначУ<вКарту>>());
}

template <class вКарту, class Less>
проц стабСортируйПоКлючу(вКарту& map, const Less& less)
{
	typename вКарту::КонтейнерКлючей k = map.подбериКлючи();
	typename вКарту::КонтейнерЗначений v = map.подбериЗначения();
	стабИндексСортируй(k, v, less);
	map = вКарту(пикуй(k), пикуй(v));
}

template <class вКарту>
проц стабСортируйПоКлючу(вКарту& map)
{
	стабСортируйПоКлючу(map, std::less<typename вКарту::ТипКлюча>());
}

template <class вКарту, class Less>
проц стабСортируйПоЗнач(вКарту& map, const Less& less)
{
	typename вКарту::КонтейнерКлючей k = map.подбериКлючи();
	typename вКарту::КонтейнерЗначений v = map.подбериЗначения();
	стабИндексСортируй(v, k, less);
	map = вКарту(пикуй(k), пикуй(v));
}

template <class вКарту>
проц стабСортируйПоЗнач(вКарту& map)
{
	стабСортируйПоЗнач(map, std::less<ТипЗначУ<вКарту>>());
}

template <class Индекс, class Less>
проц сортируйИндекс(Индекс& индекс, const Less& less)
{
	typename Индекс::КонтейнерЗначений k = индекс.подбериКлючи();
	сортируй(k, less);
	индекс = Индекс(пикуй(k));
}

template <class Индекс>
проц сортируйИндекс(Индекс& индекс)
{
	сортируйИндекс(индекс, std::less<ТипЗначУ<Индекс>>());
}

template <class Индекс, class Less>
проц стабСортируйИндекс(Индекс& индекс, const Less& less)
{
	typename Индекс::КонтейнерЗначений k = индекс.подбериКлючи();
	стабСортируй(k, less);
	индекс = Индекс(пикуй(k));
}

template <class Индекс>
проц стабСортируйИндекс(Индекс& индекс)
{
	стабСортируйИндекс(индекс, std::less<ТипЗначУ<Индекс>>());
}
//////////
//Sorted.h
template <class C, class L>
C& приставьОтсорт(C& приёмник, const C& ист, const L& less)
{
	if(ист.пустой())
		return приёмник;
	if(приёмник.пустой())
	{
		приёмник <<= ист;
		return приёмник;
	}
	if(!less(*ист, приёмник.верх()))
	{
		приёмник.приставь(ист);
		return приёмник;
	}
	if(!less(*приёмник, ист.верх()))
	{
		приёмник.вставь(0, ист);
		return приёмник;
	}
	цел dc = приёмник.дайСчёт();
	цел sc = ист.дайСчёт();
	приёмник.устСчёт(dc + sc);
	typename C::Обходчик de = приёмник.стоп();
	typename C::КонстОбходчик se = ист.стоп(), pe = приёмник.дайОбх(dc);
	--se;
	--pe;
	for(;;)
	{
		if(less(*se, *pe))
		{
			*--de = *pe;
			if(pe == приёмник.старт())
			{ // приёмник items are finished
				*--de = *se;
				while(se != ист.старт())
					*--de = *--se;
				return приёмник;
			}
			--pe;
		}
		else
		{
			*--de = *se;
			if(se == ист.старт())
				return приёмник; // ист items are finished, приёмник items are in place
			--se;
		}
	}
	return приёмник;
}

template <class C>
C& приставьОтсорт(C& приёмник, const C& ист)
{
	typedef ТипЗначУ<C> VT;
	return приставьОтсорт(приёмник, ист, std::less<VT>());
}

template <class C, class L>
C& соединиОтсорт(C& приёмник, const C& ист, const L& less)
{
	if(ист.пустой())
		return приёмник;
	if(приёмник.пустой())
	{
		приёмник <<= ист;
		return приёмник;
	}
	if(less(приёмник.верх(), *ист))
	{
		приёмник.приставь(ист);
		return приёмник;
	}
	if(less(ист.верх(), *приёмник))
	{
		приёмник.вставь(0, ист);
		return приёмник;
	}
	цел dc = приёмник.дайСчёт();
	цел sc = ист.дайСчёт();
	приёмник.устСчёт(dc + sc);
	typename C::Обходчик de = приёмник.стоп();
	typename C::КонстОбходчик se = ист.стоп(), pe = приёмник.дайОбх(dc);
	--se;
	--pe;
	for(;;)
	{
		if(less(*se, *pe))
		{
			*--de = *pe;
			if(pe == приёмник.старт())
			{ // приёмник items are finished
				*--de = *se;
				while(se != ист.старт())
					*--de = *--se;
				приёмник.удали(0, приёмник.дайИндекс(*de));
				return приёмник;
			}
			--pe;
		}
		else
		{
			*--de = *se;
			if(!less(*pe, *se))
			{
				if(pe == приёмник.старт())
				{
					while(se != ист.старт())
						*--de = *--se;
					приёмник.удали(0, приёмник.дайИндекс(*de));
					return приёмник;
				}
				--pe;
			}
			if(se == ист.старт())
			{
				цел pi = (pe - приёмник.старт()) + 1;
				приёмник.удали(pi, (de - приёмник.старт()) - pi);
				return приёмник; // ист items are finished, приёмник items are in place
			}
			--se;
		}
	}
	return приёмник;
}

template <class C>
C& соединиОтсорт(C& приёмник, const C& ист)
{
	typedef ТипЗначУ<C> VT;
	return соединиОтсорт(приёмник, ист, std::less<VT>());
}

template <class C, class L>
C& удалиОтсорт(C& from, const C& what, const L& less)
{
	if(from.пустой() || what.пустой() ||
	   less(from.верх(), *what.старт()) || less(what.верх(), *from.старт()))
		return from;
	typename C::КонстОбходчик we = what.стоп(), wp = BinFind(what, from[0], less);
	if(wp == we)
		return from;
	typename C::Обходчик fp = from.старт() + BinFindIndex(from, *wp), fe = from.стоп(), fd = fp;
	if(fp == fe)
		return from;
	for(;;)
	{
		while(less(*fp, *wp))
		{
			*fd = *fp;
			++fd;
			if(++fp == fe)
			{
				from.устСчёт(fd - from.старт());
				return from;
			}
		}
		if(less(*wp, *fp))
		{
			do
				if(++wp == we)
				{
					копируй(fd, fp, fe);
					fd += (fe - fp);
					from.устСчёт(fd - from.старт());
					return from;
				}
			while(less(*wp, *fp));
		}
		else
		{
			const ТипЗначУ<C>& значение = *fp;
			while(!less(значение, *fp))
				if(++fp == fe)
				{
					from.устСчёт(fd - from.старт());
					return from;
				}
			do
				if(++wp == we)
				{
					копируй(fd, fp, fe);
					fd += (fe - fp);
					from.устСчёт(fd - from.старт());
					return from;
				}
			while(!less(значение, *wp));
		}
	}
}

template <class C>
C& удалиОтсорт(C& from, const C& what)
{
	typedef ТипЗначУ<C> VT;
	return удалиОтсорт(from, what, std::less<VT>());
}

template <class D, class S, class L>
D& пересекиОтсорт(D& приёмник, const S& ист, const L& less)
{
	if(приёмник.пустой())
		return приёмник;
	if(ист.пустой() || less(приёмник.верх(), ист[0]) || less(ист.верх(), приёмник[0]))
	{ // empty source set or disjunct intervals
		приёмник.очисть();
		return приёмник;
	}
	typename S::КонстОбходчик ss = BinFind(ист, приёмник[0], less), se = ист.стоп();
	if(ss == se)
	{
		приёмник.очисть();
		return приёмник;
	}
	typename D::КонстОбходчик ds = BinFind(приёмник, ист[0], less), de = приёмник.стоп();
	if(ds == de)
	{
		приёмник.очисть();
		return приёмник;
	}
	typename D::Обходчик d = приёмник.старт();
	цел count = 0;
	for(;;)
	{
		if(less(*ss, *ds))
		{
			if(++ss == se)
				break;
		}
		else
		{
			if(!less(*ds, *ss))
			{
				*d = *ds;
				++d;
				count++;
			}
			if(++ds == de)
				break;
		}
	}
	приёмник.устСчёт(count);
	return приёмник;
}

template <class D, class S>
D& пересекиОтсорт(D& приёмник, const S& ист)
{
	typedef ТипЗначУ<D> VT;
	return пересекиОтсорт(приёмник, ист, std::less<VT>());
}

/////////////////////
///CoSort.h
template <class I, class Less>
проц соСорт__(СоРабота& cw, I l, I h, const Less& less)
{
	цел count = цел(h - l);
	I middle = l + (count >> 1);        // get the middle element

	for(;;) {
		if(count < 200) { // too little elements to gain anything with parellel processing
			Sort__(l, h, less);
			return;
		}

		if(count > 1000) {
			middle = l + (count >> 1); // iterators cannot point to the same object!
			I q = l + 1 + (цел)случ((count >> 1) - 2);
			I w = middle + 1 + (цел)случ((count >> 1) - 2);
			OrderIter5__(l, q, middle, w, h - 1, less);
		}
		else
			OrderIter3__(l, middle, h - 1, less);

		I pivot = h - 2;
		IterSwap(pivot, middle); // move median pivot to h - 2
		I i = l;
		I j = h - 2; // l, h - 2, h - 1 already sorted above
		for(;;) { // Hoare’s partition (modified):
			while(less(*++i, *pivot));
			do
				if(!(i < j)) goto done;
			while(!less(*--j, *pivot));
			IterSwap(i, j);
		}
	done:
		IterSwap(i, h - 2);                 // put pivot back in between partitions

		I ih = i;
		while(ih + 1 != h && !less(*i, *(ih + 1))) // найди middle range of elements equal to pivot
			++ih;

		цел count_l = i - l;
		if(count_l == 1) // this happens if there are many elements equal to pivot, фильтр them out
			for(I q = ih + 1; q != h; ++q)
				if(!less(*i, *q))
					IterSwap(++ih, q);

		цел count_h = h - ih - 1;

		if(count_l < count_h) {       // recurse on smaller partition, tail on larger
			cw & [=, &cw] { соСорт__(cw, l, i, less); };
			l = ih + 1;
			count = count_h;
		}
		else {
			cw & [=, &cw] { соСорт__(cw, ih + 1, h, less); };
			h = i;
			count = count_l;
		}

		if(count > 8 && мин(count_l, count_h) < (макс(count_l, count_h) >> 2)) // If unbalanced,
			middle = l + 1 + случ(count - 2); // randomize the next step
		else
			middle = l + (count >> 1); // the middle is probably still the best guess otherwise
	}
}

template <class I, class Less>
проц соСорт__(I l, I h, const Less& less)
{
	СоРабота cw;
	соСорт__(cw, l, h, less);
}

template <class Диапазон, class Less>
проц соСортируй(Диапазон&& c, const Less& less)
{
	соСорт__(c.begin(), c.end(), less);
}

template <class Диапазон>
проц соСортируй(Диапазон&& c)
{
	соСорт__(c.begin(), c.end(), std::less<ТипЗначУ<Диапазон>>());
}

template <class Диапазон, class Less>
проц соСтабСортируй(Диапазон&& r, const Less& less)
{
	auto begin = r.begin();
	auto end = r.end();
	typedef ТипЗначУ<Диапазон> VT;
	typedef decltype(begin) I;
	цел count = (цел)(uintptr_t)(end - begin);
	Буфер<цел> h(count);
	for(цел i = 0; i < count; i++)
		h[i] = i;
	соСорт__(ОбходчикСтабСорта__<I, VT>(begin, ~h), ОбходчикСтабСорта__<I, VT>(end, ~h + count),
	         СтабСортМеньшее__<VT, Less>(less));
}

template <class Диапазон>
проц соСтабСортируй(Диапазон&& r)
{
	соСтабСортируй(r, std::less<ТипЗначУ<Диапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Less>
проц соИндексСортируй(МастерДиапазон&& r, Диапазон2&& r2, const Less& less)
{
	ПРОВЕРЬ(r.дайСчёт() == r2.дайСчёт());
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef ТипЗначУ<МастерДиапазон> VT;
	if(r.дайСчёт() == 0)
		return;
	соСорт__(ИндексСортОбходчик__<I, I2, VT>(r.begin(), r2.begin()),
	         ИндексСортОбходчик__<I, I2, VT>(r.end(), r2.end()),
		     less);
}

template <class МастерДиапазон, class Диапазон2>
проц соИндексСортируй(МастерДиапазон&& r, Диапазон2&& r2)
{
	соИндексСортируй(r, r2, std::less<ТипЗначУ<МастерДиапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Less>
проц соСтабИндексСортируй(МастерДиапазон&& r, Диапазон2&& r2, const Less& less)
{
	ПРОВЕРЬ(r.дайСчёт() == r2.дайСчёт());
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef ТипЗначУ<МастерДиапазон> VT;
	if(r.дайСчёт() == 0)
		return;
	соСтабСортируй(СубДиапазон(ИндексСортОбходчик__<I, I2, VT>(r.begin(), r2.begin()),
		                  ИндексСортОбходчик__<I, I2, VT>(r.end(), r2.end())).пиши(),
	             less);
}

template <class МастерДиапазон, class Диапазон2>
проц соСтабИндексСортируй(МастерДиапазон&& r, Диапазон2&& r2)
{
	соСтабИндексСортируй(r, r2, std::less<ТипЗначУ<МастерДиапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Less>
проц соИндексСортируй2(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, const Less& less)
{
	ПРОВЕРЬ(r.дайСчёт() == r2.дайСчёт());
	ПРОВЕРЬ(r.дайСчёт() == r3.дайСчёт());
	if(r.дайСчёт() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef ТипЗначУ<МастерДиапазон> VT;
	соСорт__(ИндексСорт2Обходчик__<I, I2, I3, VT>(r.begin(), r2.begin(), r3.begin()),
	         ИндексСорт2Обходчик__<I, I2, I3, VT>(r.end(), r2.end(), r3.end()),
		     less);
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3>
проц соИндексСортируй2(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3)
{
	соИндексСортируй2(r, r2, r3, std::less<ТипЗначУ<МастерДиапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Less>
проц соСтабИндексСортируй2(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, const Less& less)
{
	ПРОВЕРЬ(r.дайСчёт() == r2.дайСчёт());
	ПРОВЕРЬ(r.дайСчёт() == r3.дайСчёт());
	if(r.дайСчёт() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef ТипЗначУ<МастерДиапазон> VT;
	соСтабСортируй(СубДиапазон(ИндексСорт2Обходчик__<I, I2, I3, VT>(r.begin(), r2.begin(), r3.begin()),
	                    ИндексСорт2Обходчик__<I, I2, I3, VT>(r.end(), r2.end(), r3.end())).пиши(),
		         less);
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3>
проц соСтабИндексСортируй2(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3)
{
	соСтабИндексСортируй2(r, r2, r3, std::less<ТипЗначУ<МастерДиапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Диапазон4, class Less>
проц соИндексСортируй3(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, Диапазон4&& r4, const Less& less)
{
	ПРОВЕРЬ(r.дайСчёт() == r2.дайСчёт());
	ПРОВЕРЬ(r.дайСчёт() == r3.дайСчёт());
	ПРОВЕРЬ(r.дайСчёт() == r4.дайСчёт());
	if(r.дайСчёт() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef decltype(r4.begin()) I4;
	typedef ТипЗначУ<МастерДиапазон> VT;
	соСорт__(ИндексСорт3Обходчик__<I, I2, I3, I4, VT>(r.begin(), r2.begin(), r3.begin(), r4.begin()),
	         ИндексСорт3Обходчик__<I, I2, I3, I4, VT>(r.end(), r2.end(), r3.end(), r4.end()),
		     less);
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Диапазон4>
проц соИндексСортируй3(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, Диапазон4&& r4)
{
	соИндексСортируй3(r, r2, r3, r4, std::less<ТипЗначУ<МастерДиапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Диапазон4, class Less>
проц соСтабИндексСортируй3(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, Диапазон4&& r4, const Less& less)
{
	ПРОВЕРЬ(r.дайСчёт() == r2.дайСчёт());
	ПРОВЕРЬ(r.дайСчёт() == r3.дайСчёт());
	ПРОВЕРЬ(r.дайСчёт() == r4.дайСчёт());
	if(r.дайСчёт() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef decltype(r4.begin()) I4;
	typedef ТипЗначУ<МастерДиапазон> VT;
	соСтабСортируй(СубДиапазон(ИндексСорт3Обходчик__<I, I2, I3, I4, VT>(r.begin(), r2.begin(), r3.begin(), r4.begin()),
	                      ИндексСорт3Обходчик__<I, I2, I3, I4, VT>(r.end(), r2.end(), r3.end(), r4.end())).пиши(),
		         less);
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Диапазон4>
проц соСтабИндексСортируй3(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, Диапазон4&& r4)
{
	соСтабИндексСортируй3(r, r2, r3, r4, std::less<ТипЗначУ<МастерДиапазон>>());
}

template <class Диапазон, class Less>
Вектор<цел> соДайПорядокСорт(const Диапазон& r, const Less& less)
{
	auto begin = r.begin();
	Вектор<цел> индекс;
	индекс.устСчёт(r.дайСчёт());
	for(цел i = индекс.дайСчёт(); --i >= 0; индекс[i] = i)
		;
	typedef ОбходчикПорядкаСорта__<decltype(begin), ТипЗначУ<Диапазон>> It;
	соСорт__(It(индекс.begin(), begin), It(индекс.end(), begin), less);
	return индекс;
}

template <class Диапазон>
Вектор<цел> соДайПорядокСорт(const Диапазон& r)
{
	return соДайПорядокСорт(r, std::less<ТипЗначУ<Диапазон>>());
}

template <class Диапазон, class Less>
Вектор<цел> соДайСтабПорядокСорт(const Диапазон& r, const Less& less)
{
	Вектор<цел> индекс;
	индекс.устСчёт(r.дайСчёт());
	for(цел i = индекс.дайСчёт(); --i >= 0; индекс[i] = i)
		;
	auto begin = r.begin();
	typedef ТипЗначУ<Диапазон> VT;
	typedef ОбходчикПорядкаСтабСорта__<decltype(begin), VT> It;
	соСорт__(It(индекс.begin(), begin), It(индекс.end(), begin), СтабСортМеньшее__<VT, Less>(less));
	return индекс;
}

template <class Диапазон>
Вектор<цел> соДайСтабПорядокСорт(const Диапазон& r)
{
	return соДайПорядокСорт(r, std::less<ТипЗначУ<Диапазон>>());
}

template <class вКарту, class Less>
проц соСортируйПоКлючу(вКарту& map, const Less& less)
{
	typename вКарту::КонтейнерКлючей k = map.подбериКлючи();
	typename вКарту::КонтейнерЗначений v = map.подбериЗначения();
	соИндексСортируй(k, v, less);
	map = вКарту(пикуй(k), пикуй(v));
}

template <class вКарту>
проц соСортируйПоКлючу(вКарту& map)
{
	соСортируйПоКлючу(map, std::less<typename вКарту::ТипКлюча>());
}

template <class вКарту, class Less>
проц соСортируйПоЗначению(вКарту& map, const Less& less)
{
	typename вКарту::КонтейнерКлючей k = map.подбериКлючи();
	typename вКарту::КонтейнерЗначений v = map.подбериЗначения();
	соИндексСортируй(v, k, less);
	map = вКарту(пикуй(k), пикуй(v));
}

template <class вКарту>
проц соСортируйПоЗначению(вКарту& map)
{
	соСортируйПоЗначению(map, std::less<ТипЗначУ<вКарту>>());
}

template <class вКарту, class Less>
проц соСтабСортируйПоКлючу(вКарту& map, const Less& less)
{
	typename вКарту::КонтейнерКлючей k = map.подбериКлючи();
	typename вКарту::КонтейнерЗначений v = map.подбериЗначения();
	соСтабИндексСортируй(k, v, less);
	map = вКарту(пикуй(k), пикуй(v));
}

template <class вКарту>
проц соСтабСортируйПоКлючу(вКарту& map)
{
	соСтабСортируйПоКлючу(map, std::less<typename вКарту::ТипКлюча>());
}

template <class вКарту, class Less>
проц соСтабСортируйПоЗначению(вКарту& map, const Less& less)
{
	typename вКарту::КонтейнерКлючей k = map.подбериКлючи();
	typename вКарту::КонтейнерЗначений v = map.подбериЗначения();
	соСтабИндексСортируй(v, k, less);
	map = вКарту(пикуй(k), пикуй(v));
}

template <class вКарту>
проц соСтабСортируйПоЗначению(вКарту& map)
{
	соСтабСортируйПоЗначению(map, std::less<ТипЗначУ<вКарту>>());
}

template <class Индекс, class Less>
проц соСортИндекс(Индекс& индекс, const Less& less)
{
	typename Индекс::КонтейнерЗначений k = индекс.подбериКлючи();
	соСортируй(k, less);
	индекс = Индекс(пикуй(k));
}

template <class Индекс>
проц соСортИндекс(Индекс& индекс)
{
	соСортИндекс(индекс, std::less<ТипЗначУ<Индекс>>());
}

template <class Индекс, class Less>
проц соСтабСортИндекс(Индекс& индекс, const Less& less)
{
	typename Индекс::КонтейнерЗначений k = индекс.подбериКлючи();
	соСтабСортируй(k, less);
	индекс = Индекс(пикуй(k));
}

template <class Индекс>
проц соСтабСортИндекс(Индекс& индекс)
{
	соСтабСортИндекс(индекс, std::less<ТипЗначУ<Индекс>>());
}

template<> inline Ткст какТкст(const крат& a)           { return фмтЦел(a); }
template<> inline Ткст какТкст(const бкрат& a)  { return фмтБцел(a); }
template<> inline Ткст какТкст(const цел& a)             { return фмтЦел(a); }
template<> inline Ткст какТкст(const бцел& a)    { return фмтБцел(a); }
template<> inline Ткст какТкст(const long& a)            { return фмтЦел64(a); }
template<> inline Ткст какТкст(const unsigned long& a)   { return фмтБцел64(a); }
template<> inline Ткст какТкст(const дол& a)           { return фмтЦел64(a); }
template<> inline Ткст какТкст(const бдол& a)          { return фмтБцел64(a); }
template<> inline Ткст какТкст(const дво& a)          { return фмтДвоЧ(a); }
template<> inline Ткст какТкст(const плав& a)           { return фмтДво(a); }

template <typename... Арги>
Ткст фмтЧ(цел язык, кткст0 s, const Арги& ...арги)
{
	return фмт(язык, s, gather<Вектор<Значение>>(арги...));
}

template <typename... Арги>
Ткст фмтЧ(кткст0 s, const Арги& ...арги)
{
	return фмт(s, gather<Вектор<Значение>>(арги...));
}

template <typename T>
auto пикуй(T&& x) noexcept -> decltype(std::move(x)) { return std::move(x); }

template <class T>
inline цел зн(T a) { return a > 0 ? 1 : a < 0 ? -1 : 0; }

template <class T>
inline T табы(T a) { return (a >= 0 ? a : -a); }

template <class T> inline T абс(T a) { return (a >= 0 ? a : -a); }

template <class T>
inline цел сравни(const T& a, const T& b) { return a > b ? 1 : a < b ? -1 : 0; }

#ifdef CPU_SIMD
template <class T>
проц т_копирпам(ук t, const T *s, т_мера счёт)
{
#ifdef CPU_X86
	if((sizeof(T) & 15) == 0)
		копирпам128(t, s, счёт * (sizeof(T) >> 4));
	else
	if((sizeof(T) & 7) == 0)
		копирпам64(t, s, счёт * (sizeof(T) >> 3));
	else
#endif
	if((sizeof(T) & 3) == 0)
		копирпам32(t, s, счёт * (sizeof(T) >> 2));
	else
	if((sizeof(T) & 1) == 0)
		копирпам16(t, s, счёт * (sizeof(T) >> 1));
	else
		копирпам8(t, s, счёт * sizeof(T));
}
#else
template <class T>
проц т_копирпам(ук t, const T *s, т_мера count)
{
	копирпам8(t, s, count * sizeof(T));
}
#endif

template <class T>
бул т_равнпам(const T *p, const T *q, т_мера счёт)
{
	if((sizeof(T) & 15) == 0)
		return равнпам128(p, q, счёт * (sizeof(T) >> 4));
	else
	if((sizeof(T) & 7) == 0)
		return равнпам64(p, q, счёт * (sizeof(T) >> 3));
	else
	if((sizeof(T) & 3) == 0)
		return равнпам32(p, q, счёт * (sizeof(T) >> 2));
	else
	if((sizeof(T) & 1) == 0)
		return равнпам16(p, q, счёт * (sizeof(T) >> 1));
	else
		return равнпам8(p, q, счёт * sizeof(T));
}

template <class T>
цел сравниЧисла__(const T& a, const T& b)
{
	if(a < b) return -1;
	if(a > b) return 1;
	return 0;
}

template <class T>
inline т_хэш дайХэшЗнач(T *укз)                             { return ::дайХэшЗначУкз(reinterpret_cast<кук>(укз)); }

template <class T>
inline т_хэш дайХэшЗнач(const T& x)                              { return x.дайХэшЗнач(); }

template <class T>
inline проц заполни(T *dst, const T *lim, const T& x) {
	while(dst < lim)
		*dst++ = x;
}

template <class T>
inline проц копируй(T *dst, const T *ист, const T *lim) {
	while(ист < lim)
		*dst++ = *ист++;
}

template <class T>
inline проц копируй(T *dst, const T *ист, цел n) {
	копируй(dst, ист, ист + n);
}

template <class I>
inline проц IterSwap(I a, I b) { разверни(*a, *b); }

template<class T>
inline цел сравниЗнак(const T& a, const T& b)
{
	return a.сравни(b);
}

template <class C>
бул равныМапы(const C& a, const C& b)
{
	if(a.дайСчёт() != b.дайСчёт())
		return false;
	for(цел i = 0; i < a.дайСчёт(); i++)
		if(a.дайКлюч(i) != b.дайКлюч(i) || a[i] != b[i])
			return false;
	return true;
}

template <class C>
цел сравниМап(const C& a, const C& b)
{
	цел n = мин(a.дайСчёт(), b.дайСчёт());
	for(цел i = 0; i < n; i++) {
		цел q = сравниЗнак(a.дайКлюч(i), b.дайКлюч(i));
		if(q)
			return q;
		q = сравниЗнак(a[i], b[i]);
		if(q)
			return q;
	}
	return сравниЗнак(a.дайСчёт(), b.дайСчёт());
}

template <class T>
constexpr const T& мин(const T& a, const T& b)
{
	return a < b ? a : b;
}

template <class T, typename... Арги>
constexpr const T& мин(const T& a, const T& b, const Арги& ...арги)
{
	return мин(a, мин(b, арги...));
}

template <class T>
constexpr const T& макс(const T& a, const T& b)
{
	return a > b ? a : b;
}

template <class T, typename... Арги>
constexpr const T& макс(const T& a, const T& b, const Арги& ...арги)
{
	return макс(a, макс(b, арги...));
}

//$+

template <class T>
constexpr T clamp(const T& x, const T& min_, const T& max_)
{
	return мин(макс(x, min_), max_);
}

//$-constexpr цел найдиарг(const T& x, const T1& p0, ...);

template <class T, class K>
constexpr цел найдиарг(const T& x, const K& k)
{
	return x == k ? 0 : -1;
}

template <class T, class K, typename... L>
constexpr цел найдиарг(const T& sel, const K& k, const L& ...арги)
{
	if(sel == k)
		return 0;
	цел q = найдиарг(sel, арги...);
	return q >= 0 ? q + 1 : -1;
}

//$-constexpr auto раскодируй(const T& x, const T1& p0, const V1& v0, ...);

template <class T>
constexpr кткст0 decode_chr_(const T& sel, кткст0 опр)
{
	return опр;
}

template <class T, class K, typename... L>
constexpr кткст0 decode_chr_(const T& sel, const K& k, кткст0 v, const L& ...арги)
{
	return sel == k ? v : decode_chr_(sel, арги...);
}

template <class T, class K, typename... L>
constexpr кткст0 раскодируй(const T& sel, const K& k, кткст0 v, const L& ...арги)
{
	return decode_chr_(sel, k, v, арги...);
}

template <class T, class V>
constexpr const V& раскодируй(const T& sel, const V& опр)
{
	return опр;
}

template <class T, class K, class V, typename... L>
constexpr V раскодируй(const T& sel, const K& k, const V& v, const L& ...арги)
{
	return sel == k ? v : (V)раскодируй(sel, арги...);
}

//$-constexpr T get_i(цел i, const T& p0, const T1& p1, ...);

template <typename A, typename... T>
constexpr A get_i(цел i, const A& p0, const T& ...арги)
{
	A list[] = { p0, (A)арги... };
	return list[clamp(i, 0, (цел)sizeof...(арги))];
}

template <typename P, typename... T>
constexpr const P *get_i(цел i, const P* p0, const T& ...арги)
{
	const P *list[] = { p0, арги... };
	return list[clamp(i, 0, (цел)sizeof...(арги))];
}

//$-проц foreach_arg(F фн, const T& p0, const T1& p1, ...);

template <class F, class V>
проц foreach_arg(F фн, V&& v)
{
	фн(std::forward<V>(v));
}

template <class F, class V, typename... Арги>
проц foreach_arg(F фн, V&& v, Арги&& ...арги)
{
	фн(std::forward<V>(v));
	foreach_arg(фн, std::forward<Арги>(арги)...);
}

//$+

template <class I, typename... Арги>
проц iter_set(I t, Арги&& ...арги)
{
	foreach_arg([&](auto&& v) { *t++ = std::forward<decltype(v)>(v); }, std::forward<Арги>(арги)...);
}

template <class I, typename... Арги>
проц iter_get(I s, Арги& ...арги)
{
	foreach_arg([&](auto& v) { v = *s++; }, арги...);
}

template <class C, typename... Арги>
C gather(Арги&& ...арги)
{
	C x(sizeof...(арги));
	iter_set(x.begin(), std::forward<Арги>(арги)...);
	return x;
}

template <class C, typename... Арги>
цел scatter_n(цел n, const C& c, Арги& ...арги)
{
	if(n < (цел)sizeof...(арги))
		return 0;
	iter_get(c.begin(), арги...);
	return sizeof...(арги);
}

template <class C, typename... Арги>
цел scatter(const C& c, Арги& ...арги)
{
	return scatter_n(c.size(), c, арги...);
}

// DEPRECATED

template <class T> // deprecated имя, use clamp
inline T минмакс(T x, T _min, T _max)                           { return мин(макс(x, _min), _max); }


template <class T, class... Арги>
T *tiny_new(Арги... арги)
{
	return new(разместиТини(sizeof(T))) T(арги...);
}

template <class T>
проц tiny_delete(T *укз)
{
	укз->~T();
	освободиТини(sizeof(T), укз);
}

template <class T>
inline бцел номерТипаЗнач(const T *)                 { return СтатичНомТипа<T>() + 0x8000000; }

template <class T> проц устПусто(T& x) { x = Null; }

template <class T> бул пусто_ли(const T& x)       { return x.экзПусто_ли(); }

template <class T>
inline проц разверни(T& a, T& b) { T tmp = пикуй(a); a = пикуй(b); b = пикуй(tmp); }

///
#ifdef _ОТЛАДКА
проц всёКогдаПодобрано(ук укз);
проц ВСЁ_КОГДА_ПОДОБРАНО__(ук укз);

template <class T>
проц ВСЁ_КОГДА_ПОДОБРАНО(T& x)
{
	ВСЁ_КОГДА_ПОДОБРАНО__(&x);
}
#endif

template <class T>
inline проц стройЗаполниГлубКопию(T *t, const T *end, const T& x) {
	while(t != end)
		new(t++) T(клонируй(x));
}

template <class T>
inline проц строй(T *t, const T *lim) {
	while(t < lim)
		new(t++) T;
}

template <class T>
inline проц разрушь(T *t, const T *end)
{
	while(t != end) {
		t->~T();
		t++;
	}
}

template <class T, class S>
inline проц стройГлубКопию(T *t, const S *s, const S *end) {
	while(s != end)
		new (t++) T(клонируй(*s++));
}


template <class Ук, class Класс, class Рез, class... ТипыАрг>
Функция<Рез (ТипыАрг...)> памФн(Ук object, Рез (Класс::*method)(ТипыАрг...))
{
	return [=, this](ТипыАрг... арги) { return (object->*method)(арги...); };
}

#define THISFN(x)   памФн(this, &ИМЯ_КЛАССА::x)

//////////////////////////////////////////////

//#include "AString.hpp"
template <class B>
форс_инлайн
проц АТкст<B>::вставь(цел pos, кткст0 s)
{
	вставь(pos, s, длинтекс__(s));
}

template <class B>
проц АТкст<B>::кат(цел c, цел count)
{
	tchar *s = B::вставь(дайДлину(), count, NULL);
	while(count--)
		*s++ = c;
}

template <class B>
форс_инлайн
проц АТкст<B>::кат(const tchar *s)
{
	кат(s, длинтекс__(s));
}

template <class B>
цел АТкст<B>::сравни(const tchar *b) const
{
	const tchar *a = B::старт();
	const tchar *ae = стоп();
	for(;;) {
		if(a >= ae)
			return *b == 0 ? 0 : -1;
		if(*b == 0)
			return 1;
		цел q = сравнзн__(*a++) - сравнзн__(*b++);
		if(q)
			return q;
	}
}

template <class B>
typename АТкст<B>::Ткст АТкст<B>::середина(цел from, цел count) const
{
	цел l = дайДлину();
	if(from > l) from = l;
	if(from < 0) from = 0;
	if(count < 0)
		count = 0;
	if(from + count > l)
		count = l - from;
	return Ткст(B::старт() + from, count);
}

template <class B>
цел АТкст<B>::найди(цел chr, цел from) const
{
	ПРОВЕРЬ(from >= 0 && from <= дайДлину());
	const tchar *e = стоп();
	const tchar *укз = B::старт();
	for(const tchar *s = укз + from; s < e; s++)
		if(*s == chr)
			return (цел)(s - укз);
	return -1;
}

template <class B>
цел АТкст<B>::найдирек(цел chr, цел from) const
{
	ПРОВЕРЬ(from >= 0 && from <= дайДлину());
	if(from < дайДлину()) {
		const tchar *укз = B::старт();
		for(const tchar *s = укз + from; s >= укз; s--)
			if(*s == chr)
				return (цел)(s - укз);
	}
	return -1;
}

template <class B>
цел АТкст<B>::найдирек(цел len, const tchar *s, цел from) const
{
	ПРОВЕРЬ(from >= 0 && from <= дайДлину());
	if(from < дайДлину()) {
		const tchar *укз = B::старт();
		const tchar *p = укз + from - len + 1;
		len *= sizeof(tchar);
		while(p >= укз) {
			if(*s == *p && memcmp(s, p, len) == 0)
				return (цел)(p - укз);
			p--;
		}
	}
	return -1;
}

template <class B>
цел АТкст<B>::найдирекПосле(цел len, const tchar *s, цел from) const
{
	цел q = найдирек(len, s, from);
	return q >= 0 ? q + len : -1;
}

template <class B>
проц АТкст<B>::замени(const tchar *найди, цел findlen, const tchar *replace, цел replacelen)
{
	if(findlen == 0)
		return;
	Ткст r;
	цел i = 0;
	const tchar *p = B::старт();
	for(;;) {
		цел j = найди(findlen, найди, i);
		if(j < 0)
			break;
		r.кат(p + i, j - i);
		r.кат(replace, replacelen);
		i = j + findlen;
	}
	r.кат(p + i, B::дайСчёт() - i);
	B::освободи();
	B::уст0(r);
}

template <class B>
цел АТкст<B>::найдирек(const tchar *s, цел from) const
{
	return найдирек(длинтекс__(s), s, from);
}

template <class B>
цел АТкст<B>::найдирекПосле(const tchar *s, цел from) const
{
	return найдирекПосле(длинтекс__(s), s, from);
}

template <class B>
цел АТкст<B>::найдирек(цел chr) const
{
	return B::дайСчёт() ? найдирек(chr, B::дайСчёт() - 1) : -1;
}

template <class B>
проц АТкст<B>::замени(const Ткст& найди, const Ткст& replace)
{
	замени(~найди, найди.дайСчёт(), ~replace, replace.дайСчёт());
}

template <class B>
форс_инлайн
проц АТкст<B>::замени(const tchar *найди, const tchar *replace)
{
	замени(найди, (цел)длинтекс__(найди), replace, (цел)длинтекс__(replace));
}

template <class B>
форс_инлайн
проц АТкст<B>::замени(const Ткст& найди, const tchar *replace)
{
	замени(~найди, найди.дайСчёт(), replace, (цел)длинтекс__(replace));
}

template <class B>
форс_инлайн
проц АТкст<B>::замени(const tchar *найди, const Ткст& replace)
{
	замени(найди, (цел)длинтекс__(найди), ~replace, replace.дайСчёт());
}

template <class B>
бул АТкст<B>::начинаетсяС(const tchar *s, цел len) const
{
	if(len > дайДлину()) return false;
	return memcmp(s, B::старт(), len * sizeof(tchar)) == 0;
}

template <class B>
форс_инлайн
бул АТкст<B>::начинаетсяС(const tchar *s) const
{
	return начинаетсяС(s, длинтекс__(s));
}

template <class B>
бул АТкст<B>::заканчиваетсяНа(const tchar *s, цел len) const
{
	цел l = дайДлину();
	if(len > l) return false;
	return memcmp(s, B::старт() + l - len, len * sizeof(tchar)) == 0;
}

template <class B>
форс_инлайн
бул АТкст<B>::заканчиваетсяНа(const tchar *s) const
{
	return заканчиваетсяНа(s, длинтекс__(s));
}

template <class B>
цел АТкст<B>::найдиПервыйИз(цел len, const tchar *s, цел from) const
{
	ПРОВЕРЬ(from >= 0 && from <= дайДлину());
	const tchar *укз = B::старт();
	const tchar *e = B::стоп();
	const tchar *se = s + len;
	if(len == 1) {
		tchar c1 = s[0];
		for(const tchar *bs = укз + from; bs < e; bs++) {
			if(*bs == c1)
				return (цел)(bs - укз);
		}
		return -1;
	}
	if(len == 2) {
		tchar c1 = s[0];
		tchar c2 = s[1];
		for(const tchar *bs = укз + from; bs < e; bs++) {
			tchar ch = *bs;
			if(ch == c1 || ch == c2)
				return (цел)(bs - укз);
		}
		return -1;
	}
	if(len == 3) {
		tchar c1 = s[0];
		tchar c2 = s[1];
		tchar c3 = s[2];
		for(const tchar *bs = укз + from; bs < e; bs++) {
			tchar ch = *bs;
			if(ch == c1 || ch == c2 || ch == c3)
				return (цел)(bs - укз);
		}
		return -1;
	}
	if(len == 4) {
		tchar c1 = s[0];
		tchar c2 = s[1];
		tchar c3 = s[2];
		tchar c4 = s[3];
		for(const tchar *bs = укз + from; bs < e; bs++) {
			tchar ch = *bs;
			if(ch == c1 || ch == c2 || ch == c3 || ch == c4)
				return (цел)(bs - укз);
		}
		return -1;
	}
	for(const tchar *bs = укз + from; bs < e; bs++)
		for(const tchar *ss = s; ss < se; ss++)
			if(*bs == *ss)
				return (цел)(bs - укз);
	return -1;
}

////////////

template <class T>
бцел дайНомТипаЗнач() { return номерТипаЗнач((T*)NULL); }

template <class T>
inline Ткст какТкст(const T& x)
{
	return x.вТкст();
}

template <class T>
inline Ткст какТкст(T *x)
{
	return фмтУк(x);
}

template <class T>
inline Ткст& operator<<(Ткст& s, const T& x)
{
	s.кат(какТкст(x));
	return s;
}

template<>
inline Ткст& operator<<(Ткст& s, кткст0  const &x)
{
	s.кат(x);
	return s;
}

template<>
inline Ткст& operator<<(Ткст& s, const Ткст &x)
{
	s.кат(x);
	return s;
}

template<>
inline Ткст& operator<<(Ткст& s, const сим& x)
{
	s.кат(x);
	return s;
}

template <class T>
inline Ткст& operator<<(Ткст&& s, const T& x)
{
	s.кат(какТкст(x));
	return s;
}

template<>
inline Ткст& operator<<(Ткст&& s, кткст0  const &x)
{
	s.кат(x);
	return s;
}

template<>
inline Ткст& operator<<(Ткст&& s, const Ткст &x)
{
	s.кат(x);
	return s;
}

template<>
inline Ткст& operator<<(Ткст&& s, const сим& x)
{
	s.кат(x);
	return s;
}

template<>
inline бул  пусто_ли(const Ткст& s)       { return s.пустой(); }

template<>
inline Ткст какТкст(const Ткст& s)     { return s; }

template<>
inline т_хэш дайХэшЗнач(const Ткст& s) { return s.дайХэшЗнач(); }

template <class T>
inline ТкстБуф& operator<<(ТкстБуф& s, const T& x)
{
	s.кат(какТкст(x));
	return s;
}

template<>
inline ТкстБуф& operator<<(ТкстБуф& s, кткст0  const &x)
{
	s.кат(x);
	return s;
}

template<>
inline ТкстБуф& operator<<(ТкстБуф& s, const Ткст &x)
{
	s.кат(x);
	return s;
}

template<>
inline ТкстБуф& operator<<(ТкстБуф& s, const сим& x)
{
	s.кат(x);
	return s;
}

template <class Делец>
inline
Ткст Ткст::сделай(цел alloc, Делец m)
{
	Ткст s;
	if(alloc <= 14) {
		цел len = m(s.chr);
		ПРОВЕРЬ(len <= alloc);
		s.мДлин() = len;
		s.Dsyn();
	}
	else {
		if(alloc < 32) {
			s.chr[РОД] = СРЕДНИЙ;
			s.укз = (сим *)разместиПам32();
		}
		else
			s.укз = s.размести(alloc, s.chr[РОД]);
		цел len = m(s.укз);
		ПРОВЕРЬ(len <= alloc);
		s.укз[len] = 0;
		s.бДлин() = len;
		s.мДлин() = 15;
		if(alloc >= 32 && alloc > 2 * len)
			s.сожми();
	}
	return s;
}

template <class T>
проц сырКат(Ткст& s, const T& x)
{
	s.кат((кткст0 )&x, sizeof(x));
}

template <class T>
проц сырКат(ТкстБуф& s, const T& x)
{
	s.кат((кткст0 )&x, sizeof(x));
}

#ifndef _HAVE_NO_STDWSTRING
inline std::wstring вСтдстр(const ШТкст& s)             { return s.вСтд(); }
#endif

template<>
inline бул  пусто_ли(const ШТкст& s)       { return s.пустой(); }

//template<>
//inline Ткст какТкст(const ШТкст& s)     { return s; }

template<>
inline т_хэш дайХэшЗнач(const ШТкст& s) { return хэшпам((кук)~s, 2 * s.дайДлину()); }

template<> inline Ткст какТкст(кткст0  const &s)    { return s; }
template<> inline Ткст какТкст(сим * const &s)          { return s; }
template<> inline Ткст какТкст(кткст0 s)            { return s; }
template<> inline Ткст какТкст(сим *s)                  { return s; }
template<> inline Ткст какТкст(const сим& a)            { return Ткст(a, 1); }
template<> inline Ткст какТкст(const байт& a)     { return Ткст(a, 1); }
template<> inline Ткст какТкст(const ббайт& a)   { return Ткст(a, 1); }
template<> inline Ткст какТкст(const бул& a)            { return a ? "true" : "false"; }
template<> inline Ткст какТкст(const std::string& s)     { return Ткст(s); }
template<> inline Ткст какТкст(const std::wstring& s)    { return ШТкст(s).вТкст(); }


template <class I>
КлассСубДиапазон<I> СубДиапазон(I l, I h)
{
	return КлассСубДиапазон<I>(l, h);
}

template <class I>
КлассСубДиапазон<I> СубДиапазон(I l, цел count)
{
	return КлассСубДиапазон<I>(l, count);
}

template <class C>
auto СубДиапазон(C&& c, цел pos, цел count) -> decltype(СубДиапазон(c.begin() + pos, count))
{
	return СубДиапазон(c.begin() + pos, count);
}

template <class T>
КлассКонстДиапазон<T> КонстДиапазон(const T& значение, цел count)
{
	return КлассКонстДиапазон<T>(значение, count);
}

template <class T>
КлассКонстДиапазон<T> КонстДиапазон(цел count)
{
	return КлассКонстДиапазон<T>(count);
}

template <class ДиапазонОснова>
КлассРеверсДиапазон<ДиапазонОснова> РеверсДиапазон(ДиапазонОснова&& r)
{
	return КлассРеверсДиапазон<ДиапазонОснова>(r);
}

template <class ДиапазонОснова>
КлассДиапазонОбзора<ДиапазонОснова> ДиапазонОбзора(ДиапазонОснова&& r, Вектор<цел>&& ndx)
{
	return КлассДиапазонОбзора<ДиапазонОснова>(r, пикуй(ndx));
}

template <class ДиапазонОснова, class Предикат>
КлассДиапазонОбзора<ДиапазонОснова> ДиапазонФильтр(ДиапазонОснова&& r, Предикат p)
{
	return КлассДиапазонОбзора<ДиапазонОснова>(r, найдиВсе(r, p));
}

template <class ДиапазонОснова, class Предикат>
КлассДиапазонОбзора<ДиапазонОснова> СортированныйДиапазон(ДиапазонОснова&& r, Предикат p)
{
	return КлассДиапазонОбзора<ДиапазонОснова>(r, дайСортПорядок(r, p));
}

template <class ДиапазонОснова>
КлассДиапазонОбзора<ДиапазонОснова> СортированныйДиапазон(ДиапазонОснова&& r)
{
	return КлассДиапазонОбзора<ДиапазонОснова>(r, дайСортПорядок(r));
}
/////////
//Algo.h
template <class Диапазон>
проц реверс(Диапазон&& r)
{
	auto start = r.begin();
	auto end = r.end();
	if(start != end && --end != start)
		do
			IterSwap(start, end);
		while(++start != end && --end != start);
}

template <class Диапазон, class Накопитель>
проц накопи(const Диапазон& r, Накопитель& a)
{
	for(const auto& e : r)
		a(e);
}

template <class Диапазон>
ТипЗначУ<Диапазон> сумма(const Диапазон& r, const ТипЗначУ<Диапазон>& ноль)
{
	ТипЗначУ<Диапазон> сум = ноль;
	for(const auto& e : r)
		сум += e;
	return сум;
}

template <class T>
ТипЗначУ<T> сумма(const T& c)
{
	return сумма(c, (ТипЗначУ<T>)0);
}

template <class Диапазон, class V>
цел счёт(const Диапазон& r, const V& знач)
{
	цел count = 0;
	for(const auto& x : r)
		if(x == знач)
			count++;
	return count;
}

template <class Диапазон, class Предикат>
цел счётЕсли(const Диапазон& r, const Предикат& p)
{
	цел count = 0;
	for(const auto& x : r)
		if(p(x))
			count++;
	return count;
}

template <class Диапазон, class Предикат>
цел найдиЛучший(const Диапазон& r, const Предикат& pred)
{
	цел count = r.дайСчёт();
	if(count == 0)
		return -1;
	auto *m = &r[0];
	цел mi = 0;
	for(цел i = 1; i < count; i++)
		if(pred(r[i], *m)) {
			m = &r[i];
			mi = i;
		}
	return mi;
}

template <class Диапазон>
цел найдиМин(const Диапазон& r)
{
	return найдиЛучший(r, std::less<ТипЗначУ<Диапазон>>());
}

template <class Диапазон>
const ТипЗначУ<Диапазон>& мин(const Диапазон& r)
{
	return r[найдиМин(r)];
}

template <class Диапазон>
const ТипЗначУ<Диапазон>& мин(const Диапазон& r, const ТипЗначУ<Диапазон>& опр)
{
	цел q = найдиМин(r);
	return q < 0 ? опр : r[q];
}

template <class Диапазон>
цел найдиМакс(const Диапазон& r)
{
	return найдиЛучший(r, std::greater<ТипЗначУ<Диапазон>>());
}

template <class Диапазон>
const ТипЗначУ<Диапазон>& макс(const Диапазон& r)
{
	return r[найдиМакс(r)];
}

template <class Диапазон>
const ТипЗначУ<Диапазон>& макс(const Диапазон& r, const ТипЗначУ<Диапазон>& опр)
{
	цел q = найдиМакс(r);
	return q < 0 ? опр : r[q];
}

template <class Диапазон1, class Диапазон2>
бул диапазоныРавны(const Диапазон1& a, const Диапазон2& b)
{
	if(a.дайСчёт() != b.дайСчёт())
		return false;
	for(цел i = 0; i < a.дайСчёт(); i++)
		if(!(a[i] == b[i]))
			return false;
	return true;
}

template <class Диапазон1, class Диапазон2, class Сравн>
цел сравниДиапазоны(const Диапазон1& a, const Диапазон2& b, Сравн сравни)
{
	цел n = min(a.дайСчёт(), b.дайСчёт());
	for(цел i = 0; i < n; i++) {
		цел q = сравни(a[i], b[i]);
		if(q)
			return q;
	}
	return сравни(a.дайСчёт(), b.дайСчёт());
}

template <class Диапазон1, class Диапазон2>
цел сравниДиапазоны(const Диапазон1& a, const Диапазон2& b)
{
	цел n = min(a.дайСчёт(), b.дайСчёт());
	for(цел i = 0; i < n; i++) {
		цел q = сравниЗнак(a[i], b[i]);
		if(q)
			return q;
	}
	return сравниЗнак(a.дайСчёт(), b.дайСчёт());
}

template <class Диапазон, class C>
цел найдиСовпадение(const Диапазон& r, const C& match, цел from = 0)
{
	for(цел i = from; i < r.дайСчёт(); i++)
		if(match(r[i])) return i;
	return -1;
}

template <class Диапазон, class V>
цел найдиИндекс(const Диапазон& r, const V& значение, цел from = 0)
{
	for(цел i = from; i < r.дайСчёт(); i++)
		if(r[i] == значение) return i;
	return -1;
}

template <class Диапазон, class Предикат>
Вектор<цел> найдиВсе(const Диапазон& r, Предикат match, цел from = 0)
{
	Вектор<цел> ndx;
	for(цел i = from; i < r.дайСчёт(); i++)
		if(match(r[i]))
			ndx.добавь(i);
	return ndx;
}

template <class Диапазон, class Предикат>
Вектор<цел> найдиВсеи(const Диапазон& r, Предикат match, цел from = 0)
{
	Вектор<цел> ndx;
	for(цел i = from; i < r.дайСчёт(); i++)
		if(match(i))
			ndx.добавь(i);
	return ndx;
}

template <class Диапазон, class T, class Less>
цел найдиНижнГран(const Диапазон& r, const T& знач, const Less& less)
{
	цел pos = 0;
	цел count = r.дайСчёт();
	while(count > 0) {
		цел half = count >> 1;
		цел m = pos + half;
		if(less(r[m], знач)) {
			pos = m + 1;
			count = count - half - 1;
		}
		else
			count = half;
	}
	return pos;
}

template <class Диапазон, class T>
цел найдиНижнГран(const Диапазон& r, const T& знач)
{
	return найдиНижнГран(r, знач, std::less<T>());
}

template <class Диапазон, class T, class L>
цел найдиВерхнГран(const Диапазон& r, const T& знач, const L& less)
{
	цел pos = 0;
	цел count = r.дайСчёт();
	while(count > 0) {
		цел half = count >> 1;
		цел m = pos + half;
		if(less(знач, r[m]))
			count = half;
		else {
			pos = m + 1;
			count = count - half - 1;
		}
	}
	return pos;
}

template <class Диапазон, class T>
цел найдиВерхнГран(const Диапазон& r, const T& знач)
{
	return найдиВерхнГран(r, знач, std::less<T>());
}

template <class Диапазон, class T, class L>
цел найдиБинар(const Диапазон& r, const T& знач, const L& less)
{
	цел i = найдиНижнГран(r, знач, less);
	return i < r.дайСчёт() && !less(знач, r[i]) ? i : -1;
}

template <class Диапазон, class T>
цел найдиБинар(const Диапазон& r, const T& знач)
{
	return найдиБинар(r, знач, std::less<T>());
}

template <class Контейнер, class T>
проц добавьЛру(Контейнер& lru, T значение, цел limit = 10)
{
	цел q = найдиИндекс(lru, значение);
	if(q >= 0)
		lru.удали(q);
	lru.вставь(0, значение);
	if(lru.дайСчёт() > limit)
		lru.устСчёт(limit);
}

template <class C = Вектор<цел>, class V>
C сделайЙоту(V end, V start = 0, V step = 1)
{
	C x;
	x.резервируй((end - start) / step);
	for(V i = start; i < end; i += step)
		x.добавь(i);
	return x;
}

template <class T, class... Арги>
T& Сингл(Арги... арги) {
	static T o(арги...);
	return o;
}

template <class T> // Workaround for GCC bug - specialization needed...
T& Сингл() {
	static T o;
	return o;
}

template <class T>
цел СтатичНомТипа() {
	static цел typeno = регистрируйНомТипа__(typeid(T).name());
	return typeno;
}

template <class T>
T& Митор<T>::дай(цел i) const
{
	ПРОВЕРЬ(i >= 0 && i < дайСчёт());
	return i == 0 ? *(T*)elem0 : (*const_cast<Вектор<T>*>(vector))[i - 1];
}

template <class T>
проц Митор<T>::подбери(Митор&& m)
{
	m.проверь();
	vector = m.vector;
	memcpy(&elem0, &m.elem0, sizeof(T));
	m.count = 2;
}

template <class T>
проц Митор<T>::копируй(const Митор& m)
{
	m.проверь();
	if(m.count > 0)
		стройГлубКопию(elem0, (const T*)m.elem0);
	if(m.count > 1)
		vector = new Вектор<T>(*m.vector, 1);
}

template <class T>
цел Митор<T>::дайСчёт() const
{
	проверь();
	return count > 1 ? vector->дайСчёт() + 1 : count;
}

template <class T>
T& Митор<T>::добавь()
{
	проверь();
	if(count == 0) {
		count = 1;
		return *new(elem0) T;
	}
	if(count == 1)
		vector = new Вектор<T>;
	return vector->добавь();
}

template <class T>
проц Митор<T>::добавь(const T& x)
{
	проверь();
	if(count == 0) {
		count = 1;
		new((T*) elem0) T(x);
	}
	else {
		if(count == 1)
			vector = new Вектор<T>;
		vector->добавь(x);
	}
}

template <class T>
проц Митор<T>::очисть()
{
	if(count > 2)
		delete vector;
	if(count && count != 2)
		((T*)elem0)->~T();
	count = 0;
}

template <class T>
проц Митор<T>::сожми()
{
	if(count > 2)
		vector->сожми();
}

template <typename Фн>
проц соФор(цел n, Фн iterator)
{
	std::atomic<цел> ii(0);
	соДелай([&] {
		for(цел i = ii++; i < n; i = ii++)
			iterator(i);
	});
}

template <typename Фн>
проц соФор_ОН(цел n, Фн iterator)
{
	for(цел i = 0; i < n; i++)
		iterator(i);
}

template <typename Фн>
проц соФор(бул co, цел n, Фн iterator)
{
	if(co)
		соФор(n, iterator);
	else
		соФор_ОН(n, iterator);
}

template< class Функция, class... Арги>
АсинхРабота<std::invoke_result_t<Функция, Арги...>>
Асинх(Функция&& f, Арги&&... арги)
{
	АсинхРабота<std::invoke_result_t<Функция, Арги...>> h;
	h.делай(f, арги...);
	return пикуй(h);
}

template <class C, class MC>
inline т_мера соЧанк__(C count, MC min_chunk = КО_ПАРТИЦ_МИН, MC max_chunk = КО_ПАРТИЦ_МАКС)
{
	цел n = min(count / цпбЯдра(), (C)max_chunk);
	return n < (C)min_chunk ? 0 : n;
}

template <class Обх, class Лямбда>
проц соОтсек(Обх begin, Обх end, const Лямбда& lambda, цел min_chunk = КО_ПАРТИЦ_МИН, цел max_chunk = КО_ПАРТИЦ_МАКС)
{
	т_мера chunk = соЧанк__(end - begin, min_chunk, max_chunk);
	if(chunk) {
		СоРабота co;
		while(begin < end) {
			Обх e = Обх(begin + мин(chunk, т_мера(end - begin)));
			co & [=, this] {
				lambda(begin, e);
			};
			begin = e;
		}
	}
	else
		lambda(begin, end);
}

template <class Диапазон, class Лямбда>
проц соОтсек(Диапазон&& r, const Лямбда& lambda)
{
	т_мера chunk = соЧанк__(r.дайСчёт(), КО_ПАРТИЦ_МИН);
	if(chunk) {
		СоРабота co;
		auto begin = r.begin();
		auto end = r.end();
		while(begin < end) {
			auto e = begin + мин(chunk, т_мера(end - begin));
			co & [=, this] {
				auto sr = СубДиапазон(begin, e); // we need l-значение
				lambda(sr);
			};
			begin = e;
		}
	}
	else
		lambda(СубДиапазон(r.begin(), r.end()));
}

template <class Диапазон, class Накопитель>
проц соНакопи(const Диапазон& r, Накопитель& result)
{
	typedef КонстОбходчикУ<Диапазон> I;
	соОтсек(r.begin(), r.end(),
		[=, &result](I i, I e) {
			Накопитель h;
			while(i < e)
				h(*i++);
			СоРабота::финБлок();
			result(h);
		}
	);
}

template <class Диапазон>
ТипЗначУ<Диапазон> соСумма(const Диапазон& r, const ТипЗначУ<Диапазон>& zero)
{
	typedef ТипЗначУ<Диапазон> VT;
	typedef КонстОбходчикУ<Диапазон> I;
	VT sum = zero;
	соОтсек(r.begin(), r.end(),
		[=, &sum](I i, I e) {
			VT h = zero;
			while(i < e)
				h += *i++;
			СоРабота::финБлок();
			sum += h;
		}
	);
	return sum;
}

template <class T>
ТипЗначУ<T> соСумма(const T& c)
{
	return соСумма(c, (ТипЗначУ<T>)0);
}

template <class Диапазон, class V>
цел соСчёт(const Диапазон& r, const V& знач)
{
	цел count = 0;
	соОтсек(r, [=, &знач, &count](const СубДиапазонУ<const Диапазон>& r) {
		цел n = счёт(r, знач);
		СоРабота::финБлок();
		count += n;
	});
	return count;
}

template <class Диапазон, class Предикат>
цел соСчётЕсли(const Диапазон& r, const Предикат& p)
{
	цел count = 0;
	соОтсек(r, [=, &p, &count](const СубДиапазонУ<const Диапазон>& r) {
		цел n = счётЕсли(r, p);
		СоРабота::финБлок();
		count += n;
	});
	return count;
}

template <class Диапазон, class Better>
цел соНайдиЛучш(const Диапазон& r, const Better& better)
{
	if(r.дайСчёт() <= 0)
		return -1;
	typedef КонстОбходчикУ<Диапазон> I;
	I best = r.begin();
	соОтсек(r.begin() + 1, r.end(),
		[=, &best](I i, I e) {
			I b = i++;
			while(i < e) {
				if(better(*i, *b))
					b = i;
				i++;
			}
			СоРабота::финБлок();
			if(better(*b, *best) || !better(*best, *b) && b < best)
				best = b;
		}
	);
	return best - r.begin();
}

template <class Диапазон>
цел соНайдиМин(const Диапазон& r)
{
	return соНайдиЛучш(r, std::less<ТипЗначУ<Диапазон>>());
}

template <class Диапазон>
const ТипЗначУ<Диапазон>& соМин(const Диапазон& r)
{
	return r[соНайдиМин(r)];
}

template <class Диапазон>
const ТипЗначУ<Диапазон>& соМин(const Диапазон& r, const ТипЗначУ<Диапазон>& опр)
{
	цел q = соНайдиМин(r);
	return q < 0 ? опр : r[q];
}

template <class Диапазон>
цел соНайдиМакс(const Диапазон& r)
{
	return соНайдиЛучш(r, std::greater<ТипЗначУ<Диапазон>>());
}

template <class Диапазон>
const ТипЗначУ<Диапазон>& соМакс(const Диапазон& r)
{
	return r[соНайдиМакс(r)];
}

template <class Диапазон>
const ТипЗначУ<Диапазон>& соМакс(const Диапазон& r, const ТипЗначУ<Диапазон>& опр)
{
	цел q = соНайдиМакс(r);
	return q < 0 ? опр : r[q];
}

template <class Диапазон, class Свер>
цел соНайдиСовпад(const Диапазон& r, const Свер& eq, цел from = 0)
{
	цел count = r.дайСчёт();
	std::atomic<цел> found;
	found = count;
	соОтсек(from, count,
		[=, &r, &found](цел i, цел e) {
			for(; i < e; i++) {
				if(found < i)
					break;
				if(eq(r[i])) {
					СоРабота::финБлок();
					if(i < found)
						found = i;
					return;
				}
			}
		}
	);
	return found < (цел)count ? (цел)found : -1;
}

template <class Диапазон, class V>
цел соНайдиИндекс(const Диапазон& r, const V& значение, цел from = 0)
{
	return соНайдиСовпад(r, [=, &значение](const ТипЗначУ<Диапазон>& m) { return m == значение; }, from);
}

template <class Диапазон1, class Диапазон2>
цел соДиапазоныРавны(const Диапазон1& r1, const Диапазон2& r2)
{
	if(r1.дайСчёт() != r2.дайСчёт())
		return false;
	std::atomic<бул> equal(true);
	соОтсек(0, r1.дайСчёт(),
		[=, &equal, &r1, &r2](цел i, цел e) {
			while(i < e && equal) {
				if(r1[i] != r2[i]) {
					СоРабота::финБлок();
					equal = false;
				}
				i++;
			}
		}
	);
	return equal;
}

template <class Диапазон, class Предикат>
Вектор<цел> соНайдиВсе(const Диапазон& r, Предикат match, цел from = 0)
{
	Вектор<Вектор<цел>> rs;
	цел total = 0;
	соОтсек(from, r.дайСчёт(), [=, &r, &rs, &total](цел begin, цел end) {
		Вектор<цел> v = найдиВсе(СубДиапазон(r, 0, end), match, begin);
		СоРабота::финБлок();
		if(v.дайСчёт()) {
			total += v.дайСчёт();
			rs.добавь(пикуй(v));
		}
	});
	соСортируй(rs, [](const Вектор<цел>& a, const Вектор<цел>& b) { return a[0] < b[0]; });
	Вектор<цел> result;
	for(const auto& s : rs)
		result.приставь(s);
	return result;
}

template <class T>
inline Поток& operator%(Поток& s, T& x)
{
	x.сериализуй(s);
	return s;
}

template <class T>
inline Поток& operator<<(Поток& s, const T& x) {
	s << какТкст(x);
	return s;
}

template <class T>
Ткст Ук<T>::вТкст() const
{
	return prec ? фмтУк(дай()) : Ткст("0x0");
}

template <class T>
проц вДжейсон(ДжейсонВВ& io, T& var)
{
	var.вДжейсон(io);
}

template <class T>
ДжейсонВВ& ДжейсонВВ::operator()(кткст0 ключ, T& значение)
{
	if(грузится()) {
		const Значение& v = (*ист)[ключ];
		if(!v.проц_ли()) {
			ДжейсонВВ jio(v);
			вДжейсон(jio, значение);
		}
	}
	else {
		ПРОВЕРЬ(tgt.проц_ли());
		if(!map)
			map.создай();
		ДжейсонВВ jio;
		вДжейсон(jio, значение);
		if(jio.map)
			map->добавь(ключ, *jio.map);
		else
			map->добавь(ключ, jio.tgt);
	}
	return *this;
}

template <class T, class X>
ДжейсонВВ& ДжейсонВВ::Var(кткст0 ключ, T& значение, X jsonize)
{
	if(грузится()) {
		const Значение& v = (*ист)[ключ];
		if(!v.проц_ли()) {
			ДжейсонВВ jio(v);
			jsonize(jio, значение);
		}
	}
	else {
		ПРОВЕРЬ(tgt.проц_ли());
		if(!map)
			map.создай();
		ДжейсонВВ jio;
		jsonize(jio, значение);
		if(jio.map)
			map->добавь(ключ, *jio.map);
		else
			map->добавь(ключ, jio.tgt);
	}
	return *this;
}


template <class T, class X>
проц джейсонируйМассив(ДжейсонВВ& io, T& array, X item_jsonize)
{
	if(io.грузится()) {
		const Значение& va = io.дай();
		array.устСчёт(va.дайСчёт());
		for(цел i = 0; i < va.дайСчёт(); i++) {
			ДжейсонВВ jio(va[i]);
			item_jsonize(jio, array[i]);
		}
	}
	else {
		Вектор<Значение> va;
		va.устСчёт(array.дайСчёт());
		for(цел i = 0; i < array.дайСчёт(); i++) {
			ДжейсонВВ jio;
			item_jsonize(jio, array[i]);
			jio.помести(va[i]);
		}
		io.уст(МассивЗнач(пикуй(va)));
	}
}

template <class T, class X> ДжейсонВВ& ДжейсонВВ::Массив(кткст0 ключ, T& значение, X item_jsonize, кткст0 )
{
	if(грузится()) {
		const Значение& v = (*ист)[ключ];
		if(!v.проц_ли()) {
			ДжейсонВВ jio(v);
			джейсонируйМассив(jio, значение, item_jsonize);
		}
	}
	else {
		ПРОВЕРЬ(tgt.проц_ли());
		if(!map)
			map.создай();
		ДжейсонВВ jio;
		джейсонируйМассив(jio, значение, item_jsonize);
		if(jio.map)
			map->добавь(ключ, *jio.map);
		else
			map->добавь(ключ, jio.tgt);
	}
	return *this;
}

template <class T>
ДжейсонВВ& ДжейсонВВ::operator()(кткст0 ключ, T& значение, const T& defvalue)
{
	if(грузится()) {
		const Значение& v = (*ист)[ключ];
		if(v.проц_ли())
			значение = defvalue;
		else {
			ДжейсонВВ jio(v);
			вДжейсон(jio, значение);
		}
	}
	else {
		ПРОВЕРЬ(tgt.проц_ли());
		if(!map)
			map.создай();
		ДжейсонВВ jio;
		вДжейсон(jio, значение);
		if(jio.map)
			map->добавь(ключ, *jio.map);
		else
			map->добавь(ключ, jio.tgt);
	}
	return *this;
}

template <class T>
Значение сохраниКакЗначДжейсон(const T& var)
{
	ДжейсонВВ io;
	вДжейсон(io, const_cast<T&>(var));
	return io.дайРез();
}

template <class T>
проц загрузиИзЗначДжейсон(T& var, const Значение& x)
{
	ДжейсонВВ io(x);
	вДжейсон(io, var);
}

template <class T>
Ткст сохраниКакДжейсон(const T& var, бул pretty = false)
{
	return какДжейсон(сохраниКакЗначДжейсон(var), pretty);
}

template <class T>
бул загрузиИзДжейсон(T& var, кткст0 json)
{
	try {
		Значение jv = разбериДжейсон(json);
		if(jv.ошибка_ли())
			return false;
		загрузиИзЗначДжейсон(var, jv);
	}
	catch(ОшибкаТипаЗначения) {
		return false;
	}
	catch(ОшДжейсонизации) {
		return false;
	}
	return true;
}

template <class T>
бул сохраниКакФайлДжейсон(const T& var, кткст0 file = NULL, бул pretty = false)
{
	return сохраниФайл(sДжейсонFile(file), сохраниКакДжейсон(var, pretty));;
}

template <class T>
бул загрузиИзФайлаДжейсон(T& var, кткст0 file = NULL)
{
	return загрузиИзДжейсон(var, загрузиФайл(sДжейсонFile(file)));
}

template<> проц вДжейсон(ДжейсонВВ& io, цел& var);
template<> проц вДжейсон(ДжейсонВВ& io, ббайт& var);
template<> проц вДжейсон(ДжейсонВВ& io, крат& var);
template<> проц вДжейсон(ДжейсонВВ& io, дол& var);
template<> проц вДжейсон(ДжейсонВВ& io, дво& var);
template<> проц вДжейсон(ДжейсонВВ& io, бул& var);
template<> проц вДжейсон(ДжейсонВВ& io, Ткст& var);
template<> проц вДжейсон(ДжейсонВВ& io, ШТкст& var);
template<> проц вДжейсон(ДжейсонВВ& io, Дата& var);
template<> проц вДжейсон(ДжейсонВВ& io, Время& var);

template <class T>
проц джейсонируйМассив(ДжейсонВВ& io, T& array)
{
	джейсонируйМассив(io, array, [](ДжейсонВВ& io, ТипЗначУ<T>& элт) { вДжейсон(io, элт); });
}

template <class T, class K, class V>
проц джейсонируйМап(ДжейсонВВ& io, T& map, кткст0 keyid, кткст0 valueid)
{
	if(io.грузится()) {
		map.очисть();
		const Значение& va = io.дай();
		map.резервируй(va.дайСчёт());
		for(цел i = 0; i < va.дайСчёт(); i++) {
			K ключ;
			V значение;
			загрузиИзЗначДжейсон(ключ, va[i][keyid]);
			загрузиИзЗначДжейсон(значение, va[i][valueid]);
			map.добавь(ключ, пикуй(значение));
		}
	}
	else {
		Вектор<Значение> va;
		va.устСчёт(map.дайСчёт());
		for(цел i = 0; i < map.дайСчёт(); i++)
			if(!map.отлинкован(i)) {
				МапЗнач элт;
				элт.добавь(keyid, сохраниКакЗначДжейсон(map.дайКлюч(i)));
				элт.добавь(valueid, сохраниКакЗначДжейсон(map[i]));
				va[i] = элт;
			}
		io.уст(МассивЗнач(пикуй(va)));
	}
}

template <class T, class K, class V>
проц джейсонируйОтсортМап(ДжейсонВВ& io, T& map, кткст0 keyid, кткст0 valueid)
{
	if(io.грузится()) {
		map.очисть();
		const Значение& va = io.дай();
		for(цел i = 0; i < va.дайСчёт(); i++) {
			K ключ;
			V значение;
			загрузиИзЗначДжейсон(ключ, va[i][keyid]);
			загрузиИзЗначДжейсон(значение, va[i][valueid]);
			map.добавь(ключ, пикуй(значение));
		}
	}
	else {
		Вектор<Значение> va;
		va.устСчёт(map.дайСчёт());
		for(цел i = 0; i < map.дайСчёт(); i++) {
			МапЗнач элт;
			элт.добавь(keyid, сохраниКакЗначДжейсон(map.дайКлюч(i)));
			элт.добавь(valueid, сохраниКакЗначДжейсон(map[i]));
			va[i] = элт;
		}
		io.уст(МассивЗнач(пикуй(va)));
	}
}

template <class T, class K, class V>
проц джейсонируйТкстМап(ДжейсонВВ& io, T& map)
{
	if(io.грузится()) {
		map.очисть();
		const МапЗнач& va = io.дай();
		map.резервируй(va.дайСчёт());
		for(цел i = 0; i < va.дайСчёт(); i++) {
			V значение;
			Ткст ключ = va.дайКлюч(i);
			загрузиИзЗначДжейсон(ключ, va.дайКлюч(i));
			загрузиИзЗначДжейсон(значение, va.дайЗначение(i));
			map.добавь(ключ, пикуй(значение));
		}
	}
	else {
		Индекс<Значение>  индекс;
		Вектор<Значение> values;
		индекс.резервируй(map.дайСчёт());
		values.резервируй(map.дайСчёт());
		for (цел i=0; i<map.дайСчёт(); ++i)
		{
			индекс.добавь(сохраниКакЗначДжейсон(map.дайКлюч(i)));
			values.добавь(сохраниКакЗначДжейсон(map[i]));
		}
		МапЗнач vm(пикуй(индекс), пикуй(values));
		io.уст(vm);
	}
}

template <class K, class V>
проц ткстМап(ДжейсонВВ& io, ВекторМап<K, V>& map)
{
	джейсонируйТкстМап<ВекторМап<K, V>, K, V>(io, map);
}

template <class K, class V>
проц ткстМап(ДжейсонВВ& io, МассивМап<K, V>& map)
{
	джейсонируйТкстМап<МассивМап<K, V>, K, V>(io, map);
}

template <class T, class V>
проц джейсонируйИндекс(ДжейсонВВ& io, T& индекс)
{
	if(io.грузится()) {
		const Значение& va = io.дай();
		индекс.резервируй(va.дайСчёт());
		for(цел i = 0; i < va.дайСчёт(); i++) {
			V v;
			загрузиИзЗначДжейсон(v, va[i]);
			индекс.добавь(пикуй(v));
		}
	}
	else {
		Вектор<Значение> va;
		for(цел i = 0; i < индекс.дайСчёт(); i++)
			if(!индекс.отлинкован(i))
				va.добавь(сохраниКакЗначДжейсон(индекс[i]));
		io.уст(МассивЗнач(пикуй(va)));
	}
}

template <class T>
проц джейсонируйСериализуя(ДжейсонВВ& jio, T& x)
{
	Ткст h;
	if(jio.сохраняется())
	   h = гексТкст(сохраниКакТкст(x));
	jio("data", h);
	if(jio.грузится())
		try {
			грузиИзТкст(x, сканГексТкст(h));
		}
		catch(ОшЗагрузки) {
			throw ОшДжейсонизации("Ошибка джейсонизации сериализуя");
		}
}

template <class T>
бцел дайНомТипаЗнач() { return номерТипаЗнач((T*)NULL); }

template <class T> бул  вмещаетсяВЗначСво()                    { return sizeof(T) <= 8; }
template <class T> Значение своВЗнач(const T& x)            { return Значение(x, Значение::МАЛЫЙ); }

template <class T> Значение богатыйВЗнач(const T& данные);

template <class T> Значение сырВЗнач(const T& данные);
template <class T> Значение сырПикуйВЗнач(T&& данные);
template <class T> Значение сырГлубВЗнач(const T& данные);
template <class T> T&    создайСырЗнач(Значение& v);

template <class T>
inline бул полиРавны(const T& x, const Значение& v) {
	return false;
}

template <class T>
inline т_хэш дайХэшЗначЗначения(const T& x) {
	return дайХэшЗнач(x);
}

template<> inline бул  пусто_ли(const Дата& d)    { return d.year == -32768; }

template<>
inline Ткст какТкст(const Дата& date) { return фмт(date); }

template<> inline бул  пусто_ли(const Время& t)    { return t.year == -32768; }

template<>
inline Ткст какТкст(const Время& time) { return фмт(time); }

template <> Ткст какТкст(const КЗСА& c);

template<> inline т_хэш дайХэшЗнач(const КЗСА& a) { return *(бцел *)&a; }

template<>
Ткст какТкст(const Цвет& c);

template<> inline бул пусто_ли(const Комплекс& r) { return пусто_ли(r.real()) || пусто_ли(r.imag()); }
template<> inline т_хэш дайХэшЗнач(const Комплекс& x) { return КомбХэш(x.real(), x.imag()); }
template<> inline Ткст какТкст(const std::complex<дво>& x) { return Ткст().кат() << "(" << x.real() << "," << x.imag() << ")"; }
template<> inline Ткст какТкст(const Комплекс& x) { return какТкст((const std::complex<дво>&)x); }

template<> inline бцел номерТипаЗнач(const Комплекс*) { return COMPLEX_V; }

template<>
inline бул полиРавны(const Комплекс& x, const Значение& v) {
	return число_ли(v) && x.imag() == 0 && x.real() == (дво)v;
}

template<>
inline цел  полиСравни(const Комплекс& a, const Значение& b)
{
	НИКОГДА(); return 0;
}

template<>
Ткст какТкст(const МассивЗнач& v);

template <class T>
inline T              Nvl(T a, T b, T c)                       { return Nvl(Nvl(a, b), c); }

template <class T>
inline T              Nvl(T a, T b, T c, T d)                  { return Nvl(Nvl(a, b), c, d); }

template <class T>
inline T              Nvl(T a, T b, T c, T d, T e)             { return Nvl(Nvl(a, b), c, d, e); }

template <typename F, typename S, class R>
const auto& лямбдаПреобр(F формат, S scan, R фильтр)
{
	static КлассЛямбдаПреобр<F, S, R> x(формат, scan, фильтр);
	return x;
}

template <typename F, typename S>
const auto& лямбдаПреобр(F формат, S scan)
{
	return лямбдаПреобр(формат, scan, [](цел ch) { return ch; });
}

template <typename F>
const auto& лямбдаПреобр(F формат)
{
	return лямбдаПреобр(формат, [](const Значение& v) { return v; });
}

///////////////////////////////
// Dumping templates

template <class T>
проц дампКонтейнер(Поток& s, T укз, T end) {
	цел i = 0;
	s << LOG_BEGIN;
	while(укз != end)
		s << '[' << i++ << "] = " << *укз++ << EOL;
	s << LOG_END;
}

template <class C>
проц дампКонтейнер(Поток& s, const C& c) {
	дампКонтейнер(s, c.begin(), c.end());
}

template <class T>
проц дампКонтейнер2(Поток& s, T укз, T end) {
	цел i = 0;
	s << LOG_BEGIN;
	while(укз != end) {
		s << '[' << i++ << "] =" << EOL;
		дампКонтейнер(s, (*укз).begin(), (*укз).end());
		укз++;
	}
	s << LOG_END;
}

template <class C>
проц дампКонтейнер2(Поток& s, const C& c) {
	дампКонтейнер2(s, c.begin(), c.end());
}

template <class T>
проц дампКонтейнер3(Поток& s, T укз, T end) {
	цел i = 0;
	s << LOG_BEGIN;
	while(укз != end) {
		s << '[' << i++ << "] =" << EOL;
		дампКонтейнер2(s, (*укз).begin(), (*укз).end());
		укз++;
	}
	s << LOG_END;
}

template <class C>
проц дампКонтейнер3(Поток& s, const C& c) {
	дампКонтейнер3(s, c.begin(), c.end());
}

template <class T>
проц дампМап(Поток& s, const T& t) {
	s << LOG_BEGIN;
	for(цел i = 0; i < t.дайСчёт(); i++)
		s << '[' << i << "] = (" << t.дайКлюч(i) << ") " << t[i] << EOL;
	s << LOG_END;
}

template <class IO, class IZE>
проц LambdaIze(IO& io, кткст0 ид, IZE ize)
{
	LambdaIzeVar<IZE> var(ize);
	io(ид, var);
}

template <class T>
T& дайРеф(Реф r, T *x = NULL) {
	ПРОВЕРЬ(дайНомТипаЗнач<T>() == r.дайТип());
	return *(T *) r.дайПроцУк();
}

template <class T>
Реф какРеф(T& x) {
	return Реф(&x, &Сингл< СтдРеф<T> >());
}

template <class T, бцел тип, class B>
ТипЗнач<T, тип, B>::operator РефНаТипЗнач()
{
	РефНаТипЗнач h;
	h.укз = this;
	h.m = &Сингл< СтдРеф<T> >();
	return h;
}


template <class T>
проц обнули(T& obj)
{
	::memset(&obj, 0, sizeof(obj));
}

template <class T>
T& перестрой(T& object)
{
	object.~T();
	::new(&object) T;
	return object;
}

template <class T>
inline проц Dbl_Unlink(T *x)
{
	x->prev->next = x->next; x->next->prev = x->prev;
}

template <class T>
inline проц Dbl_LinkAfter(T *x, T *lnk)
{
	x->prev = lnk; x->next = lnk->next; x->next->prev = x; lnk->next = x;
}

template <class T>
inline проц Dbl_Self(T *x)
{
	x->prev = x->next = x;
}

template <class T>
проц сериализуйТФн(Поток &s, T *x)
{
	s % *x;
}

template <class T>
Событие<Поток&> сериализуйБк(T& x)
{
	return callback1(сериализуйТФн<T>, &x);
}

template <class T>
бул грузи(T& x, Поток& s, цел версия = Null) {
	return грузи(сериализуйБк(x), s, версия);
}

template <class T>
бул сохрани(T& x, Поток& s, цел версия = Null) {
	return сохрани(сериализуйБк(x), s, версия);
}

template <class T>
бул грузиИзФайла(T& x, кткст0 имя = NULL, цел версия = Null) {
	return грузиИзФайла(сериализуйБк(x), имя, версия);
}

template <class T>
бул сохраниВФайл(T& x, кткст0 имя = NULL, цел версия = Null) {
	return сохраниВФайл(сериализуйБк(x), имя, версия);
}

template <class T>
Ткст сохраниКакТкст(T& x) {
	ТкстПоток ss;
	сохрани(x, ss);
	return ss;
}

template <class T>
бул грузиИзТкст(T& x, const Ткст& s) {
	ТкстПоток ss(s);
	return грузи(x, ss);
}

template <class T>
бул грузиИзГлоба(T& x, кткст0 имя)
{
	ТкстПоток ss(дайДанныеГлобКонфига(имя));
	return ss.кф_ли() || грузи(x, ss);
}

template <class T>
проц сохраниВГлоб(T& x, кткст0 имя)
{
	ТкстПоток ss;
	сохрани(x, ss);
	устДанныеГлобКонфига(имя, ss);
}

template <class T>
т_хэш хэшПоСериализу(const T& cont)
{
	ТаймСтоп tm;
	ххХэшПоток xxh;
	const_cast<T&>(cont).сериализуй(xxh);
	return xxh.финиш();
}

template <class T>
т_мера размерПоСериализу(const T& cont)
{
	ТаймСтоп tm;
	РазмПоток szs;
	const_cast<T&>(cont).сериализуй(szs);
	return (т_мера)szs;
}

template <class T>
бул равныПоСериализу(const T& a, const T& b)
{
	ТкстПоток sa, sb;
	const_cast<T&>(a).сериализуй(sa);
	const_cast<T&>(b).сериализуй(sb);
	return sa.дайРез() == sb.дайРез();
}

template<>
inline Ткст какТкст(const Ууид& ид) { return фмт(ид); }
////////
#endif
