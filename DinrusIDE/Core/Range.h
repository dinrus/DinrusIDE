template <class T>
T *DeclPtr__();

template <class Диапазон>
using типЗначУ = typename std::remove_reference<decltype(*DeclPtr__<typename std::remove_reference<Диапазон>::type>()->begin())>::type;

template <class Диапазон>
using ОбходчикУ = decltype(DeclPtr__<typename std::remove_reference<Диапазон>::type>()->begin());

template <class Диапазон>
using КонстОбходчикУ = decltype(DeclPtr__<const typename std::remove_reference<Диапазон>::type>()->begin());

template <class I>
class SubRangeClass {
	I   l;
	int count;

public:
	typedef typename std::remove_reference<decltype(*l)>::type value_type;

	int дайСчёт() const { return count; }

	SubRangeClass& пиши()                   { return *this; }

	value_type& operator[](int i) const { ПРОВЕРЬ(i >= 0 && i < count); return l[i]; }
	I  begin() const { return l; }
	I  end() const { return l + count; }

	Ткст   вТкст() const                            { return AsStringArray(*this); }
	template <class B> bool operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
	template <class B> int  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> bool operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return сравни(x) > 0; }

	SubRangeClass(I begin, int count) : l(begin), count(count)   {}
	SubRangeClass(I begin, I end) : l(begin), count(int(end - begin)) {}
	SubRangeClass() {} // MSC bug workaround
};

template <class I>
SubRangeClass<I> СубДиапазон(I l, I h)
{
	return SubRangeClass<I>(l, h);
}

template <class I>
SubRangeClass<I> СубДиапазон(I l, int count)
{
	return SubRangeClass<I>(l, count);
}

template <class C>
auto СубДиапазон(C&& c, int pos, int count) -> decltype(СубДиапазон(c.begin() + pos, count))
{
	return СубДиапазон(c.begin() + pos, count);
}

template <class C> using СубДиапазонУ = decltype(СубДиапазон(((C *)0)->begin(), ((C *)0)->end()));

template <class T>
struct ConstRangeClass {
	T   значение;
	int count;

	typedef T value_type;
	typedef value_type ТипЗнач;

	const value_type& operator[](int i) const { return значение; }
	int дайСчёт() const                      { return count; }

	typedef ConstIIterator<ConstRangeClass> Обходчик;

	Обходчик begin() const { return Обходчик(*this, 0); }
	Обходчик end() const { return Обходчик(*this, count); }

	Ткст   вТкст() const                            { return AsStringArray(*this); }
	template <class B> bool operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
	template <class B> int  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> bool operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return сравни(x) > 0; }

	ConstRangeClass(const T& значение, int count) : значение(значение), count(count) {}
	ConstRangeClass(int count) : count(count) {}
	ConstRangeClass() {} // MSC bug workaround
};

template <class T>
ConstRangeClass<T> ConstRange(const T& значение, int count)
{
	return ConstRangeClass<T>(значение, count);
}

template <class T>
ConstRangeClass<T> ConstRange(int count)
{
	return ConstRangeClass<T>(count);
}

template <class BaseRange>
struct ReverseRangeClass {
	typename std::remove_reference<BaseRange>::type& r;

	typedef типЗначУ<BaseRange>                       value_type;
	typedef value_type                                   ТипЗнач;

	const value_type& operator[](int i) const            { return r[r.дайСчёт() - i - 1]; }
	value_type& operator[](int i)                        { return r[r.дайСчёт() - i - 1]; }
	int дайСчёт() const                                 { return r.дайСчёт(); }

	typedef IIterator<ReverseRangeClass>                  Обходчик;
	typedef ConstIIterator<ReverseRangeClass>             КонстОбходчик;

	ReverseRangeClass& пиши()                           { return *this; }

	КонстОбходчик begin() const                          { return КонстОбходчик(*this, 0); }
	КонстОбходчик end() const                            { return КонстОбходчик(*this, r.дайСчёт()); }

	Обходчик begin()                                     { return Обходчик(*this, 0); }
	Обходчик end()                                       { return Обходчик(*this, r.дайСчёт()); }

	Ткст   вТкст() const                            { return AsStringArray(*this); }
	template <class B> bool operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
	template <class B> int  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> bool operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return сравни(x) > 0; }

	ReverseRangeClass(BaseRange& r) : r(r) {}
	ReverseRangeClass() {} // MSC bug workaround
};

template <class BaseRange>
ReverseRangeClass<BaseRange> ReverseRange(BaseRange&& r)
{
	return ReverseRangeClass<BaseRange>(r);
}

template <class BaseRange>
struct ViewRangeClass {
	typename std::remove_reference<BaseRange>::тип *r;
	Вектор<int> ndx;

	typedef типЗначУ<BaseRange> value_type;
	typedef value_type ТипЗнач;

	const value_type& operator[](int i) const { return (*r)[ndx[i]]; }
	value_type& operator[](int i)             { return (*r)[ndx[i]]; }
	int дайСчёт() const                      { return ndx.дайСчёт(); }

	typedef IIterator<ViewRangeClass> Обходчик;
	typedef ConstIIterator<ViewRangeClass> КонстОбходчик;

	ViewRangeClass& пиши()                   { return *this; }

	КонстОбходчик begin() const { return КонстОбходчик(*this, 0); }
	КонстОбходчик end() const  { return КонстОбходчик(*this, ndx.дайСчёт()); }

	Обходчик begin() { return Обходчик(*this, 0); }
	Обходчик end()   { return Обходчик(*this, ndx.дайСчёт()); }

	Ткст   вТкст() const                            { return AsStringArray(*this); }
	template <class B> bool operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
	template <class B> int  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> bool operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return сравни(x) > 0; }

	ViewRangeClass(BaseRange& r, Вектор<int>&& ndx) : r(&r), ndx(pick(ndx)) {}
	ViewRangeClass() {} // MSC bug workaround
};

template <class BaseRange>
ViewRangeClass<BaseRange> ViewRange(BaseRange&& r, Вектор<int>&& ndx)
{
	return ViewRangeClass<BaseRange>(r, pick(ndx));
}

template <class BaseRange, class Предикат>
ViewRangeClass<BaseRange> FilterRange(BaseRange&& r, Предикат p)
{
	return ViewRangeClass<BaseRange>(r, найдиВсе(r, p));
}

template <class BaseRange, class Предикат>
ViewRangeClass<BaseRange> SortedRange(BaseRange&& r, Предикат p)
{
	return ViewRangeClass<BaseRange>(r, GetSortOrder(r, p));
}

template <class BaseRange>
ViewRangeClass<BaseRange> SortedRange(BaseRange&& r)
{
	return ViewRangeClass<BaseRange>(r, GetSortOrder(r));
}