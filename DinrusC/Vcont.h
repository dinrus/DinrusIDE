#ifdef _ОТЛАДКА
void всёКогдаПодобрано(void *ptr);
void ВСЁ_КОГДА_ПОДОБРАНО__(void *ptr);

template <class T>
void ВСЁ_КОГДА_ПОДОБРАНО(T& x)
{
	ВСЁ_КОГДА_ПОДОБРАНО__(&x);
}
#endif

template <class T>
inline void стройЗаполниГлубКопию(T *t, const T *end, const T& x) {
	while(t != end)
		new(t++) T(clone(x));
}

template <class T>
inline void строй(T *t, const T *lim) {
	while(t < lim)
		new(t++) T;
}

template <class T>
inline void разрушь(T *t, const T *end)
{
	while(t != end) {
		t->~T();
		t++;
	}
}

template <class T, class S>
inline void стройГлубКопию(T *t, const S *s, const S *end) {
	while(s != end)
		new (t++) T(clone(*s++));
}

template <class T>
class Буфер : Движимое< Буфер<T> > {
	T *ptr;
	
	void разместпам(size_t size) {
		if(std::is_trivially_destructible<T>::value)
			ptr = (T *)MemoryAlloc(size * sizeof(T));
		else {
			void *p = MemoryAlloc(size * sizeof(T) + 16);
			*(size_t *)p = size;
			ptr = (T *)((byte *)p + 16);
		}
	}
	void нов(size_t size) {
		разместпам(size);
		строй(ptr, ptr + size);
	}
	void нов(size_t size, const T& in) {
		разместпам(size);
		стройЗаполниГлубКопию(ptr, ptr + size, in);
	}
	void освободи() {
		if(ptr) {
			if(std::is_trivially_destructible<T>::value)
				MemoryFree(ptr);
			else {
				void *p = (byte *)ptr - 16;
				size_t size = *(size_t *)p;
				разрушь(ptr, ptr + size);
				MemoryFree(p);
			}
		}
	}

public:
	operator T*()                        { return ptr; }
	operator const T*() const            { return ptr; }
	T *operator~()                       { return ptr; }
	const T *operator~() const           { return ptr; }
	T          *дай()                    { return ptr; }
	const T    *дай() const              { return ptr; }
	T          *begin()                  { return ptr; }
	const T    *begin() const            { return ptr; }

	void размести(size_t size)              { очисть(); нов(size); }
	void размести(size_t size, const T& in) { очисть(); нов(size, in); }

	void очисть()                         { освободи(); ptr = NULL; }
	bool пустой() const                 { return ptr == NULL; }

	Буфер()                             { ptr = NULL; }
	Буфер(size_t size)                  { нов(size); }
	Буфер(size_t size, const T& init)   { нов(size, init); }
	~Буфер()                            { освободи(); }

	void operator=(Буфер&& v)           { if(&v != this) { очисть(); ptr = v.ptr; v.ptr = NULL; } }
	Буфер(Буфер&& v)                   { ptr = v.ptr; v.ptr = NULL; }

	Буфер(size_t size, std::initializer_list<T> init) {
		разместпам(size); T *t = ptr; for(const auto& i : init) new (t++) T(i);
	}
	Буфер(std::initializer_list<T> init) : Буфер(init.size(), init) {}
};

template <class T>
class Один : ОпцияДвижимогоИГлубКопии< Один<T> > {
	mutable T  *ptr;

	void        освободи()                     { if(ptr && ptr != (T*)1) delete ptr; }
	template <class TT>
	void        подбери(Один<TT>&& данные)       { ptr = данные.открепи(); }

public:
	void        прикрепи(T *данные)            { освободи(); ptr = данные; }
	T          *открепи()                   { T *t = ptr; ptr = NULL; return t; }
	void        очисть()                    { освободи(); ptr = NULL; }

	void        operator=(T *данные)         { прикрепи(данные); }
	
	template <class TT>
	void        operator=(Один<TT>&& d)     { if((void *)this != (void *)&d) { освободи(); подбери(pick(d)); }}

	const T    *operator->() const         { ПРОВЕРЬ(ptr); return ptr; }
	T          *operator->()               { ПРОВЕРЬ(ptr); return ptr; }
	const T    *operator~() const          { return ptr; }
	T          *operator~()                { return ptr; }
	const T    *дай() const                { return ptr; }
	T          *дай()                      { return ptr; }
	const T&    operator*() const          { ПРОВЕРЬ(ptr); return *ptr; }
	T&          operator*()                { ПРОВЕРЬ(ptr); return *ptr; }

	template <class TT, class... Арги>
	TT&         создай(Арги&&... арги)     { TT *q = new TT(std::forward<Арги>(арги)...); прикрепи(q); return *q; }
	template <class TT> // with C++ conforming compiler, this would not be needed - GCC bug workaround
	TT&         создай()                   { TT *q = new TT; прикрепи(q); return *q; }
	template <class... Арги>
	T&          создай(Арги&&... арги)     { T *q = new T(std::forward<Арги>(арги)...); прикрепи(q); return *q; }
	T&          создай()                   { T *q = new T; прикрепи(q); return *q; }

	template <class TT>
	bool        является() const                 { return dynamic_cast<const TT *>(ptr); }

	bool        пустой() const            { return !ptr; }

	operator bool() const                  { return ptr; }
	
	Ткст вТкст() const                { return ptr ? какТкст(*ptr) : "<empty>"; }

	Один()                                  { ptr = NULL; }
	Один(T *newt)                           { ptr = newt; }
	template <class TT>
	Один(Один<TT>&& p)                       { подбери(pick(p)); }
	Один(const Один<T>& p, int)              { ptr = p.пустой() ? NULL : new T(clone(*p)); }
	Один(const Один<T>& p) = delete;
	void operator=(const Один<T>& p) = delete;
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
	int      items;
	int      alloc;

	static void    освободиРяд(T *ptr)            { if(ptr) MemoryFree(ptr); }
	static T      *разместиРяд(int& n);

	void     обнули()                          { vector = NULL; items = alloc = 0; }
	void     подбери(Вектор<T>&& v);

	T       *Rdd()                           { return vector + items++; }

	void     освободи();
	void     __глубКопия(const Вектор& ист);
	T&       дай(int i) const                { ПРОВЕРЬ(i >= 0 && i < items); return vector[i]; }
	bool     перемест(int alloc);
	void     ReAllocF(int alloc);
	bool     GrowSz();
	void     GrowF();
	T&       нарастиДобавь(const T& x);
	T&       нарастиДобавь(T&& x);
	void     вставьРяд(int q, int count);

	template <class U> friend class Индекс;

public:
	T&       добавь()                           { if(items >= alloc) GrowF(); return *(::new(Rdd()) T); }
	T&       добавь(const T& x)                 { return items < alloc ? *(new(Rdd()) T(x)) : нарастиДобавь(x); }
	T&       добавь(T&& x)                      { return items < alloc ? *(::new(Rdd()) T(pick(x))) : нарастиДобавь(pick(x)); }
	template <class... Арги>
	T&       создай(Арги&&... арги)          { if(items >= alloc) GrowF(); return *(::new(Rdd()) T(std::forward<Арги>(арги)...)); }
	void     добавьН(int n);
	const T& operator[](int i) const         { return дай(i); }
	T&       operator[](int i)               { return дай(i); }
	const T& дай(int i, const T& опр) const  { return i >= 0 && i < дайСчёт() ? дай(i) : опр; }
	T&       дай(int i, T& опр)              { return i >= 0 && i < дайСчёт() ? дай(i) : опр; }
	int      дайСчёт() const                { return items; }
	bool     пустой() const                 { return items == 0; }
	void     обрежь(int n);
	void     устСчёт(int n);
	void     устСчёт(int n, const T& init);
	void     устСчётР(int n);
	void     устСчётР(int n, const T& init);
	void     очисть();

	T&       по(int i)                  { if(i >= items) устСчётР(i + 1); return (*this)[i]; }
	T&       по(int i, const T& x)      { if(i >= items) устСчётР(i + 1, x); return (*this)[i]; }

	void     сожми()                   { if(items != alloc) ReAllocF(items); }
	void     резервируй(int n);
	int      дайРазмест() const           { return alloc; }

	void     уст(int i, const T& x, int count);
	T&       уст(int i, const T& x)     { уст(i, x, 1); return дай(i); }
	T&       уст(int i, T&& x)          { return по(i) = pick(x); }
	template <class Диапазон>
	void     устДиапазон(int i, const Диапазон& r);

	void     удали(int i, int count = 1);
	void     удали(const int *sorted_list, int n);
	void     удали(const Вектор<int>& sorted_list);
	template <class Условие>
	void     удалиЕсли(Условие c);

	void     вставьН(int i, int count = 1);
	T&       вставь(int i)              { вставьН(i); return дай(i); }
	void     вставь(int i, const T& x, int count);
	T&       вставь(int i, const T& x)  { вставь(i, x, 1); return дай(i); }
	T&       вставь(int i, T&& x);
	void     вставь(int i, const Вектор& x);
	void     вставь(int i, const Вектор& x, int offset, int count);
	void     вставь(int i, Вектор&& x);
	template <class Диапазон>
	void     вставьДиапазон(int i, const Диапазон& r);
	void     приставь(const Вектор& x)               { вставь(дайСчёт(), x); }
	void     приставь(const Вектор& x, int o, int c) { вставь(дайСчёт(), x, o, c); }
	void     приставь(Вектор&& x)                    { вставь(дайСчёт(), pick(x)); }
	template <class Диапазон>
	void     приставьДиапазон(const Диапазон& r)           { вставьДиапазон(дайСчёт(), r); }

	void     вставьРазбей(int i, Вектор<T>& v, int from);

	void     разверни(int i1, int i2)    { РНЦП::разверни(дай(i1), дай(i2)); }

	void     сбрось(int n = 1)         { ПРОВЕРЬ(n <= дайСчёт()); обрежь(items - n); }
	T&       верх()                   { ПРОВЕРЬ(дайСчёт()); return дай(items - 1); }
	const T& верх() const             { ПРОВЕРЬ(дайСчёт()); return дай(items - 1); }
	T        вынь()                   { T h = pick(верх()); сбрось(); return h; }

	operator T*()                    { return (T*)vector; }
	operator const T*() const        { return (T*)vector; }

	Вектор&  operator<<(const T& x)  { добавь(x); return *this; }
	Вектор&  operator<<(T&& x)            { добавь(pick(x)); return *this; }

#ifdef РНЦП
	void     сериализуй(Поток& s)                        { StreamContainer(s, *this); }
	void     вРяр(РярВВ& xio, const char *itemtag = "элт");
	void     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
	hash_t   дайХэшЗнач() const                        { return хэшПоСериализу(*this); }
	template <class B> bool operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
	template <class B> int  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> bool operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return сравни(x) > 0; }
#endif

	Вектор()                                  { обнули(); }
	explicit Вектор(int n)                    { items = n; vector = разместиРяд(n); alloc = n; строй(vector, vector + n); }
	explicit Вектор(int n, const T& init)     { items = n; vector = разместиРяд(n); alloc = n; стройЗаполниГлубКопию(vector, vector + n, init); }
	~Вектор() {
		освободи();
		return; // Constraints:
		проверьДвижимое((T *)0);  // T must be moveable
	}

// подбери assignment & copy. Picked source can only do очисть(), ~Вектор(), operator=, operator <<=
	Вектор(Вектор&& v)               { подбери(pick(v)); }
	void operator=(Вектор&& v)       { if(this != &v) { освободи(); подбери(pick(v)); } }

	void     вставь(int i, std::initializer_list<T> init);
	void     приставь(std::initializer_list<T> init) { вставь(дайСчёт(), init); }
	Вектор(std::initializer_list<T> init) { vector = NULL; items = alloc = 0; вставь(0, init); }

// Deep copy
	Вектор(const Вектор& v, int)     { __глубКопия(v); }

// Standard container interface
	const T         *begin() const          { return (T*)vector; }
	const T         *end() const            { return (T*)vector + items; }
	T               *begin()                { return (T*)vector; }
	T               *end()                  { return (T*)vector + items; }

// Optimizations
	friend void разверни(Вектор& a, Вектор& b)  { РНЦП::разверни(a.items, b.items); РНЦП::разверни(a.alloc, b.alloc); РНЦП::разверни(a.vector, b.vector); }

#ifdef DEPRECATED
	T&       индексируй(int i)             { return по(i); }
	T&       индексируй(int i, const T& x) { return по(i, x); }
	T&       добавьПодбор(T&& x)             { return items < alloc ? *(::new(Rdd()) T(pick(x))) : нарастиДобавь(pick(x)); }
	int      дайИндекс(const T& элт) const;
	T&       вставьПодбор(int i, T&& x)   { return вставь(i, pick(x)); }
	void     вставьПодбор(int i, Вектор&& x) { вставь(i, pick(x)); }
	void     приставьПодбор(Вектор&& x)                { вставьПодбор(дайСчёт(), pick(x)); }
	typedef T       *Обходчик;
	typedef const T *КонстОбходчик;
	КонстОбходчик    дайОбх(int i) const   { ПРОВЕРЬ(i >= 0 && i <= items); return begin() + i; }
	Обходчик         дайОбх(int i)         { ПРОВЕРЬ(i >= 0 && i <= items); return begin() + i; }
	typedef T        ТипЗнач;
	friend void приставь(Вектор& dst, const Вектор& ист)         { dst.приставь(ист); }

	STL_VECTOR_COMPATIBILITY(Вектор<T>)
#endif
};

template <class T>
class Массив : public ОпцияДвижимогоИГлубКопии< Массив<T> > {
protected:
#ifdef _ОТЛАДКА
	typedef T *ТипУказатель;
#else
	typedef void *ТипУказатель;
#endif
	Вектор<ТипУказатель> vector;

	void     освободи();
	void     __глубКопия(const Массив& v);
	T&       дай(int i) const                           { return *(T *)vector[i]; }
	T      **дайУк(int i) const                        { return (T **)vector.begin() + i; }

	void     уд(ТипУказатель *ptr, ТипУказатель *lim)              { while(ptr < lim) delete (T *) *ptr++; }
	void     иниц(ТипУказатель *ptr, ТипУказатель *lim)             { while(ptr < lim) *ptr++ = new T; }
	void     иниц(ТипУказатель *ptr, ТипУказатель *lim, const T& x) { while(ptr < lim) *ptr++ = new T(clone(x)); }

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
	const T& operator[](int i) const         { return дай(i); }
	T&       operator[](int i)               { return дай(i); }
	const T& дай(int i, const T& опр) const  { return i >= 0 && i < дайСчёт() ? дай(i) : опр; }
	T&       дай(int i, T& опр)              { return i >= 0 && i < дайСчёт() ? дай(i) : опр; }
	int      дайСчёт() const                { return vector.дайСчёт(); }
	bool     пустой() const                 { return vector.пустой(); }
	void     обрежь(int n);
	void     устСчёт(int n);
	void     устСчёт(int n, const T& init);
	void     устСчётР(int n);
	void     устСчётР(int n, const T& init);
	void     очисть()                    { освободи(); vector.очисть(); }

	T&       по(int i)                  { if(i >= дайСчёт()) устСчётР(i + 1); return дай(i); }
	T&       по(int i, const T& x)      { if(i >= дайСчёт()) устСчётР(i + 1, x); return дай(i); }

	void     сожми()                   { vector.сожми(); }
	void     резервируй(int xtra)          { vector.резервируй(xtra); }
	int      дайРазмест() const           { return vector.дайРазмест(); }

	void     уст(int i, const T& x, int count);
	T&       уст(int i, const T& x)     { уст(i, x, 1); return дай(i); }
	T&       уст(int i, T&& x)          { return по(i) = pick(x); }
	void     удали(int i, int count = 1);
	void     удали(const int *sorted_list, int n);
	void     удали(const Вектор<int>& sorted_list);
	template <class Условие>
	void     удалиЕсли(Условие c);
	void     вставьН(int i, int count = 1);
	T&       вставь(int i)              { вставьН(i); return дай(i); }
	void     вставь(int i, const T& x, int count);
	T&       вставь(int i, const T& x)  { вставь(i, x, 1); return дай(i); }
	T&       вставь(int i, T&& x);
	void     вставь(int i, const Массив& x);
	void     вставь(int i, const Массив& x, int offset, int count);
	template <class Диапазон>
	void     вставьДиапазон(int i, const Диапазон& r);
	void     вставь(int i, Массив&& x)             { vector.вставь(i, pick(x.vector)); }
	void     приставь(const Массив& x)               { вставь(дайСчёт(), x); }
	void     приставь(const Массив& x, int o, int c) { вставь(дайСчёт(), x, o, c); }
	void     приставь(Массив&& x)                    { вставьПодбор(дайСчёт(), pick(x)); }
	template <class Диапазон>
	void     приставьДиапазон(const Диапазон& r)          { вставьДиапазон(дайСчёт(), r); }

	void     разверни(int i1, int i2)                 { РНЦП::разверни(vector[i1], vector[i2]); }
	void     перемести(int i1, int i2);

	T&       уст(int i, T *newt)        { delete (T *)vector[i]; vector[i] = newt; return *newt; }
	T&       вставь(int i, T *newt);

	void     сбрось(int n = 1)            { обрежь(дайСчёт() - n); }
	T&       верх()                      { return дай(дайСчёт() - 1); }
	const T& верх() const                { return дай(дайСчёт() - 1); }

	T       *открепи(int i)              { T *t = &дай(i); vector.удали(i); return t; }
	T       *разверни(int i, T *newt)       { T *tmp = (T*)vector[i]; vector[i] = newt; return tmp; }
	T       *выньОткрепи()                { return (T *) vector.вынь(); }

	void     разверни(Массив& b)             { разверни(vector, b.vector); }

	Массив& operator<<(const T& x)       { добавь(x); return *this; }
	Массив& operator<<(T&& x)            { добавь(pick(x)); return *this; }
	Массив& operator<<(T *newt)          { добавь(newt); return *this; }

	void     сериализуй(Поток& s)       { StreamContainer(s, *this); }
	void     вРяр(РярВВ& xio, const char *itemtag = "элт");
	void     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
	hash_t   дайХэшЗнач() const       { return хэшПоСериализу(*this); }

	template <class B> bool operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
	template <class B> int  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> bool operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return сравни(x) > 0; }

	Массив()                                          {}
	explicit Массив(int n) : vector(n)                { иниц(vector.begin(), vector.end()); }
	explicit Массив(int n, const T& init) : vector(n) { иниц(vector.begin(), vector.end(), init); }
	~Массив()                                         { освободи(); }

// подбери assignment & copy. Picked source can only очисть(), ~Вектор(), operator=, operator<<=
	Массив(Массив&& v) : vector(pick(v.vector))  {}
	void operator=(Массив&& v)           { if(this != &v) { освободи(); vector = pick(v.vector); } }

// Deep copy
	Массив(const Массив& v, int)          { __глубКопия(v); }

	void     вставь(int i, std::initializer_list<T> init);
	void     приставь(std::initializer_list<T> init) { вставь(дайСчёт(), init); }
	Массив(std::initializer_list<T> init) { вставь(0, init); }

	class Обходчик;

	class КонстОбходчик {
	protected:
		T **ptr;
		КонстОбходчик(T **p)                    { ptr = p; }

		friend class Массив<T>;
		struct NP { int dummy; };

	public:
		const T& operator*() const              { return **ptr; }
		const T *operator->() const             { return *ptr; }
		const T& operator[](int i) const        { return *ptr[i]; }

		КонстОбходчик& operator++()             { ptr++; return *this; }
		КонстОбходчик& operator--()             { ptr--; return *this; }
		КонстОбходчик  operator++(int)          { КонстОбходчик t = *this; ++*this; return t; }
		КонстОбходчик  operator--(int)          { КонстОбходчик t = *this; --*this; return t; }

		КонстОбходчик& operator+=(int i)        { ptr += i; return *this; }
		КонстОбходчик& operator-=(int i)        { ptr -= i; return *this; }

		КонстОбходчик operator+(int i) const    { return ptr + i; }
		КонстОбходчик operator-(int i) const    { return ptr - i; }

		int  operator-(КонстОбходчик x) const   { return (int)(ptr - x.ptr); }

		bool operator==(КонстОбходчик x) const  { return ptr == x.ptr; }
		bool operator!=(КонстОбходчик x) const  { return ptr != x.ptr; }
		bool operator<(КонстОбходчик x) const   { return ptr < x.ptr; }
		bool operator>(КонстОбходчик x) const   { return ptr > x.ptr; }
		bool operator<=(КонстОбходчик x) const  { return ptr <= x.ptr; }
		bool operator>=(КонстОбходчик x) const  { return ptr >= x.ptr; }

		operator bool() const                   { return ptr; }

		КонстОбходчик()                         {}
		КонстОбходчик(NP *null)                 { ПРОВЕРЬ(null == NULL); ptr = NULL; }
		
		STL_ITERATOR_COMPATIBILITY
	};

	class Обходчик : public КонстОбходчик {
		friend class Массив<T>;
		Обходчик(T **p) : КонстОбходчик(p)      {}
		typedef КонстОбходчик B;
		struct NP { int dummy; };

	public:
		T& operator*()                          { return **B::ptr; }
		T *operator->()                         { return *B::ptr; }
		T& operator[](int i)                    { return *B::ptr[i]; }

		const T& operator*() const              { return **B::ptr; }
		const T *operator->() const             { return *B::ptr; }
		const T& operator[](int i) const        { return *B::ptr[i]; }

		Обходчик& operator++()                  { B::ptr++; return *this; }
		Обходчик& operator--()                  { B::ptr--; return *this; }
		Обходчик  operator++(int)               { Обходчик t = *this; ++*this; return t; }
		Обходчик  operator--(int)               { Обходчик t = *this; --*this; return t; }

		Обходчик& operator+=(int i)             { B::ptr += i; return *this; }
		Обходчик& operator-=(int i)             { B::ptr -= i; return *this; }

		Обходчик operator+(int i) const         { return B::ptr + i; }
		Обходчик operator-(int i) const         { return B::ptr - i; }

		int      operator-(Обходчик x) const    { return B::operator-(x); }

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
	friend void разверни(Массив& a, Массив& b)                   { РНЦП::разверни(a.vector, b.vector); }
	//GCC forced move from Обходчик, ugly workaround
private:
	static void IterSwap0(Обходчик a, Обходчик b)          { РНЦП::разверни(*a.ptr, *b.ptr); }
public:
	friend void IterSwap(Обходчик a, Обходчик b)           { Массив<T>::IterSwap0(a, b); }

#ifdef DEPRECATED
	int      дайИндекс(const T& элт) const;
	void     вставьПодбор(int i, Массив&& x)       { vector.вставьПодбор(i, pick(x.vector)); }
	void     приставьПодбор(Массив&& x)              { вставьПодбор(дайСчёт(), pick(x)); }
	T&       индексируй(int i)                     { return по(i); }
	T&       индексируй(int i, const T& x)         { return по(i, x); }
	T&       добавьПодбор(T&& x)                     { T *q = new T(pick(x)); vector.добавь(q); return *q; }
	T&       вставьПодбор(int i, T&& x)           { return вставь(i, pick(x)); }
	typedef T        ТипЗнач;
	Обходчик         дайОбх(int pos)           { return (T **)vector.дайОбх(pos); }
	КонстОбходчик    дайОбх(int pos) const     { return (T **)vector.дайОбх(pos); }
#endif
	STL_VECTOR_COMPATIBILITY(Массив<T>)
};
