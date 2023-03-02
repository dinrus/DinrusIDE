struct ИндексОбщее {
	enum { HIBIT = 0x80000000 };

	struct Хэш : Движимое<Хэш> {
		int   next; // also link for unlinked items...
		dword hash;
		int   prev;
	};
	
	int         *map;
	Хэш        *hash;
	dword        mask;
	int          unlinked;

	
	static int empty[1];

	static dword Smear(hash_t h)          { return FoldHash(h) | HIBIT; }

	void линкуй(int& m, Хэш& hh, int ii);
	void линкуй(int ii, dword sh);
	void уд(int& m, Хэш& hh, int ii);
	void Ins(int& m, Хэш& hh, int ii);

	void сделайМап(int count);
	void ремапируй(int count);
	void реиндексируй(int count);
	void нарастиМап(int count);
	void освободиМап();
	void освободи();

	void уст(int ii, dword h);
	
	Вектор<int> дайОтлинкованно() const;
	
	void очисть();
	void обрежь(int n, int count);
	void смети(int n);
	void резервируй(int n);
	void сожми();
	void AdjustMap(int count, int alloc);
	
	void копируй(const ИндексОбщее& b, int count);
	void подбери(ИндексОбщее& b);
	void разверни(ИндексОбщее& b);
	
	ИндексОбщее();
	~ИндексОбщее();
};

template <class K, class T, class V> class АМап;

template <class T>
class Индекс : ОпцияДвижимогоИГлубКопии<Индекс<T>>, ИндексОбщее {
	Вектор<T> ключ;

	static dword Smear(const T& k)   { return ИндексОбщее::Smear(дайХэшЗнач(k)); }

	int  найдиОт(int i, dword sh, const T& k, int end) const;
	int  FindBack(int i, dword sh, const T& k, int end) const;


	void ReallocHash(int n);
	template <typename U> void нарастиДобавь(U&& k, dword sh);
	template <typename U> void AddS(int& m, U&& k, dword sh);
	template <typename U> void AddS(U&& k, dword sh);

	template <class OP, class U> int найдиДобавь(U&& k, OP add_op);
	template <class U> int FindPut0(U&& k);

	template <typename U> int Put0(U&& k, dword sh);
	template <typename U> void уст0(int i, U&& k);

	template <typename, typename, typename> friend class АМап;
	
	void        фиксируйХэш(bool makemap = true);

public:
	void        добавь(const T& k)             { AddS(k, Smear(k)); }
	void        добавь(T&& k)                  { AddS(pick(k), Smear(k)); }
	Индекс&      operator<<(const T& x)      { добавь(x); return *this; }
	Индекс&      operator<<(T&& x)           { добавь(pick(x)); return *this; }

	int         найди(const T& k) const;
	int         найдиСледщ(int i) const;
	int         найдиПоследн(const T& k) const;
	int         найдиПредш(int i) const;
	
	int         найдиДобавь(const T& k)         { return найдиДобавь(k, []{}); }
	int         найдиДобавь(T&& k)              { return найдиДобавь(pick(k), []{}); }

	int         помести(const T& k)             { return Put0(k, Smear(k)); }
	int         помести(T&& k)                  { return Put0(pick(k), Smear(k)); }
	int         найдиПомести(const T& k)         { return FindPut0(k); }
	int         найдиПомести(T&& k)              { return FindPut0(pick(k)); }

	void        отлинкуй(int i);
	int         отлинкуйКлюч(const T& k);
	bool        отлинкован(int i) const      { return hash[i].hash == 0; }
	bool        HasUnlinked() const          { return unlinked >= 0; }
	Вектор<int> дайОтлинкованно() const          { return ИндексОбщее::дайОтлинкованно(); }

	void        смети();

	void        уст(int i, const T& k)       { уст0(i, k); }
	void        уст(int i, T&& k)            { уст0(i, pick(k)); }

	const T&    operator[](int i) const      { return ключ[i]; }
	int         дайСчёт() const             { return ключ.дайСчёт(); }
	bool        пустой() const              { return ключ.пустой(); }
	
	void        очисть()                      { ключ.очисть(); ИндексОбщее::очисть(); }

	void        обрежь(int n = 0)              { ИндексОбщее::обрежь(n, дайСчёт()); ключ.обрежь(n); }
	void        сбрось(int n = 1)              { обрежь(дайСчёт() - 1); }
	const T&    верх() const                  { return ключ.верх(); }
	T           вынь()                        { T x = pick(верх()); сбрось(); return x; }

	void        резервируй(int n);
	void        сожми();
	int         дайРазмест() const             { return ключ.дайРазмест(); }

	Вектор<T>        подбериКлючи()              { Вектор<T> r = pick(ключ); очисть(); return r; }
	const Вектор<T>& дайКлючи() const         { return ключ; }

	void     удали(const int *sorted_list, int count);
	void     удали(const Вектор<int>& sorted_list)         { удали(sorted_list, sorted_list.дайСчёт()); }
	template <typename Предикат> void удалиЕсли(Предикат p)          { удали(найдиВсеи(ключ, p)); }
	
	Индекс()                                                 {}
	Индекс(Индекс&& s) : ключ(pick(s.ключ))                     { ИндексОбщее::подбери(s); }
	Индекс(const Индекс& s, int) : ключ(s.ключ, 0)              { ReallocHash(0); ИндексОбщее::копируй(s, ключ.дайСчёт()); } // TODO: Unlinked!
	explicit Индекс(Вектор<T>&& s) : ключ(pick(s))            { фиксируйХэш(); }
	Индекс(const Вектор<T>& s, int) : ключ(s, 0)              { фиксируйХэш(); }

	Индекс& operator=(Вектор<T>&& x)                         { ключ = pick(x); фиксируйХэш(); return *this; }
	Индекс& operator=(Индекс<T>&& x)                          { ключ = pick(x.ключ); ИндексОбщее::подбери(x); return *this; }

	Индекс(std::initializer_list<T> init) : ключ(init)        { фиксируйХэш(); }

	void     сериализуй(Поток& s);
	void     вРяр(РярВВ& xio, const char *itemtag = "ключ");
	void     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
	template <class B> bool operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
	template <class B> int  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> bool operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return сравни(x) > 0; }

// Standard container interface
	typedef КонстОбходчикУ<Вектор<T>> КонстОбходчик;
	КонстОбходчик begin() const                             { return ключ.begin(); }
	КонстОбходчик end() const                               { return ключ.end(); }

	friend void разверни(Индекс& a, Индекс& b)                    { a.ИндексОбщее::разверни(b); РНЦП::разверни(a.ключ, b.ключ); }

// deprecated:
#ifdef DEPRECATED
	T&       вставь(int i, const T& k)                      { ключ.вставь(i, k); фиксируйХэш(); return ключ[i]; }
	void     удали(int i, int count)                       { ключ.удали(i, count); фиксируйХэш(); }
	void     удали(int i)                                  { удали(i, 1); }
	int      удалиКлюч(const T& k)                          { int i = найди(k); if(i >= 0) удали(i); return i; }

	unsigned дайХэш(int i) const                           { return hash[i].hash; }

	Индекс& operator<<=(const Вектор<T>& s)                  { *this = clone(s); return *this; }
	typedef T                ТипЗнач;
	typedef Вектор<T>        КонтейнерЗначений;
	КонстОбходчик  дайОбх(int pos) const                   { return ключ.дайОбх(pos); }

	void     ClearIndex()                    {}
	void     реиндексируй(int n)                  {}
	void     реиндексируй()                       {}

	typedef T             value_type;
	typedef КонстОбходчик const_iterator;
	typedef const T&      const_reference;
	typedef int           size_type;
	typedef int           difference_type;
	const_iterator        старт() const          { return begin(); }
	const_iterator        стоп() const            { return end(); }
	void                  clear()                { очисть(); }
	size_type             size()                 { return дайСчёт(); }
	bool                  empty() const          { return пустой(); }
#endif

#ifdef _ОТЛАДКА
	Ткст дамп() const;
#endif
};
