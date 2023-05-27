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

template <class T>
class Буфер : Движ< Буфер<T> > {
	T *укз;
	
	проц разместпам(т_мера size) {
		if(std::is_trivially_destructible<T>::value)
			укз = (T *)разместиПам(size * sizeof(T));
		else {
			ук p = разместиПам(size * sizeof(T) + 16);
			*(т_мера *)p = size;
			укз = (T *)((ббайт *)p + 16);
		}
	}
	проц нов(т_мера size) {
		разместпам(size);
		строй(укз, укз + size);
	}
	проц нов(т_мера size, const T& in) {
		разместпам(size);
		стройЗаполниГлубКопию(укз, укз + size, in);
	}
	проц освободи() {
		if(укз) {
			if(std::is_trivially_destructible<T>::value)
				освободиПам(укз);
			else {
				ук p = (ббайт *)укз - 16;
				т_мера size = *(т_мера *)p;
				разрушь(укз, укз + size);
				освободиПам(p);
			}
		}
	}

public:
	operator T*()                        { return укз; }
	operator const T*() const            { return укз; }
	T *operator~()                       { return укз; }
	const T *operator~() const           { return укз; }
	T          *дай()                    { return укз; }
	const T    *дай() const              { return укз; }
	T          *begin()                  { return укз; }
	const T    *begin() const            { return укз; }

	проц размести(т_мера size)              { очисть(); нов(size); }
	проц размести(т_мера size, const T& in) { очисть(); нов(size, in); }

	проц очисть()                         { освободи(); укз = NULL; }
	бул пустой() const                 { return укз == NULL; }

	Буфер()                             { укз = NULL; }
	Буфер(т_мера size)                  { нов(size); }
	Буфер(т_мера size, const T& init)   { нов(size, init); }
	~Буфер()                            { освободи(); }

	проц operator=(Буфер&& v)           { if(&v != this) { очисть(); укз = v.укз; v.укз = NULL; } }
	Буфер(Буфер&& v)                   { укз = v.укз; v.укз = NULL; }

	Буфер(т_мера size, std::initializer_list<T> init) {
		разместпам(size); T *t = укз; for(const auto& i : init) new (t++) T(i);
	}
	Буфер(std::initializer_list<T> init) : Буфер(init.size(), init) {}
};

template <class T>
class Один : ОпцияДвижимогоИГлубКопии< Один<T> > {
	mutable T  *укз;

	проц        освободи()                     { if(укз && укз != (T*)1) delete укз; }
	template <class TT>
	проц        подбери(Один<TT>&& данные)       { укз = данные.открепи(); }

public:
	проц        прикрепи(T *данные)            { освободи(); укз = данные; }
	T          *открепи()                   { T *t = укз; укз = NULL; return t; }
	проц        очисть()                    { освободи(); укз = NULL; }

	проц        operator=(T *данные)         { прикрепи(данные); }
	
	template <class TT>
	проц        operator=(Один<TT>&& d)     { if((проц *)this != (проц *)&d) { освободи(); подбери(pick(d)); }}

	const T    *operator->() const         { ПРОВЕРЬ(укз); return укз; }
	T          *operator->()               { ПРОВЕРЬ(укз); return укз; }
	const T    *operator~() const          { return укз; }
	T          *operator~()                { return укз; }
	const T    *дай() const                { return укз; }
	T          *дай()                      { return укз; }
	const T&    operator*() const          { ПРОВЕРЬ(укз); return *укз; }
	T&          operator*()                { ПРОВЕРЬ(укз); return *укз; }

	template <class TT, class... Арги>
	TT&         создай(Арги&&... арги)     { TT *q = new TT(std::forward<Арги>(арги)...); прикрепи(q); return *q; }
	template <class TT> // with C++ conforming compiler, this would not be needed - GCC bug workaround
	TT&         создай()                   { TT *q = new TT; прикрепи(q); return *q; }
	template <class... Арги>
	T&          создай(Арги&&... арги)     { T *q = new T(std::forward<Арги>(арги)...); прикрепи(q); return *q; }
	T&          создай()                   { T *q = new T; прикрепи(q); return *q; }

	template <class TT>
	бул        является() const                 { return dynamic_cast<const TT *>(укз); }

	бул        пустой() const            { return !укз; }

	operator бул() const                  { return укз; }
	
	Ткст вТкст() const                { return укз ? какТкст(*укз) : "<empty>"; }

	Один()                                  { укз = NULL; }
	Один(T *newt)                           { укз = newt; }
	template <class TT>
	Один(Один<TT>&& p)                       { подбери(pick(p)); }
	Один(const Один<T>& p, цел)              { укз = p.пустой() ? NULL : new T(клонируй(*p)); }
	Один(const Один<T>& p) = delete;
	проц operator=(const Один<T>& p) = delete;
	~Один()                                 { освободи(); }
};

template <class T, class... Арги>
Один<T> сделайОдин(Арги&&... арги) {
	Один<T> r;
	r.создай(std::forward<Арги>(арги)...);
	return r;
}

template <class U> class Индекс;

template <class T>
class Вектор : public ОпцияДвижимогоИГлубКопии< Вектор<T> > {
	T       *vector;
	цел      items;
	цел      alloc;

	static проц    освободиРяд(T *укз)            { if(укз) освободиПам(укз); }
	static T      *разместиРяд(цел& n);

	проц     обнули()                          { vector = NULL; items = alloc = 0; }
	проц     подбери(Вектор<T>&& v);

	T       *Rdd()                           { return vector + items++; }

	проц     освободи();
	проц     __глубКопия(const Вектор& ист);
	T&       дай(цел i) const                { ПРОВЕРЬ(i >= 0 && i < items); return vector[i]; }
	бул     перемест(цел alloc);
	проц     ReAllocF(цел alloc);
	бул     GrowSz();
	проц     GrowF();
	T&       нарастиДобавь(const T& x);
	T&       нарастиДобавь(T&& x);
	проц     вставьРяд(цел q, цел count);

	template <class U> friend class Индекс;

public:
	T&       добавь()                           { if(items >= alloc) GrowF(); return *(::new(Rdd()) T); }
	T&       добавь(const T& x)                 { return items < alloc ? *(new(Rdd()) T(x)) : нарастиДобавь(x); }
	T&       добавь(T&& x)                      { return items < alloc ? *(::new(Rdd()) T(pick(x))) : нарастиДобавь(pick(x)); }
	template <class... Арги>
	T&       создай(Арги&&... арги)          { if(items >= alloc) GrowF(); return *(::new(Rdd()) T(std::forward<Арги>(арги)...)); }
	проц     добавьН(цел n);
	const T& operator[](цел i) const         { return дай(i); }
	T&       operator[](цел i)               { return дай(i); }
	const T& дай(цел i, const T& опр) const  { return i >= 0 && i < дайСчёт() ? дай(i) : опр; }
	T&       дай(цел i, T& опр)              { return i >= 0 && i < дайСчёт() ? дай(i) : опр; }
	цел      дайСчёт() const                { return items; }
	бул     пустой() const                 { return items == 0; }
	проц     обрежь(цел n);
	проц     устСчёт(цел n);
	проц     устСчёт(цел n, const T& init);
	проц     устСчётР(цел n);
	проц     устСчётР(цел n, const T& init);
	проц     очисть();

	T&       по(цел i)                  { if(i >= items) устСчётР(i + 1); return (*this)[i]; }
	T&       по(цел i, const T& x)      { if(i >= items) устСчётР(i + 1, x); return (*this)[i]; }

	проц     сожми()                   { if(items != alloc) ReAllocF(items); }
	проц     резервируй(цел n);
	цел      дайРазмест() const           { return alloc; }

	проц     уст(цел i, const T& x, цел count);
	T&       уст(цел i, const T& x)     { уст(i, x, 1); return дай(i); }
	T&       уст(цел i, T&& x)          { return по(i) = pick(x); }
	template <class Диапазон>
	проц     устДиапазон(цел i, const Диапазон& r);

	проц     удали(цел i, цел count = 1);
	проц     удали(const цел *sorted_list, цел n);
	проц     удали(const Вектор<цел>& sorted_list);
	template <class Условие>
	проц     удалиЕсли(Условие c);

	проц     вставьН(цел i, цел count = 1);
	T&       вставь(цел i)              { вставьН(i); return дай(i); }
	проц     вставь(цел i, const T& x, цел count);
	T&       вставь(цел i, const T& x)  { вставь(i, x, 1); return дай(i); }
	T&       вставь(цел i, T&& x);
	проц     вставь(цел i, const Вектор& x);
	проц     вставь(цел i, const Вектор& x, цел offset, цел count);
	проц     вставь(цел i, Вектор&& x);
	template <class Диапазон>
	проц     вставьДиапазон(цел i, const Диапазон& r);
	проц     приставь(const Вектор& x)               { вставь(дайСчёт(), x); }
	проц     приставь(const Вектор& x, цел o, цел c) { вставь(дайСчёт(), x, o, c); }
	проц     приставь(Вектор&& x)                    { вставь(дайСчёт(), pick(x)); }
	template <class Диапазон>
	проц     приставьДиапазон(const Диапазон& r)           { вставьДиапазон(дайСчёт(), r); }

	проц     вставьРазбей(цел i, Вектор<T>& v, цел from);

	проц     разверни(цел i1, цел i2)    { РНЦП::разверни(дай(i1), дай(i2)); }

	проц     сбрось(цел n = 1)         { ПРОВЕРЬ(n <= дайСчёт()); обрежь(items - n); }
	T&       верх()                   { ПРОВЕРЬ(дайСчёт()); return дай(items - 1); }
	const T& верх() const             { ПРОВЕРЬ(дайСчёт()); return дай(items - 1); }
	T        вынь()                   { T h = pick(верх()); сбрось(); return h; }

	operator T*()                    { return (T*)vector; }
	operator const T*() const        { return (T*)vector; }

	Вектор&  operator<<(const T& x)  { добавь(x); return *this; }
	Вектор&  operator<<(T&& x)            { добавь(pick(x)); return *this; }

#ifdef РНЦП
	проц     сериализуй(Поток& s)                        { StreamContainer(s, *this); }
	проц     вРяр(РярВВ& xio, кткст0 itemtag = "элт");
	проц     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
	т_хэш   дайХэшЗнач() const                        { return хэшПоСериализу(*this); }
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }
#endif

	Вектор()                                  { обнули(); }
	explicit Вектор(цел n)                    { items = n; vector = разместиРяд(n); alloc = n; строй(vector, vector + n); }
	explicit Вектор(цел n, const T& init)     { items = n; vector = разместиРяд(n); alloc = n; стройЗаполниГлубКопию(vector, vector + n, init); }
	~Вектор() {
		освободи();
		return; // Constraints:
		проверьДвиж((T *)0);  // T must be moveable
	}

// подбери assignment & copy. Picked source can only do очисть(), ~Вектор(), operator=, operator <<=
	Вектор(Вектор&& v)               { подбери(pick(v)); }
	проц operator=(Вектор&& v)       { if(this != &v) { освободи(); подбери(pick(v)); } }

	проц     вставь(цел i, std::initializer_list<T> init);
	проц     приставь(std::initializer_list<T> init) { вставь(дайСчёт(), init); }
	Вектор(std::initializer_list<T> init) { vector = NULL; items = alloc = 0; вставь(0, init); }

// Deep copy
	Вектор(const Вектор& v, цел)     { __глубКопия(v); }

// Standard container interface
	const T         *begin() const          { return (T*)vector; }
	const T         *end() const            { return (T*)vector + items; }
	T               *begin()                { return (T*)vector; }
	T               *end()                  { return (T*)vector + items; }

// Optimizations
	friend проц разверни(Вектор& a, Вектор& b)  { РНЦП::разверни(a.items, b.items); РНЦП::разверни(a.alloc, b.alloc); РНЦП::разверни(a.vector, b.vector); }

#ifdef DEPRECATED
	T&       индексируй(цел i)             { return по(i); }
	T&       индексируй(цел i, const T& x) { return по(i, x); }
	T&       добавьПодбор(T&& x)             { return items < alloc ? *(::new(Rdd()) T(pick(x))) : нарастиДобавь(pick(x)); }
	цел      дайИндекс(const T& элт) const;
	T&       вставьПодбор(цел i, T&& x)   { return вставь(i, pick(x)); }
	проц     вставьПодбор(цел i, Вектор&& x) { вставь(i, pick(x)); }
	проц     приставьПодбор(Вектор&& x)                { вставьПодбор(дайСчёт(), pick(x)); }
	typedef T       *Обходчик;
	typedef const T *КонстОбходчик;
	КонстОбходчик    дайОбх(цел i) const   { ПРОВЕРЬ(i >= 0 && i <= items); return begin() + i; }
	Обходчик         дайОбх(цел i)         { ПРОВЕРЬ(i >= 0 && i <= items); return begin() + i; }
	typedef T        ТипЗнач;
	friend проц приставь(Вектор& dst, const Вектор& ист)         { dst.приставь(ист); }

	STL_VECTOR_COMPATIBILITY(Вектор<T>)
#endif
};

template <class T>
class Массив : public ОпцияДвижимогоИГлубКопии< Массив<T> > {
protected:
#ifdef _ОТЛАДКА
	typedef T *ТипУказатель;
#else
	typedef ук ТипУказатель;
#endif
	Вектор<ТипУказатель> vector;

	проц     освободи();
	проц     __глубКопия(const Массив& v);
	T&       дай(цел i) const                           { return *(T *)vector[i]; }
	T      **дайУк(цел i) const                        { return (T **)vector.begin() + i; }

	проц     уд(ТипУказатель *укз, ТипУказатель *lim)              { while(укз < lim) delete (T *) *укз++; }
	проц     иниц(ТипУказатель *укз, ТипУказатель *lim)             { while(укз < lim) *укз++ = new T; }
	проц     иниц(ТипУказатель *укз, ТипУказатель *lim, const T& x) { while(укз < lim) *укз++ = new T(клонируй(x)); }

public:
	T&       добавь()                           { T *q = new T; vector.добавь(q); return *q; }
	T&       добавь(const T& x)                 { T *q = new T(x); vector.добавь(q); return *q; }
	T&       добавь(T&& x)                      { T *q = new T(pick(x)); vector.добавь(q); return *q; }
	T&       добавь(T *newt)                    { vector.добавь(newt); return *newt; }
	T&       добавь(Один<T>&& one)               { ПРОВЕРЬ(one); return добавь(one.открепи()); }
	template<class... Арги>
	T&       создай(Арги&&... арги)          { T *q = new T(std::forward<Арги>(арги)...); добавь(q); return *q; }
	template<class TT, class... Арги>
	TT&      создай(Арги&&... арги)          { TT *q = new TT(std::forward<Арги>(арги)...); добавь(q); return *q; }
	const T& operator[](цел i) const         { return дай(i); }
	T&       operator[](цел i)               { return дай(i); }
	const T& дай(цел i, const T& опр) const  { return i >= 0 && i < дайСчёт() ? дай(i) : опр; }
	T&       дай(цел i, T& опр)              { return i >= 0 && i < дайСчёт() ? дай(i) : опр; }
	цел      дайСчёт() const                { return vector.дайСчёт(); }
	бул     пустой() const                 { return vector.пустой(); }
	проц     обрежь(цел n);
	проц     устСчёт(цел n);
	проц     устСчёт(цел n, const T& init);
	проц     устСчётР(цел n);
	проц     устСчётР(цел n, const T& init);
	проц     очисть()                    { освободи(); vector.очисть(); }

	T&       по(цел i)                  { if(i >= дайСчёт()) устСчётР(i + 1); return дай(i); }
	T&       по(цел i, const T& x)      { if(i >= дайСчёт()) устСчётР(i + 1, x); return дай(i); }

	проц     сожми()                   { vector.сожми(); }
	проц     резервируй(цел xtra)          { vector.резервируй(xtra); }
	цел      дайРазмест() const           { return vector.дайРазмест(); }

	проц     уст(цел i, const T& x, цел count);
	T&       уст(цел i, const T& x)     { уст(i, x, 1); return дай(i); }
	T&       уст(цел i, T&& x)          { return по(i) = pick(x); }
	проц     удали(цел i, цел count = 1);
	проц     удали(const цел *sorted_list, цел n);
	проц     удали(const Вектор<цел>& sorted_list);
	template <class Условие>
	проц     удалиЕсли(Условие c);
	проц     вставьН(цел i, цел count = 1);
	T&       вставь(цел i)              { вставьН(i); return дай(i); }
	проц     вставь(цел i, const T& x, цел count);
	T&       вставь(цел i, const T& x)  { вставь(i, x, 1); return дай(i); }
	T&       вставь(цел i, T&& x);
	проц     вставь(цел i, const Массив& x);
	проц     вставь(цел i, const Массив& x, цел offset, цел count);
	template <class Диапазон>
	проц     вставьДиапазон(цел i, const Диапазон& r);
	проц     вставь(цел i, Массив&& x)             { vector.вставь(i, pick(x.vector)); }
	проц     приставь(const Массив& x)               { вставь(дайСчёт(), x); }
	проц     приставь(const Массив& x, цел o, цел c) { вставь(дайСчёт(), x, o, c); }
	проц     приставь(Массив&& x)                    { вставьПодбор(дайСчёт(), pick(x)); }
	template <class Диапазон>
	проц     приставьДиапазон(const Диапазон& r)          { вставьДиапазон(дайСчёт(), r); }

	проц     разверни(цел i1, цел i2)                 { РНЦП::разверни(vector[i1], vector[i2]); }
	проц     перемести(цел i1, цел i2);

	T&       уст(цел i, T *newt)        { delete (T *)vector[i]; vector[i] = newt; return *newt; }
	T&       вставь(цел i, T *newt);

	проц     сбрось(цел n = 1)            { обрежь(дайСчёт() - n); }
	T&       верх()                      { return дай(дайСчёт() - 1); }
	const T& верх() const                { return дай(дайСчёт() - 1); }

	T       *открепи(цел i)              { T *t = &дай(i); vector.удали(i); return t; }
	T       *разверни(цел i, T *newt)       { T *tmp = (T*)vector[i]; vector[i] = newt; return tmp; }
	T       *выньОткрепи()                { return (T *) vector.вынь(); }

	проц     разверни(Массив& b)             { разверни(vector, b.vector); }

	Массив& operator<<(const T& x)       { добавь(x); return *this; }
	Массив& operator<<(T&& x)            { добавь(pick(x)); return *this; }
	Массив& operator<<(T *newt)          { добавь(newt); return *this; }

	проц     сериализуй(Поток& s)       { StreamContainer(s, *this); }
	проц     вРяр(РярВВ& xio, кткст0 itemtag = "элт");
	проц     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
	т_хэш   дайХэшЗнач() const       { return хэшПоСериализу(*this); }

	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	Массив()                                          {}
	explicit Массив(цел n) : vector(n)                { иниц(vector.begin(), vector.end()); }
	explicit Массив(цел n, const T& init) : vector(n) { иниц(vector.begin(), vector.end(), init); }
	~Массив()                                         { освободи(); }

// подбери assignment & copy. Picked source can only очисть(), ~Вектор(), operator=, operator<<=
	Массив(Массив&& v) : vector(pick(v.vector))  {}
	проц operator=(Массив&& v)           { if(this != &v) { освободи(); vector = pick(v.vector); } }

// Deep copy
	Массив(const Массив& v, цел)          { __глубКопия(v); }

	проц     вставь(цел i, std::initializer_list<T> init);
	проц     приставь(std::initializer_list<T> init) { вставь(дайСчёт(), init); }
	Массив(std::initializer_list<T> init) { вставь(0, init); }

	class Обходчик;

	class КонстОбходчик {
	protected:
		T **укз;
		КонстОбходчик(T **p)                    { укз = p; }

		friend class Массив<T>;
		struct NP { цел dummy; };

	public:
		const T& operator*() const              { return **укз; }
		const T *operator->() const             { return *укз; }
		const T& operator[](цел i) const        { return *укз[i]; }

		КонстОбходчик& operator++()             { укз++; return *this; }
		КонстОбходчик& operator--()             { укз--; return *this; }
		КонстОбходчик  operator++(цел)          { КонстОбходчик t = *this; ++*this; return t; }
		КонстОбходчик  operator--(цел)          { КонстОбходчик t = *this; --*this; return t; }

		КонстОбходчик& operator+=(цел i)        { укз += i; return *this; }
		КонстОбходчик& operator-=(цел i)        { укз -= i; return *this; }

		КонстОбходчик operator+(цел i) const    { return укз + i; }
		КонстОбходчик operator-(цел i) const    { return укз - i; }

		цел  operator-(КонстОбходчик x) const   { return (цел)(укз - x.укз); }

		бул operator==(КонстОбходчик x) const  { return укз == x.укз; }
		бул operator!=(КонстОбходчик x) const  { return укз != x.укз; }
		бул operator<(КонстОбходчик x) const   { return укз < x.укз; }
		бул operator>(КонстОбходчик x) const   { return укз > x.укз; }
		бул operator<=(КонстОбходчик x) const  { return укз <= x.укз; }
		бул operator>=(КонстОбходчик x) const  { return укз >= x.укз; }

		operator бул() const                   { return укз; }

		КонстОбходчик()                         {}
		КонстОбходчик(NP *null)                 { ПРОВЕРЬ(null == NULL); укз = NULL; }
		
		STL_ITERATOR_COMPATIBILITY
	};

	class Обходчик : public КонстОбходчик {
		friend class Массив<T>;
		Обходчик(T **p) : КонстОбходчик(p)      {}
		typedef КонстОбходчик B;
		struct NP { цел dummy; };

	public:
		T& operator*()                          { return **B::укз; }
		T *operator->()                         { return *B::укз; }
		T& operator[](цел i)                    { return *B::укз[i]; }

		const T& operator*() const              { return **B::укз; }
		const T *operator->() const             { return *B::укз; }
		const T& operator[](цел i) const        { return *B::укз[i]; }

		Обходчик& operator++()                  { B::укз++; return *this; }
		Обходчик& operator--()                  { B::укз--; return *this; }
		Обходчик  operator++(цел)               { Обходчик t = *this; ++*this; return t; }
		Обходчик  operator--(цел)               { Обходчик t = *this; --*this; return t; }

		Обходчик& operator+=(цел i)             { B::укз += i; return *this; }
		Обходчик& operator-=(цел i)             { B::укз -= i; return *this; }

		Обходчик operator+(цел i) const         { return B::укз + i; }
		Обходчик operator-(цел i) const         { return B::укз - i; }

		цел      operator-(Обходчик x) const    { return B::operator-(x); }

		Обходчик()                               {}
		Обходчик(NP *null) : КонстОбходчик(null) {}

		STL_ITERATOR_COMPATIBILITY
	};

// Standard container interface
	Обходчик         begin()                    { return (T **)vector.begin(); }
	Обходчик         end()                      { return (T **)vector.end(); }
	КонстОбходчик    begin() const              { return (T **)vector.begin(); }
	КонстОбходчик    end() const                { return (T **)vector.end(); }

// Optimalization
	friend проц разверни(Массив& a, Массив& b)                   { РНЦП::разверни(a.vector, b.vector); }
	//GCC forced move from Обходчик, ugly workaround
private:
	static проц IterSwap0(Обходчик a, Обходчик b)          { РНЦП::разверни(*a.укз, *b.укз); }
public:
	friend проц IterSwap(Обходчик a, Обходчик b)           { Массив<T>::IterSwap0(a, b); }

#ifdef DEPRECATED
	цел      дайИндекс(const T& элт) const;
	проц     вставьПодбор(цел i, Массив&& x)       { vector.вставьПодбор(i, pick(x.vector)); }
	проц     приставьПодбор(Массив&& x)              { вставьПодбор(дайСчёт(), pick(x)); }
	T&       индексируй(цел i)                     { return по(i); }
	T&       индексируй(цел i, const T& x)         { return по(i, x); }
	T&       добавьПодбор(T&& x)                     { T *q = new T(pick(x)); vector.добавь(q); return *q; }
	T&       вставьПодбор(цел i, T&& x)           { return вставь(i, pick(x)); }
	typedef T        ТипЗнач;
	Обходчик         дайОбх(цел pos)           { return (T **)vector.дайОбх(pos); }
	КонстОбходчик    дайОбх(цел pos) const     { return (T **)vector.дайОбх(pos); }
#endif
	STL_VECTOR_COMPATIBILITY(Массив<T>)
};
