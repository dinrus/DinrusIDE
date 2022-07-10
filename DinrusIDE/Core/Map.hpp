// --------------------

template <class K, class T, class V>
template <class KK>
int АМап<K, T, V>::FindAdd_(KK&& k) {
	return ключ.найдиДобавь(std::forward<KK>(k), [&] { значение.добавь(); });
}

template <class K, class T, class V>
template <class KK, class TT>
int АМап<K, T, V>::FindAdd_(KK&& k, TT&& x) {
	return ключ.найдиДобавь(std::forward<KK>(k), [&] { значение.добавь(std::forward<TT>(x)); });
}


template <class K, class T, class V>
template <class KK>
T&  АМап<K, T, V>::Put_(KK&& k)
{
	int i = ключ.помести(std::forward<KK>(k));
	if(i < значение.дайСчёт()) {
		разрушь(&значение[i], &значение[i] + 1);
		строй(&значение[i], &значение[i] + 1);
		return значение[i];
	}
	else {
		ПРОВЕРЬ(i == значение.дайСчёт());
		return значение.добавь();
	}
}

template <class K, class T, class V>
template <class KK, class TT>
int АМап<K, T, V>::Put_(KK&& k, TT&& x)
{
	int i = ключ.помести(std::forward<KK>(k));
	if(i < значение.дайСчёт())
		значение[i] = std::forward<TT>(x);
	else {
		ПРОВЕРЬ(i == значение.дайСчёт());
		значение.добавь(std::forward<TT>(x));
	}
	return i;
}

template <class K, class T, class V>
template <class KK>
int АМап<K, T, V>::PutDefault_(KK&& k)
{
	int i = ключ.помести(std::forward<KK>(k));
	if(i >= значение.дайСчёт()) {
		ПРОВЕРЬ(i == значение.дайСчёт());
		значение.добавь();
	}
	else {
		разрушь(&значение[i], &значение[i] + 1);
		строй(&значение[i], &значение[i] + 1);
	}
	return i;
}

template <class K, class T, class V>
template <class KK>
int АМап<K, T, V>::FindPut_(KK&& k)
{
	int i = ключ.найдиПомести(std::forward<KK>(k));
	if(i >= значение.дайСчёт())
		значение.добавь();
	return i;
}

template <class K, class T, class V>
template <class KK, class TT>
int АМап<K, T, V>::FindPut_(KK&& k, TT&& init)
{
	int i = ключ.найдиПомести(std::forward<KK>(k));
	if(i >= значение.дайСчёт())
		значение.добавь(std::forward<TT>(init));
	return i;
}

template <class K, class T, class V>
template <class KK>
T&  АМап<K, T, V>::GetAdd_(KK&& k)
{
	return значение[найдиДобавь(std::forward<KK>(k))];
}

template <class K, class T, class V>
template <class KK, class TT>
T&  АМап<K, T, V>::GetAdd_(KK&& k, TT&& x)
{
	return значение[найдиДобавь(std::forward<KK>(k), std::forward<TT>(x))];
}

template <class K, class T, class V>
template <class KK>
T&  АМап<K, T, V>::GetPut_(KK&& k)
{
	int i = ключ.найдиПомести(std::forward<KK>(k));
	return i < значение.дайСчёт() ? значение[i] : значение.добавь();
}

template <class K, class T, class V>
template <class KK, class TT>
T&  АМап<K, T, V>::GetPut_(KK&& k, TT&& x)
{
	int i = ключ.найдиПомести(std::forward<KK>(k));
	return i < значение.дайСчёт() ? значение[i] : значение.добавь(std::forward<TT>(x));
}

template <class K, class T, class V>
void АМап<K, T, V>::сериализуй(Поток& s) {
	int версия = 0;
	s / версия % ключ % значение;
}

template <class K, class T, class V>
void АМап<K, T, V>::вРяр(РярВВ& xio)
{
	мапВРяр<K, T, АМап<K, T, V> >(xio, "ключ", "значение", *this);
}

template <class K, class T, class V>
void АМап<K, T, V>::вДжейсон(ДжейсонВВ& jio)
{
	JsonizeMap<АМап<K, T, V>, K, T>(jio, *this, "ключ", "значение");
}

template <class K, class T, class V>
Ткст АМап<K, T, V>::вТкст() const
{
	Ткст r;
	r = "{";
	for(int i = 0; i < дайСчёт(); i++) {
		if(i)
			r << ", ";
		if(отлинкован(i))
			r << "UNLINKED ";
		r << дайКлюч(i) << ": " << (*this)[i];
	}
	r << '}';
	return r;
}

template <class K, class T, class V>
int АМап<K, T, V>::удалиКлюч(const K& k)
{
	Вектор<int> rk;
	int q = найди(k);
	while(q >= 0) {
		rk.добавь(q);
		q = найдиСледщ(q);
	}
	удали(rk);
	return rk.дайСчёт();
}

template <class K, class T, class V>
void АМап<K, T, V>::смети()
{
	значение.удалиЕсли([&](int i) { return ключ.отлинкован(i); });
	ключ.смети();
}

#ifdef РНЦП
template <class K, class T, class V, class Less>
void ФиксАМап<K, T, V, Less>::сериализуй(Поток& s) {
	s % ключ % значение;
}

template <class K, class T, class V, class Less>
void ФиксАМап<K, T, V, Less>::вРяр(РярВВ& xio)
{
	XmlizeSortedMap<K, T, ФиксАМап<K, T, V, Less> >(xio, "ключ", "значение", *this);
}

template <class K, class T, class V, class Less>
void ФиксАМап<K, T, V, Less>::вДжейсон(ДжейсонВВ& jio)
{
	JsonizeSortedMap<ФиксАМап<K, T, V, Less>, K, T>(jio, *this, "ключ", "значение");
}

template <class K, class T, class V, class Less>
Ткст ФиксАМап<K, T, V, Less>::вТкст() const
{
	Ткст r;
	r = "{";
	for(int i = 0; i < дайСчёт(); i++) {
		if(i)
			r << ", ";
		r << дайКлюч(i) << ": " << (*this)[i];
	}
	r << '}';
	return r;
}

#endif
