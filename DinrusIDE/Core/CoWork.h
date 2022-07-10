class СоРабота : БезКопий {
	struct МРабота : Движимое<МРабота>, Линк<МРабота, 2> {
		Функция<void ()> фн;
		СоРабота           *work = NULL;
		bool              looper = false;
	};
	
	enum { SCHEDULED_MAX = 2048 };

public:
	struct Пул {
		МРабота             *free;
		Линк<МРабота, 2>     jobs;
		МРабота              slot[SCHEDULED_MAX];
		int               waiting_threads;
		Массив<Нить>     threads;
		bool              quit;

		Стопор             lock;
		ПеременнаяУсловия waitforjob;
		
		void              освободи(МРабота& m);
		void              работай(МРабота& m);
		void              суньРаботу(Функция<void ()>&& фн, СоРабота *work, bool looper = false);

		void              иницНити(int nthreads);
		void              покиньНити();

		Пул();
		~Пул();

		static thread_local bool    финблок;

		bool работай();
		static void пускНити(int tno);
	};
	
	friend struct Пул;

	static Пул& дайПул();

	static thread_local int индекс_трудяги;
	static thread_local СоРабота *текущ;

	ПеременнаяУсловия  waitforfinish;
	Линк<МРабота, 2>      jobs; // global stack and СоРабота stack as double-linked lists
	int                todo;
	bool               canceled;
	std::exception_ptr exc = nullptr; // workaround for sanitizer bug(?)
	Функция<void ()>  looper_fn;
	int                looper_count;

	void делай0(Функция<void ()>&& фн, bool looper);

	void отмени0();
	void финишируй0();
	
	Атомар             индекс;

// experimental pipe support
	Стопор stepmutex;
	Массив<БиВектор<Функция<void ()>>> step;
	Вектор<bool> steprunning;
	
public:
	static bool пробуйПлан(Функция<void ()>&& фн);
	static bool пробуйПлан(const Функция<void ()>& фн)      { return пробуйПлан(clone(фн)); }
	static void планируй(Функция<void ()>&& фн);
	static void планируй(const Функция<void ()>& фн)         { return планируй(clone(фн)); }

	void     делай(Функция<void ()>&& фн)                       { делай0(pick(фн), false); }
	void     делай(const Функция<void ()>& фн)                  { делай(clone(фн)); }

	СоРабота&  operator&(const Функция<void ()>& фн)           { делай(фн); return *this; }
	СоРабота&  operator&(Функция<void ()>&& фн)                { делай(pick(фн)); return *this; }
	
	void     цикл(Функция<void ()>&& фн);
	void     цикл(const Функция<void ()>& фн)                { цикл(clone(фн)); }

	СоРабота&  operator*(const Функция<void ()>& фн)           { цикл(фн); return *this; }
	СоРабота&  operator*(Функция<void ()>&& фн)                { цикл(pick(фн)); return *this; }

	int      следщ()                                           { return ++индекс - 1; }

	int  дайСчётПланируемых() const                            { return todo; }
	void пайп(int stepi, Функция<void ()>&& lambda); // experimental

	static void финБлок();
	
	void отмена();
	static bool отменён();

	void финиш();
	
	bool финишировал();
	
	void переустанов();

	static bool трудяга_ли()                                    { return дайИндексТрудяги() >= 0; }
	static int  дайИндексТрудяги();
	static int  дайРазмерПула();
	static void устРазмерПула(int n);

	СоРабота();
	~СоРабота() noexcept(false);
};

struct CoWorkNX : СоРабота {
	~CoWorkNX() noexcept(true) {}
};

inline
void соДелай(Функция<void ()>&& фн)
{
	СоРабота co;
	co * фн;
}

inline
void соДелай_ОН(Функция<void ()>&& фн)
{
	фн();
}

inline
void соДелай(bool co, Функция<void ()>&& фн)
{
	if(co)
		соДелай(pick(фн));
	else
		соДелай_ОН(pick(фн));
}

template <typename Фн>
void соФор(int n, Фн iterator)
{
	std::atomic<int> ii(0);
	соДелай([&] {
		for(int i = ii++; i < n; i = ii++)
			iterator(i);
	});
}

template <typename Фн>
void соФор_ОН(int n, Фн iterator)
{
	for(int i = 0; i < n; i++)
		iterator(i);
}

template <typename Фн>
void соФор(bool co, int n, Фн iterator)
{
	if(co)
		соФор(n, iterator);
	else
		соФор_ОН(n, iterator);
}

template <class T>
class соРесурсыТрудяги {
	int          workercount;
	Буфер<T>    res;
	
public:
	int дайСчёт() const  { return workercount + 1; }
	T& operator[](int i)  { return res[i]; }

	T& дай()              { int i = СоРабота::дайИндексТрудяги(); return res[i < 0 ? workercount : i]; }
	T& operator~()        { return дай(); }
	
	T *begin()            { return ~res; }
	T *end()              { return ~res + дайСчёт(); }
	
	соРесурсыТрудяги()   { workercount = СоРабота::дайРазмерПула(); res.размести(дайСчёт()); }

	соРесурсыТрудяги(Событие<T&> initializer) : соРесурсыТрудяги() {
		for(int i = 0; i < дайСчёт(); i++)
			initializer(res[i]);
	}
};

template <class Ret>
class АсинхРабота {
	template <class Ret2>
	struct Imp {
		СоРабота co;
		Ret2   ret;
	
		template<class Функция, class... Арги>
		void        делай(Функция&& f, Арги&&... арги) { co.делай([=]() { ret = f(арги...); }); }
		const Ret2& дай()                            { return ret; }
		Ret2        подбери()                           { return pick(ret); }
	};

	struct ImpVoid {
		СоРабота co;
	
		template<class Функция, class... Арги>
		void        делай(Функция&& f, Арги&&... арги) { co.делай([=]() { f(арги...); }); }
		void        дай()                            {}
		void        подбери()                           {}
	};
	
	using ImpType = typename std::conditional<std::is_void<Ret>::значение, ImpVoid, Imp<Ret>>::тип;
	
	Один<ImpType> imp;
	
public:
	template< class Функция, class... Арги>
	void  делай(Функция&& f, Арги&&... арги)          { imp.создай().делай(f, арги...); }

	void        отмена()                            { if(imp) imp->co.отмена(); }
	static bool отменён()                        { return СоРабота::отменён(); }
	bool        финишировал()                        { return imp && imp->co.финишировал(); }
	Ret         дай()                               { ПРОВЕРЬ(imp); imp->co.финиш(); return imp->дай(); }
	Ret         operator~()                         { return дай(); }
	Ret         подбери()                              { ПРОВЕРЬ(imp); imp->co.финиш(); return imp->подбери(); }
	
	АсинхРабота& operator=(АсинхРабота&&) = default;
	АсинхРабота(АсинхРабота&&) = default;

	АсинхРабота()                                     {}
	~АсинхРабота()                                    { if(imp) imp->co.отмена(); }
};

template< class Функция, class... Арги>
АсинхРабота<
	typename std::result_of<
		typename std::decay<Функция>::тип
			(typename std::decay<Арги>::тип...)
	>::тип
>
Async(Функция&& f, Арги&&... арги)
{
	АсинхРабота<
		typename std::result_of<
			typename std::decay<Функция>::тип
				(typename std::decay<Арги>::тип...)
		>::тип
	> h;
	h.делай(f, арги...);
	return pick(h);
}
