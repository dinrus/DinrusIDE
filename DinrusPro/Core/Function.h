enum CNULLer { CNULL }; // Deprecated, use plain Null

template<typename Рез, typename... ТипыАрг>
class Функция<Рез(ТипыАрг...)> : Движ<Функция<Рез(ТипыАрг...)>> {
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

	ОбёрткаОснова *укз;
	
	static проц освободи(ОбёрткаОснова *укз) {
		if(укз && атомнДек(укз->refcount) == 0)
			delete укз;
	}
	
	проц копируй(const Функция& a) {
		укз = a.укз;
		if(укз)
			атомнИнк(укз->refcount);
	}
	
	проц подбери(Функция&& ист) {
		укз = ист.укз;
		ист.укз = NULL;
	}

public:
	Функция()                                 { укз = NULL; }
	Функция(CNULLer)                          { укз = NULL; }
	Функция(const Обнул&)                    { укз = NULL; }
	
	template <class F> Функция(F фн)          { укз = new Обёртка<F>(pick(фн)); }
	
	Функция(const Функция& ист)              { копируй(ист); }
	Функция& operator=(const Функция& ист)   { auto b = укз; копируй(ист); освободи(b); return *this; }

	Функция(Функция&& ист)                   { подбери(pick(ист)); }
	Функция& operator=(Функция&& ист)        { if(&ист != this) { освободи(укз); укз = ист.укз; ист.укз = NULL; } return *this; }
	
	Функция прокси() const                     { return [=] (ТипыАрг... арги) { return (*this)(арги...); }; }

	template <class F>
	Функция& operator<<(F фн)                 { if(!укз) { подбери(pick(фн)); return *this; }
	                                             ОбёрткаОснова *b = укз; укз = new Обёртка2<F>(*this, pick(фн)); освободи(b); return *this; }

	Функция& operator<<(const Функция& фн)   { if(!укз) { копируй(фн); return *this; }
	                                             ОбёрткаОснова *b = укз; укз = new Обёртка2<Функция>(*this, фн); освободи(b); return *this; }

	Функция& operator<<(Функция&& фн)        { if(!укз) { подбери(pick(фн)); return *this; }
	                                             ОбёрткаОснова *b = укз; укз = new Обёртка2<Функция>(*this, pick(фн)); освободи(b); return *this; }

	Рез operator()(ТипыАрг... арги) const     { return укз ? укз->выполни(арги...) : Рез(); }
	
	operator бул() const                      { return укз; }
	проц очисть()                               { освободи(укз); укз = NULL; }

	~Функция()                                { освободи(укз); }

	friend Функция прокси(const Функция& a)   { return a.прокси(); }
	friend проц разверни(Функция& a, Функция& b) { РНЦП::разверни(a.укз, b.укз); }
};

template <typename... ТипыАрг>
using Событие = Функция<проц (ТипыАрг...)>;

template <typename... ТипыАрг>
using Врата = Функция<бул (ТипыАрг...)>;

template <class Ук, class Класс, class Рез, class... ТипыАрг>
Функция<Рез (ТипыАрг...)> памФн(Ук object, Рез (Класс::*method)(ТипыАрг...))
{
	return [=](ТипыАрг... арги) { return (object->*method)(арги...); };
}

#define THISFN(x)   памФн(this, &ИМЯ_КЛАССА::x)
