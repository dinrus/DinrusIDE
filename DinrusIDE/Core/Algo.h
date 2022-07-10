template <class T>
inline int зн(T a) { return a > 0 ? 1 : a < 0 ? -1 : 0; }

template <class T>
inline T tabs(T a) { return (a >= 0 ? a : -a); }

// there is a general mess about абс, this is not ideal but best we can do now... as long as we
// do not want абс(1.1) == 1 surprise in GCC
template <class T> inline T абс(T a) { return (a >= 0 ? a : -a); }

template <class T>
inline int сравни(const T& a, const T& b) { return a > b ? 1 : a < b ? -1 : 0; }

template <class Диапазон>
void реверс(Диапазон&& r)
{
	auto start = r.begin();
	auto end = r.end();
	if(start != end && --end != start)
		do
			IterSwap(start, end);
		while(++start != end && --end != start);
}

template <class Диапазон, class Накопитель>
void накопи(const Диапазон& r, Накопитель& a)
{
	for(const auto& e : r)
		a(e);
}

template <class Диапазон>
типЗначУ<Диапазон> сумма(const Диапазон& r, const типЗначУ<Диапазон>& zero)
{
	типЗначУ<Диапазон> sum = zero;
	for(const auto& e : r)
		sum += e;
	return sum;
}

template <class T>
типЗначУ<T> сумма(const T& c)
{
	return сумма(c, (типЗначУ<T>)0);
}

template <class Диапазон, class V>
int счёт(const Диапазон& r, const V& val)
{
	int count = 0;
	for(const auto& x : r)
		if(x == val)
			count++;
	return count;
}

template <class Диапазон, class Предикат>
int счётЕсли(const Диапазон& r, const Предикат& p)
{
	int count = 0;
	for(const auto& x : r)
		if(p(x))
			count++;
	return count;
}

template <class Диапазон, class Предикат>
int найдиЛучший(const Диапазон& r, const Предикат& pred)
{
	int count = r.дайСчёт();
	if(count == 0)
		return -1;
	auto *m = &r[0];
	int mi = 0;
	for(int i = 1; i < count; i++)
		if(pred(r[i], *m)) {
			m = &r[i];
			mi = i;
		}
	return mi;
}

template <class Диапазон>
int найдиМин(const Диапазон& r)
{
	return найдиЛучший(r, std::less<типЗначУ<Диапазон>>());
}

template <class Диапазон>
const типЗначУ<Диапазон>& мин(const Диапазон& r)
{
	return r[найдиМин(r)];
}

template <class Диапазон>
const типЗначУ<Диапазон>& мин(const Диапазон& r, const типЗначУ<Диапазон>& опр)
{
	int q = найдиМин(r);
	return q < 0 ? опр : r[q];
}


template <class Диапазон>
int найдиМакс(const Диапазон& r)
{
	return найдиЛучший(r, std::greater<типЗначУ<Диапазон>>());
}

template <class Диапазон>
const типЗначУ<Диапазон>& макс(const Диапазон& r)
{
	return r[найдиМакс(r)];
}

template <class Диапазон>
const типЗначУ<Диапазон>& макс(const Диапазон& r, const типЗначУ<Диапазон>& опр)
{
	int q = найдиМакс(r);
	return q < 0 ? опр : r[q];
}

template <class Диапазон1, class Диапазон2>
bool диапазоныРавны(const Диапазон1& a, const Диапазон2& b)
{
	if(a.дайСчёт() != b.дайСчёт())
		return false;
	for(int i = 0; i < a.дайСчёт(); i++)
		if(!(a[i] == b[i]))
			return false;
	return true;
}

template <class Диапазон1, class Диапазон2, class Сравн>
int сравниДиапазоны(const Диапазон1& a, const Диапазон2& b, Сравн сравни)
{
	int n = min(a.дайСчёт(), b.дайСчёт());
	for(int i = 0; i < n; i++) {
		int q = сравни(a[i], b[i]);
		if(q)
			return q;
	}
	return сравни(a.дайСчёт(), b.дайСчёт());
}

template <class Диапазон1, class Диапазон2>
int сравниДиапазоны(const Диапазон1& a, const Диапазон2& b)
{
	int n = min(a.дайСчёт(), b.дайСчёт());
	for(int i = 0; i < n; i++) {
		int q = сравниЗнак(a[i], b[i]);
		if(q)
			return q;
	}
	return сравниЗнак(a.дайСчёт(), b.дайСчёт());
}

template <class Диапазон, class C>
int найдиСовпадение(const Диапазон& r, const C& match, int from = 0)
{
	for(int i = from; i < r.дайСчёт(); i++)
		if(match(r[i])) return i;
	return -1;
}

template <class Диапазон, class V>
int найдиИндекс(const Диапазон& r, const V& значение, int from = 0)
{
	for(int i = from; i < r.дайСчёт(); i++)
		if(r[i] == значение) return i;
	return -1;
}

template <class Диапазон, class Предикат>
Вектор<int> найдиВсе(const Диапазон& r, Предикат match, int from = 0)
{
	Вектор<int> ndx;
	for(int i = from; i < r.дайСчёт(); i++)
		if(match(r[i]))
			ndx.добавь(i);
	return ndx;
}

template <class Диапазон, class Предикат>
Вектор<int> найдиВсеи(const Диапазон& r, Предикат match, int from = 0)
{
	Вектор<int> ndx;
	for(int i = from; i < r.дайСчёт(); i++)
		if(match(i))
			ndx.добавь(i);
	return ndx;
}

template <class Диапазон, class T, class Less>
int найдиНижнГран(const Диапазон& r, const T& val, const Less& less)
{
	int pos = 0;
	int count = r.дайСчёт();
	while(count > 0) {
		int half = count >> 1;
		int m = pos + half;
		if(less(r[m], val)) {
			pos = m + 1;
			count = count - half - 1;
		}
		else
			count = half;
	}
	return pos;
}

template <class Диапазон, class T>
int найдиНижнГран(const Диапазон& r, const T& val)
{
	return найдиНижнГран(r, val, std::less<T>());
}

template <class Диапазон, class T, class L>
int найдиВерхнГран(const Диапазон& r, const T& val, const L& less)
{
	int pos = 0;
	int count = r.дайСчёт();
	while(count > 0) {
		int half = count >> 1;
		int m = pos + half;
		if(less(val, r[m]))
			count = half;
		else {
			pos = m + 1;
			count = count - half - 1;
		}
	}
	return pos;
}

template <class Диапазон, class T>
int найдиВерхнГран(const Диапазон& r, const T& val)
{
	return найдиВерхнГран(r, val, std::less<T>());
}

template <class Диапазон, class T, class L>
int найдиБинар(const Диапазон& r, const T& val, const L& less)
{
	int i = найдиНижнГран(r, val, less);
	return i < r.дайСчёт() && !less(val, r[i]) ? i : -1;
}

template <class Диапазон, class T>
int найдиБинар(const Диапазон& r, const T& val)
{
	return найдиБинар(r, val, std::less<T>());
}

template <class Контейнер, class T>
void LruAdd(Контейнер& lru, T значение, int limit = 10)
{
	int q = найдиИндекс(lru, значение);
	if(q >= 0)
		lru.удали(q);
	lru.вставь(0, значение);
	if(lru.дайСчёт() > limit)
		lru.устСчёт(limit);
}

template <class C = Вектор<int>, class V>
C сделайЙоту(V end, V start = 0, V step = 1)
{
	C x;
	x.резервируй((end - start) / step);
	for(V i = start; i < end; i += step)
		x.добавь(i);
	return x;
}
