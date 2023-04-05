#ifdef DEPRECATED

template<class T>
struct StdEqual
{
	bool operator () (const T& a, const T& b) const { return a == b; }
};

template<class T>
struct StdLess {
	bool operator () (const T& a, const T& b) const { return a < b; }
};

template<class T>
struct StdGreater
{
	bool operator () (const T& a, const T& b) const { return a > b; }
};

template <class M, class T, class R>
class MethodRelationCls {
	M        method;
	const R& relation;

public:
	MethodRelationCls(M method, const R& relation) : method(method), relation(relation) {}
	bool operator () (const T& t1, const T& t2) const {
		return relation((t1.*method)(), (t2.*method)());
	}
};

template <class O, class T, class R>
inline MethodRelationCls<O (T::*)(), T, R>
	MethodRelation(O (T::*method)(), const R& relation)
{ return MethodRelationCls<O (T::*)(), T, R>(method, relation); }

template <class O, class T, class R>
inline MethodRelationCls<O (T::*)() const, T, R>
	MethodRelation(O (T::*method)() const, const R& relation)
{ return MethodRelationCls<O (T::*)() const, T, R>(method, relation); }

template <class I, class C>
int IterCompare(I a, I a_end, I b, I b_end, const C& compare)
{
	for(;;) {
		if(a >= a_end)
			return b < b_end ? -1 : 0;
		if(b >= b_end)
			return a < a_end ? 1 : 0;
		int q = compare(*a++, *b++);
		if(q)
			return q;
	}
}

template <class I, class K, class X>
int BinFindCompIndex(I begin, I end, const K& ключ, const X& comp)
{
	if(begin == end) // empty array
		return 0;
	int min = 0;
	int max = end - begin;
	while(min < max)
	{
		int mid = (max + min) >> 1;
		if(comp.сравни(ключ, *(begin + mid)) > 0)
			min = mid + 1;
		else
			max = mid;
	}
	return min;
}

template <class C, class K, class X>
inline int BinFindCompIndex(const C& container, const K& ключ, const X& comp)
{
	return BinFindCompIndex(container.старт(), container.стоп(), ключ, comp);
}

template <class I, class K, class X>
inline I BinFindComp(I begin, I end, const K& ключ, const X& comp)
{
	return begin + BinFindCompIndex(begin, end, ключ, comp);
}

template <class C, class K, class X>
inline typename C::КонстОбходчик BinFindComp(const C& container, const K& ключ, const X& comp)
{
	return BinFindComp(container.старт(), container.стоп(), ключ, comp);
}

template <class T, class V>
void приставь(T& dst, V ptr, V end)
{
	for(; ptr != end; ++ptr)
		dst.добавь(*ptr);
}

template <class T, class V>
void приставь(T& dst, V ptr, int n)
{
	for(; n--; ++ptr)
		dst.добавь(*ptr);
}

template <class T, class V>
void приставь(T& dst, const V& ист)
{
	приставь(dst, ист.старт(), ист.стоп());
}

template <class C, class I>
C& FindAppend(C& dest, I begin, I end)
{
	for(; begin != end; ++begin)
		dest.найдиДобавь(*begin);
	return dest;
}

template <class C, class S>
inline C& FindAppend(C& dest, const S& source)
{
	return FindAppend(dest, source.старт(), source.стоп());
}

template <class C>
int MinIndex(const C& c) { return найдиМин(c); }

template <class C>
int MaxIndex(const C& c) { return найдиМакс(c); }

template <class C>
typename C::ТипЗнач сумма0(const C& c) { return сумма(c); }

template <class I, class F, class O, class E>
I FindField(I begin, I end, F field, const O& object, const E& equal)
{
	for(; begin != end && !equal((*begin).*field, object); ++begin)
		;
	return begin;
}

template <class I, class F, class O>
I FindField(I begin, I end, F field, const O& object)
{ return FindField(begin, end, field, object, StdEqual<O>()); }

template <class C, class F, class O, class E>
int найдиИндексПоля(const C& container, F field, const O& object, const E& equal)
{
	int i = 0;
	for(typename C::КонстОбходчик b = container.старт(), e = container.стоп(); b != e; ++b, ++i)
		if(equal((*b).*field, object))
			return i;
	return -1;
}

template <class C, class F, class O>
int найдиИндексПоля(const C& container, F field, const O& object)
{ return найдиИндексПоля(container, field, object, StdEqual<O>()); }

template <class DC, class I, class F>
void GetFieldContainer(DC& dest, I begin, I end, F field)
{
	for(; begin != end; ++begin)
		dest.добавь((*begin).*field);
}

template <class DC, class SC, class F>
void GetFieldContainer(DC& dest, const SC& ист, F field)
{ GetFieldContainer<DC, typename SC::КонстОбходчик, F>(dest, ист.старт(), ист.стоп(), field); }


template <class C>
int найдиМин(const C& c, int pos, int count)
{
	return найдиМин(СубДиапазон(c, pos, count));
}

template <class T, class C>
bool равен(T ptr1, T end1, T ptr2, T end2, const C& equal)
{
	for(; ptr1 != end1 && ptr2 != end2; ++ptr1, ++ptr2)
		if(!equal(*ptr1, *ptr2)) return false;
	return ptr1 == end1 && ptr2 == end2;
}

template <class T, class C>
bool равен(const T& c1, const T& c2, const C& equal)
{
	return равен(c1.старт(), c1.стоп(), c2.старт(), c2.стоп(), equal);
}

template <class T>
bool равен(const T& c1, const T& c2)
{
	typedef typename T::ТипЗнач VT;
	return равен(c1, c2, std::equal<VT>());
}

template <class T, class V, class C>
T найди(T ptr, T end, const V& значение, const C& equal)
{
	while(ptr != end) {
		if(equal(*ptr, значение)) return ptr;
		ptr++;
	}
	return NULL;
}

template <class T, class V>
T найди(T ptr, T end, const V& значение)
{
	return найди(ptr, end, значение, std::equal<V>());
}

template <class I, class K, class L>
size_t BinFindIndex(I begin, I end, const K& ключ, const L& less)
{
	if(begin == end)
		return 0;
	size_t min = 0;
	size_t max = end - begin;

	while(min < max)
	{
		size_t mid = (max + min) >> 1;
		if(less(*(begin + mid), ключ))
			min = mid + 1;
		else
			max = mid;
	}
	return min;
}

template <class C, class K, class L>
inline int BinFindIndex(const C& container, const K& ключ, const L& less)
{
	return BinFindIndex(container.старт(), container.стоп(), ключ, less);
}

template <class C, class K>
inline int BinFindIndex(const C& container, const K& ключ)
{
	typedef typename C::ТипЗнач VT;
	return BinFindIndex(container, ключ, std::less<VT>());
}

template <class I, class K, class L>
inline I BinFind(I begin, I end, const K& ключ, const L& less)
{
	return begin + BinFindIndex(begin, end, ключ, less);
}

template <class C, class K, class L>
inline typename C::КонстОбходчик BinFind(const C& container, const K& ключ, const L& less)
{
	return BinFind(container.старт(), container.стоп(), ключ, less);
}

template <class C, class K>
inline typename C::КонстОбходчик BinFind(const C& container, const K& ключ)
{
	typedef typename C::ТипЗнач VT;
	return BinFind(container, ключ, std::less<VT>());
}

template <class C, class T, class L>
int найдиНижнГран(const C& v, int pos, int count, const T& val, const L& less)
{
	return pos + найдиНижнГран(СубДиапазон(v, pos, count), val, less);
}

template <class I, class T, class L>
I FindLowerBoundIter(I begin, I end, const T& val, const L& less)
{
	return begin + найдиНижнГран(СубДиапазон(begin, end), val, less);
}

template <class I, class T>
I FindLowerBoundIter(I begin, I end, const T& val)
{
	return begin + найдиНижнГран(СубДиапазон(begin, end), val, std::less<T>());
}

template <class C, class T, class L>
int найдиВерхнГран(const C& v, int pos, int count, const T& val, const L& less)
{
	return pos + найдиВерхнГран(СубДиапазон(v, pos, count), val, less);
}

template <class I, class T, class L>
I FindUpperBoundIter(I begin, I end, const T& val, const L& less)
{
	return begin + найдиВерхнГран(СубДиапазон(begin, end), val, less);
}

template <class I, class T>
I FindUpperBoundIter(I begin, I end, const T& val)
{
	return begin + найдиВерхнГран(СубДиапазон(begin, end), val, std::less<T>());
}

template <class C, class T, class L>
int найдиБинар(const C& v, const T& val, int pos, int count, const L& less)
{
	return pos + найдиБинар(СубДиапазон(v, pos, count), val, less);
}

template <class I, class T, class L>
I FindBinaryIter(I begin, I end, const T& val, const L& less)
{
	return begin + найдиБинар(СубДиапазон(begin, end), val, less);
}

template <class C, class T>
int найдиБинар(const C& v, const T& val, int pos, int count)
{
	return pos + найдиБинар(СубДиапазон(v, pos, count), val, std::less<T>());
}

template <class I, class T>
I FindBinaryIter(I begin, I end, const T& val)
{
	return begin + найдиБинар(СубДиапазон(begin, end), val, std::less<T>());
}

template <class T, class V>
void сумма(V& sum, T ptr, T end)
{
	while(ptr != end)
		sum += *ptr++;
}

// -----------------------------------------------------------

#if 0

template <class I, class Less>
void StableSort(I begin, I end, const Less& less)
{
	if(begin != end)
		StableSort__(begin, end, less, &*begin);
}

template <class T, class Сравн>
struct StableSortLessCmp_ {
	const Сравн& сравни;
	bool operator()(const StableSortItem<T>& a, const StableSortItem<T>& b) const {
		int q = сравни(a.значение, b.значение);
		return q ? q < 0 : a.индекс < b.индекс;
	}

	StableSortLessCmp_(const Сравн& сравни) : сравни(сравни) {}
};

template <class I, class Сравн, class T>
void StableSortCmp_(I begin, I end, const Сравн& сравни, const T *)
{
	int count = end - begin;
	Буфер<int> h(count);
	for(int i = 0; i < count; i++)
		h[i] = i;
	сортируй(StableSortIterator<I, T>(begin, ~h),
	     StableSortIterator<I, T>(end, ~h + count),
	     StableSortLessCmp_<T, Сравн>(сравни));
}

template <class I, class Сравн>
void StableSortCmp(I begin, I end, const Сравн& сравни)
{
	if(begin != end)
		StableSortCmp_(begin, end, сравни, &*begin);
}

template <class T, class Сравн>
void StableSortCmp(T& c, const Сравн& сравни)
{
	StableSortCmp(c.begin(), c.end(), сравни);
}

template <class T>
struct StdCmp {
	int operator()(const T& a, const T& b) const {
		return сравниЗнак(a, b);
	}
};

template <class T>
void StableSortCmp(T& c)
{
	StableSortCmp(c.begin(), c.end(), StdCmp<typename T::ТипЗнач>());
}


template <class I, class T, class Сравн>
inline void __StableSortOrderCmp(int *ibegin, int *iend, I данные, const Сравн& сравни, const T *)
{
	сортируй(StableSortOrderIterator<I, T>(ibegin, данные),
	     StableSortOrderIterator<I, T>(iend, данные),
	     StableSortLessCmp_<T, Сравн>(сравни));
}

template <class I, class Сравн>
inline Вектор<int> GetStableSortOrderCmp(I begin, I end, const Сравн& сравни)
{
	Вектор<int> индекс;
	индекс.устСчёт((int)(end - begin));
	for(int i = индекс.дайСчёт(); --i >= 0; индекс[i] = i)
		;
	if(begin != end)
		__StableSortOrderCmp(индекс.begin(), индекс.end(), begin, сравни, &*begin);
	return индекс;
}

template <class C, class Сравн>
inline Вектор<int> GetStableSortOrderCmp(const C& container, const Сравн& сравни)
{
	return GetStableSortOrderCmp(container.begin(), container.end(), сравни);
}

template <class C>
inline Вектор<int> GetStableSortOrderCmp(const C& container)
{
	typedef typename C::ТипЗнач V;
	return GetStableSortOrderCmp(container.begin(), container.end(), StdCmp<V>());
}
#endif

template <class O, class T, class R>
class FieldRelationCls {
	O        T::*member;
	const R& relation;

public:
	FieldRelationCls(O (T::*member), const R& relation) : member(member), relation(relation) {}
	bool operator () (const T& t1, const T& t2) const { return relation(t1.*member, t2.*member); }
};

template <class O, class T, class R>
inline FieldRelationCls<O, T, R> FieldRelation(O (T::*member), const R& relation)
{ return FieldRelationCls<O, T, R>(member, relation); }

#endif
