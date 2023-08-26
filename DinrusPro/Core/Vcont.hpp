template <class T>
проц StreamContainer(Поток& s, T& cont)
{
	цел n = cont.дайСчёт();
	s / n;
	if(n < 0) {
		s.загрузиОш();
		return;
	}
	if(s.грузится())
	{
		cont.очисть();
		cont.резервируй(мин(n, цел(256*1024 / sizeof(T)))); // protect against invalid streams...
		while(n--)
			s % cont.добавь();
	}
	else
	{
		for(auto укз = cont.begin(); n--; ++укз)
			s % *укз;
	}
}

template <class T>
T * Вектор<T>::разместиРяд(цел& n)
{
	т_мера sz0 = n * sizeof(T);
	т_мера sz = sz0;
	ук q = разместиПамТн(sz);
	n += (цел)((sz - sz0) / sizeof(T));
	return (T *)q;
}

template <class T>
бул Вектор<T>::перемест(цел newalloc)
{
	ПРОВЕРЬ(newalloc >= items);
	т_мера sz0 = (т_мера)newalloc * sizeof(T);
	т_мера sz = sz0;
	ук newvector;
	бул  alloced = true;
	if(пробуйПереместПам(vector, sz)) {
		newvector = vector;
		vector = NULL;
		alloced = false;
	}
	else
		newvector = newalloc ? разместиПамТн(sz) : NULL;
	alloc = newalloc == INT_MAX ? INT_MAX // maximum alloc reached
	        : (цел)((sz - sz0) / sizeof(T) + newalloc); // adjust alloc to real memory size
	if(vector && newvector)
		т_копирпам((T *)newvector, vector, items);
	vector = (T *)newvector;
	return alloced;
}

template <class T>
проц Вектор<T>::ReAllocF(цел newalloc)
{
	ук prev = vector;
	if(перемест(newalloc) && prev)
		освободиПам(prev);
}

template <class T>
бул Вектор<T>::GrowSz()
{
	if(alloc == INT_MAX)
		паника("Too many items in container!");
#ifdef _ОТЛАДКА
	return перемест(макс(alloc + 1, alloc >= INT_MAX / 2 ? INT_MAX : 2 * alloc));
#else
	return перемест(макс(alloc + 1, alloc >= цел((дол)2 * INT_MAX / 3) ? INT_MAX : alloc + (alloc >> 1)));
#endif
}

template <class T>
проц Вектор<T>::GrowF()
{
	ук prev = vector;
	if(GrowSz())
		освободиПам(prev);
}

template <class T>
проц Вектор<T>::подбери(Вектор<T>&& v)
{
	vector = v.vector;
	items = v.items;
	alloc = v.alloc;
	v.обнули();
}

#ifdef DEPRECATED
template <class T>
цел  Вектор<T>::дайИндекс(const T& элт) const
{
	if(vector == NULL) return -1;
	цел n = &элт - vector;
	return n >= 0 && n < items ? n : -1;
}
#endif

template <class T>
проц Вектор<T>::резервируй(цел n)
{
	if(n > alloc)
		ReAllocF(n);
}

template <class T>
проц Вектор<T>::освободи() {
	if(vector && items >= 0)
		разрушь((T *)vector, (T *)vector + items);
	освободиРяд(vector);
}

template <class T>
проц Вектор<T>::очисть() {
	освободи();
	alloc = items = 0;
	vector = NULL;
}

template <class T>
проц Вектор<T>::__глубКопия(const Вектор& ист) {
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
	бул b = GrowSz();
	T *q = new(Rdd()) T(x);
	if(b) освободиРяд(prev);
	return *q;
}

template <class T>
T& Вектор<T>::нарастиДобавь(T&& x) {
	T *prev = vector;
	бул b = GrowSz();
	T *q = ::new(Rdd()) T(пикуй(x));
	if(b) освободиРяд(prev);
	return *q;
}

template <class T> inline
проц Вектор<T>::добавьН(цел n)
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
проц Вектор<T>::обрежь(цел n)
{
	ПРОВЕРЬ(n >= 0 && n <= items);
	разрушь(vector + n, vector + items);
	items = n;
}

template <class T>
проц Вектор<T>::устСчёт(цел n) {
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
проц Вектор<T>::устСчёт(цел n, const T& init) {
	ПРОВЕРЬ(n >= 0);
	if(n == items) return;
	if(n < items)
		разрушь(vector + n, vector + items);
	else {
		if(n > alloc) {
			T *prev = vector;
			бул b = перемест(n); // because init can be in original vector
			стройЗаполниГлубКопию(vector + items, vector + n, init);
			if(b) освободиРяд(prev);
		}
		else
			стройЗаполниГлубКопию(vector + items, vector + n, init);
	}
	items = n;
}

template <class T>
проц Вектор<T>::устСчётР(цел n) {
	if(n > alloc)
		ReAllocF(alloc + макс(alloc, n - items));
	устСчёт(n);
}

template <class T>
проц Вектор<T>::устСчётР(цел n, const T& init) {
	ПРОВЕРЬ(n >= 0);
	if(n == items) return;
	if(n < items)
		разрушь(vector + n, vector + items);
	else
		if(n > alloc) {
			T *prev = vector;
			бул b = перемест(alloc + макс(alloc, n - items));  // because init can be in original vector
			стройЗаполниГлубКопию(vector + items, vector + n, init);
			if(b) освободиРяд(prev);
		}
		else
			стройЗаполниГлубКопию(vector + items, vector + n, init);
	items = n;
}

template <class T>
проц Вектор<T>::удали(цел q, цел count) {
	ПРОВЕРЬ(q >= 0 && q <= items - count && count >= 0);
	if(count == 0) return;
	разрушь(vector + q, vector + q + count);
	memmove((проц *)(vector + q), (проц *)(vector + q + count), (items - q - count) * sizeof(T));
	items -= count;
}

template <class T>
проц Вектор<T>::удали(const цел *sorted_list, цел n)
{
	if(!n) return;
	цел pos = *sorted_list;
	цел npos = pos;
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
проц Вектор<T>::удали(const Вектор<цел>& v)
{
	удали(v, v.дайСчёт());
}

template <class T>
template <class Условие>
проц Вектор<T>::удалиЕсли(Условие c)
{
	цел i = 0;
	for(; i < items; i++) // run to the first element without moving
		if(c(i)) {
			(vector + i)->~T();
			break;
		}
	цел ti = i++;
	for(; i < items; i++)
		if(c(i))
			(vector + i)->~T();
		else
			*((Data_S_<sizeof(T)>*)vector + ti++) = *((Data_S_<sizeof(T)>*)vector + i);
	items = ti;
}

template <class T>
проц Вектор<T>::вставьРяд(цел q, цел count)
{
	ПРОВЕРЬ(count >= 0);
	ПРОВЕРЬ(q >= 0 && q <= items);
	if(!count) return;
	if(items + count > alloc) {
		T *newvector = разместиРяд(alloc = макс(alloc + count, цел(alloc + ((unsigned)alloc >> 1))));
		if(vector) {
			т_копирпам(newvector, vector, q);
			т_копирпам(newvector + q + count, vector + q, items - q);
			освободиРяд(vector);
		}
		vector = newvector;
	}
	else
		memmove((проц *)(vector + q + count), (проц *)(vector + q), (items - q) * sizeof(T));
	items += count;
}

template <class T>
проц Вектор<T>::вставьН(цел q, цел count) {
	ПРОВЕРЬ(count >= 0);
	ПРОВЕРЬ(q >= 0 && q <= items);
	вставьРяд(q, count);
	строй(vector + q, vector + q + count);
}

template <class T>
проц Вектор<T>::вставь(цел q, const T& x, цел count) {
	if(!count) return;
	ПРОВЕРЬ(!(&x >= vector && &x < vector + items));
	вставьРяд(q, count);
	стройЗаполниГлубКопию(vector + q, vector + q + count, x);
}

template <class T>
T& Вектор<T>::вставь(цел q, T&& x)
{
	ПРОВЕРЬ(&x < vector || &x > vector + items);
	вставьРяд(q, 1);
	::new(vector + q) T(пикуй(x));
	return дай(q);
}

template <class T>
проц Вектор<T>::вставь(цел q, const Вектор& x, цел offset, цел count) {
	ПРОВЕРЬ(offset >= 0 && count >= 0 && offset + count <= x.дайСчёт());
	ПРОВЕРЬ(!vector || x.vector != vector);
	вставьРяд(q, count);
	стройГлубКопию(vector + q, x.vector + offset, x.vector + offset + count);
}

template <class T>
проц Вектор<T>::вставь(цел i, std::initializer_list<T> init)
{
	вставьРяд(i, (цел)init.size());
	T *t = vector + i;
	for(const auto& q : init)
		new(t++) T(клонируй(q));
}

template <class T>
template <class Диапазон>
проц Вектор<T>::вставьДиапазон(цел i, const Диапазон& r)
{
	вставьРяд(i, r.дайСчёт());
	T *t = vector + i;
	for(const auto& q : r)
		new(t++) T(клонируй(q));
}

template <class T>
проц Вектор<T>::вставь(цел q, const Вектор& x) {
	if(!x.дайСчёт()) return;
	вставь(q, x, 0, x.дайСчёт());
}

template <class T>
проц Вектор<T>::вставь(цел i, Вектор<T>&& v) {
	ПРОВЕРЬ(!vector || v.vector != vector);
	if(v.items) {
		вставьРяд(i, v.items);
		т_копирпам(vector + i, v.vector, v.items);
	}
	освободиРяд(v.vector);
	v.обнули();
}

template <class T>
проц Вектор<T>::вставьРазбей(цел i, Вектор<T>& v, цел from)
{
	ПРОВЕРЬ(!vector || v.vector != vector && from <= v.дайСчёт());
	цел n = v.дайСчёт() - from;
	if(n) {
		вставьРяд(i, n);
		т_копирпам(vector + i, v.vector + from, n);
		v.items = from;
	}
}

template <class T>
проц Вектор<T>::уст(цел i, const T& x, цел count) {
	ПРОВЕРЬ(i >= 0 && count >= 0);
	if(count == 0) return;
	if(&x >= vector && &x < vector + items) {
		T copy = x;
		по(i + count - 1);
		заполни(vector + i, vector + i + count, copy);
	}
	else {
		по(i + count - 1);
		заполни(vector + i, vector + i + count, x);
	}
}

template <class T>
template <class Диапазон>
проц Вектор<T>::устДиапазон(цел i, const Диапазон& r)
{
	цел count = r.дайСчёт();
	ПРОВЕРЬ(i >= 0 && count >= 0);
	if(count == 0) return;
	по(i + count - 1);
	count += i;
	for(; i < count; i++)
		vector[i] = клонируй(r[i]);
}

#ifdef РНЦП
template <class T>
проц Вектор<T>::вРяр(РярВВ& xio, кткст0 itemtag)
{
	контейнерВРяр(xio, itemtag, *this);
}

template <class T>
проц Вектор<T>::вДжейсон(ДжейсонВВ& jio)
{
	джейсонируйМассив<Вектор<T>>(jio, *this);
}

template <class C>
Ткст AsStringArray(const C& v)
{
	Ткст r;
	r << '[';
	for(цел i = 0; i < v.дайСчёт(); i++) {
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
проц Массив<T>::освободи() {
	for(цел i = 0; i < vector.дайСчёт(); i++)
		delete (T *) vector[i];
}

template <class T>
проц Массив<T>::__глубКопия(const Массив<T>& v) {
	цел n = v.дайСчёт();
	vector.устСчёт(n);
	for(цел i = 0; i < n; i++)
		vector[i] = new T(клонируй(v[i]));
}

template <class T>
проц  Массив<T>::обрежь(цел n)
{
	ПРОВЕРЬ(n >= 0 && n <= дайСчёт());
	уд(vector.begin() + n, vector.end());
	vector.обрежь(n);
}

template <class T>
проц Массив<T>::устСчёт(цел n) {
	ПРОВЕРЬ(n >= 0);
	цел  lc = vector.дайСчёт();
	уд(vector.begin() + n, vector.end());
	vector.устСчёт(n);
	иниц(vector.begin() + lc, vector.begin() + n);
}

template <class T>
проц Массив<T>::устСчёт(цел n, const T& init) {
	ПРОВЕРЬ(n >= 0);
	цел  lc = vector.дайСчёт();
	уд(vector.begin() + n, vector.end());
	vector.устСчёт(n);
	иниц(vector.begin() + lc, vector.begin() + n, init);
}

template <class T>
проц Массив<T>::устСчётР(цел n) {
	ПРОВЕРЬ(n >= 0);
	цел  lc = vector.дайСчёт();
	уд(vector.begin() + n, vector.end());
	vector.устСчётР(n);
	иниц(vector.begin() + lc, vector.begin() + n);
}

template <class T>
проц Массив<T>::устСчётР(цел n, const T& init) {
	ПРОВЕРЬ(n >= 0);
	цел  lc = vector.дайСчёт();
	уд(vector.begin() + n, vector.end());
	vector.устСчётР(n);
	иниц(vector.begin() + lc, vector.begin() + n, init);
}

#ifdef DEPRECATED
template <class T>
цел  Массив<T>::дайИндекс(const T& элт) const {
	for(ТипУказатель const *укз = vector.begin(); укз < vector.end(); укз++)
		if(*укз == (ТипУказатель)&элт) return (цел)(укз - vector.begin());
	return -1;
}
#endif

template <class T>
проц Массив<T>::перемести(цел i1, цел i2) {
	ТипУказатель q = vector[i1];
	vector.удали(i1);
	vector.вставь(i2 - (i2 > i1)) = (T*)q;
}

template <class T>
проц Массив<T>::удали(цел i, цел count) {
	ПРОВЕРЬ(i + count <= дайСчёт() && count >= 0 && i >= 0);
	уд(vector.begin() + i, vector.begin() + i + count);
	vector.удали(i, count);
}

template <class T>
проц Массив<T>::удали(const цел *sorted_list, цел n)
{
	const цел *q = sorted_list;
	const цел *e = sorted_list + n;
	while(q < e) {
		ПРОВЕРЬ(*q >= 0 && *q < дайСчёт());
		delete (T *)vector[*q++];
	}
	vector.удали(sorted_list, n);
}

template <class T>
проц Массив<T>::удали(const Вектор<цел>& sorted_list)
{
	удали(sorted_list, sorted_list.дайСчёт());
}

template <class T>
template <class Условие>
проц Массив<T>::удалиЕсли(Условие c)
{
	цел ti = 0;
	for(цел i = 0; i < vector.дайСчёт(); i++)
		if(c(i))
			delete (T *)vector[i];
		else
			vector[ti++] = vector[i];
	vector.обрежь(ti);
}


template <class T>
проц Массив<T>::уст(цел i, const T& x, цел count) {
	ПРОВЕРЬ(i >= 0 && count >= 0);
	if(i + count >= дайСчёт())
		устСчётР(i + count);
	for(ТипУказатель *укз = vector.begin() + i; укз < vector.begin() + i + count; укз++) {
		delete (T *) *укз;
		*укз = new T(x);
	}
}

template <class T>
проц Массив<T>::вставьН(цел i, цел count) {
	ПРОВЕРЬ(i >= 0 && count >= 0);
	vector.вставьН(i, count);
	иниц(vector.begin() + i, vector.begin() + i + count);
}

template <class T>
проц Массив<T>::вставь(цел i, const T& x, цел count) {
	vector.вставьН(i, count);
	иниц(vector.begin() + i, vector.begin() + i + count, x);
}

template <class T>
template <class Диапазон>
проц Массив<T>::вставьДиапазон(цел i, const Диапазон& r)
{
	vector.вставьН(i, r.дайСчёт());
	T **t = дайУк(i);
	for(const auto& x : r)
		*t++ = new T(клонируй(x));
}

template <class T>
T& Массив<T>::вставь(цел i, T&& x)
{
	vector.вставьН(i, 1);
	vector[i] = new T(пикуй(x));
	return дай(i);
}

template <class T>
T& Массив<T>::вставь(цел i, T *newt) {
	vector.вставьН(i, 1);
	vector[i] = newt;
	return *newt;
}

template <class T>
проц Массив<T>::вставь(цел i, const Массив& x) {
	вставь(i, x, 0, x.дайСчёт());
}

template <class T>
проц Массив<T>::вставь(цел i, const Массив& x, цел offset, цел count) {
	vector.вставьН(i, count);
	for(цел q = 0; q < count; q++)
		vector[q + i] = new T(x[q + offset]);
}

template <class T>
проц Массив<T>::вставь(цел i, std::initializer_list<T> init)
{
	vector.вставьН(i, (цел)init.size());
	for(const auto& q : init)
		vector[i++] = new T(q);
}

#ifdef РНЦП
template <class T>
проц Массив<T>::вРяр(РярВВ& xio, кткст0 itemtag)
{
	контейнерВРяр(xio, itemtag, *this);
}

template <class T>
проц Массив<T>::вДжейсон(ДжейсонВВ& jio)
{
	джейсонируйМассив<Массив<T>>(jio, *this);
}

template <class T>
Ткст Массив<T>::вТкст() const
{
	return AsStringArray(*this);
}

#endif

// ------------------

template <class T>
проц БиВектор<T>::перемест(цел newalloc) {
	ПРОВЕРЬ(items <= newalloc && items >= 0);
	T *newvector = newalloc ? (T *) разместиПам(newalloc * sizeof(T)) : NULL;
	if(items) {
		цел end = start + items;
		if(end <= alloc)
			т_копирпам(newvector, vector + start, end - start);
		else {
			т_копирпам(newvector, vector + start, alloc - start);
			т_копирпам(newvector + alloc - start, vector, end - alloc);
		}
		освободиПам(vector);
	}
	vector = newvector;
	alloc = newalloc;
	start = 0;
}

template <class T>
проц БиВектор<T>::глубокаяКопия0(const БиВектор& ист) {
	alloc = items = ист.items;
	vector = alloc ? (T *) new ббайт[alloc * sizeof(T)] : NULL;
	if(items) {
		цел end = ист.start + ист.items;
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
проц БиВектор<T>::очисть() {
	освободи();
	start = items = alloc = 0;
	vector = NULL;
}

template <class T>
проц БиВектор<T>::добавь0() {
	ПРОВЕРЬ(items >= 0);
	if(items >= alloc)
		перемест(макс(2 * items, 4));
	items++;
}

template <class T>
проц БиВектор<T>::сожми() {
	ПРОВЕРЬ(items >= 0);
	if(items < alloc)
		перемест(items);
}

template <class T>
проц БиВектор<T>::резервируй(цел n) {
	ПРОВЕРЬ(items >= 0);
	n += items;
	if(n > alloc)
		перемест(n);
}

template <class T>
проц БиВектор<T>::освободи() {
	if(vector && items >= 0) {
		цел end = start + items;
		if(end <= alloc)
			разрушь(vector + start, vector + end);
		else {
			разрушь(vector + start, vector + alloc);
			разрушь(vector, vector + end - alloc);
		}
		освободиПам(vector);
	}
}

#ifdef РНЦП
template <class T>
проц БиВектор<T>::сериализуй(Поток& s) {
	цел n = items;
	s / n;
	if(s.грузится()) {
		очисть();
		while(n--)
			s % добавьХвост();
	}
	else
		for(цел i = 0; i < items; i++)
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
	alloc = items = (цел)init.size();
	if(!alloc) {
		vector = NULL;
		return;
	}
	vector = (T *) new ббайт[alloc * sizeof(T)];
	T *t = vector;
	for(const auto& q : init)
		new (t++) T(q);
}

// ------------------

template <class T>
проц БиМассив<T>::освободи() {
	for(цел i = 0; i < дайСчёт(); i++)
		delete (T *) bv[i];
}

template <class T>
проц БиМассив<T>::глубокаяКопия0(const БиМассив<T>& v) {
	цел n = v.дайСчёт();
	bv.очисть();
	bv.резервируй(v.дайСчёт());
	for(цел i = 0; i < n; i++)
		bv.добавьХвост() = new T(клонируй(v[i]));
}

#ifdef РНЦП
template <class T>
проц БиМассив<T>::сериализуй(Поток& s) {
	цел n = bv.дайСчёт();
	s / n;
	if(s.грузится()) {
		очисть();
		while(n--)
			s % добавьХвост();
	}
	else
		for(цел i = 0; i < bv.дайСчёт(); i++)
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
проц   Биты::уст(цел i, бцел bits, цел count)
{
	ПРОВЕРЬ(i >= 0 && count >= 0 && count <= 32);
	if(!count) // note: this also avoids problem with (бцел)-1 >> 32 being undefined
		return;
	цел q = i >> 5;
	i &= 31;
	if(q >= alloc)
		расширь(q);
	if(i == 0 && count == 32) {
		bp[q] = bits;
		return;
	}
	бцел mask = (бцел)-1 >> (32 - count); // does not work for count == 0
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
бцел Биты::дай(цел i, цел count)
{
	ПРОВЕРЬ(i >= 0 && count >= 0 && count <= 32);
	if(!count) // note: this also avoids problem with (бцел)-1 >> 32 being undefined
		return 0;
	цел q = i >> 5;
	i &= 31;
	if(q >= alloc)
		return 0;
	if(i == 0 && count == 32)
		return bp[q];
	бцел mask = (бцел)-1 >> (32 - count); // does not work for count == 0
	бцел ret = (bp[q] & (mask << i)) >> i;
	if(i + count <= 32 || ++q >= alloc)
		return ret;
	i = 32 - i;
	return ret | ((bp[q] & (mask >> i)) << i);
}

inline
проц Биты::уст64(цел i, бдол bits, цел count)
{
	if(count > 32) {
		уст(i, МЛБЦЕЛ(bits), 32);
		уст(i + 32, СДЕЛАЙБЦЕЛ(bits), count - 32);
	}
	else
		уст(i, МЛБЦЕЛ(bits), count);
}

inline
бдол Биты::дай64(цел i, цел count)
{
	if(count > 32) {
		бцел l = дай(i, 32);
		return СДЕЛАЙБДОЛ(l, дай(i + 32, count - 32));
	}
	else
		return дай(i, count);
}

inline
проц Биты::устН(цел i, бул b, цел count)
{
	ПРОВЕРЬ(i >= 0);
	if(!count) // note: this also avoids problem with (бцел)-1 >> 32 being undefined
		return;
	
	бцел bits = (бцел)0 - b;

	цел q = i >> 5;
	i &= 31;
	if(q >= alloc)
		расширь(q);

	цел n = мин(32 - i, count); // number of bits to бцел boundary (or count if smaller)
	бцел mask = (бцел)-1 >> (32 - n);
	bp[q] = (bp[q] & ~(mask << i)) | ((bits & mask) << i);
	count -= n;
	if(!count)
		return;
	
	цел dw_count = count >> 5;
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
	mask = (бцел)-1 >> (32 - count);
	bp[q] = (bp[q] & ~mask) | (bits & mask);
}
