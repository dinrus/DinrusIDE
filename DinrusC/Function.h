enum CNULLer { CNULL }; // Deprecated, use plain Null

template<typename Рез, typename... ТипыАрг>
class Функция<Рез(ТипыАрг...)> : Движимое<Функция<Рез(ТипыАрг...)>> {
	struct ОбёрткаОснова {
		Атомар  refcount;

		virtual Рез выполни(ТипыАрг... арги) = 0;
		
		ОбёрткаОснова() { refcount = 1; }
		virtual ~ОбёрткаОснова() {}
	};

	template <class F>
	struct Обёртка : ОбёрткаОснова {
		F фн;
		virtual Рез выполни(ТипыАрг... арги) { return фн(арги...); }

		Обёртка(F&& фн) : фн(pick(фн)) {}
	};

	template <class F>
	struct Обёртка2 : ОбёрткаОснова {
		Функция l;
		F        фн;

		virtual Рез выполни(ТипыАрг... арги) { l(арги...); return фн(арги...); }

		Обёртка2(const Функция& l, F&& фн) : l(l), фн(pick(фн)) {}
		Обёртка2(const Функция& l, const F& фн) : l(l), фн(фн) {}
	};

	ОбёрткаОснова *ptr;
	
	static void освободи(ОбёрткаОснова *ptr) {
		if(ptr && атомнДек(ptr->refcount) == 0)
			delete ptr;
	}
	
	void копируй(const Функция& a) {
		ptr = a.ptr;
		if(ptr)
			атомнИнк(ptr->refcount);
	}
	
	void подбери(Функция&& ист) {
		ptr = ист.ptr;
		ист.ptr = NULL;
	}

public:
	Функция()                                 { ptr = NULL; }
	Функция(CNULLer)                          { ptr = NULL; }
	Функция(const Обнул&)                    { ptr = NULL; }
	
	template <class F> Функция(F фн)          { ptr = new Обёртка<F>(pick(фн)); }
	
	Функция(const Функция& ист)              { копируй(ист); }
	Функция& operator=(const Функция& ист)   { auto b = ptr; копируй(ист); освободи(b); return *this; }

	Функция(Функция&& ист)                   { подбери(pick(ист)); }
	Функция& operator=(Функция&& ист)        { if(&ист != this) { освободи(ptr); ptr = ист.ptr; ист.ptr = NULL; } return *this; }
	
	Функция прокси() const                     { return [=] (ТипыАрг... арги) { return (*this)(арги...); }; }

	template <class F>
	Функция& operator<<(F фн)                 { if(!ptr) { подбери(pick(фн)); return *this; }
	                                             ОбёрткаОснова *b = ptr; ptr = new Обёртка2<F>(*this, pick(фн)); освободи(b); return *this; }

	Функция& operator<<(const Функция& фн)   { if(!ptr) { копируй(фн); return *this; }
	                                             ОбёрткаОснова *b = ptr; ptr = new Обёртка2<Функция>(*this, фн); освободи(b); return *this; }

	Функция& operator<<(Функция&& фн)        { if(!ptr) { подбери(pick(фн)); return *this; }
	                                             ОбёрткаОснова *b = ptr; ptr = new Обёртка2<Функция>(*this, pick(фн)); освободи(b); return *this; }

	Рез operator()(ТипыАрг... арги) const     { return ptr ? ptr->выполни(арги...) : Рез(); }
	
	operator bool() const                      { return ptr; }
	void очисть()                               { освободи(ptr); ptr = NULL; }

	~Функция()                                { освободи(ptr); }

	friend Функция прокси(const Функция& a)   { return a.прокси(); }
	friend void разверни(Функция& a, Функция& b) { РНЦП::разверни(a.ptr, b.ptr); }
};

template <typename... ТипыАрг>
using Событие = Функция<void (ТипыАрг...)>;

template <typename... ТипыАрг>
using Врата = Функция<bool (ТипыАрг...)>;

template <class Ук, class Класс, class Рез, class... ТипыАрг>
Функция<Рез (ТипыАрг...)> памФн(Ук object, Рез (Класс::*method)(ТипыАрг...))
{
	return [=](ТипыАрг... арги) { return (object->*method)(арги...); };
}

#define THISFN(x)   памФн(this, &ИМЯ_КЛАССА::x)
