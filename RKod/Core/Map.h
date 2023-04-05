template <class K, class V>
struct КлючЗначРеф {
	const K& ключ;
	V&       значение;
	
	КлючЗначРеф(const K& ключ, V& значение) : ключ(ключ), значение(значение) {}
};

template <class вКарту, class K, class V>
struct ДиапазонКЗМап {
	вКарту& map;
	
	struct Обходчик {
		вКарту& map;
		int  ii;
		
		void SkipUnlinked()               { while(ii < map.дайСчёт() && map.отлинкован(ii)) ii++; }
		
		void operator++()                 { ++ii; SkipUnlinked(); }
		КлючЗначРеф<K, V> operator*()     { return КлючЗначРеф<K, V>(map.дайКлюч(ii), map[ii]); }
		bool operator!=(Обходчик b) const { return ii != b.ii; }
		
		Обходчик(вКарту& map, int ii) : map(map), ii(ii) { SkipUnlinked(); }
	};

	Обходчик begin() const             { return Обходчик(map, 0); }
	Обходчик end() const               { return Обходчик(map, map.дайСчёт()); }
	
	ДиапазонКЗМап(вКарту& map) : map(map) {}
};

template <class K, class T, class V>
class АМап {
protected:
	Индекс<K> ключ;
	V        значение;
	
	template <class KK>           int FindAdd_(KK&& k);
	template <class KK, class TT> int FindAdd_(KK&& k, TT&& init);
	template <class KK>           T&  Put_(KK&& k);
	template <class KK, class TT> int Put_(KK&& k, TT&& x);
	template <class KK>           int PutDefault_(KK&& k);
	template <class KK>           int FindPut_(KK&& k);
	template <class KK, class TT> int FindPut_(KK&& k, TT&& init);
	template <class KK>           T&  GetAdd_(KK&& k);
	template <class KK, class TT> T&  GetAdd_(KK&& k, TT&& init);
	template <class KK>           T&  GetPut_(KK&& k);
	template <class KK, class TT> T&  GetPut_(KK&& k, TT&& init);

public:
	T&       добавь(const K& k, const T& x)            { ключ.добавь(k); return значение.добавь(x); }
	T&       добавь(const K& k, T&& x)                 { ключ.добавь(k); return значение.добавь(pick(x)); }
	T&       добавь(const K& k)                        { ключ.добавь(k); return значение.добавь(); }
	T&       добавь(K&& k, const T& x)                 { ключ.добавь(pick(k)); return значение.добавь(x); }
	T&       добавь(K&& k, T&& x)                      { ключ.добавь(pick(k)); return значение.добавь(pick(x)); }
	T&       добавь(K&& k)                             { ключ.добавь(pick(k)); return значение.добавь(); }

	int      найди(const K& k) const                 { return ключ.найди(k); }
	int      найдиСледщ(int i) const                  { return ключ.найдиСледщ(i); }
	int      найдиПоследн(const K& k) const             { return ключ.найдиПоследн(k); }
	int      найдиПредш(int i) const                  { return ключ.найдиПредш(i); }

	int      найдиДобавь(const K& k)                    { return FindAdd_(k); }
	int      найдиДобавь(const K& k, const T& init)     { return FindAdd_(k, init); }
	int      найдиДобавь(const K& k, T&& init)          { return FindAdd_(k, pick(init)); }
	int      найдиДобавь(K&& k)                         { return FindAdd_(pick(k)); }
	int      найдиДобавь(K&& k, const T& init)          { return FindAdd_(pick(k), init); }
	int      найдиДобавь(K&& k, T&& init)               { return FindAdd_(pick(k), pick(init)); }

	T&       помести(const K& k)                        { return Put_(k); }
	int      помести(const K& k, const T& x)            { return Put_(k, x); }
	int      помести(const K& k, T&& x)                 { return Put_(k, pick(x)); }
	T&       помести(K&& k)                             { return Put_(pick(k)); }
	int      помести(K&& k, const T& x)                 { return Put_(pick(k), x); }
	int      помести(K&& k, T&& x)                      { return Put_(pick(k), pick(x)); }

	int      поместиДефолт(const K& k)                 { return PutDefault_(k); }
	int      поместиДефолт(K&& k)                      { return PutDefault_(pick(k)); }

	int      найдиПомести(const K& k)                    { return FindPut_(k); }
	int      найдиПомести(const K& k, const T& init)     { return FindPut_(k, init); }
	int      найдиПомести(const K& k, T&& init)          { return FindPut_(k, pick(init)); }
	int      найдиПомести(K&& k)                         { return FindPut_(pick(k)); }
	int      найдиПомести(K&& k, const T& init)          { return FindPut_(pick(k), init); }
	int      найдиПомести(K&& k, T&& init)               { return FindPut_(pick(k), pick(init)); }

	T&       дай(const K& k)                        { return значение[найди(k)]; }
	const T& дай(const K& k) const                  { return значение[найди(k)]; }
	const T& дай(const K& k, const T& d) const      { int i = найди(k); return i >= 0 ? значение[i] : d; }

	T&       дайДобавь(const K& k)                     { return GetAdd_(k); }
	T&       дайДобавь(const K& k, const T& x)         { return GetAdd_(k, x); }
	T&       дайДобавь(const K& k, T&& x)              { return GetAdd_(k, pick(x)); }
	T&       дайДобавь(K&& k)                          { return GetAdd_(pick(k)); }
	T&       дайДобавь(K&& k, const T& x)              { return GetAdd_(pick(k), x); }
	T&       дайДобавь(K&& k, T&& x)                   { return GetAdd_(pick(k), pick(x)); }

	T&       дайПомести(const K& k)                     { return GetPut_(k); }
	T&       дайПомести(const K& k, const T& x)         { return GetPut_(k, x); }
	T&       дайПомести(const K& k, T&& x)              { return GetPut_(k, pick(x)); }
	T&       дайПомести(K&& k)                          { return GetPut_(pick(k)); }
	T&       дайПомести(K&& k, const T& x)              { return GetPut_(pick(k), x); }
	T&       дайПомести(K&& k, T&& x)                   { return GetPut_(pick(k), pick(x)); }

	void     устКлюч(int i, const K& k)              { ключ.уст(i, k); }
	void     устКлюч(int i, K&& k)                   { ключ.уст(i, pick(k)); }

	T       *найдиУк(const K& k)                    { int i = найди(k); return i >= 0 ? &значение[i] : NULL; }
	const T *найдиУк(const K& k) const              { int i = найди(k); return i >= 0 ? &значение[i] : NULL; }

	T       *найдиПоследнУк(const K& k)                { int i = найдиПоследн(k); return i >= 0 ? &значение[i] : NULL; }
	const T *найдиПоследнУк(const K& k) const          { int i = найдиПоследн(k); return i >= 0 ? &значение[i] : NULL; }

	void     отлинкуй(int i)                          { ключ.отлинкуй(i); }
	int      отлинкуйКлюч(const K& k, unsigned h)      { return ключ.отлинкуйКлюч(k, h); }
	int      отлинкуйКлюч(const K& k)                  { return ключ.отлинкуйКлюч(k); }
	bool     отлинкован(int i) const                { return ключ.отлинкован(i); }
	void     смети();
	bool     HasUnlinked() const                    { return ключ.HasUnlinked(); }

	const T& operator[](int i) const                { return значение[i]; }
	T&       operator[](int i)                      { return значение[i]; }
	int      дайСчёт() const                       { return значение.дайСчёт(); }
	bool     пустой() const                        { return значение.пустой(); }
	void     очисть()                                { ключ.очисть(); значение.очисть(); }
	void     сожми()                               { значение.сожми(); ключ.сожми(); }
	void     резервируй(int xtra)                      { значение.резервируй(xtra); ключ.резервируй(xtra); }
	int      дайРазмест() const                       { return значение.дайРазмест(); }

	unsigned дайХэш(int i) const                   { return ключ.дайХэш(i); }

	void     сбрось(int n = 1)                        { ключ.сбрось(n); значение.сбрось(n); }
	T&       верх()                                  { return значение.верх(); }
	const T& верх() const                            { return значение.верх(); }
	const K& TopKey() const                         { return ключ.верх(); }
//	T        вынь()                                  { T h = верх(); сбрось(); return h; }
	K        PopKey()                               { K h = TopKey(); сбрось(); return h; }
	void     обрежь(int n)                            { ключ.обрежь(n); значение.обрежь(n); }

	const K& дайКлюч(int i) const                    { return ключ[i]; }

	void     удали(const int *sl, int n)           { ключ.удали(sl, n); значение.удали(sl, n); }
	void     удали(const Вектор<int>& sl)          { удали(sl, sl.дайСчёт()); }
	template <typename P> void удалиЕсли(P p)        { удали(найдиВсеи(p)); }

	void     сериализуй(Поток& s);
	void     вРяр(РярВВ& xio);
	void     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
	hash_t   дайХэшЗнач() const                   { return хэшПоСериализу(*this); }
	bool     operator==(const АМап& b) const        { ПРОВЕРЬ(!HasUnlinked()); return IsEqualMap(*this, b); }
	bool     operator!=(const АМап& b) const        { return !operator==(b); }
	int      сравни(const АМап& b) const           { ПРОВЕРЬ(!HasUnlinked()); return CompareMap(*this, b); }
	bool     operator<=(const АМап& x) const        { return сравни(x) <= 0; }
	bool     operator>=(const АМап& x) const        { return сравни(x) >= 0; }
	bool     operator<(const АМап& x) const         { return сравни(x) < 0; }
	bool     operator>(const АМап& x) const         { return сравни(x) > 0; }

	void     разверни(АМап& x)                          { РНЦП::разверни(значение, x.значение); РНЦП::разверни(ключ, x.ключ); }
	const Индекс<K>&  дайИндекс() const               { return ключ; }
	Индекс<K>         PickIndex()                    { return pick(ключ); }

	const Вектор<K>& дайКлючи() const                { return ключ.дайКлючи(); }
	Вектор<K>        подбериКлючи()                     { return ключ.подбериКлючи(); }

	const V&         дайЗначения() const              { return значение; }
	V&               дайЗначения()                    { return значение; }
	V                подбериЗначения()                   { return pick(значение); }
	
	ДиапазонКЗМап<АМап, K, T> operator~()                   { return ДиапазонКЗМап<АМап, K, T>(*this); }
	ДиапазонКЗМап<const АМап, K, const T> operator~() const { return ДиапазонКЗМап<const АМап, K, const T>(*this); }
	
	АМап& operator()(const K& k, const T& v)        { добавь(k, v); return *this; }

	АМап()                                          {}
	АМап(const АМап& s, int) : ключ(s.ключ, 0), значение(s.значение, 0) {}
	АМап(Индекс<K>&& ndx, V&& val) : ключ(pick(ndx)), значение(pick(val)) {}
	АМап(Вектор<K>&& ndx, V&& val) : ключ(pick(ndx)), значение(pick(val)) {}
	АМап(std::initializer_list<std::pair<K, T>> init) { for(const auto& i : init) добавь(clone(i.first), clone(i.second)); }

	typedef ОбходчикУ<V>           Обходчик;
	typedef КонстОбходчикУ<V>      КонстОбходчик;

	Обходчик         begin()                        { return значение.begin(); }
	Обходчик         end()                          { return значение.end(); }
	КонстОбходчик    begin() const                  { return значение.begin(); }
	КонстОбходчик    end() const                    { return значение.end(); }

#ifdef DEPRECATED
	typedef V                          КонтейнерЗначений;
	typedef T                          ТипЗнач;

	typedef Вектор<K> КонтейнерКлючей;
	typedef K         ТипКлюча;
	typedef КонстОбходчикУ<Индекс<K>> КонстОбходчикКлючей;

	friend int     дайСчёт(const АМап& v)                        { return v.дайСчёт(); }
	int      PutPick(const K& k, T&& x)                           { return помести(k, pick(x)); }
	T&       добавьПодбор(const K& k, T&& x)                           { return добавь(k, pick(x)); }
	int      FindAddPick(const K& k, T&& init)                    { return найдиДобавь(k, pick(init)); }
	int      FindPutPick(const K& k, T&& init)                    { return найдиПомести(k, pick(init)); }
	T&       GetAddPick(const K& k, T&& x)                        { return дайДобавь(k, pick(x)); }
	T&       GetPutPick(const K& k, T&& x)                        { return дайПомести(k, pick(x)); }

	КонстОбходчикКлючей дайОбхКлючей(int pos) const                    { return ключ.дайОбх(pos); }
	Обходчик         дайОбх(int pos)                             { return значение.дайОбх(pos); }
	КонстОбходчик    дайОбх(int pos) const                       { return значение.дайОбх(pos); }

	КонстОбходчикКлючей стартКлючи() const                             { return ключ.begin(); }
	КонстОбходчикКлючей стопКлючи() const                               { return ключ.end(); }

	T&       вставь(int i, const K& k)             { ключ.вставь(i, k); return значение.вставь(i); }
	T&       вставь(int i, const K& k, const T& x) { ключ.вставь(i, k); return значение.вставь(i, x); }
	T&       вставь(int i, const K& k, T&& x)      { ключ.вставь(i, k); return значение.вставь(i, pick(x)); }
	T&       вставь(int i, K&& k)                  { ключ.вставь(i, pick(k)); return значение.вставь(i); }
	T&       вставь(int i, K&& k, const T& x)      { ключ.вставь(i, pick(k)); return значение.вставь(i, x); }
	T&       вставь(int i, K&& k, T&& x)           { ключ.вставь(i, pick(k)); return значение.вставь(i, pick(x)); }

	void     удали(int i)                         { ключ.удали(i); значение.удали(i); }
	void     удали(int i, int count)              { ключ.удали(i, count); значение.удали(i, count); }
	int      удалиКлюч(const K& k);
#endif
};

template <class K, class T>
class ВекторМап : public ОпцияДвижимогоИГлубКопии<ВекторМап<K, T>>,
                  public АМап<K, T, Вектор<T>> {
    typedef АМап<K, T, Вектор<T>> B;
public:
	T        вынь()                            { T h = pick(B::верх()); B::сбрось(); return h; }

	ВекторМап(const ВекторМап& s, int) : АМап<K, T, Вектор<T>>(s, 1) {}
	ВекторМап(Индекс<K>&& ndx, Вектор<T>&& val) : АМап<K, T, Вектор<T>>(pick(ndx), pick(val)) {}
	ВекторМап(Вектор<K>&& ndx, Вектор<T>&& val) : АМап<K, T, Вектор<T>>(pick(ndx), pick(val)) {}
	ВекторМап()                                                       {}
	ВекторМап(std::initializer_list<std::pair<K, T>> init) : B::АМап(init) {}

	template <class KK, class TT>
	ВекторМап& operator()(KK&& k, TT&& v)                { B::добавь(std::forward<KK>(k), std::forward<TT>(v)); return *this; }

	friend void    разверни(ВекторМап& a, ВекторМап& b)      { a.B::разверни(b); }

	typedef typename АМап< K, T, Вектор<T>>::КонстОбходчик КонстОбходчик;
	typedef typename АМап< K, T, Вектор<T>>::Обходчик      Обходчик;
	STL_MAP_COMPATIBILITY(ВекторМап<K _cm_ T _cm_ HashFn>)
};

template <class K, class T>
class МассивМап : public ОпцияДвижимогоИГлубКопии<МассивМап<K, T>>,
                 public АМап<K, T, Массив<T>> {
	typedef АМап<K, T, Массив<T>> B;
public:
	T&        добавь(const K& k, const T& x)          { return B::добавь(k, x); }
	T&        добавь(const K& k, T&& x)               { return B::добавь(k, pick(x)); }
	T&        добавь(const K& k)                      { return B::добавь(k); }
	T&        добавь(const K& k, T *newt)             { B::ключ.добавь(k); return B::значение.добавь(newt); }
	T&        добавь(K&& k, const T& x)               { return B::добавь(pick(k), x); }
	T&        добавь(K&& k, T&& x)                    { return B::добавь(pick(k), pick(x)); }
	T&        добавь(K&& k)                           { return B::добавь(pick(k)); }
	T&        добавь(K&& k, T *newt)                  { B::ключ.добавь(pick(k)); return B::значение.добавь(newt); }

	T&        вставь(int i, const K& k, T *newt)   { B::ключ.вставь(i, k); return B::значение.вставь(i, newt); }
	T&        вставь(int i, K&& k, T *newt)        { B::ключ.вставь(i, pick(k)); return B::значение.вставь(i, newt); }
	using B::вставь;

	template <class TT, class... Арги>
	TT& создай(const K& k, Арги&&... арги)         { TT *q = new TT(std::forward<Арги>(арги)...); B::ключ.добавь(k); return static_cast<TT&>(B::значение.добавь(q)); }
	template <class TT, class... Арги>
	TT& создай(K&& k, Арги&&... арги)              { TT *q = new TT(std::forward<Арги>(арги)...); B::ключ.добавь(pick(k)); return static_cast<TT&>(B::значение.добавь(q)); }

	T&        уст(int i, T *ptr)                   { return B::значение.уст(i, ptr); }
	T        *выньОткрепи()                          { B::ключ.сбрось(); return B::значение.выньОткрепи(); }
	T        *открепи(int i)                        { B::ключ.удали(i); return B::значение.открепи(i); }
	T        *разверни(int i, T *newt)                 { return B::значение.разверни(i, newt); }

	МассивМап(const МассивМап& s, int) : АМап<K, T, Массив<T>>(s, 1) {}
	МассивМап(Индекс<K>&& ndx, Массив<T>&& val) : АМап<K, T, Массив<T>>(pick(ndx), pick(val)) {}
	МассивМап(Вектор<K>&& ndx, Массив<T>&& val) : АМап<K, T, Массив<T>>(pick(ndx), pick(val)) {}
	МассивМап() {}

	МассивМап(std::initializer_list<std::pair<K, T>> init) : B::АМап(init) {}

	template <class KK, class TT>
	МассивМап& operator()(KK&& k, TT&& v)           { добавь(std::forward<KK>(k), std::forward<TT>(v)); return *this; }

	friend void    разверни(МассивМап& a, МассивМап& b)        { a.B::разверни(b); }

	typedef typename АМап< K, T, Массив<T>>::КонстОбходчик КонстОбходчик;
	typedef typename АМап< K, T, Массив<T>>::Обходчик      Обходчик;
	STL_MAP_COMPATIBILITY(МассивМап<K _cm_ T _cm_ HashFn>)
};
