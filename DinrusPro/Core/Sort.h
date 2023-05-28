template <class I, class Less>
force_inline
проц OrderIter2__(I a, I b, const Less& less)
{
	if(less(*b, *a))
		IterSwap(a, b);
}

template <class I, class Less>
force_inline // 2-3 compares, 0-2 swaps
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
force_inline // 3-6 compares, 0-5 swaps
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
force_inline // 4-10 compares, 0-9 swaps
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
force_inline // 9-15 compares, 0-14 swaps
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
force_inline // 16-26 compares, 0-20 swaps
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

бцел  случ(бцел n);

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
	Sort__(c.begin(), c.end(), std::less<типЗначУ<Диапазон>>());
}

template <class T>
struct StableSortItem__ {
	const T& значение;
	цел      индекс;

	StableSortItem__(const T& значение, цел индекс) : значение(значение), индекс(индекс) {}
};

template <class II, class T>
struct StableSortIterator__ {
	II          ii;
	цел        *vi;

	typedef StableSortIterator__<II, T> Обх;

	Обх&       operator ++ ()               { ++ii; ++vi; return *this; }
	Обх&       operator -- ()               { --ii; --vi; return *this; }
	Обх        operator +  (цел i) const    { return Обх(ii + i, vi + i); }
	Обх        operator -  (цел i) const    { return Обх(ii - i, vi - i); }
	цел         operator -  (Обх b) const   { return (цел)(ii - b.ii); }
	бул        operator == (Обх b) const   { return ii == b.ii; }
	бул        operator != (Обх b) const   { return ii != b.ii; }
	бул        operator <  (Обх b) const   { return ii <  b.ii; }
	бул        operator <= (Обх b) const   { return ii <=  b.ii; }

	StableSortItem__<T> operator*() const    { return StableSortItem__<T>(*ii, *vi); }

	friend проц IterSwap(Обх a, Обх b)     { IterSwap(a.ii, b.ii); IterSwap(a.vi, b.vi); }

	StableSortIterator__(II ii, цел *vi) : ii(ii), vi(vi) {}
};

template <class T, class Less>
struct StableSortLess__ {
	const Less& less;

	бул operator()(const StableSortItem__<T>& a, const StableSortItem__<T>& b) const {
		if(less(a.значение, b.значение)) return true;
		return less(b.значение, a.значение) ? false : a.индекс < b.индекс;
	}

	StableSortLess__(const Less& less) : less(less) {}
};

template <class Диапазон, class Less>
проц StableSort(Диапазон&& r, const Less& less)
{
	auto begin = r.begin();
	auto end = r.end();
	typedef типЗначУ<Диапазон> VT;
	typedef decltype(begin) I;
	цел count = (цел)(uintptr_t)(end - begin);
	Буфер<цел> h(count);
	for(цел i = 0; i < count; i++)
		h[i] = i;
	Sort__(StableSortIterator__<I, VT>(begin, ~h), StableSortIterator__<I, VT>(end, ~h + count),
	       StableSortLess__<VT, Less>(less));
}

template <class Диапазон>
проц StableSort(Диапазон&& r)
{
	StableSort(r, std::less<типЗначУ<Диапазон>>());
}

template <class II, class VI, class K>
struct ИндексСортОбходчик__
{
	typedef ИндексСортОбходчик__<II, VI, K> Обх;

	ИндексСортОбходчик__(II ii, VI vi) : ii(ii), vi(vi) {}

	Обх&       operator ++ ()               { ++ii; ++vi; return *this; }
	Обх&       operator -- ()               { --ii; --vi; return *this; }
	const K&    operator *  () const         { return *ii; }
	Обх        operator +  (цел i) const    { return Обх(ii + i, vi + i); }
	Обх        operator -  (цел i) const    { return Обх(ii - i, vi - i); }
	цел         operator -  (Обх b) const   { return (цел)(ii - b.ii); }
	бул        operator == (Обх b) const   { return ii == b.ii; }
	бул        operator != (Обх b) const   { return ii != b.ii; }
	бул        operator <  (Обх b) const   { return ii <  b.ii; }
	бул        operator <= (Обх b) const   { return ii <=  b.ii; }
	friend проц IterSwap    (Обх a, Обх b) { IterSwap(a.ii, b.ii); IterSwap(a.vi, b.vi); }

	II          ii;
	VI          vi;
};

template <class МастерДиапазон, class Диапазон2, class Less>
проц IndexSort(МастерДиапазон&& r, Диапазон2&& r2, const Less& less)
{
	ПРОВЕРЬ(r.дайСчёт() == r2.дайСчёт());
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef типЗначУ<МастерДиапазон> VT;
	if(r.дайСчёт() == 0)
		return;
	Sort__(ИндексСортОбходчик__<I, I2, VT>(r.begin(), r2.begin()),
	       ИндексСортОбходчик__<I, I2, VT>(r.end(), r2.end()),
		   less);
}

template <class МастерДиапазон, class Диапазон2>
проц IndexSort(МастерДиапазон&& r, Диапазон2&& r2)
{
	IndexSort(r, r2, std::less<типЗначУ<МастерДиапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Less>
проц StableIndexSort(МастерДиапазон&& r, Диапазон2&& r2, const Less& less)
{
	ПРОВЕРЬ(r.дайСчёт() == r2.дайСчёт());
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef типЗначУ<МастерДиапазон> VT;
	if(r.дайСчёт() == 0)
		return;
	StableSort(СубДиапазон(ИндексСортОбходчик__<I, I2, VT>(r.begin(), r2.begin()),
		                ИндексСортОбходчик__<I, I2, VT>(r.end(), r2.end())).пиши(),
	           less);
}

template <class МастерДиапазон, class Диапазон2>
проц StableIndexSort(МастерДиапазон&& r, Диапазон2&& r2)
{
	StableIndexSort(r, r2, std::less<типЗначУ<МастерДиапазон>>());
}

template <class II, class VI, class WI, class K>
struct ИндексСорт2Обходчик__
{
	typedef ИндексСорт2Обходчик__<II, VI, WI, K> Обх;

	ИндексСорт2Обходчик__(II ii, VI vi, WI wi) : ii(ii), vi(vi), wi(wi) {}

	Обх&       operator ++ ()               { ++ii; ++vi; ++wi; return *this; }
	Обх&       operator -- ()               { --ii; --vi; --wi; return *this; }
	const K&    operator *  () const         { return *ii; }
	Обх        operator +  (цел i) const    { return Обх(ii + i, vi + i, wi + i); }
	Обх        operator -  (цел i) const    { return Обх(ii - i, vi - i, wi - i); }
	цел         operator -  (Обх b) const   { return (цел)(ii - b.ii); }
	бул        operator == (Обх b) const   { return ii == b.ii; }
	бул        operator != (Обх b) const   { return ii != b.ii; }
	бул        operator <  (Обх b) const   { return ii <  b.ii; }
	бул        operator <= (Обх b) const   { return ii <=  b.ii; }
	friend проц IterSwap    (Обх a, Обх b) { IterSwap(a.ii, b.ii); IterSwap(a.vi, b.vi); IterSwap(a.wi, b.wi); }

	II          ii;
	VI          vi;
	WI          wi;
};

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Less>
проц IndexSort2(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, const Less& less)
{
	ПРОВЕРЬ(r.дайСчёт() == r2.дайСчёт());
	ПРОВЕРЬ(r.дайСчёт() == r3.дайСчёт());
	if(r.дайСчёт() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef типЗначУ<МастерДиапазон> VT;
	Sort__(ИндексСорт2Обходчик__<I, I2, I3, VT>(r.begin(), r2.begin(), r3.begin()),
	       ИндексСорт2Обходчик__<I, I2, I3, VT>(r.end(), r2.end(), r3.end()),
		   less);
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3>
проц IndexSort2(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3)
{
	IndexSort2(r, r2, r3, std::less<типЗначУ<МастерДиапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Less>
проц StableIndexSort2(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, const Less& less)
{
	ПРОВЕРЬ(r.дайСчёт() == r2.дайСчёт());
	ПРОВЕРЬ(r.дайСчёт() == r3.дайСчёт());
	if(r.дайСчёт() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef типЗначУ<МастерДиапазон> VT;
	StableSort(СубДиапазон(ИндексСорт2Обходчик__<I, I2, I3, VT>(r.begin(), r2.begin(), r3.begin()),
	                    ИндексСорт2Обходчик__<I, I2, I3, VT>(r.end(), r2.end(), r3.end())).пиши(),
		       less);
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3>
проц StableIndexSort2(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3)
{
	StableIndexSort2(r, r2, r3, std::less<типЗначУ<МастерДиапазон>>());
}

template <class II, class VI, class WI, class XI, class K>
struct ИндексСорт3Обходчик__
{
	typedef ИндексСорт3Обходчик__<II, VI, WI, XI, K> Обх;

	ИндексСорт3Обходчик__(II ii, VI vi, WI wi, XI xi) : ii(ii), vi(vi), wi(wi), xi(xi) {}

	Обх&       operator ++ ()               { ++ii; ++vi; ++wi; ++xi; return *this; }
	Обх&       operator -- ()               { --ii; --vi; --wi; --xi; return *this; }
	const K&    operator *  () const         { return *ii; }
	Обх        operator +  (цел i) const    { return Обх(ii + i, vi + i, wi + i, xi + i); }
	Обх        operator -  (цел i) const    { return Обх(ii - i, vi - i, wi - i, xi - i); }
	цел         operator -  (Обх b) const   { return (цел)(ii - b.ii); }
	бул        operator == (Обх b) const   { return ii == b.ii; }
	бул        operator != (Обх b) const   { return ii != b.ii; }
	бул        operator <  (Обх b) const   { return ii <  b.ii; }
	бул        operator <= (Обх b) const   { return ii <=  b.ii; }
	friend проц IterSwap    (Обх a, Обх b) { IterSwap(a.ii, b.ii); IterSwap(a.vi, b.vi); IterSwap(a.wi, b.wi); IterSwap(a.xi, b.xi); }

	II          ii;
	VI          vi;
	WI          wi;
	XI          xi;
};

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Диапазон4, class Less>
проц IndexSort3(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, Диапазон4&& r4, const Less& less)
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
	typedef типЗначУ<МастерДиапазон> VT;
	Sort__(ИндексСорт3Обходчик__<I, I2, I3, I4, VT>(r.begin(), r2.begin(), r3.begin(), r4.begin()),
	       ИндексСорт3Обходчик__<I, I2, I3, I4, VT>(r.end(), r2.end(), r3.end(), r4.end()),
		   less);
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Диапазон4>
проц IndexSort3(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, Диапазон4&& r4)
{
	IndexSort3(r, r2, r3, r4, std::less<типЗначУ<МастерДиапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Диапазон4, class Less>
проц StableIndexSort3(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, Диапазон4&& r4, const Less& less)
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
	typedef типЗначУ<МастерДиапазон> VT;
	StableSort(СубДиапазон(ИндексСорт3Обходчик__<I, I2, I3, I4, VT>(r.begin(), r2.begin(), r3.begin(), r4.begin()),
	                    ИндексСорт3Обходчик__<I, I2, I3, I4, VT>(r.end(), r2.end(), r3.end(), r4.end())).пиши(),
		       less);
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Диапазон4>
проц StableIndexSort3(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, Диапазон4&& r4)
{
	StableIndexSort3(r, r2, r3, r4, std::less<типЗначУ<МастерДиапазон>>());
}

template <class I, class V>
struct ОбходчикПорядкаСорта__ : ОпыПостфикс< ОбходчикПорядкаСорта__<I, V> >
{
	typedef ОбходчикПорядкаСорта__<I, V> Обх;

	ОбходчикПорядкаСорта__(цел *ii, I vi) : ii(ii), vi(vi) {}

	Обх&       operator ++ ()               { ++ii; return *this; }
	Обх&       operator -- ()               { --ii; return *this; }
	const V&    operator *  () const         { return *(vi + *ii); }
	Обх        operator +  (цел i) const    { return Обх(ii + i, vi); }
	Обх        operator -  (цел i) const    { return Обх(ii - i, vi); }
	цел         operator -  (Обх b) const   { return цел(ii - b.ii); }
	бул        operator == (Обх b) const   { return ii == b.ii; }
	бул        operator != (Обх b) const   { return ii != b.ii; }
	бул        operator <  (Обх b) const   { return ii < b.ii; }
	бул        operator <= (Обх b) const   { return ii <=  b.ii; }
	friend проц IterSwap    (Обх a, Обх b) { IterSwap(a.ii, b.ii); }

	цел        *ii;
	I           vi;
};

template <class Диапазон, class Less>
Вектор<цел> дайСортПорядок(const Диапазон& r, const Less& less)
{
	auto begin = r.begin();
	Вектор<цел> индекс;
	индекс.устСчёт(r.дайСчёт());
	for(цел i = индекс.дайСчёт(); --i >= 0; индекс[i] = i)
		;
	typedef ОбходчикПорядкаСорта__<decltype(begin), типЗначУ<Диапазон>> It;
	Sort__(It(индекс.begin(), begin), It(индекс.end(), begin), less);
	return индекс;
}

template <class Диапазон>
Вектор<цел> дайСортПорядок(const Диапазон& r)
{
	return дайСортПорядок(r, std::less<типЗначУ<Диапазон>>());
}

template <class I, class T>
struct ОбходчикПорядкаСтабилСорта__ : ОпыПостфикс< ОбходчикПорядкаСтабилСорта__<I, T> >
{
	typedef ОбходчикПорядкаСтабилСорта__<I, T> Обх;

	ОбходчикПорядкаСтабилСорта__(цел *ii, I vi) : ii(ii), vi(vi) {}

	Обх&       operator ++ ()               { ++ii; return *this; }
	Обх&       operator -- ()               { --ii; return *this; }
	Обх        operator +  (цел i) const    { return Обх(ii + i, vi); }
	Обх        operator -  (цел i) const    { return Обх(ii - i, vi); }
	цел         operator -  (Обх b) const   { return цел(ii - b.ii); }
	бул        operator == (Обх b) const   { return ii == b.ii; }
	бул        operator != (Обх b) const   { return ii != b.ii; }
	бул        operator <  (Обх b) const   { return ii < b.ii; }
	бул        operator <= (Обх b) const   { return ii <=  b.ii; }
	friend проц IterSwap    (Обх a, Обх b) { IterSwap(a.ii, b.ii); }

	StableSortItem__<T> operator*() const    { return StableSortItem__<T>(*(vi + *ii), *ii); }

	цел        *ii;
	I           vi;
};

template <class Диапазон, class Less>
Вектор<цел> GetStableSortOrder(const Диапазон& r, const Less& less)
{
	Вектор<цел> индекс;
	индекс.устСчёт(r.дайСчёт());
	for(цел i = индекс.дайСчёт(); --i >= 0; индекс[i] = i)
		;
	auto begin = r.begin();
	typedef типЗначУ<Диапазон> VT;
	typedef ОбходчикПорядкаСтабилСорта__<decltype(begin), VT> It;
	Sort__(It(индекс.begin(), begin), It(индекс.end(), begin), StableSortLess__<VT, Less>(less));
	return индекс;
}

template <class Диапазон>
Вектор<цел> GetStableSortOrder(const Диапазон& r)
{
	return GetStableSortOrder(r, std::less<типЗначУ<Диапазон>>());
}

template <class вКарту, class Less>
проц SortByKey(вКарту& map, const Less& less)
{
	typename вКарту::КонтейнерКлючей k = map.подбериКлючи();
	typename вКарту::КонтейнерЗначений v = map.подбериЗначения();
	IndexSort(k, v, less);
	map = вКарту(пикуй(k), пикуй(v));
}

template <class вКарту>
проц SortByKey(вКарту& map)
{
	SortByKey(map, std::less<typename вКарту::ТипКлюча>());
}

template <class вКарту, class Less>
проц SortByValue(вКарту& map, const Less& less)
{
	typename вКарту::КонтейнерКлючей k = map.подбериКлючи();
	typename вКарту::КонтейнерЗначений v = map.подбериЗначения();
	IndexSort(v, k, less);
	map = вКарту(пикуй(k), пикуй(v));
}

template <class вКарту>
проц SortByValue(вКарту& map)
{
	SortByValue(map, std::less<типЗначУ<вКарту>>());
}

template <class вКарту, class Less>
проц StableSortByKey(вКарту& map, const Less& less)
{
	typename вКарту::КонтейнерКлючей k = map.подбериКлючи();
	typename вКарту::КонтейнерЗначений v = map.подбериЗначения();
	StableIndexSort(k, v, less);
	map = вКарту(пикуй(k), пикуй(v));
}

template <class вКарту>
проц StableSortByKey(вКарту& map)
{
	StableSortByKey(map, std::less<typename вКарту::ТипКлюча>());
}

template <class вКарту, class Less>
проц StableSortByValue(вКарту& map, const Less& less)
{
	typename вКарту::КонтейнерКлючей k = map.подбериКлючи();
	typename вКарту::КонтейнерЗначений v = map.подбериЗначения();
	StableIndexSort(v, k, less);
	map = вКарту(пикуй(k), пикуй(v));
}

template <class вКарту>
проц StableSortByValue(вКарту& map)
{
	StableSortByValue(map, std::less<типЗначУ<вКарту>>());
}

template <class Индекс, class Less>
проц SortIndex(Индекс& индекс, const Less& less)
{
	typename Индекс::КонтейнерЗначений k = индекс.подбериКлючи();
	сортируй(k, less);
	индекс = Индекс(пикуй(k));
}

template <class Индекс>
проц SortIndex(Индекс& индекс)
{
	SortIndex(индекс, std::less<типЗначУ<Индекс>>());
}

template <class Индекс, class Less>
проц StableSortIndex(Индекс& индекс, const Less& less)
{
	typename Индекс::КонтейнерЗначений k = индекс.подбериКлючи();
	StableSort(k, less);
	индекс = Индекс(пикуй(k));
}

template <class Индекс>
проц StableSortIndex(Индекс& индекс)
{
	StableSortIndex(индекс, std::less<типЗначУ<Индекс>>());
}
