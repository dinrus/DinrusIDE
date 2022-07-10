inline
void МапЗнач::добавь(const Значение& ключ, const Значение& значение) {
	Данные& d = UnShare();
	d.ключ.добавь(ключ);
	d.значение.добавь(значение);
}

inline
int сравниСтдЗнач(const Значение& a, const Значение& b, const LanguageInfo& f)
{
	return ткст_ли(a) && ткст_ли(b) ? CompareStrings(a, b, f) : a.сравни(b);
}

inline
int сравниСтдЗнач(const Значение& a, const Значение& b, int язык)
{
	return сравниСтдЗнач(a, b, GetLanguageInfo(язык));
}

inline
int сравниСтдЗнач(const Значение& a, const Значение& b)
{
	return сравниСтдЗнач(a, b, GetLanguageInfo());
}

inline
int сравниДескСтдЗнач(const Значение& a, const Значение& b, const LanguageInfo& f)
{
	return -сравниСтдЗнач(a, b, f);
}

inline
int сравниДескСтдЗнач(const Значение& a, const Значение& b, int язык)
{
	return -сравниСтдЗнач(a, b, язык);
}

inline
int сравниДескСтдЗнач(const Значение& a, const Значение& b)
{
	return -сравниСтдЗнач(a, b);
}


#ifdef DEPRECATED
template <class T>
struct СыройРеф : public РефМенеджер {
	virtual void  устЗначение(void *p, const Значение& v)       { *(T *) p = СыроеЗначение<T>::извлеки(v); }
	virtual Значение дайЗначение(const void *p)                 { return СыроеЗначение<T>(*(const T *) p); }
	virtual int   дайТип()                               { return дайНомТипаЗнач<T>(); }
	virtual ~СыройРеф() {}
};

template <class T>
Реф сыройКакРеф(T& x) {
	return Реф(&x, &Single< СыройРеф<T> >());
}

template <class T>
struct БогатыйРеф : public СыройРеф<T> {
	virtual Значение дайЗначение(const void *p)                 { return богатыйВЗнач(*(T *) p); }
	virtual bool  пусто_ли(const void *p)                   { return РНЦП::пусто_ли(*(T *) p); }
	virtual void  устЗначение(void *p, const Значение& v)       { *(T *) p = T(v); }
	virtual void  устПусто(void *p)                        { РНЦП::устПусто(*(T *)p); }
};

template <class T>
Реф богатыйКакРеф(T& x) {
	return Реф(&x, &Single< БогатыйРеф<T> >());
}
#endif