enum {
	КО_ПАРТИЦ_МИН = 128,
	КО_ПАРТИЦ_МАКС = 1024*1024,
};

template <class C, class MC>
inline size_t соЧанк__(C count, MC min_chunk = КО_ПАРТИЦ_МИН, MC max_chunk = КО_ПАРТИЦ_МАКС)
{
	int n = min(count / цпбЯдра(), (C)max_chunk);
	return n < (C)min_chunk ? 0 : n;
}

template <class Обх, class Лямбда>
void соОтсек(Обх begin, Обх end, const Лямбда& lambda, int min_chunk = КО_ПАРТИЦ_МИН, int max_chunk = КО_ПАРТИЦ_МАКС)
{
	size_t chunk = соЧанк__(end - begin, min_chunk, max_chunk);
	if(chunk) {
		СоРабота co;
		while(begin < end) {
			Обх e = Обх(begin + min(chunk, size_t(end - begin)));
			co & [=] {
				lambda(begin, e);
			};
			begin = e;
		}
	}
	else
		lambda(begin, end);
}

template <class Диапазон, class Лямбда>
void соОтсек(Диапазон&& r, const Лямбда& lambda)
{
	size_t chunk = соЧанк__(r.дайСчёт(), КО_ПАРТИЦ_МИН);
	if(chunk) {
		СоРабота co;
		auto begin = r.begin();
		auto end = r.end();
		while(begin < end) {
			auto e = begin + min(chunk, size_t(end - begin));
			co & [=] {
				auto sr = СубДиапазон(begin, e); // we need l-значение
				lambda(sr);
			};
			begin = e;
		}
	}
	else
		lambda(СубДиапазон(r.begin(), r.end()));
}

template <class Диапазон, class Накопитель>
void соНакопи(const Диапазон& r, Накопитель& result)
{
	typedef КонстОбходчикУ<Диапазон> I;
	соОтсек(r.begin(), r.end(),
		[=, &result](I i, I e) {
			Накопитель h;
			while(i < e)
				h(*i++);
			СоРабота::финБлок();
			result(h);
		}
	);
}

template <class Диапазон>
типЗначУ<Диапазон> соСумма(const Диапазон& r, const типЗначУ<Диапазон>& zero)
{
	typedef типЗначУ<Диапазон> VT;
	typedef КонстОбходчикУ<Диапазон> I;
	VT sum = zero;
	соОтсек(r.begin(), r.end(),
		[=, &sum](I i, I e) {
			VT h = zero;
			while(i < e)
				h += *i++;
			СоРабота::финБлок();
			sum += h;
		}
	);
	return sum;
}

template <class T>
типЗначУ<T> соСумма(const T& c)
{
	return соСумма(c, (типЗначУ<T>)0);
}

template <class Диапазон, class V>
int соСчёт(const Диапазон& r, const V& val)
{
	int count = 0;
	соОтсек(r, [=, &val, &count](const СубДиапазонУ<const Диапазон>& r) {
		int n = счёт(r, val);
		СоРабота::финБлок();
		count += n;
	});
	return count;
}

template <class Диапазон, class Предикат>
int соСчётЕсли(const Диапазон& r, const Предикат& p)
{
	int count = 0;
	соОтсек(r, [=, &p, &count](const СубДиапазонУ<const Диапазон>& r) {
		int n = счётЕсли(r, p);
		СоРабота::финБлок();
		count += n;
	});
	return count;
}

template <class Диапазон, class Better>
int соНайдиЛучш(const Диапазон& r, const Better& better)
{
	if(r.дайСчёт() <= 0)
		return -1;
	typedef КонстОбходчикУ<Диапазон> I;
	I best = r.begin();
	соОтсек(r.begin() + 1, r.end(),
		[=, &best](I i, I e) {
			I b = i++;
			while(i < e) {
				if(better(*i, *b))
					b = i;
				i++;
			}
			СоРабота::финБлок();
			if(better(*b, *best) || !better(*best, *b) && b < best)
				best = b;
		}
	);
	return best - r.begin();
}

template <class Диапазон>
int соНайдиМин(const Диапазон& r)
{
	return соНайдиЛучш(r, std::less<типЗначУ<Диапазон>>());
}

template <class Диапазон>
const типЗначУ<Диапазон>& соМин(const Диапазон& r)
{
	return r[соНайдиМин(r)];
}

template <class Диапазон>
const типЗначУ<Диапазон>& соМин(const Диапазон& r, const типЗначУ<Диапазон>& опр)
{
	int q = соНайдиМин(r);
	return q < 0 ? опр : r[q];
}


template <class Диапазон>
int соНайдиМакс(const Диапазон& r)
{
	return соНайдиЛучш(r, std::greater<типЗначУ<Диапазон>>());
}

template <class Диапазон>
const типЗначУ<Диапазон>& соМакс(const Диапазон& r)
{
	return r[соНайдиМакс(r)];
}

template <class Диапазон>
const типЗначУ<Диапазон>& соМакс(const Диапазон& r, const типЗначУ<Диапазон>& опр)
{
	int q = соНайдиМакс(r);
	return q < 0 ? опр : r[q];
}

template <class Диапазон, class Свер>
int соНайдиСовпад(const Диапазон& r, const Свер& eq, int from = 0)
{
	int count = r.дайСчёт();
	std::atomic<int> found;
	found = count;
	соОтсек(from, count,
		[=, &r, &found](int i, int e) {
			for(; i < e; i++) {
				if(found < i)
					break;
				if(eq(r[i])) {
					СоРабота::финБлок();
					if(i < found)
						found = i;
					return;
				}
			}
		}
	);
	return found < (int)count ? (int)found : -1;
}

template <class Диапазон, class V>
int соНайдиИндекс(const Диапазон& r, const V& значение, int from = 0)
{
	return соНайдиСовпад(r, [=, &значение](const типЗначУ<Диапазон>& m) { return m == значение; }, from);
}

template <class Диапазон1, class Диапазон2>
int соДиапазоныРавны(const Диапазон1& r1, const Диапазон2& r2)
{
	if(r1.дайСчёт() != r2.дайСчёт())
		return false;
	std::atomic<bool> equal(true);
	соОтсек(0, r1.дайСчёт(),
		[=, &equal, &r1, &r2](int i, int e) {
			while(i < e && equal) {
				if(r1[i] != r2[i]) {
					СоРабота::финБлок();
					equal = false;
				}
				i++;
			}
		}
	);
	return equal;
}

template <class Диапазон, class Предикат>
Вектор<int> соНайдиВсе(const Диапазон& r, Предикат match, int from = 0)
{
	Вектор<Вектор<int>> rs;
	int total = 0;
	соОтсек(from, r.дайСчёт(), [=, &r, &rs, &total](int begin, int end) {
		Вектор<int> v = найдиВсе(СубДиапазон(r, 0, end), match, begin);
		СоРабота::финБлок();
		if(v.дайСчёт()) {
			total += v.дайСчёт();
			rs.добавь(pick(v));
		}
	});
	соСортируй(rs, [](const Вектор<int>& a, const Вектор<int>& b) { return a[0] < b[0]; });
	Вектор<int> result;
	for(const auto& s : rs)
		result.приставь(s);
	return result;
}
