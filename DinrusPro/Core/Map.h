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
		цел  ii;
		
		проц SkipUnlinked()               { while(ii < map.дайСчёт() && map.отлинкован(ii)) ii++; }
		
		проц operator++()                 { ++ii; SkipUnlinked(); }
		КлючЗначРеф<K, V> operator*()     { return КлючЗначРеф<K, V>(map.дайКлюч(ii), map[ii]); }
		бул operator!=(Обходчик b) const { return ii != b.ii; }
		
		Обходчик(вКарту& map, цел ii) : map(map), ii(ii) { SkipUnlinked(); }
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
	
	template <class KK>           цел FindAdd_(KK&& k);
	template <class KK, class TT> цел FindAdd_(KK&& k, TT&& init);
	template <class KK>           T&  Put_(KK&& k);
	template <class KK, class TT> цел Put_(KK&& k, TT&& x);
	template <class KK>           цел PutDefault_(KK&& k);
	template <class KK>           цел FindPut_(KK&& k);
	template <class KK, class TT> цел FindPut_(KK&& k, TT&& init);
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

	цел      найди(const K& k) const                 { return ключ.найди(k); }
	цел      найдиСледщ(цел i) const                  { return ключ.найдиСледщ(i); }
	цел      найдиПоследн(const K& k) const             { return ключ.найдиПоследн(k); }
	цел      найдиПредш(цел i) const                  { return ключ.найдиПредш(i); }

	цел      найдиДобавь(const K& k)                    { return FindAdd_(k); }
	цел      найдиДобавь(const K& k, const T& init)     { return FindAdd_(k, init); }
	цел      найдиДобавь(const K& k, T&& init)          { return FindAdd_(k, pick(init)); }
	цел      найдиДобавь(K&& k)                         { return FindAdd_(pick(k)); }
	цел      найдиДобавь(K&& k, const T& init)          { return FindAdd_(pick(k), init); }
	цел      найдиДобавь(K&& k, T&& init)               { return FindAdd_(pick(k), pick(init)); }

	T&       помести(const K& k)                        { return Put_(k); }
	цел      помести(const K& k, const T& x)            { return Put_(k, x); }
	цел      помести(const K& k, T&& x)                 { return Put_(k, pick(x)); }
	T&       помести(K&& k)                             { return Put_(pick(k)); }
	цел      помести(K&& k, const T& x)                 { return Put_(pick(k), x); }
	цел      помести(K&& k, T&& x)                      { return Put_(pick(k), pick(x)); }

	цел      поместиДефолт(const K& k)                 { return PutDefault_(k); }
	цел      поместиДефолт(K&& k)                      { return PutDefault_(pick(k)); }

	цел      найдиПомести(const K& k)                    { return FindPut_(k); }
	цел      найдиПомести(const K& k, const T& init)     { return FindPut_(k, init); }
	цел      найдиПомести(const K& k, T&& init)          { return FindPut_(k, pick(init)); }
	цел      найдиПомести(K&& k)                         { return FindPut_(pick(k)); }
	цел      найдиПомести(K&& k, const T& init)          { return FindPut_(pick(k), init); }
	цел      найдиПомести(K&& k, T&& init)               { return FindPut_(pick(k), pick(init)); }

	T&       дай(const K& k)                        { return значение[найди(k)]; }
	const T& дай(const K& k) const                  { return значение[найди(k)]; }
	const T& дай(const K& k, const T& d) const      { цел i = найди(k); return i >= 0 ? значение[i] : d; }

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

	проц     устКлюч(цел i, const K& k)              { ключ.уст(i, k); }
	проц     устКлюч(цел i, K&& k)                   { ключ.уст(i, pick(k)); }

	T       *найдиУк(const K& k)                    { цел i = найди(k); return i >= 0 ? &значение[i] : NULL; }
	const T *найдиУк(const K& k) const              { цел i = найди(k); return i >= 0 ? &значение[i] : NULL; }

	T       *найдиПоследнУк(const K& k)                { цел i = найдиПоследн(k); return i >= 0 ? &значение[i] : NULL; }
	const T *найдиПоследнУк(const K& k) const          { цел i = найдиПоследн(k); return i >= 0 ? &значение[i] : NULL; }

	проц     отлинкуй(цел i)                          { ключ.отлинкуй(i); }
	цел      отлинкуйКлюч(const K& k, unsigned h)      { return ключ.отлинкуйКлюч(k, h); }
	цел      отлинкуйКлюч(const K& k)                  { return ключ.отлинкуйКлюч(k); }
	бул     отлинкован(цел i) const                { return ключ.отлинкован(i); }
	проц     смети();
	бул     HasUnlinked() const                    { return ключ.HasUnlinked(); }

	const T& operator[](цел i) const                { return значение[i]; }
	T&       operator[](цел i)                      { return значение[i]; }
	цел      дайСчёт() const                       { return значение.дайСчёт(); }
	бул     пустой() const                        { return значение.пустой(); }
	проц     очисть()                                { ключ.очисть(); значение.очисть(); }
	проц     сожми()                               { значение.сожми(); ключ.сожми(); }
	проц     резервируй(цел xtra)                      { значение.резервируй(xtra); ключ.резервируй(xtra); }
	цел      дайРазмест() const                       { return значение.дайРазмест(); }

	unsigned дайХэш(цел i) const                   { return ключ.дайХэш(i); }

	проц     сбрось(цел n = 1)                        { ключ.сбрось(n); значение.сбрось(n); }
	T&       верх()                                  { return значение.верх(); }
	const T& верх() const                            { return значение.верх(); }
	const K& TopKey() const                         { return ключ.верх(); }
//	T        вынь()                                  { T h = верх(); сбрось(); return h; }
	K        PopKey()                               { K h = TopKey(); сбрось(); return h; }
	проц     обрежь(цел n)                            { ключ.обрежь(n); значение.обрежь(n); }

	const K& дайКлюч(цел i) const                    { return ключ[i]; }

	проц     удали(const цел *sl, цел n)           { ключ.удали(sl, n); значение.удали(sl, n); }
	проц     удали(const Вектор<цел>& sl)          { удали(sl, sl.дайСчёт()); }
	template <typename P> проц удалиЕсли(P p)        { удали(найдиВсеи(p)); }

	проц     сериализуй(Поток& s);
	проц     вРяр(РярВВ& xio);
	проц     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
	т_хэш   дайХэшЗнач() const                   { return хэшПоСериализу(*this); }
	бул     operator==(const АМап& b) const        { ПРОВЕРЬ(!HasUnlinked()); return равныМапы(*this, b); }
	бул     operator!=(const АМап& b) const        { return !operator==(b); }
	цел      сравни(const АМап& b) const           { ПРОВЕРЬ(!HasUnlinked()); return сравниМап(*this, b); }
	бул     operator<=(const АМап& x) const        { return сравни(x) <= 0; }
	бул     operator>=(const АМап& x) const        { return сравни(x) >= 0; }
	бул     operator<(const АМап& x) const         { return сравни(x) < 0; }
	бул     operator>(const АМап& x) const         { return сравни(x) > 0; }

	проц     разверни(АМап& x)                          { РНЦП::разверни(значение, x.значение); РНЦП::разверни(ключ, x.ключ); }
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
	АМап(const АМап& s, цел) : ключ(s.ключ, 0), значение(s.значение, 0) {}
	АМап(Индекс<K>&& ndx, V&& знач) : ключ(pick(ndx)), значение(pick(знач)) {}
	АМап(Вектор<K>&& ndx, V&& знач) : ключ(pick(ndx)), значение(pick(знач)) {}
	АМап(std::initializer_list<std::pair<K, T>> init) { for(const auto& i : init) добавь(клонируй(i.first), клонируй(i.second)); }

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

	friend цел     дайСчёт(const АМап& v)                        { return v.дайСчёт(); }
	цел      PutPick(const K& k, T&& x)                           { return помести(k, pick(x)); }
	T&       добавьПодбор(const K& k, T&& x)                           { return добавь(k, pick(x)); }
	цел      FindAddPick(const K& k, T&& init)                    { return найдиДобавь(k, pick(init)); }
	цел      FindPutPick(const K& k, T&& init)                    { return найдиПомести(k, pick(init)); }
	T&       GetAddPick(const K& k, T&& x)                        { return дайДобавь(k, pick(x)); }
	T&       GetPutPick(const K& k, T&& x)                        { return дайПомести(k, pick(x)); }

	КонстОбходчикКлючей дайОбхКлючей(цел pos) const                    { return ключ.дайОбх(pos); }
	Обходчик         дайОбх(цел pos)                             { return значение.дайОбх(pos); }
	КонстОбходчик    дайОбх(цел pos) const                       { return значение.дайОбх(pos); }

	КонстОбходчикКлючей стартКлючи() const                             { return ключ.begin(); }
	КонстОбходчикКлючей стопКлючи() const                               { return ключ.end(); }

	T&       вставь(цел i, const K& k)             { ключ.вставь(i, k); return значение.вставь(i); }
	T&       вставь(цел i, const K& k, const T& x) { ключ.вставь(i, k); return значение.вставь(i, x); }
	T&       вставь(цел i, const K& k, T&& x)      { ключ.вставь(i, k); return значение.вставь(i, pick(x)); }
	T&       вставь(цел i, K&& k)                  { ключ.вставь(i, pick(k)); return значение.вставь(i); }
	T&       вставь(цел i, K&& k, const T& x)      { ключ.вставь(i, pick(k)); return значение.вставь(i, x); }
	T&       вставь(цел i, K&& k, T&& x)           { ключ.вставь(i, pick(k)); return значение.вставь(i, pick(x)); }

	проц     удали(цел i)                         { ключ.удали(i); значение.удали(i); }
	проц     удали(цел i, цел count)              { ключ.удали(i, count); значение.удали(i, count); }
	цел      удалиКлюч(const K& k);
#endif
};

template <class K, class T>
class ВекторМап : public ОпцияДвижимогоИГлубКопии<ВекторМап<K, T>>,
                  public АМап<K, T, Вектор<T>> {
    typedef АМап<K, T, Вектор<T>> B;
public:
	T        вынь()                            { T h = pick(B::верх()); B::сбрось(); return h; }

	ВекторМап(const ВекторМап& s, цел) : АМап<K, T, Вектор<T>>(s, 1) {}
	ВекторМап(Индекс<K>&& ndx, Вектор<T>&& знач) : АМап<K, T, Вектор<T>>(pick(ndx), pick(знач)) {}
	ВекторМап(Вектор<K>&& ndx, Вектор<T>&& знач) : АМап<K, T, Вектор<T>>(pick(ndx), pick(знач)) {}
	ВекторМап()                                                       {}
	ВекторМап(std::initializer_list<std::pair<K, T>> init) : B::АМап(init) {}

	template <class KK, class TT>
	ВекторМап& operator()(KK&& k, TT&& v)                { B::добавь(std::forward<KK>(k), std::forward<TT>(v)); return *this; }

	friend проц    разверни(ВекторМап& a, ВекторМап& b)      { a.B::разверни(b); }

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

	T&        вставь(цел i, const K& k, T *newt)   { B::ключ.вставь(i, k); return B::значение.вставь(i, newt); }
	T&        вставь(цел i, K&& k, T *newt)        { B::ключ.вставь(i, pick(k)); return B::значение.вставь(i, newt); }
	using B::вставь;

	template <class TT, class... Арги>
	TT& создай(const K& k, Арги&&... арги)         { TT *q = new TT(std::forward<Арги>(арги)...); B::ключ.добавь(k); return static_cast<TT&>(B::значение.добавь(q)); }
	template <class TT, class... Арги>
	TT& создай(K&& k, Арги&&... арги)              { TT *q = new TT(std::forward<Арги>(арги)...); B::ключ.добавь(pick(k)); return static_cast<TT&>(B::значение.добавь(q)); }

	T&        уст(цел i, T *укз)                   { return B::значение.уст(i, укз); }
	T        *выньОткрепи()                          { B::ключ.сбрось(); return B::значение.выньОткрепи(); }
	T        *открепи(цел i)                        { B::ключ.удали(i); return B::значение.открепи(i); }
	T        *разверни(цел i, T *newt)                 { return B::значение.разверни(i, newt); }

	МассивМап(const МассивМап& s, цел) : АМап<K, T, Массив<T>>(s, 1) {}
	МассивМап(Индекс<K>&& ndx, Массив<T>&& знач) : АМап<K, T, Массив<T>>(pick(ndx), pick(знач)) {}
	МассивМап(Вектор<K>&& ndx, Массив<T>&& знач) : АМап<K, T, Массив<T>>(pick(ndx), pick(знач)) {}
	МассивМап() {}

	МассивМап(std::initializer_list<std::pair<K, T>> init) : B::АМап(init) {}

	template <class KK, class TT>
	МассивМап& operator()(KK&& k, TT&& v)           { добавь(std::forward<KK>(k), std::forward<TT>(v)); return *this; }

	friend проц    разверни(МассивМап& a, МассивМап& b)        { a.B::разверни(b); }

	typedef typename АМап< K, T, Массив<T>>::КонстОбходчик КонстОбходчик;
	typedef typename АМап< K, T, Массив<T>>::Обходчик      Обходчик;
	STL_MAP_COMPATIBILITY(МассивМап<K _cm_ T _cm_ HashFn>)
};
