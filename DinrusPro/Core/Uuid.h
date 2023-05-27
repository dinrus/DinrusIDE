struct Uuid : ПрисвойНомТипаЗнач<Uuid, 50, Движ<Uuid> > {
	бдол v[2];

	проц сериализуй(Поток& s);
	проц вРяр(РярВВ& xio);
	проц вДжейсон(ДжейсонВВ& jio);
	бул экзПусто_ли() const   { return (v[0] | v[1]) == 0; }
	проц устПусто()                { v[0] = v[1] = 0; }

	operator Значение() const        { return богатыйВЗнач(*this); }
	Uuid(const Значение& q)          { *this = q.дай<Uuid>(); }
	Uuid(const Обнул&)           { устПусто(); }
	Uuid()                        {}

	т_хэш   дайХэшЗнач() const { return комбинируйХэш(v[0], v[1]); }
	Ткст   вТкст() const;
	Ткст   ToStringWithDashes() const;
	
	проц     нов();

	static Uuid создай()          { Uuid uuid; uuid.нов(); return uuid; }
};

Ткст фмт(const Uuid& ид);
Ткст FormatWithDashes(const Uuid& ид);
Uuid   ScanUuid(кткст0 s);

inline бул  operator==(const Uuid& u, const Uuid& w) {
	return ((u.v[0] ^ w.v[0]) | (u.v[1] ^ w.v[1])) == 0;
}

inline бул  operator!=(const Uuid& u, const Uuid& w) {
	return !(u == w);
}

template<>
inline Ткст какТкст(const Uuid& ид) { return фмт(ид); }

ГенЗначения& UuidValueGen();
