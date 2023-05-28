struct ИндексОбщее {
	enum { HIBIT = 0x80000000 };

	struct Хэш : Движ<Хэш> {
		цел   next; // also link for unlinked items...
		бцел hash;
		цел   prev;
	};
	
	цел         *map;
	Хэш        *hash;
	бцел        mask;
	цел          unlinked;

	
	static цел empty[1];

	static бцел Smear(т_хэш h)          { return складиХэш(h) | HIBIT; }

	проц линкуй(цел& m, Хэш& hh, цел ii);
	проц линкуй(цел ii, бцел sh);
	проц уд(цел& m, Хэш& hh, цел ii);
	проц Ins(цел& m, Хэш& hh, цел ii);

	проц сделайМап(цел count);
	проц ремапируй(цел count);
	проц реиндексируй(цел count);
	проц нарастиМап(цел count);
	проц освободиМап();
	проц освободи();

	проц уст(цел ii, бцел h);
	
	Вектор<цел> дайОтлинкованно() const;
	
	проц очисть();
	проц обрежь(цел n, цел count);
	проц смети(цел n);
	проц резервируй(цел n);
	проц сожми();
	проц AdjustMap(цел count, цел alloc);
	
	проц копируй(const ИндексОбщее& b, цел count);
	проц подбери(ИндексОбщее& b);
	проц разверни(ИндексОбщее& b);
	
	ИндексОбщее();
	~ИндексОбщее();
};

template <class K, class T, class V> class АМап;

template <class T>
class Индекс : ОпцияДвижимогоИГлубКопии<Индекс<T>>, ИндексОбщее {
	Вектор<T> ключ;

	static бцел Smear(const T& k)   { return ИндексОбщее::Smear(дайХэшЗнач(k)); }

	цел  найдиОт(цел i, бцел sh, const T& k, цел end) const;
	цел  FindBack(цел i, бцел sh, const T& k, цел end) const;


	проц ReallocHash(цел n);
	template <typename U> проц нарастиДобавь(U&& k, бцел sh);
	template <typename U> проц AddS(цел& m, U&& k, бцел sh);
	template <typename U> проц AddS(U&& k, бцел sh);

	template <class OP, class U> цел найдиДобавь(U&& k, OP add_op);
	template <class U> цел FindPut0(U&& k);

	template <typename U> цел Put0(U&& k, бцел sh);
	template <typename U> проц уст0(цел i, U&& k);

	template <typename, typename, typename> friend class АМап;
	
	проц        фиксируйХэш(бул makemap = true);

public:
	проц        добавь(const T& k)             { AddS(k, Smear(k)); }
	проц        добавь(T&& k)                  { AddS(пикуй(k), Smear(k)); }
	Индекс&      operator<<(const T& x)      { добавь(x); return *this; }
	Индекс&      operator<<(T&& x)           { добавь(пикуй(x)); return *this; }

	цел         найди(const T& k) const;
	цел         найдиСледщ(цел i) const;
	цел         найдиПоследн(const T& k) const;
	цел         найдиПредш(цел i) const;
	
	цел         найдиДобавь(const T& k)         { return найдиДобавь(k, []{}); }
	цел         найдиДобавь(T&& k)              { return найдиДобавь(пикуй(k), []{}); }

	цел         помести(const T& k)             { return Put0(k, Smear(k)); }
	цел         помести(T&& k)                  { return Put0(пикуй(k), Smear(k)); }
	цел         найдиПомести(const T& k)         { return FindPut0(k); }
	цел         найдиПомести(T&& k)              { return FindPut0(пикуй(k)); }

	проц        отлинкуй(цел i);
	цел         отлинкуйКлюч(const T& k);
	бул        отлинкован(цел i) const      { return hash[i].hash == 0; }
	бул        HasUnlinked() const          { return unlinked >= 0; }
	Вектор<цел> дайОтлинкованно() const          { return ИндексОбщее::дайОтлинкованно(); }

	проц        смети();

	проц        уст(цел i, const T& k)       { уст0(i, k); }
	проц        уст(цел i, T&& k)            { уст0(i, пикуй(k)); }

	const T&    operator[](цел i) const      { return ключ[i]; }
	цел         дайСчёт() const             { return ключ.дайСчёт(); }
	бул        пустой() const              { return ключ.пустой(); }
	
	проц        очисть()                      { ключ.очисть(); ИндексОбщее::очисть(); }

	проц        обрежь(цел n = 0)              { ИндексОбщее::обрежь(n, дайСчёт()); ключ.обрежь(n); }
	проц        сбрось(цел n = 1)              { обрежь(дайСчёт() - 1); }
	const T&    верх() const                  { return ключ.верх(); }
	T           вынь()                        { T x = пикуй(верх()); сбрось(); return x; }

	проц        резервируй(цел n);
	проц        сожми();
	цел         дайРазмест() const             { return ключ.дайРазмест(); }

	Вектор<T>        подбериКлючи()              { Вектор<T> r = пикуй(ключ); очисть(); return r; }
	const Вектор<T>& дайКлючи() const         { return ключ; }

	проц     удали(const цел *sorted_list, цел count);
	проц     удали(const Вектор<цел>& sorted_list)         { удали(sorted_list, sorted_list.дайСчёт()); }
	template <typename Предикат> проц удалиЕсли(Предикат p)          { удали(найдиВсеи(ключ, p)); }
	
	Индекс()                                                 {}
	Индекс(Индекс&& s) : ключ(пикуй(s.ключ))                     { ИндексОбщее::подбери(s); }
	Индекс(const Индекс& s, цел) : ключ(s.ключ, 0)              { ReallocHash(0); ИндексОбщее::копируй(s, ключ.дайСчёт()); } // TODO: Unlinked!
	explicit Индекс(Вектор<T>&& s) : ключ(пикуй(s))            { фиксируйХэш(); }
	Индекс(const Вектор<T>& s, цел) : ключ(s, 0)              { фиксируйХэш(); }

	Индекс& operator=(Вектор<T>&& x)                         { ключ = пикуй(x); фиксируйХэш(); return *this; }
	Индекс& operator=(Индекс<T>&& x)                          { ключ = пикуй(x.ключ); ИндексОбщее::подбери(x); return *this; }

	Индекс(std::initializer_list<T> init) : ключ(init)        { фиксируйХэш(); }

	проц     сериализуй(Поток& s);
	проц     вРяр(РярВВ& xio, кткст0 itemtag = "ключ");
	проц     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

// Standard container interface
	typedef КонстОбходчикУ<Вектор<T>> КонстОбходчик;
	КонстОбходчик begin() const                             { return ключ.begin(); }
	КонстОбходчик end() const                               { return ключ.end(); }

	friend проц разверни(Индекс& a, Индекс& b)                    { a.ИндексОбщее::разверни(b); РНЦП::разверни(a.ключ, b.ключ); }

// deprecated:
#ifdef DEPRECATED
	T&       вставь(цел i, const T& k)                      { ключ.вставь(i, k); фиксируйХэш(); return ключ[i]; }
	проц     удали(цел i, цел count)                       { ключ.удали(i, count); фиксируйХэш(); }
	проц     удали(цел i)                                  { удали(i, 1); }
	цел      удалиКлюч(const T& k)                          { цел i = найди(k); if(i >= 0) удали(i); return i; }

	unsigned дайХэш(цел i) const                           { return hash[i].hash; }

	Индекс& operator<<=(const Вектор<T>& s)                  { *this = клонируй(s); return *this; }
	typedef T                ТипЗнач;
	typedef Вектор<T>        КонтейнерЗначений;
	КонстОбходчик  дайОбх(цел pos) const                   { return ключ.дайОбх(pos); }

	проц     ClearIndex()                    {}
	проц     реиндексируй(цел n)                  {}
	проц     реиндексируй()                       {}

	typedef T             value_type;
	typedef КонстОбходчик const_iterator;
	typedef const T&      const_reference;
	typedef цел           size_type;
	typedef цел           difference_type;
	const_iterator        старт() const          { return begin(); }
	const_iterator        стоп() const            { return end(); }
	проц                  clear()                { очисть(); }
	size_type             size()                 { return дайСчёт(); }
	бул                  empty() const          { return пустой(); }
#endif

#ifdef _ОТЛАДКА
	Ткст дамп() const;
#endif
};
