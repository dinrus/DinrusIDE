template <class T>
class БиВектор : ОпцияДвижимогоИГлубКопии< БиВектор<T> > {
protected:
	T       *vector;
	int      start;
	int      items;
	int      alloc;

	int      Ix(int i) const         { return i + start < alloc ? i + start : i + start - alloc; }

	int      EI() const              { return Ix(items - 1); }
	void     перемест(int newalloc);
	void     добавь0();
	void     глубокаяКопия0(const БиВектор& ист);
	T       *добавьГолову0()              { проверьДвижимое<T>(); добавь0(); return &vector[start = Ix(alloc - 1)/*(start + alloc - 1) % alloc*/]; }
	T       *добавьХвост0()              { проверьДвижимое<T>(); добавь0(); return &vector[EI()]; }
	void     обнули()                  { start = items = alloc = 0; vector = NULL; }
	void     освободи();
	void     подбери(БиВектор&& x)      { vector = pick(x.vector); start = x.start; items = x.items;
	                                   alloc = x.alloc; ((БиВектор&)x).items = -1; }
	void     копируй(T *dst, int start, int count) const;

public:
	int      дайСчёт() const        { return items; }
	bool     пустой() const         { return items == 0; }
	void     очисть();

	T&       добавьГолову()               { return *new(добавьГолову0()) T; }
	T&       добавьХвост()               { return *new(добавьХвост0()) T; }
	void     добавьГолову(const T& x)     { new(добавьГолову0()) T(x); }
	void     добавьХвост(const T& x)     { new(добавьХвост0()) T(x); }
	void     добавьГолову(T&& x)          { new(добавьГолову0()) T(pick(x)); }
	void     добавьХвост(T&& x)          { new(добавьХвост0()) T(pick(x)); }
	T&       дайГолову()                  { ПРОВЕРЬ(items > 0); return vector[start]; }
	T&       дайХвост()                  { ПРОВЕРЬ(items > 0); return vector[EI()]; }
	const T& дайГолову() const            { ПРОВЕРЬ(items > 0); return vector[start]; }
	const T& дайХвост() const            { ПРОВЕРЬ(items > 0); return vector[EI()]; }
	void     сбросьГолову()              { (&дайГолову())->~T(); items--; start = Ix(1); }
	void     сбросьХвост()              { (&дайХвост())->~T(); items--; }
	T        выньГолову()               { T x = дайГолову(); сбросьГолову(); return x; }
	T        выньХвост()               { T x = дайХвост(); сбросьХвост(); return x; }
	void     сбросьГолову(int n)         { while(n-- > 0) БиВектор<T>::сбросьГолову(); }
	void     сбросьХвост(int n)         { while(n-- > 0) БиВектор<T>::сбросьХвост(); }
	const T& operator[](int i) const { ПРОВЕРЬ(i >= 0 && i < items); return vector[Ix(i)]; }
	T&       operator[](int i)       { ПРОВЕРЬ(i >= 0 && i < items); return vector[Ix(i)]; }
	void     сожми();
	void     резервируй(int n);
	int      дайРазмест() const        { return alloc; }

	void     сериализуй(Поток& s);
	Ткст   вТкст() const;
	hash_t   дайХэшЗнач() const    { return хэшПоСериализу(*this); }
	template <class B> bool operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
	template <class B> int  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> bool operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return сравни(x) > 0; }

	БиВектор(const БиВектор& ист, int)          { глубокаяКопия0(ист); }
	БиВектор(БиВектор&& ист)                    { подбери(pick(ист)); }
	void operator=(БиВектор&& ист)              { if(this != &ист) { освободи(); подбери(pick(ист)); } }
	БиВектор()                                  { обнули(); }
	~БиВектор()                                 { освободи(); } // gcc4.0 workaround!!

	БиВектор(std::initializer_list<T> init);

	typedef ConstIIterator<БиВектор> КонстОбходчик;
	typedef IIterator<БиВектор>      Обходчик;

	КонстОбходчик    begin() const              { return КонстОбходчик(*this, 0); }
	КонстОбходчик    end() const                { return КонстОбходчик(*this, дайСчёт()); }
	Обходчик         begin()                    { return Обходчик(*this, 0); }
	Обходчик         end()                      { return Обходчик(*this, дайСчёт()); }

	friend void разверни(БиВектор& a, БиВектор& b)  { РНЦП::разверни(a.vector, b.vector);
	                                              РНЦП::разверни(a.start, b.start);
	                                              РНЦП::разверни(a.items, b.items);
	                                              РНЦП::разверни(a.alloc, b.alloc); }

#ifdef DEPRECATED
	void     AddHeadPick(T&& x)      { new(добавьГолову0()) T(pick(x)); }
	void     AddTailPick(T&& x)      { new(добавьХвост0()) T(pick(x)); }
	КонстОбходчик    дайОбх(int pos) const     { return КонстОбходчик(*this, pos); }
	Обходчик         дайОбх(int pos)           { return Обходчик(*this, pos); }
	typedef T        ТипЗнач;
	STL_BI_COMPATIBILITY(БиВектор<T>)
#endif
};

template <class T>
class БиМассив : ОпцияДвижимогоИГлубКопии< БиМассив<T> > {
protected:
	БиВектор<void *> bv;

	void     освободи();
	void     глубокаяКопия0(const БиМассив<T>& v);

public:
	int      дайСчёт() const              { return bv.дайСчёт(); }
	bool     пустой() const               { return дайСчёт() == 0; }
	void     очисть()                       { освободи(); bv.очисть(); }

	T&       добавьГолову()                     { T *q = new T; bv.добавьГолову(q); return *q; }
	T&       добавьХвост()                     { T *q = new T; bv.добавьХвост(q); return *q; }
	void     добавьГолову(const T& x)           { bv.добавьГолову(new T(x)); }
	void     добавьХвост(const T& x)           { bv.добавьХвост(new T(x)); }
	T&       добавьГолову(T *newt)              { bv.добавьГолову(newt); return *newt; }
	T&       добавьХвост(T *newt)              { bv.добавьХвост(newt); return *newt; }
	template <class TT> TT& создайГоолову()   { TT *q = new TT; bv.добавьГолову(q); return *q; }
	template <class TT> TT& создайХвост()   { TT *q = new TT; bv.добавьХвост(q); return *q; }
	T&       добавьГолову(Один<T>&& one)         { ПРОВЕРЬ(one); return добавьГолову(one.открепи()); }
	T&       добавьХвост(Один<T>&& one)         { ПРОВЕРЬ(one); return добавьХвост(one.открепи()); }
	T&       дайГолову()                        { return *(T *) bv.дайГолову(); }
	T&       дайХвост()                        { return *(T *) bv.дайХвост(); }
	const T& дайГолову() const                  { return *(const T *) bv.дайГолову(); }
	const T& дайХвост() const                  { return *(const T *) bv.дайХвост(); }
	void     сбросьГолову()                    { delete (T*) bv.дайГолову(); bv.сбросьГолову(); }
	void     сбросьХвост()                    { delete (T*) bv.дайХвост(); bv.сбросьХвост(); }
	T       *открепиГолову()                  { T *q = (T*) bv.дайГолову(); bv.сбросьГолову(); return q; }
	T       *открепиХвост()                  { T *q = (T*) bv.дайХвост(); bv.сбросьХвост(); return q; }

	T&       operator[](int i)             { return *(T *) bv[i]; }
	const T& operator[](int i) const       { return *(const T *) bv[i]; }

	void     сожми()                      { bv.сожми(); }
	void     резервируй(int n)                { bv.резервируй(n); }
	int      дайРазмест() const              { return bv.дайРазмест(); }

	void     сериализуй(Поток& s);
	Ткст   вТкст() const;
	hash_t   дайХэшЗнач() const    { return хэшПоСериализу(*this); }
	template <class B> bool operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
	template <class B> int  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> bool operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return сравни(x) > 0; }

	БиМассив(const БиМассив& v, int)           { глубокаяКопия0(v); }

	БиМассив(БиМассив&& ист) : bv(pick(ист.bv)){}
	void operator=(БиМассив&& ист)            { if(this != &ист) { освободи(); bv = pick(ист.bv); } }
	БиМассив()                                {}
	~БиМассив()                               { освободи(); }

	БиМассив(std::initializer_list<T> init);

	typedef ConstIIterator<БиМассив> КонстОбходчик;
	typedef IIterator<БиМассив>      Обходчик;

	КонстОбходчик    begin() const              { return КонстОбходчик(*this, 0); }
	КонстОбходчик    end() const                { return КонстОбходчик(*this, дайСчёт()); }
	Обходчик         begin()                    { return Обходчик(*this, 0); }
	Обходчик         end()                      { return Обходчик(*this, дайСчёт()); }

	friend void разверни(БиМассив& a, БиМассив& b)    { РНЦП::разверни(a.bv, b.bv); }

#ifdef DEPRECATED
	void     AddHeadPick(T&& x)            { bv.добавьГолову(new T(pick(x))); }
	void     AddTailPick(T&& x)            { bv.добавьХвост(new T(pick(x))); }
	КонстОбходчик    дайОбх(int pos) const     { return КонстОбходчик(*this, pos); }
	Обходчик         дайОбх(int pos)           { return Обходчик(*this, pos); }
	typedef T        ТипЗнач;
	STL_BI_COMPATIBILITY(БиМассив<T>)
#endif
};
