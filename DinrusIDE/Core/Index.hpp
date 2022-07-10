force_inline
void ИндексОбщее::линкуй(int& m, Хэш& hh, int ii)
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

force_inline
void ИндексОбщее::линкуй(int ii, dword sh)
{
	линкуй(map[sh & mask], hash[ii], ii);
}

force_inline
void ИндексОбщее::уд(int& m, Хэш& hh, int ii)
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
never_inline
void Индекс<T>::ReallocHash(int n)
{ // realloc hash to have the same capacity as ключ, copy n elements from previous alloc
	if(ключ.дайРазмест()) {
		size_t sz = ключ.дайРазмест() * sizeof(Хэш);
		if(!MemoryTryRealloc(hash, sz)) {
			Хэш *h = (Хэш *)MemoryAlloc(sz);
			if(hash) {
				if(n)
					memcpy_t(h, hash, n);
				MemoryFree(hash);
			}
			hash = h;
		}
	}
	else {
		MemoryFree(hash);
		hash = NULL;
	}
}

template <typename T>
never_inline
void Индекс<T>::фиксируйХэш(bool makemap)
{
	ReallocHash(0);
	unlinked = -1;
	for(int i = 0; i < ключ.дайСчёт(); i++)
		hash[i].hash = Smear(ключ[i]);
	if(makemap)
		сделайМап(ключ.дайСчёт());
	else
		ремапируй(ключ.дайСчёт());
}

template <typename T>
template <typename U>
never_inline
void Индекс<T>::нарастиДобавь(U&& k, dword sh)
{
	int n = ключ.дайСчёт();
	ключ.нарастиДобавь(std::forward<U>(k));
	ReallocHash(n);
}

template <typename T>
template <typename U>
void Индекс<T>::AddS(int& m, U&& k, dword sh)
{
	int ii = ключ.дайСчёт();
	if(ii >= ключ.дайРазмест())
		нарастиДобавь(std::forward<U>(k), sh);
	else
		new(ключ.Rdd()) T(std::forward<U>(k));
	Хэш& hh = hash[ii];
	hh.hash = sh;
	if(ii >= (int)mask)
		нарастиМап(ключ.дайСчёт());
	else
		линкуй(m, hh, ii);
}

template <typename T>
template <typename U>
void Индекс<T>::AddS(U&& k, dword sh)
{
	AddS(map[sh & mask], std::forward<U>(k), sh);
}

template <typename T>
force_inline
int Индекс<T>::найдиОт(int i, dword sh, const T& k, int end) const
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
int Индекс<T>::найди(const T& k) const
{
	dword sh = Smear(k);
	int& m = map[sh & mask];
	return найдиОт(m, sh, k, m);
}

template <class T>
int Индекс<T>::найдиСледщ(int i) const
{
	const Хэш& hh = hash[i];
	int end = map[hash[i].hash & mask];
	return hh.next == end ? -1 : найдиОт(hh.next, hh.hash, ключ[i], end);
}

template <typename T>
force_inline
int Индекс<T>::FindBack(int i, dword sh, const T& k, int end) const
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
int Индекс<T>::найдиПоследн(const T& k) const
{
	dword sh = Smear(k);
	int& m = map[sh & mask];
	return m < 0 ? -1 : FindBack(hash[m].prev, sh, k, hash[m].prev);
}

template <class T>
int Индекс<T>::найдиПредш(int i) const
{
	const Хэш& hh = hash[i];
	int end = map[hash[i].hash & mask];
	return hh.prev == hash[end].prev ? -1 : FindBack(hh.prev, hh.hash, ключ[i], hash[end].prev);
}

template <class T>
template <class OP, class U>
force_inline
int Индекс<T>::найдиДобавь(U&& k, OP op) {
	dword sh = Smear(k);
	int& m = map[sh & mask];
	int i = m;
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
void Индекс<T>::отлинкуй(int ii)
{
	Хэш& hh = hash[ii];
	уд(map[hh.hash & mask], hh, ii);
	линкуй(unlinked, hh, ii);
	hh.hash = 0;
}

template <typename T>
int Индекс<T>::отлинкуйКлюч(const T& k)
{
	dword sh = Smear(k);
	int& m = map[sh & mask];
	int i = m;
	int n = 0;
	if(i >= 0)
		for(;;) {
			Хэш& hh = hash[i];
			int ni = hh.next;
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
int Индекс<T>::Put0(U&& k, dword sh)
{
	int i;
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
force_inline
int Индекс<T>::FindPut0(U&& k) {
	dword sh = Smear(k);
	int& m = map[sh & mask];
	int i = m;
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
void Индекс<T>::уст0(int ii, U&& k)
{
	Хэш& hh = hash[ii];
	if(отлинкован(ii))
		уд(unlinked, hh, ii);
	else
		уд(map[hh.hash & mask], hh, ii);
	
	dword sh = Smear(k);
	hh.hash = sh;
	линкуй(map[sh & mask], hh, ii);
	ключ[ii] = std::forward<U>(k);
}

template <typename T>
never_inline
void Индекс<T>::смети()
{
	if(unlinked >= 0) {
		int n = ключ.дайСчёт();
		ключ.удалиЕсли([&](int i) { return hash[i].hash == 0; });
		ИндексОбщее::смети(n);
	}
}

template <typename T>
never_inline
void Индекс<T>::резервируй(int n)
{
	int a = ключ.дайРазмест();
	ключ.резервируй(n);
	if(a != ключ.дайРазмест()) {
		ReallocHash(ключ.дайСчёт());
		AdjustMap(ключ.дайСчёт(), n);
	}
}

template <typename T>
never_inline
void Индекс<T>::сожми()
{
	int a = ключ.дайРазмест();
	ключ.сожми();
	if(a != ключ.дайРазмест()) {
		ReallocHash(ключ.дайСчёт());
		AdjustMap(ключ.дайСчёт(), ключ.дайСчёт());
	}
}

template <typename T>
void Индекс<T>::удали(const int *sorted_list, int count)
{
	if(HasUnlinked()) {
		Вектор<bool> u;
		u.устСчёт(дайСчёт());
		for(int i = 0; i < дайСчёт(); i++)
			u[i] = отлинкован(i);
		ключ.удали(sorted_list, count);
		u.удали(sorted_list, count);
		фиксируйХэш(false);
		for(int i = 0; i < дайСчёт(); i++)
			if(u[i])
				отлинкуй(i);
	}
	else {
		ключ.удали(sorted_list, count);
		фиксируйХэш(false);
	}
}

template <typename T>
never_inline
void Индекс<T>::сериализуй(Поток& s)
{
	ключ.сериализуй(s);
	if(s.грузится())
		фиксируйХэш();

	int версия = 1;
	s / версия;
	if(версия == 0) { // support previous версия
		Вектор<unsigned> h;
		h.сериализуй(s);
		if(s.грузится())
			for(int i = 0; i < h.дайСчёт(); i++)
				if(h[i] & 0x80000000)
					отлинкуй(i);
	}
	else {
		Вектор<int> u = дайОтлинкованно();
		u.сериализуй(s);
		if(s.грузится())
			for(int i : ReverseRange(u)) // реверс range to ensure the correct order of помести
				отлинкуй(i);
	}
}

template <class T>
void Индекс<T>::вРяр(РярВВ& xio, const char *itemtag)
{
	индексВРяр<T, Индекс<T> >(xio, itemtag, *this);
}

template <class T>
void Индекс<T>::вДжейсон(ДжейсонВВ& jio)
{
	JsonizeIndex<Индекс<T>, T>(jio, *this);
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
	for(int i = 0; i < ключ.дайСчёт(); i++) {
		if(i)
			h << "; ";
		if(отлинкован(i))
			h << "#";
		h << i << ": " << ключ[i] << '/' << (hash[i].hash & mask) << " -> " << hash[i].prev << ":" << hash[i].next;
	}
	return h;
}
#endif