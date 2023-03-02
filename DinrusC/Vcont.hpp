template <class T>
void StreamContainer(Поток& s, T& cont)
{
	int n = cont.дайСчёт();
	s / n;
	if(n < 0) {
		s.загрузиОш();
		return;
	}
	if(s.грузится())
	{
		cont.очисть();
		cont.резервируй(min(n, int(256*1024 / sizeof(T)))); // protect against invalid streams...
		while(n--)
			s % cont.добавь();
	}
	else
	{
		for(auto ptr = cont.begin(); n--; ++ptr)
			s % *ptr;
	}
}

template <class T>
T * Вектор<T>::разместиРяд(int& n)
{
	size_t sz0 = n * sizeof(T);
	size_t sz = sz0;
	void *q = MemoryAllocSz(sz);
	n += (int)((sz - sz0) / sizeof(T));
	return (T *)q;
}

template <class T>
bool Вектор<T>::перемест(int newalloc)
{
	ПРОВЕРЬ(newalloc >= items);
	size_t sz0 = (size_t)newalloc * sizeof(T);
	size_t sz = sz0;
	void *newvector;
	bool  alloced = true;
	if(MemoryTryRealloc(vector, sz)) {
		newvector = vector;
		vector = NULL;
		alloced = false;
	}
	else
		newvector = newalloc ? MemoryAllocSz(sz) : NULL;
	alloc = newalloc == INT_MAX ? INT_MAX // maximum alloc reached
	        : (int)((sz - sz0) / sizeof(T) + newalloc); // adjust alloc to real memory size
	if(vector && newvector)
		memcpy_t((T *)newvector, vector, items);
	vector = (T *)newvector;
	return alloced;
}

template <class T>
void Вектор<T>::ReAllocF(int newalloc)
{
	void *prev = vector;
	if(перемест(newalloc) && prev)
		MemoryFree(prev);
}

template <class T>
bool Вектор<T>::GrowSz()
{
	if(alloc == INT_MAX)
		паника("Too many items in container!");
#ifdef _ОТЛАДКА
	return перемест(max(alloc + 1, alloc >= INT_MAX / 2 ? INT_MAX : 2 * alloc));
#else
	return перемест(max(alloc + 1, alloc >= int((int64)2 * INT_MAX / 3) ? INT_MAX : alloc + (alloc >> 1)));
#endif
}

template <class T>
void Вектор<T>::GrowF()
{
	void *prev = vector;
	if(GrowSz())
		MemoryFree(prev);
}

template <class T>
void Вектор<T>::подбери(Вектор<T>&& v)
{
	vector = v.vector;
	items = v.items;
	alloc = v.alloc;
	v.обнули();
}

#ifdef DEPRECATED
template <class T>
int  Вектор<T>::дайИндекс(const T& элт) const
{
	if(vector == NULL) return -1;
	int n = &элт - vector;
	return n >= 0 && n < items ? n : -1;
}
#endif

template <class T>
void Вектор<T>::резервируй(int n)
{
	if(n > alloc)
		ReAllocF(n);
}

template <class T>
void Вектор<T>::освободи() {
	if(vector && items >= 0)
		разрушь((T *)vector, (T *)vector + items);
	освободиРяд(vector);
}

template <class T>
void Вектор<T>::очисть() {
	освободи();
	alloc = items = 0;
	vector = NULL;
}

template <class T>
void Вектор<T>::__глубКопия(const Вектор& ист) {
	items = alloc = ист.items;
	if(ист.vector) {
		vector = разместиРяд(alloc);
		стройГлубКопию(vector, ист.vector, ист.vector + items);
	}
	else
		vector = NULL;
}

template <class T>
T& Вектор<T>::нарастиДобавь(const T& x) {
	T *prev = vector;
	bool b = GrowSz();
	T *q = new(Rdd()) T(x);
	if(b) освободиРяд(prev);
	return *q;
}

template <class T>
T& Вектор<T>::нарастиДобавь(T&& x) {
	T *prev = vector;
	bool b = GrowSz();
	T *q = ::new(Rdd()) T(pick(x));
	if(b) освободиРяд(prev);
	return *q;
}

template <class T> inline
void Вектор<T>::добавьН(int n)
{
	ПРОВЕРЬ(n >= 0);
	if(items + n <= alloc) {
		строй(vector + items, vector + items + n);
		items += n;
	}
	else
		устСчётР(items + n);
}

template <class T>
void Вектор<T>::обрежь(int n)
{
	ПРОВЕРЬ(n >= 0 && n <= items);
	разрушь(vector + n, vector + items);
	items = n;
}

template <class T>
void Вектор<T>::устСчёт(int n) {
	ПРОВЕРЬ(n >= 0);
	if(n == items) return;
	if(n < items)
		обрежь(n);
	else {
		if(n > alloc) ReAllocF(n);
		строй(vector + items, vector + n);
		items = n;
	}
}

template <class T>
void Вектор<T>::устСчёт(int n, const T& init) {
	ПРОВЕРЬ(n >= 0);
	if(n == items) return;
	if(n < items)
		разрушь(vector + n, vector + items);
	else {
		if(n > alloc) {
			T *prev = vector;
			bool b = перемест(n); // because init can be in original vector
			стройЗаполниГлубКопию(vector + items, vector + n, init);
			if(b) освободиРяд(prev);
		}
		else
			стройЗаполниГлубКопию(vector + items, vector + n, init);
	}
	items = n;
}

template <class T>
void Вектор<T>::устСчётР(int n) {
	if(n > alloc)
		ReAllocF(alloc + max(alloc, n - items));
	устСчёт(n);
}

template <class T>
void Вектор<T>::устСчётР(int n, const T& init) {
	ПРОВЕРЬ(n >= 0);
	if(n == items) return;
	if(n < items)
		разрушь(vector + n, vector + items);
	else
		if(n > alloc) {
			T *prev = vector;
			bool b = перемест(alloc + max(alloc, n - items));  // because init can be in original vector
			стройЗаполниГлубКопию(vector + items, vector + n, init);
			if(b) освободиРяд(prev);
		}
		else
			стройЗаполниГлубКопию(vector + items, vector + n, init);
	items = n;
}

template <class T>
void Вектор<T>::удали(int q, int count) {
	ПРОВЕРЬ(q >= 0 && q <= items - count && count >= 0);
	if(count == 0) return;
	разрушь(vector + q, vector + q + count);
	memmove((void *)(vector + q), (void *)(vector + q + count), (items - q - count) * sizeof(T));
	items -= count;
}

template <class T>
void Вектор<T>::удали(const int *sorted_list, int n)
{
	if(!n) return;
	int pos = *sorted_list;
	int npos = pos;
	for(;;) {
		ПРОВЕРЬ(pos < items);
		if(pos == *sorted_list) {
			(vector + pos)->~T();
			pos++;
			sorted_list++;
			if(--n == 0) break;
			ПРОВЕРЬ(*sorted_list >= pos);
		}
		else
			*((Data_S_<sizeof(T)>*)vector + npos++)
				= *((Data_S_<sizeof(T)>*)vector + pos++);
	}
	while(pos < items)
		*((Data_S_<sizeof(T)>*)vector + npos++) = *((Data_S_<sizeof(T)>*)vector + pos++);
	items = npos;
}

template <class T>
void Вектор<T>::удали(const Вектор<int>& v)
{
	удали(v, v.дайСчёт());
}

template <class T>
template <class Условие>
void Вектор<T>::удалиЕсли(Условие c)
{
	int i = 0;
	for(; i < items; i++) // run to the first element without moving
		if(c(i)) {
			(vector + i)->~T();
			break;
		}
	int ti = i++;
	for(; i < items; i++)
		if(c(i))
			(vector + i)->~T();
		else
			*((Data_S_<sizeof(T)>*)vector + ti++) = *((Data_S_<sizeof(T)>*)vector + i);
	items = ti;
}

template <class T>
void Вектор<T>::вставьРяд(int q, int count)
{
	ПРОВЕРЬ(count >= 0);
	ПРОВЕРЬ(q >= 0 && q <= items);
	if(!count) return;
	if(items + count > alloc) {
		T *newvector = разместиРяд(alloc = max(alloc + count, int(alloc + ((unsigned)alloc >> 1))));
		if(vector) {
			memcpy_t(newvector, vector, q);
			memcpy_t(newvector + q + count, vector + q, items - q);
			освободиРяд(vector);
		}
		vector = newvector;
	}
	else
		memmove((void *)(vector + q + count), (void *)(vector + q), (items - q) * sizeof(T));
	items += count;
}

template <class T>
void Вектор<T>::вставьН(int q, int count) {
	ПРОВЕРЬ(count >= 0);
	ПРОВЕРЬ(q >= 0 && q <= items);
	вставьРяд(q, count);
	строй(vector + q, vector + q + count);
}

template <class T>
void Вектор<T>::вставь(int q, const T& x, int count) {
	if(!count) return;
	ПРОВЕРЬ(!(&x >= vector && &x < vector + items));
	вставьРяд(q, count);
	стройЗаполниГлубКопию(vector + q, vector + q + count, x);
}

template <class T>
T& Вектор<T>::вставь(int q, T&& x)
{
	ПРОВЕРЬ(&x < vector || &x > vector + items);
	вставьРяд(q, 1);
	::new(vector + q) T(pick(x));
	return дай(q);
}

template <class T>
void Вектор<T>::вставь(int q, const Вектор& x, int offset, int count) {
	ПРОВЕРЬ(offset >= 0 && count >= 0 && offset + count <= x.дайСчёт());
	ПРОВЕРЬ(!vector || x.vector != vector);
	вставьРяд(q, count);
	стройГлубКопию(vector + q, x.vector + offset, x.vector + offset + count);
}

template <class T>
void Вектор<T>::вставь(int i, std::initializer_list<T> init)
{
	вставьРяд(i, (int)init.size());
	T *t = vector + i;
	for(const auto& q : init)
		new(t++) T(clone(q));
}

template <class T>
template <class Диапазон>
void Вектор<T>::вставьДиапазон(int i, const Диапазон& r)
{
	вставьРяд(i, r.дайСчёт());
	T *t = vector + i;
	for(const auto& q : r)
		new(t++) T(clone(q));
}

template <class T>
void Вектор<T>::вставь(int q, const Вектор& x) {
	if(!x.дайСчёт()) return;
	вставь(q, x, 0, x.дайСчёт());
}

template <class T>
void Вектор<T>::вставь(int i, Вектор<T>&& v) {
	ПРОВЕРЬ(!vector || v.vector != vector);
	if(v.items) {
		вставьРяд(i, v.items);
		memcpy_t(vector + i, v.vector, v.items);
	}
	освободиРяд(v.vector);
	v.обнули();
}

template <class T>
void Вектор<T>::вставьРазбей(int i, Вектор<T>& v, int from)
{
	ПРОВЕРЬ(!vector || v.vector != vector && from <= v.дайСчёт());
	int n = v.дайСчёт() - from;
	if(n) {
		вставьРяд(i, n);
		memcpy_t(vector + i, v.vector + from, n);
		v.items = from;
	}
}

template <class T>
void Вектор<T>::уст(int i, const T& x, int count) {
	ПРОВЕРЬ(i >= 0 && count >= 0);
	if(count == 0) return;
	if(&x >= vector && &x < vector + items) {
		T copy = x;
		по(i + count - 1);
		Fill(vector + i, vector + i + count, copy);
	}
	else {
		по(i + count - 1);
		Fill(vector + i, vector + i + count, x);
	}
}

template <class T>
template <class Диапазон>
void Вектор<T>::устДиапазон(int i, const Диапазон& r)
{
	int count = r.дайСчёт();
	ПРОВЕРЬ(i >= 0 && count >= 0);
	if(count == 0) return;
	по(i + count - 1);
	count += i;
	for(; i < count; i++)
		vector[i] = clone(r[i]);
}

#ifdef РНЦП
template <class T>
void Вектор<T>::вРяр(РярВВ& xio, const char *itemtag)
{
	контейнерВРяр(xio, itemtag, *this);
}

template <class T>
void Вектор<T>::вДжейсон(ДжейсонВВ& jio)
{
	JsonizeArray<Вектор<T>>(jio, *this);
}

template <class C>
Ткст AsStringArray(const C& v)
{
	Ткст r;
	r << '[';
	for(int i = 0; i < v.дайСчёт(); i++) {
		if(i)
			r << ", ";
		r << v[i];
	}
	r << ']';
	return r;
}

template <class T>
Ткст Вектор<T>::вТкст() const
{
	return AsStringArray(*this);
}

#endif

// ------------------

template <class T>
void Массив<T>::освободи() {
	for(int i = 0; i < vector.дайСчёт(); i++)
		delete (T *) vector[i];
}

template <class T>
void Массив<T>::__глубКопия(const Массив<T>& v) {
	int n = v.дайСчёт();
	vector.устСчёт(n);
	for(int i = 0; i < n; i++)
		vector[i] = new T(clone(v[i]));
}

template <class T>
void  Массив<T>::обрежь(int n)
{
	ПРОВЕРЬ(n >= 0 && n <= дайСчёт());
	уд(vector.begin() + n, vector.end());
	vector.обрежь(n);
}

template <class T>
void Массив<T>::устСчёт(int n) {
	ПРОВЕРЬ(n >= 0);
	int  lc = vector.дайСчёт();
	уд(vector.begin() + n, vector.end());
	vector.устСчёт(n);
	иниц(vector.begin() + lc, vector.begin() + n);
}

template <class T>
void Массив<T>::устСчёт(int n, const T& init) {
	ПРОВЕРЬ(n >= 0);
	int  lc = vector.дайСчёт();
	уд(vector.begin() + n, vector.end());
	vector.устСчёт(n);
	иниц(vector.begin() + lc, vector.begin() + n, init);
}

template <class T>
void Массив<T>::устСчётР(int n) {
	ПРОВЕРЬ(n >= 0);
	int  lc = vector.дайСчёт();
	уд(vector.begin() + n, vector.end());
	vector.устСчётР(n);
	иниц(vector.begin() + lc, vector.begin() + n);
}

template <class T>
void Массив<T>::устСчётР(int n, const T& init) {
	ПРОВЕРЬ(n >= 0);
	int  lc = vector.дайСчёт();
	уд(vector.begin() + n, vector.end());
	vector.устСчётР(n);
	иниц(vector.begin() + lc, vector.begin() + n, init);
}

#ifdef DEPRECATED
template <class T>
int  Массив<T>::дайИндекс(const T& элт) const {
	for(ТипУказатель const *ptr = vector.begin(); ptr < vector.end(); ptr++)
		if(*ptr == (ТипУказатель)&элт) return (int)(ptr - vector.begin());
	return -1;
}
#endif

template <class T>
void Массив<T>::перемести(int i1, int i2) {
	ТипУказатель q = vector[i1];
	vector.удали(i1);
	vector.вставь(i2 - (i2 > i1)) = (T*)q;
}

template <class T>
void Массив<T>::удали(int i, int count) {
	ПРОВЕРЬ(i + count <= дайСчёт() && count >= 0 && i >= 0);
	уд(vector.begin() + i, vector.begin() + i + count);
	vector.удали(i, count);
}

template <class T>
void Массив<T>::удали(const int *sorted_list, int n)
{
	const int *q = sorted_list;
	const int *e = sorted_list + n;
	while(q < e) {
		ПРОВЕРЬ(*q >= 0 && *q < дайСчёт());
		delete (T *)vector[*q++];
	}
	vector.удали(sorted_list, n);
}

template <class T>
void Массив<T>::удали(const Вектор<int>& sorted_list)
{
	удали(sorted_list, sorted_list.дайСчёт());
}

template <class T>
template <class Условие>
void Массив<T>::удалиЕсли(Условие c)
{
	int ti = 0;
	for(int i = 0; i < vector.дайСчёт(); i++)
		if(c(i))
			delete (T *)vector[i];
		else
			vector[ti++] = vector[i];
	vector.обрежь(ti);
}


template <class T>
void Массив<T>::уст(int i, const T& x, int count) {
	ПРОВЕРЬ(i >= 0 && count >= 0);
	if(i + count >= дайСчёт())
		устСчётР(i + count);
	for(ТипУказатель *ptr = vector.begin() + i; ptr < vector.begin() + i + count; ptr++) {
		delete (T *) *ptr;
		*ptr = new T(x);
	}
}

template <class T>
void Массив<T>::вставьН(int i, int count) {
	ПРОВЕРЬ(i >= 0 && count >= 0);
	vector.вставьН(i, count);
	иниц(vector.begin() + i, vector.begin() + i + count);
}

template <class T>
void Массив<T>::вставь(int i, const T& x, int count) {
	vector.вставьН(i, count);
	иниц(vector.begin() + i, vector.begin() + i + count, x);
}

template <class T>
template <class Диапазон>
void Массив<T>::вставьДиапазон(int i, const Диапазон& r)
{
	vector.вставьН(i, r.дайСчёт());
	T **t = дайУк(i);
	for(const auto& x : r)
		*t++ = new T(clone(x));
}

template <class T>
T& Массив<T>::вставь(int i, T&& x)
{
	vector.вставьН(i, 1);
	vector[i] = new T(pick(x));
	return дай(i);
}

template <class T>
T& Массив<T>::вставь(int i, T *newt) {
	vector.вставьН(i, 1);
	vector[i] = newt;
	return *newt;
}

template <class T>
void Массив<T>::вставь(int i, const Массив& x) {
	вставь(i, x, 0, x.дайСчёт());
}

template <class T>
void Массив<T>::вставь(int i, const Массив& x, int offset, int count) {
	vector.вставьН(i, count);
	for(int q = 0; q < count; q++)
		vector[q + i] = new T(x[q + offset]);
}

template <class T>
void Массив<T>::вставь(int i, std::initializer_list<T> init)
{
	vector.вставьН(i, (int)init.size());
	for(const auto& q : init)
		vector[i++] = new T(q);
}

#ifdef РНЦП
template <class T>
void Массив<T>::вРяр(РярВВ& xio, const char *itemtag)
{
	контейнерВРяр(xio, itemtag, *this);
}

template <class T>
void Массив<T>::вДжейсон(ДжейсонВВ& jio)
{
	JsonizeArray<Массив<T>>(jio, *this);
}

template <class T>
Ткст Массив<T>::вТкст() const
{
	return AsStringArray(*this);
}

#endif

// ------------------

template <class T>
void БиВектор<T>::перемест(int newalloc) {
	ПРОВЕРЬ(items <= newalloc && items >= 0);
	T *newvector = newalloc ? (T *) MemoryAlloc(newalloc * sizeof(T)) : NULL;
	if(items) {
		int end = start + items;
		if(end <= alloc)
			memcpy_t(newvector, vector + start, end - start);
		else {
			memcpy_t(newvector, vector + start, alloc - start);
			memcpy_t(newvector + alloc - start, vector, end - alloc);
		}
		MemoryFree(vector);
	}
	vector = newvector;
	alloc = newalloc;
	start = 0;
}

template <class T>
void БиВектор<T>::глубокаяКопия0(const БиВектор& ист) {
	alloc = items = ист.items;
	vector = alloc ? (T *) new byte[alloc * sizeof(T)] : NULL;
	if(items) {
		int end = ист.start + ист.items;
		if(end <= ист.alloc)
			стройГлубКопию(vector, ист.vector + ист.start, ист.vector + end);
		else {
			стройГлубКопию(vector, ист.vector + ист.start, ист.vector + ист.alloc);
			стройГлубКопию(vector + ист.alloc - ист.start, ист.vector,
				              ист.vector + end - ист.alloc);
		}
	}
	start = 0;
}

template <class T>
void БиВектор<T>::очисть() {
	освободи();
	start = items = alloc = 0;
	vector = NULL;
}

template <class T>
void БиВектор<T>::добавь0() {
	ПРОВЕРЬ(items >= 0);
	if(items >= alloc)
		перемест(max(2 * items, 4));
	items++;
}

template <class T>
void БиВектор<T>::сожми() {
	ПРОВЕРЬ(items >= 0);
	if(items < alloc)
		перемест(items);
}

template <class T>
void БиВектор<T>::резервируй(int n) {
	ПРОВЕРЬ(items >= 0);
	n += items;
	if(n > alloc)
		перемест(n);
}

template <class T>
void БиВектор<T>::освободи() {
	if(vector && items >= 0) {
		int end = start + items;
		if(end <= alloc)
			разрушь(vector + start, vector + end);
		else {
			разрушь(vector + start, vector + alloc);
			разрушь(vector, vector + end - alloc);
		}
		MemoryFree(vector);
	}
}

#ifdef РНЦП
template <class T>
void БиВектор<T>::сериализуй(Поток& s) {
	int n = items;
	s / n;
	if(s.грузится()) {
		очисть();
		while(n--)
			s % добавьХвост();
	}
	else
		for(int i = 0; i < items; i++)
			s % operator[](i);
}

template <class T>
Ткст БиВектор<T>::вТкст() const
{
	return AsStringArray(*this);
}

#endif

template <class T>
БиВектор<T>::БиВектор(std::initializer_list<T> init)
{
	start = items = alloc = 0; vector = NULL;

	start = 0;
	alloc = items = (int)init.size();
	if(!alloc) {
		vector = NULL;
		return;
	}
	vector = (T *) new byte[alloc * sizeof(T)];
	T *t = vector;
	for(const auto& q : init)
		new (t++) T(q);
}

// ------------------

template <class T>
void БиМассив<T>::освободи() {
	for(int i = 0; i < дайСчёт(); i++)
		delete (T *) bv[i];
}

template <class T>
void БиМассив<T>::глубокаяКопия0(const БиМассив<T>& v) {
	int n = v.дайСчёт();
	bv.очисть();
	bv.резервируй(v.дайСчёт());
	for(int i = 0; i < n; i++)
		bv.добавьХвост() = new T(clone(v[i]));
}

#ifdef РНЦП
template <class T>
void БиМассив<T>::сериализуй(Поток& s) {
	int n = bv.дайСчёт();
	s / n;
	if(s.грузится()) {
		очисть();
		while(n--)
			s % добавьХвост();
	}
	else
		for(int i = 0; i < bv.дайСчёт(); i++)
			s % operator[](i);
}

template <class T>
Ткст БиМассив<T>::вТкст() const
{
	return AsStringArray(*this);
}

template <class T>
БиМассив<T>::БиМассив(std::initializer_list<T> init)
{
	for(const auto& q : init)
		добавьХвост(q);
}

#endif

inline
void   Биты::уст(int i, dword bits, int count)
{
	ПРОВЕРЬ(i >= 0 && count >= 0 && count <= 32);
	if(!count) // note: this also avoids problem with (dword)-1 >> 32 being undefined
		return;
	int q = i >> 5;
	i &= 31;
	if(q >= alloc)
		расширь(q);
	if(i == 0 && count == 32) {
		bp[q] = bits;
		return;
	}
	dword mask = (dword)-1 >> (32 - count); // does not work for count == 0
	bits &= mask;
	bp[q] = (bp[q] & ~(mask << i)) | (bits << i);
	if(i + count <= 32)
		return;
	if(++q >= alloc)
		расширь(q);
	i = 32 - i;
	bp[q] = (bp[q] & ~(mask >> i)) | (bits >> i);
}

inline
dword Биты::дай(int i, int count)
{
	ПРОВЕРЬ(i >= 0 && count >= 0 && count <= 32);
	if(!count) // note: this also avoids problem with (dword)-1 >> 32 being undefined
		return 0;
	int q = i >> 5;
	i &= 31;
	if(q >= alloc)
		return 0;
	if(i == 0 && count == 32)
		return bp[q];
	dword mask = (dword)-1 >> (32 - count); // does not work for count == 0
	dword ret = (bp[q] & (mask << i)) >> i;
	if(i + count <= 32 || ++q >= alloc)
		return ret;
	i = 32 - i;
	return ret | ((bp[q] & (mask >> i)) << i);
}

inline
void Биты::уст64(int i, uint64 bits, int count)
{
	if(count > 32) {
		уст(i, LODWORD(bits), 32);
		уст(i + 32, HIDWORD(bits), count - 32);
	}
	else
		уст(i, LODWORD(bits), count);
}

inline
uint64 Биты::дай64(int i, int count)
{
	if(count > 32) {
		dword l = дай(i, 32);
		return MAKEQWORD(l, дай(i + 32, count - 32));
	}
	else
		return дай(i, count);
}

inline
void Биты::устН(int i, bool b, int count)
{
	ПРОВЕРЬ(i >= 0);
	if(!count) // note: this also avoids problem with (dword)-1 >> 32 being undefined
		return;
	
	dword bits = (dword)0 - b;

	int q = i >> 5;
	i &= 31;
	if(q >= alloc)
		расширь(q);

	int n = min(32 - i, count); // number of bits to dword boundary (or count if smaller)
	dword mask = (dword)-1 >> (32 - n);
	bp[q] = (bp[q] & ~(mask << i)) | ((bits & mask) << i);
	count -= n;
	if(!count)
		return;
	
	int dw_count = count >> 5;
	if(++q + dw_count >= alloc)
		расширь(q + dw_count);
	while(dw_count) {
		bp[q++] = bits;
		dw_count--;
	}
	
	count = count & 31; // remaining bits to set
	if(!count)
		return;
	if(q >= alloc)
		расширь(q);
	mask = (dword)-1 >> (32 - count);
	bp[q] = (bp[q] & ~mask) | (bits & mask);
}
