#ifdef DEPRECATED

template<class T>
struct StdEqual
{
	бул operator () (const T& a, const T& b) const { return a == b; }
};

template<class T>
struct StdLess {
	бул operator () (const T& a, const T& b) const { return a < b; }
};

template<class T>
struct StdGreater
{
	бул operator () (const T& a, const T& b) const { return a > b; }
};

template <class M, class T, class R>
class MethodRelationCls {
	M        method;
	const R& relation;

public:
	MethodRelationCls(M method, const R& relation) : method(method), relation(relation) {}
	бул operator () (const T& t1, const T& t2) const {
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
цел IterCompare(I a, I a_end, I b, I b_end, const C& compare)
{
	for(;;) {
		if(a >= a_end)
			return b < b_end ? -1 : 0;
		if(b >= b_end)
			return a < a_end ? 1 : 0;
		цел q = compare(*a++, *b++);
		if(q)
			return q;
	}
}

template <class I, class K, class X>
цел BinFindCompIndex(I begin, I end, const K& ключ, const X& comp)
{
	if(begin == end) // empty array
		return 0;
	цел мин = 0;
	цел макс = end - begin;
	while(мин < макс)
	{
		цел mid = (макс + мин) >> 1;
		if(comp.сравни(ключ, *(begin + mid)) > 0)
			мин = mid + 1;
		else
			макс = mid;
	}
	return мин;
}

template <class C, class K, class X>
inline цел BinFindCompIndex(const C& container, const K& ключ, const X& comp)
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
проц приставь(T& dst, V укз, V end)
{
	for(; укз != end; ++укз)
		dst.добавь(*укз);
}

template <class T, class V>
проц приставь(T& dst, V укз, цел n)
{
	for(; n--; ++укз)
		dst.добавь(*укз);
}

template <class T, class V>
проц приставь(T& dst, const V& ист)
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
цел MinIndex(const C& c) { return найдиМин(c); }

template <class C>
цел MaxIndex(const C& c) { return найдиМакс(c); }

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
цел найдиИндексПоля(const C& container, F field, const O& object, const E& equal)
{
	цел i = 0;
	for(typename C::КонстОбходчик b = container.старт(), e = container.стоп(); b != e; ++b, ++i)
		if(equal((*b).*field, object))
			return i;
	return -1;
}

template <class C, class F, class O>
цел найдиИндексПоля(const C& container, F field, const O& object)
{ return найдиИндексПоля(container, field, object, StdEqual<O>()); }

template <class DC, class I, class F>
проц GetFieldContainer(DC& dest, I begin, I end, F field)
{
	for(; begin != end; ++begin)
		dest.добавь((*begin).*field);
}

template <class DC, class SC, class F>
проц GetFieldContainer(DC& dest, const SC& ист, F field)
{ GetFieldContainer<DC, typename SC::КонстОбходчик, F>(dest, ист.старт(), ист.стоп(), field); }


template <class C>
цел найдиМин(const C& c, цел pos, цел count)
{
	return найдиМин(СубДиапазон(c, pos, count));
}

template <class T, class C>
бул равен(T ptr1, T end1, T ptr2, T end2, const C& equal)
{
	for(; ptr1 != end1 && ptr2 != end2; ++ptr1, ++ptr2)
		if(!equal(*ptr1, *ptr2)) return false;
	return ptr1 == end1 && ptr2 == end2;
}

template <class T, class C>
бул равен(const T& c1, const T& c2, const C& equal)
{
	return равен(c1.старт(), c1.стоп(), c2.старт(), c2.стоп(), equal);
}

template <class T>
бул равен(const T& c1, const T& c2)
{
	typedef typename T::ТипЗнач VT;
	return равен(c1, c2, std::equal<VT>());
}

template <class T, class V, class C>
T найди(T укз, T end, const V& значение, const C& equal)
{
	while(укз != end) {
		if(equal(*укз, значение)) return укз;
		укз++;
	}
	return NULL;
}

template <class T, class V>
T найди(T укз, T end, const V& значение)
{
	return найди(укз, end, значение, std::equal<V>());
}

template <class I, class K, class L>
т_мера BinFindIndex(I begin, I end, const K& ключ, const L& less)
{
	if(begin == end)
		return 0;
	т_мера мин = 0;
	т_мера макс = end - begin;

	while(мин < макс)
	{
		т_мера mid = (макс + мин) >> 1;
		if(less(*(begin + mid), ключ))
			мин = mid + 1;
		else
			макс = mid;
	}
	return мин;
}

template <class C, class K, class L>
inline цел BinFindIndex(const C& container, const K& ключ, const L& less)
{
	return BinFindIndex(container.старт(), container.стоп(), ключ, less);
}

template <class C, class K>
inline цел BinFindIndex(const C& container, const K& ключ)
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
цел найдиНижнГран(const C& v, цел pos, цел count, const T& знач, const L& less)
{
	return pos + найдиНижнГран(СубДиапазон(v, pos, count), знач, less);
}

template <class I, class T, class L>
I FindLowerBoundIter(I begin, I end, const T& знач, const L& less)
{
	return begin + найдиНижнГран(СубДиапазон(begin, end), знач, less);
}

template <class I, class T>
I FindLowerBoundIter(I begin, I end, const T& знач)
{
	return begin + найдиНижнГран(СубДиапазон(begin, end), знач, std::less<T>());
}

template <class C, class T, class L>
цел найдиВерхнГран(const C& v, цел pos, цел count, const T& знач, const L& less)
{
	return pos + найдиВерхнГран(СубДиапазон(v, pos, count), знач, less);
}

template <class I, class T, class L>
I FindUpperBoundIter(I begin, I end, const T& знач, const L& less)
{
	return begin + найдиВерхнГран(СубДиапазон(begin, end), знач, less);
}

template <class I, class T>
I FindUpperBoundIter(I begin, I end, const T& знач)
{
	return begin + найдиВерхнГран(СубДиапазон(begin, end), знач, std::less<T>());
}

template <class C, class T, class L>
цел найдиБинар(const C& v, const T& знач, цел pos, цел count, const L& less)
{
	return pos + найдиБинар(СубДиапазон(v, pos, count), знач, less);
}

template <class I, class T, class L>
I FindBinaryIter(I begin, I end, const T& знач, const L& less)
{
	return begin + найдиБинар(СубДиапазон(begin, end), знач, less);
}

template <class C, class T>
цел найдиБинар(const C& v, const T& знач, цел pos, цел count)
{
	return pos + найдиБинар(СубДиапазон(v, pos, count), знач, std::less<T>());
}

template <class I, class T>
I FindBinaryIter(I begin, I end, const T& знач)
{
	return begin + найдиБинар(СубДиапазон(begin, end), знач, std::less<T>());
}

template <class T, class V>
проц сумма(V& sum, T укз, T end)
{
	while(укз != end)
		sum += *укз++;
}

// -----------------------------------------------------------

#if 0

template <class I, class Less>
проц StableSort(I begin, I end, const Less& less)
{
	if(begin != end)
		StableSort__(begin, end, less, &*begin);
}

template <class T, class Сравн>
struct StableSortLessCmp_ {
	const Сравн& сравни;
	бул operator()(const StableSortItem<T>& a, const StableSortItem<T>& b) const {
		цел q = сравни(a.значение, b.значение);
		return q ? q < 0 : a.индекс < b.индекс;
	}

	StableSortLessCmp_(const Сравн& сравни) : сравни(сравни) {}
};

template <class I, class Сравн, class T>
проц StableSortCmp_(I begin, I end, const Сравн& сравни, const T *)
{
	цел count = end - begin;
	Буфер<цел> h(count);
	for(цел i = 0; i < count; i++)
		h[i] = i;
	сортируй(StableSortIterator<I, T>(begin, ~h),
	     StableSortIterator<I, T>(end, ~h + count),
	     StableSortLessCmp_<T, Сравн>(сравни));
}

template <class I, class Сравн>
проц StableSortCmp(I begin, I end, const Сравн& сравни)
{
	if(begin != end)
		StableSortCmp_(begin, end, сравни, &*begin);
}

template <class T, class Сравн>
проц StableSortCmp(T& c, const Сравн& сравни)
{
	StableSortCmp(c.begin(), c.end(), сравни);
}

template <class T>
struct StdCmp {
	цел operator()(const T& a, const T& b) const {
		return сравниЗнак(a, b);
	}
};

template <class T>
проц StableSortCmp(T& c)
{
	StableSortCmp(c.begin(), c.end(), StdCmp<typename T::ТипЗнач>());
}


template <class I, class T, class Сравн>
inline проц __StableSortOrderCmp(цел *ibegin, цел *iend, I данные, const Сравн& сравни, const T *)
{
	сортируй(StableSortOrderIterator<I, T>(ibegin, данные),
	     StableSortOrderIterator<I, T>(iend, данные),
	     StableSortLessCmp_<T, Сравн>(сравни));
}

template <class I, class Сравн>
inline Вектор<цел> GetStableSortOrderCmp(I begin, I end, const Сравн& сравни)
{
	Вектор<цел> индекс;
	индекс.устСчёт((цел)(end - begin));
	for(цел i = индекс.дайСчёт(); --i >= 0; индекс[i] = i)
		;
	if(begin != end)
		__StableSortOrderCmp(индекс.begin(), индекс.end(), begin, сравни, &*begin);
	return индекс;
}

template <class C, class Сравн>
inline Вектор<цел> GetStableSortOrderCmp(const C& container, const Сравн& сравни)
{
	return GetStableSortOrderCmp(container.begin(), container.end(), сравни);
}

template <class C>
inline Вектор<цел> GetStableSortOrderCmp(const C& container)
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
	бул operator () (const T& t1, const T& t2) const { return relation(t1.*member, t2.*member); }
};

template <class O, class T, class R>
inline FieldRelationCls<O, T, R> FieldRelation(O (T::*member), const R& relation)
{ return FieldRelationCls<O, T, R>(member, relation); }

#endif
