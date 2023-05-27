template <class K, class T, class V, class Less>
class ФиксАМап {
protected:
	Вектор<K> ключ;
	V значение;

public:
	T&       добавь(const K& k, const T& x)       { ключ.добавь(k); return значение.добавь(x); }
	T&       добавьПодбор(const K& k, T&& x)        { ключ.добавь(k); return значение.добавьПодбор(pick(x)); }
	T&       добавь(const K& k)                   { ключ.добавь(k); return значение.добавь(); }
	
	проц     финиш()                          { IndexSort(ключ, значение, Less()); сожми(); }

	цел      найди(const K& k) const            { return найдиБинар(ключ, k, Less()); }
	цел      найдиСледщ(цел i) const             { return i + 1 < ключ.дайСчёт() && ключ[i] == ключ[i + 1] ? i + 1 : -1; }

	T&       дай(const K& k)                   { return значение[найди(k)]; }
	const T& дай(const K& k) const             { return значение[найди(k)]; }
	const T& дай(const K& k, const T& d) const { цел i = найди(k); return i >= 0 ? значение[i] : d; }

	T       *найдиУк(const K& k)               { цел i = найди(k); return i >= 0 ? &значение[i] : NULL; }
	const T *найдиУк(const K& k) const         { цел i = найди(k); return i >= 0 ? &значение[i] : NULL; }

	const T& operator[](цел i) const           { return значение[i]; }
	T&       operator[](цел i)                 { return значение[i]; }
	цел      дайСчёт() const                  { return значение.дайСчёт(); }
	бул     пустой() const                   { return значение.пустой(); }
	проц     очисть()                           { ключ.очисть(); значение.очисть(); }
	проц     сожми()                          { значение.сожми(); ключ.сожми(); }
	проц     резервируй(цел xtra)                 { значение.резервируй(xtra); ключ.резервируй(xtra); }
	цел      дайРазмест() const                  { return значение.дайРазмест(); }

	const K& дайКлюч(цел i) const               { return ключ[i]; }

#ifdef РНЦП
	проц     сериализуй(Поток& s);
	проц     вРяр(РярВВ& xio);
	проц     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
#endif

	проц     разверни(ФиксАМап& x)                { РНЦП::разверни(значение, x.значение); РНЦП::разверни(ключ, x.ключ); }

	const Вектор<K>& дайКлючи() const           { return ключ; }
	Вектор<K>        подбериКлючи()                { return pick(ключ); }

	const V&         дайЗначения() const         { return значение; }
	V&               дайЗначения()               { return значение; }
	V                подбериЗначения()              { return pick(значение); }
	
	ФиксАМап& operator()(const K& k, const T& v)       { добавь(k, v); return *this; }

	ФиксАМап()                                         {}
	ФиксАМап(const ФиксАМап& s, цел) : ключ(s.ключ, 0), значение(s.значение, 0) {}
	ФиксАМап(Вектор<K>&& ключ, V&& знач) : ключ(pick(ключ)), значение(pick(знач)) {}

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
	КонстОбходчикКлючей дайОбхКлючей(цел pos) const                    { return ключ.дайОбх(pos); }

	Обходчик         дайОбх(цел pos)                             { return значение.дайОбх(pos); }
	КонстОбходчик    дайОбх(цел pos) const                       { return значение.дайОбх(pos); }
#endif
};

template <class K, class T, class Less = StdLess<K> >
class ФиксированнВекторМап : public ОпцияДвижимогоИГлубКопии<ФиксированнВекторМап<K, T, Less> >,
                       public ФиксАМап< K, T, Вектор<T>, Less > {
    typedef ФиксАМап< K, T, Вектор<T>, Less > B;
public:
	ФиксированнВекторМап(const ФиксированнВекторМап& s, цел) : ФиксАМап<K, T, Вектор<T>, Less>(s, 1) {}
	ФиксированнВекторМап(Вектор<K>&& ключ, Вектор<T>&& знач) : ФиксАМап<K, T, Вектор<T>, Less>(pick(ключ), pick(знач)) {}
	ФиксированнВекторМап()                                                       {}

	friend проц    разверни(ФиксированнВекторМап& a, ФиксированнВекторМап& b)      { a.B::разверни(b); }

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

	ФиксированнМассивМап(const ФиксированнМассивМап& s, цел) : ФиксАМап<K, T, Массив<T>, Less>(s, 1) {}
	ФиксированнМассивМап(Вектор<K>&& ndx, Массив<T>&& знач) : ФиксАМап<K, T, Массив<T>, Less>(pick(ndx), pick(знач)) {}
	ФиксированнМассивМап() {}

	friend проц    разверни(ФиксированнМассивМап& a, ФиксированнМассивМап& b)        { a.B::разверни(b); }

	typedef typename ФиксАМап< K, T, Массив<T>, Less >::КонстОбходчик КонстОбходчик; // GCC bug (?)
	typedef typename ФиксАМап< K, T, Массив<T>, Less >::Обходчик      Обходчик; // GCC bug (?)
	STL_MAP_COMPATIBILITY(ФиксированнМассивМап<K _cm_ T _cm_ Less>)
};
