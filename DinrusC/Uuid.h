struct Uuid : ПрисвойНомТипаЗнач<Uuid, 50, Движимое<Uuid> > {
	uint64 v[2];

	void сериализуй(Поток& s);
	void вРяр(РярВВ& xio);
	void вДжейсон(ДжейсонВВ& jio);
	bool экзПусто_ли() const   { return (v[0] | v[1]) == 0; }
	void устПусто()                { v[0] = v[1] = 0; }

	operator Значение() const        { return богатыйВЗнач(*this); }
	Uuid(const Значение& q)          { *this = q.дай<Uuid>(); }
	Uuid(const Обнул&)           { устПусто(); }
	Uuid()                        {}

	hash_t   дайХэшЗнач() const { return комбинируйХэш(v[0], v[1]); }
	Ткст   вТкст() const;
	Ткст   ToStringWithDashes() const;
	
	void     нов();

	static Uuid создай()          { Uuid uuid; uuid.нов(); return uuid; }
};

Ткст фмт(const Uuid& ид);
Ткст FormatWithDashes(const Uuid& ид);
Uuid   ScanUuid(const char *s);

inline bool  operator==(const Uuid& u, const Uuid& w) {
	return ((u.v[0] ^ w.v[0]) | (u.v[1] ^ w.v[1])) == 0;
}

inline bool  operator!=(const Uuid& u, const Uuid& w) {
	return !(u == w);
}

template<>
inline Ткст какТкст(const Uuid& ид) { return фмт(ид); }

ГенЗначения& UuidValueGen();
