template <class K, class T, class V, class Less>
class ФиксАМап {
protected:
	Вектор<K> ключ;
	V значение;

public:
	T&       добавь(const K& k, const T& x)       { ключ.добавь(k); return значение.добавь(x); }
	T&       добавьПодбор(const K& k, T&& x)        { ключ.добавь(k); return значение.добавьПодбор(pick(x)); }
	T&       добавь(const K& k)                   { ключ.добавь(k); return значение.добавь(); }
	
	void     финиш()                          { IndexSort(ключ, значение, Less()); сожми(); }

	int      найди(const K& k) const            { return найдиБинар(ключ, k, Less()); }
	int      найдиСледщ(int i) const             { return i + 1 < ключ.дайСчёт() && ключ[i] == ключ[i + 1] ? i + 1 : -1; }

	T&       дай(const K& k)                   { return значение[найди(k)]; }
	const T& дай(const K& k) const             { return значение[найди(k)]; }
	const T& дай(const K& k, const T& d) const { int i = найди(k); return i >= 0 ? значение[i] : d; }

	T       *найдиУк(const K& k)               { int i = найди(k); return i >= 0 ? &значение[i] : NULL; }
	const T *найдиУк(const K& k) const         { int i = найди(k); return i >= 0 ? &значение[i] : NULL; }

	const T& operator[](int i) const           { return значение[i]; }
	T&       operator[](int i)                 { return значение[i]; }
	int      дайСчёт() const                  { return значение.дайСчёт(); }
	bool     пустой() const                   { return значение.пустой(); }
	void     очисть()                           { ключ.очисть(); значение.очисть(); }
	void     сожми()                          { значение.сожми(); ключ.сожми(); }
	void     резервируй(int xtra)                 { значение.резервируй(xtra); ключ.резервируй(xtra); }
	int      дайРазмест() const                  { return значение.дайРазмест(); }

	const K& дайКлюч(int i) const               { return ключ[i]; }

#ifdef РНЦП
	void     сериализуй(Поток& s);
	void     вРяр(РярВВ& xio);
	void     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
#endif

	void     разверни(ФиксАМап& x)                { РНЦП::разверни(значение, x.значение); РНЦП::разверни(ключ, x.ключ); }

	const Вектор<K>& дайКлючи() const           { return ключ; }
	Вектор<K>        подбериКлючи()                { return pick(ключ); }

	const V&         дайЗначения() const         { return значение; }
	V&               дайЗначения()               { return значение; }
	V                подбериЗначения()              { return pick(значение); }
	
	ФиксАМап& operator()(const K& k, const T& v)       { добавь(k, v); return *this; }

	ФиксАМап()                                         {}
	ФиксАМап(const ФиксАМап& s, int) : ключ(s.ключ, 0), значение(s.значение, 0) {}
	ФиксАМап(Вектор<K>&& ключ, V&& val) : ключ(pick(ключ)), значение(pick(val)) {}

	typedef КонстОбходчикУ<V>  КонстОбходчик;
	typedef ОбходчикУ<V>       Обходчик;

	Обходчик         begin()                                      { return значение.begin(); }
	Обходчик         end()                                        { return значение.end(); }
	КонстОбходчик    begin() const                                { return значение.begin(); }
	КонстОбходчик    end() const                                  { return значение.end(); }

#ifdef DEPRECATED
	typedef V                          КонтейнерЗначений;
	typedef T                          ТипЗнач;

	typedef Вектор<K> КонтейнерКлючей;
	typedef K         ТипКлюча;
	typedef КонстОбходчикУ<Вектор<K>> КонстОбходчикКлючей;

	КонстОбходчикКлючей стартКлючи() const                             { return ключ.старт(); }
	КонстОбходчикКлючей стопКлючи() const                               { return ключ.стоп(); }
	КонстОбходчикКлючей дайОбхКлючей(int pos) const                    { return ключ.дайОбх(pos); }

	Обходчик         дайОбх(int pos)                             { return значение.дайОбх(pos); }
	КонстОбходчик    дайОбх(int pos) const                       { return значение.дайОбх(pos); }
#endif
};

template <class K, class T, class Less = StdLess<K> >
class ФиксированнВекторМап : public ОпцияДвижимогоИГлубКопии<ФиксированнВекторМап<K, T, Less> >,
                       public ФиксАМап< K, T, Вектор<T>, Less > {
    typedef ФиксАМап< K, T, Вектор<T>, Less > B;
public:
	ФиксированнВекторМап(const ФиксированнВекторМап& s, int) : ФиксАМап<K, T, Вектор<T>, Less>(s, 1) {}
	ФиксированнВекторМап(Вектор<K>&& ключ, Вектор<T>&& val) : ФиксАМап<K, T, Вектор<T>, Less>(pick(ключ), pick(val)) {}
	ФиксированнВекторМап()                                                       {}

	friend void    разверни(ФиксированнВекторМап& a, ФиксированнВекторМап& b)      { a.B::разверни(b); }

	typedef typename ФиксАМап< K, T, Вектор<T>, Less >::КонстОбходчик КонстОбходчик; // GCC bug (?)
	typedef typename ФиксАМап< K, T, Вектор<T>, Less >::Обходчик      Обходчик; // GCC bug (?)
	STL_MAP_COMPATIBILITY(ФиксированнВекторМап<K _cm_ T _cm_ Less>)
};

template <class K, class T, class Less = StdLess<K> >
class ФиксированнМассивМап : public ОпцияДвижимогоИГлубКопии< ФиксированнМассивМап<K, T, Less> >,
                      public ФиксАМап< K, T, Массив<T>, Less > {
	typedef ФиксАМап< K, T, Массив<T>, Less > B;
public:
	T&        добавь(const K& k, const T& x)          { return B::добавь(k, x); }
	T&        добавь(const K& k)                      { return B::добавь(k); }
	T&        добавь(const K& k, T *newt)             { B::ключ.добавь(k); return B::значение.добавь(newt); }
	template <class TT, class... Арги>
	TT&       создай(const K& k, Арги&&... арги)   { TT *q = new TT(std::forward<Арги>(арги)...); B::ключ.добавь(k); return static_cast<TT&>(B::значение.добавь(q)); }

	ФиксированнМассивМап(const ФиксированнМассивМап& s, int) : ФиксАМап<K, T, Массив<T>, Less>(s, 1) {}
	ФиксированнМассивМап(Вектор<K>&& ndx, Массив<T>&& val) : ФиксАМап<K, T, Массив<T>, Less>(pick(ndx), pick(val)) {}
	ФиксированнМассивМап() {}

	friend void    разверни(ФиксированнМассивМап& a, ФиксированнМассивМап& b)        { a.B::разверни(b); }

	typedef typename ФиксАМап< K, T, Массив<T>, Less >::КонстОбходчик КонстОбходчик; // GCC bug (?)
	typedef typename ФиксАМап< K, T, Массив<T>, Less >::Обходчик      Обходчик; // GCC bug (?)
	STL_MAP_COMPATIBILITY(ФиксированнМассивМап<K _cm_ T _cm_ Less>)
};
