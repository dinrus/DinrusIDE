форс_инлайн
проц ИндексОбщее::линкуй(цел& m, Хэш& hh, цел ii)
{
	if(m < 0)
		m = hh.prev = hh.next = ii;
	else {
		hh.next = m;
		hh.prev = hash[m].prev;
		hash[hh.prev].next = ii;
		hash[m].prev = ii;
	}
}

форс_инлайн
проц ИндексОбщее::линкуй(цел ii, бцел sh)
{
	линкуй(map[sh & mask], hash[ii], ii);
}

форс_инлайн
проц ИндексОбщее::уд(цел& m, Хэш& hh, цел ii)
{ // unlink from m
	if(ii == m) { // this is элт pointed by map
		if(hh.next == ii) { // this is the only элт in the bucket
			m = -1; // bucket is now empty
			return;
		}
		m = hh.next; // move bucket pointer to the next элт
	}
	hash[hh.next].prev = hh.prev; // unlink
	hash[hh.prev].next = hh.next;
}

template <typename T>
никогда_inline
проц Индекс<T>::переразместиХэш(цел n)
{ // realloc hash to have the same capacity as ключ, copy n elements from previous alloc
	if(ключ.дайРазмест()) {
		т_мера sz = ключ.дайРазмест() * sizeof(Хэш);
		if(!пробуйПереместПам(hash, sz)) {
			Хэш *h = (Хэш *)разместиПам(sz);
			if(hash) {
				if(n)
					т_копирпам(h, hash, n);
				освободиПам(hash);
			}
			hash = h;
		}
	}
	else {
		освободиПам(hash);
		hash = NULL;
	}
}

template <typename T>
никогда_inline
проц Индекс<T>::фиксируйХэш(бул makemap)
{
	переразместиХэш(0);
	unlinked = -1;
	for(цел i = 0; i < ключ.дайСчёт(); i++)
		hash[i].hash = Smear(ключ[i]);
	if(makemap)
		сделайМап(ключ.дайСчёт());
	else
		ремапируй(ключ.дайСчёт());
}

template <typename T>
template <typename U>
никогда_inline
проц Индекс<T>::нарастиДобавь(U&& k, бцел sh)
{
	цел n = ключ.дайСчёт();
	ключ.нарастиДобавь(std::forward<U>(k));
	переразместиХэш(n);
}

template <typename T>
template <typename U>
проц Индекс<T>::AddS(цел& m, U&& k, бцел sh)
{
	цел ii = ключ.дайСчёт();
	if(ii >= ключ.дайРазмест())
		нарастиДобавь(std::forward<U>(k), sh);
	else
		new(ключ.Rdd()) T(std::forward<U>(k));
	Хэш& hh = hash[ii];
	hh.hash = sh;
	if(ii >= (цел)mask)
		нарастиМап(ключ.дайСчёт());
	else
		линкуй(m, hh, ii);
}

template <typename T>
template <typename U>
проц Индекс<T>::AddS(U&& k, бцел sh)
{
	AddS(map[sh & mask], std::forward<U>(k), sh);
}

template <typename T>
форс_инлайн
цел Индекс<T>::найдиОт(цел i, бцел sh, const T& k, цел end) const
{
	if(i >= 0)
		do {
			if(ключ[i] == k)
				return i;
			i = hash[i].next;
		}
		while(i != end);
	return -1;
}

template <class T>
цел Индекс<T>::найди(const T& k) const
{
	бцел sh = Smear(k);
	цел& m = map[sh & mask];
	return найдиОт(m, sh, k, m);
}

template <class T>
цел Индекс<T>::найдиСледщ(цел i) const
{
	const Хэш& hh = hash[i];
	цел end = map[hash[i].hash & mask];
	return hh.next == end ? -1 : найдиОт(hh.next, hh.hash, ключ[i], end);
}

template <typename T>
форс_инлайн
цел Индекс<T>::FindBack(цел i, бцел sh, const T& k, цел end) const
{
	do {
		const Хэш& ih = hash[i];
		if(ключ[i] == k)
			return i;
		i = ih.prev;
	}
	while(i != end);
	return -1;
}

template <class T>
цел Индекс<T>::найдиПоследн(const T& k) const
{
	бцел sh = Smear(k);
	цел& m = map[sh & mask];
	return m < 0 ? -1 : FindBack(hash[m].prev, sh, k, hash[m].prev);
}

template <class T>
цел Индекс<T>::найдиПредш(цел i) const
{
	const Хэш& hh = hash[i];
	цел end = map[hash[i].hash & mask];
	return hh.prev == hash[end].prev ? -1 : FindBack(hh.prev, hh.hash, ключ[i], hash[end].prev);
}

template <class T>
template <class OP, class U>
форс_инлайн
цел Индекс<T>::найдиДобавь(U&& k, OP op) {
	бцел sh = Smear(k);
	цел& m = map[sh & mask];
	цел i = m;
	if(i >= 0)
		do {
			if(ключ[i] == k)
				return i;
			i = hash[i].next;
		}
		while(i != m);
	i = ключ.дайСчёт();
	AddS(m, std::forward<U>(k), sh);
	op();
	return i;
}

template <typename T>
проц Индекс<T>::отлинкуй(цел ii)
{
	Хэш& hh = hash[ii];
	уд(map[hh.hash & mask], hh, ii);
	линкуй(unlinked, hh, ii);
	hh.hash = 0;
}

template <typename T>
цел Индекс<T>::отлинкуйКлюч(const T& k)
{
	бцел sh = Smear(k);
	цел& m = map[sh & mask];
	цел i = m;
	цел n = 0;
	if(i >= 0)
		for(;;) {
			Хэш& hh = hash[i];
			цел ni = hh.next;
			if(ключ[i] == k) {
				уд(m, hh, i);
				линкуй(unlinked, hh, i);
				n++;
				hh.hash = 0;
				if(ni == i) // last элт removed
					break;
				i = ni;
			}
			else {
				i = ni;
				if(i == m)
					break;
			}
		}
	return n;
}

template <typename T>
template <typename U>
цел Индекс<T>::Put0(U&& k, бцел sh)
{
	цел i;
	if(HasUnlinked()) {
		i = hash[unlinked].prev;
		Хэш& hh = hash[i];
		уд(unlinked, hh, i);
		линкуй(map[sh & mask], hh, i);
		hh.hash = sh;
		ключ[i] = std::forward<U>(k);
	}
	else {
		i = дайСчёт();
		AddS(std::forward<U>(k), sh);
	}
	return i;
}

template <class T>
template <class U>
форс_инлайн
цел Индекс<T>::FindPut0(U&& k) {
	бцел sh = Smear(k);
	цел& m = map[sh & mask];
	цел i = m;
	if(i >= 0)
		do {
			if(ключ[i] == k)
				return i;
			i = hash[i].next;
		}
		while(i != m);
	return Put0(std::forward<U>(k), sh);
}

template <typename T>
template <typename U>
проц Индекс<T>::уст0(цел ii, U&& k)
{
	Хэш& hh = hash[ii];
	if(отлинкован(ii))
		уд(unlinked, hh, ii);
	else
		уд(map[hh.hash & mask], hh, ii);
	
	бцел sh = Smear(k);
	hh.hash = sh;
	линкуй(map[sh & mask], hh, ii);
	ключ[ii] = std::forward<U>(k);
}

template <typename T>
никогда_inline
проц Индекс<T>::смети()
{
	if(unlinked >= 0) {
		цел n = ключ.дайСчёт();
		ключ.удалиЕсли([&](цел i) { return hash[i].hash == 0; });
		ИндексОбщее::смети(n);
	}
}

template <typename T>
никогда_inline
проц Индекс<T>::резервируй(цел n)
{
	цел a = ключ.дайРазмест();
	ключ.резервируй(n);
	if(a != ключ.дайРазмест()) {
		переразместиХэш(ключ.дайСчёт());
		AdjustMap(ключ.дайСчёт(), n);
	}
}

template <typename T>
никогда_inline
проц Индекс<T>::сожми()
{
	цел a = ключ.дайРазмест();
	ключ.сожми();
	if(a != ключ.дайРазмест()) {
		переразместиХэш(ключ.дайСчёт());
		AdjustMap(ключ.дайСчёт(), ключ.дайСчёт());
	}
}

template <typename T>
проц Индекс<T>::удали(const цел *sorted_list, цел count)
{
	if(HasUnlinked()) {
		Вектор<бул> u;
		u.устСчёт(дайСчёт());
		for(цел i = 0; i < дайСчёт(); i++)
			u[i] = отлинкован(i);
		ключ.удали(sorted_list, count);
		u.удали(sorted_list, count);
		фиксируйХэш(false);
		for(цел i = 0; i < дайСчёт(); i++)
			if(u[i])
				отлинкуй(i);
	}
	else {
		ключ.удали(sorted_list, count);
		фиксируйХэш(false);
	}
}

template <typename T>
никогда_inline
проц Индекс<T>::сериализуй(Поток& s)
{
	ключ.сериализуй(s);
	if(s.грузится())
		фиксируйХэш();

	цел версия = 1;
	s / версия;
	if(версия == 0) { // support previous версия
		Вектор<unsigned> h;
		h.сериализуй(s);
		if(s.грузится())
			for(цел i = 0; i < h.дайСчёт(); i++)
				if(h[i] & 0x80000000)
					отлинкуй(i);
	}
	else {
		Вектор<цел> u = дайОтлинкованно();
		u.сериализуй(s);
		if(s.грузится())
			for(цел i : РеверсДиапазон(u)) // реверс range to ensure the correct order of помести
				отлинкуй(i);
	}
}

template <class T>
проц Индекс<T>::вРяр(РярВВ& xio, кткст0 itemtag)
{
	индексВРяр<T, Индекс<T> >(xio, itemtag, *this);
}

template <class T>
проц Индекс<T>::вДжейсон(ДжейсонВВ& jio)
{
	джейсонируйИндекс<Индекс<T>, T>(jio, *this);
}

template <class T>
Ткст Индекс<T>::вТкст() const
{
	return AsStringArray(*this);
}

#ifdef _ОТЛАДКА
template <typename T>
Ткст Индекс<T>::дамп() const
{
	Ткст h;
	for(цел i = 0; i < ключ.дайСчёт(); i++) {
		if(i)
			h << "; ";
		if(отлинкован(i))
			h << "#";
		h << i << ": " << ключ[i] << '/' << (hash[i].hash & mask) << " -> " << hash[i].prev << ":" << hash[i].next;
	}
	return h;
}
#endif