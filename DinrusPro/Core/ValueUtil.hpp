inline
проц МапЗнач::добавь(const Значение& ключ, const Значение& значение) {
	Данные& d = разшарь();
	d.ключ.добавь(ключ);
	d.значение.добавь(значение);
}

inline
цел сравниСтдЗнач(const Значение& a, const Значение& b, const LanguageInfo& f)
{
	return ткст_ли(a) && ткст_ли(b) ? CompareStrings(a, b, f) : a.сравни(b);
}

inline
цел сравниСтдЗнач(const Значение& a, const Значение& b, цел язык)
{
	return сравниСтдЗнач(a, b, GetLanguageInfo(язык));
}

inline
цел сравниСтдЗнач(const Значение& a, const Значение& b)
{
	return сравниСтдЗнач(a, b, GetLanguageInfo());
}

inline
цел сравниДескСтдЗнач(const Значение& a, const Значение& b, const LanguageInfo& f)
{
	return -сравниСтдЗнач(a, b, f);
}

inline
цел сравниДескСтдЗнач(const Значение& a, const Значение& b, цел язык)
{
	return -сравниСтдЗнач(a, b, язык);
}

inline
цел сравниДескСтдЗнач(const Значение& a, const Значение& b)
{
	return -сравниСтдЗнач(a, b);
}


#ifdef DEPRECATED
template <class T>
struct СыройРеф : public РефМенеджер {
	virtual проц  устЗначение(ук p, const Значение& v)       { *(T *) p = СыроеЗначение<T>::извлеки(v); }
	virtual Значение дайЗначение(const ук p)                 { return СыроеЗначение<T>(*(const T *) p); }
	virtual цел   дайТип()                               { return дайНомТипаЗнач<T>(); }
	virtual ~СыройРеф() {}
};

template <class T>
Реф сыройКакРеф(T& x) {
	return Реф(&x, &Single< СыройРеф<T> >());
}

template <class T>
struct БогатыйРеф : public СыройРеф<T> {
	virtual Значение дайЗначение(const ук p)                 { return богатыйВЗнач(*(T *) p); }
	virtual бул  пусто_ли(const ук p)                   { return РНЦП::пусто_ли(*(T *) p); }
	virtual проц  устЗначение(ук p, const Значение& v)       { *(T *) p = T(v); }
	virtual проц  устПусто(ук p)                        { РНЦП::устПусто(*(T *)p); }
};

template <class T>
Реф богатыйКакРеф(T& x) {
	return Реф(&x, &Single< БогатыйРеф<T> >());
}
#endif