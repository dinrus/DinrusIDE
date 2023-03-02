template <class I, class Less>
void соСорт__(СоРабота& cw, I l, I h, const Less& less)
{
	int count = int(h - l);
	I middle = l + (count >> 1);        // get the middle element

	for(;;) {
		if(count < 200) { // too little elements to gain anything with parellel processing
			Sort__(l, h, less);
			return;
		}

		if(count > 1000) {
			middle = l + (count >> 1); // iterators cannot point to the same object!
			I q = l + 1 + (int)случ((count >> 1) - 2);
			I w = middle + 1 + (int)случ((count >> 1) - 2);
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

		int count_l = i - l;
		if(count_l == 1) // this happens if there are many elements equal to pivot, filter them out
			for(I q = ih + 1; q != h; ++q)
				if(!less(*i, *q))
					IterSwap(++ih, q);

		int count_h = h - ih - 1;

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

		if(count > 8 && min(count_l, count_h) < (max(count_l, count_h) >> 2)) // If unbalanced,
			middle = l + 1 + случ(count - 2); // randomize the next step
		else
			middle = l + (count >> 1); // the middle is probably still the best guess otherwise
	}
}

template <class I, class Less>
void соСорт__(I l, I h, const Less& less)
{
	СоРабота cw;
	соСорт__(cw, l, h, less);
}

template <class Диапазон, class Less>
void соСортируй(Диапазон&& c, const Less& less)
{
	соСорт__(c.begin(), c.end(), less);
}

template <class Диапазон>
void соСортируй(Диапазон&& c)
{
	соСорт__(c.begin(), c.end(), std::less<типЗначУ<Диапазон>>());
}

template <class Диапазон, class Less>
void соСтабилСортируй(Диапазон&& r, const Less& less)
{
	auto begin = r.begin();
	auto end = r.end();
	typedef типЗначУ<Диапазон> VT;
	typedef decltype(begin) I;
	int count = (int)(uintptr_t)(end - begin);
	Буфер<int> h(count);
	for(int i = 0; i < count; i++)
		h[i] = i;
	соСорт__(StableSortIterator__<I, VT>(begin, ~h), StableSortIterator__<I, VT>(end, ~h + count),
	         StableSortLess__<VT, Less>(less));
}

template <class Диапазон>
void соСтабилСортируй(Диапазон&& r)
{
	соСтабилСортируй(r, std::less<типЗначУ<Диапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Less>
void соИндексСортируй(МастерДиапазон&& r, Диапазон2&& r2, const Less& less)
{
	ПРОВЕРЬ(r.дайСчёт() == r2.дайСчёт());
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef типЗначУ<МастерДиапазон> VT;
	if(r.дайСчёт() == 0)
		return;
	соСорт__(ИндексСортОбходчик__<I, I2, VT>(r.begin(), r2.begin()),
	         ИндексСортОбходчик__<I, I2, VT>(r.end(), r2.end()),
		     less);
}

template <class МастерДиапазон, class Диапазон2>
void соИндексСортируй(МастерДиапазон&& r, Диапазон2&& r2)
{
	соИндексСортируй(r, r2, std::less<типЗначУ<МастерДиапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Less>
void соСтабилИндексСортируй(МастерДиапазон&& r, Диапазон2&& r2, const Less& less)
{
	ПРОВЕРЬ(r.дайСчёт() == r2.дайСчёт());
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef типЗначУ<МастерДиапазон> VT;
	if(r.дайСчёт() == 0)
		return;
	соСтабилСортируй(СубДиапазон(ИндексСортОбходчик__<I, I2, VT>(r.begin(), r2.begin()),
		                  ИндексСортОбходчик__<I, I2, VT>(r.end(), r2.end())).пиши(),
	             less);
}

template <class МастерДиапазон, class Диапазон2>
void соСтабилИндексСортируй(МастерДиапазон&& r, Диапазон2&& r2)
{
	соСтабилИндексСортируй(r, r2, std::less<типЗначУ<МастерДиапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Less>
void соИндексСортируй2(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, const Less& less)
{
	ПРОВЕРЬ(r.дайСчёт() == r2.дайСчёт());
	ПРОВЕРЬ(r.дайСчёт() == r3.дайСчёт());
	if(r.дайСчёт() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef типЗначУ<МастерДиапазон> VT;
	соСорт__(ИндексСорт2Обходчик__<I, I2, I3, VT>(r.begin(), r2.begin(), r3.begin()),
	         ИндексСорт2Обходчик__<I, I2, I3, VT>(r.end(), r2.end(), r3.end()),
		     less);
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3>
void соИндексСортируй2(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3)
{
	соИндексСортируй2(r, r2, r3, std::less<типЗначУ<МастерДиапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Less>
void соСтабилИндексСортируй2(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, const Less& less)
{
	ПРОВЕРЬ(r.дайСчёт() == r2.дайСчёт());
	ПРОВЕРЬ(r.дайСчёт() == r3.дайСчёт());
	if(r.дайСчёт() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef типЗначУ<МастерДиапазон> VT;
	соСтабилСортируй(СубДиапазон(ИндексСорт2Обходчик__<I, I2, I3, VT>(r.begin(), r2.begin(), r3.begin()),
	                    ИндексСорт2Обходчик__<I, I2, I3, VT>(r.end(), r2.end(), r3.end())).пиши(),
		         less);
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3>
void соСтабилИндексСортируй2(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3)
{
	соСтабилИндексСортируй2(r, r2, r3, std::less<типЗначУ<МастерДиапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Диапазон4, class Less>
void соИндексСортируй3(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, Диапазон4&& r4, const Less& less)
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
	соСорт__(ИндексСорт3Обходчик__<I, I2, I3, I4, VT>(r.begin(), r2.begin(), r3.begin(), r4.begin()),
	         ИндексСорт3Обходчик__<I, I2, I3, I4, VT>(r.end(), r2.end(), r3.end(), r4.end()),
		     less);
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Диапазон4>
void соИндексСортируй3(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, Диапазон4&& r4)
{
	соИндексСортируй3(r, r2, r3, r4, std::less<типЗначУ<МастерДиапазон>>());
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Диапазон4, class Less>
void соСтабилИндексСортируй3(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, Диапазон4&& r4, const Less& less)
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
	соСтабилСортируй(СубДиапазон(ИндексСорт3Обходчик__<I, I2, I3, I4, VT>(r.begin(), r2.begin(), r3.begin(), r4.begin()),
	                      ИндексСорт3Обходчик__<I, I2, I3, I4, VT>(r.end(), r2.end(), r3.end(), r4.end())).пиши(),
		         less);
}

template <class МастерДиапазон, class Диапазон2, class Диапазон3, class Диапазон4>
void соСтабилИндексСортируй3(МастерДиапазон&& r, Диапазон2&& r2, Диапазон3&& r3, Диапазон4&& r4)
{
	соСтабилИндексСортируй3(r, r2, r3, r4, std::less<типЗначУ<МастерДиапазон>>());
}

template <class Диапазон, class Less>
Вектор<int> соДайПорядокСорт(const Диапазон& r, const Less& less)
{
	auto begin = r.begin();
	Вектор<int> индекс;
	индекс.устСчёт(r.дайСчёт());
	for(int i = индекс.дайСчёт(); --i >= 0; индекс[i] = i)
		;
	typedef ОбходчикПорядкаСорта__<decltype(begin), типЗначУ<Диапазон>> It;
	соСорт__(It(индекс.begin(), begin), It(индекс.end(), begin), less);
	return индекс;
}

template <class Диапазон>
Вектор<int> соДайПорядокСорт(const Диапазон& r)
{
	return соДайПорядокСорт(r, std::less<типЗначУ<Диапазон>>());
}

template <class Диапазон, class Less>
Вектор<int> соДайСтабилПорядокСорт(const Диапазон& r, const Less& less)
{
	Вектор<int> индекс;
	индекс.устСчёт(r.дайСчёт());
	for(int i = индекс.дайСчёт(); --i >= 0; индекс[i] = i)
		;
	auto begin = r.begin();
	typedef типЗначУ<Диапазон> VT;
	typedef ОбходчикПорядкаСтабилСорта__<decltype(begin), VT> It;
	соСорт__(It(индекс.begin(), begin), It(индекс.end(), begin), StableSortLess__<VT, Less>(less));
	return индекс;
}

template <class Диапазон>
Вектор<int> соДайСтабилПорядокСорт(const Диапазон& r)
{
	return соДайПорядокСорт(r, std::less<типЗначУ<Диапазон>>());
}

template <class вКарту, class Less>
void соСортируйПоКлючу(вКарту& map, const Less& less)
{
	typename вКарту::КонтейнерКлючей k = map.подбериКлючи();
	typename вКарту::КонтейнерЗначений v = map.подбериЗначения();
	соИндексСортируй(k, v, less);
	map = вКарту(pick(k), pick(v));
}

template <class вКарту>
void соСортируйПоКлючу(вКарту& map)
{
	соСортируйПоКлючу(map, std::less<typename вКарту::ТипКлюча>());
}

template <class вКарту, class Less>
void соСортируйПоЗначению(вКарту& map, const Less& less)
{
	typename вКарту::КонтейнерКлючей k = map.подбериКлючи();
	typename вКарту::КонтейнерЗначений v = map.подбериЗначения();
	соИндексСортируй(v, k, less);
	map = вКарту(pick(k), pick(v));
}

template <class вКарту>
void соСортируйПоЗначению(вКарту& map)
{
	соСортируйПоЗначению(map, std::less<типЗначУ<вКарту>>());
}

template <class вКарту, class Less>
void соСтабилСортируйПоКлючу(вКарту& map, const Less& less)
{
	typename вКарту::КонтейнерКлючей k = map.подбериКлючи();
	typename вКарту::КонтейнерЗначений v = map.подбериЗначения();
	соСтабилИндексСортируй(k, v, less);
	map = вКарту(pick(k), pick(v));
}

template <class вКарту>
void соСтабилСортируйПоКлючу(вКарту& map)
{
	соСтабилСортируйПоКлючу(map, std::less<typename вКарту::ТипКлюча>());
}

template <class вКарту, class Less>
void соСтабилСортируйПоЗначению(вКарту& map, const Less& less)
{
	typename вКарту::КонтейнерКлючей k = map.подбериКлючи();
	typename вКарту::КонтейнерЗначений v = map.подбериЗначения();
	соСтабилИндексСортируй(v, k, less);
	map = вКарту(pick(k), pick(v));
}

template <class вКарту>
void соСтабилСортируйПоЗначению(вКарту& map)
{
	соСтабилСортируйПоЗначению(map, std::less<типЗначУ<вКарту>>());
}

template <class Индекс, class Less>
void соСортИндекс(Индекс& индекс, const Less& less)
{
	typename Индекс::КонтейнерЗначений k = индекс.подбериКлючи();
	соСортируй(k, less);
	индекс = Индекс(pick(k));
}

template <class Индекс>
void соСортИндекс(Индекс& индекс)
{
	соСортИндекс(индекс, std::less<типЗначУ<Индекс>>());
}

template <class Индекс, class Less>
void соСтабилСортИндекс(Индекс& индекс, const Less& less)
{
	typename Индекс::КонтейнерЗначений k = индекс.подбериКлючи();
	соСтабилСортируй(k, less);
	индекс = Индекс(pick(k));
}

template <class Индекс>
void соСтабилСортИндекс(Индекс& индекс)
{
	соСтабилСортИндекс(индекс, std::less<типЗначУ<Индекс>>());
}
