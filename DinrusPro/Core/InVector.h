template <class T> struct Slaved_InVector__;
template <class T> struct Slaved_InArray__;
template <class K, class TT, class Lss, class Данные> class SortedAMap;
template <class K, class TT, class Less> class SortedVectorMap;

struct InVectorSlave__ {
	virtual проц очисть() = 0;
	virtual проц счёт(цел n) = 0;
	virtual проц разбей(цел blki, цел nextsize) = 0;
	virtual проц AddFirst() = 0;
	virtual проц вставь(цел blki, цел pos) = 0;
	virtual проц Join(цел blki) = 0;
	virtual проц удали(цел blki, цел pos, цел n) = 0;
	virtual проц RemoveBlk(цел blki, цел n) = 0;
	virtual проц Индекс(цел blki, цел n) = 0;
	virtual проц реиндексируй() = 0;
	virtual проц сожми() = 0;
};

template <class T>
class InVector : public ОпцияДвижимогоИГлубКопии< InVector<T> > {
public:
	class КонстОбходчик;
	class Обходчик;

	template <class K, class TT, class Lss, class Данные> friend class SortedAMap;
	template <class K, class TT, class Less> friend class SortedVectorMap;
	template <class TT> friend struct Slaved_InVector__;
	template <class TT> friend struct Slaved_InArray__;

private:
	Вектор< Вектор<T> > данные;
	Вектор< Вектор<цел> > индекс;

	цел    count;
	цел    hcount;
	дол  serial;
	бкрат blk_high;
	бкрат blk_low;
	бкрат slave;

	проц подбери(InVector&& b);

	InVectorSlave__ *Slave()          { return (InVectorSlave__ *)((ббайт *)this + slave); }
	проц SetSlave(InVectorSlave__ *s) { slave = (бкрат)((ббайт *)s - (ббайт *)this); }

	проц SetCache(цел blki, цел offset) const;
	проц очистьКэш() const;
	цел  FindBlock0(цел& pos, цел& off) const;
	цел  FindBlock(цел& pos, цел& off) const;
	цел  FindBlock(цел& pos) const;
	проц SetBlkPar();
	
	template <class L>
	цел  найдиВерхнГран(const T& знач, const L& less, цел& off, цел& pos) const;

	template <class L>
	цел  найдиНижнГран(const T& знач, const L& less, цел& off, цел& pos) const;

	проц реиндексируй();
	проц Индекс(цел q, цел n);
	проц счёт(цел n)                               { count += n; }
	проц Join(цел blki);

	бул JoinSmall(цел blki);
	T   *вставь0(цел ii, цел blki, цел pos, цел off, const T *знач);
	T   *вставь0(цел ii, const T *знач);
	
	проц переустанов();

	проц SetIter(КонстОбходчик& it, цел ii) const;
	проц SetBegin(КонстОбходчик& it) const;
	проц SetEnd(КонстОбходчик& it) const;

	проц     иниц();

	template <class Диапазон>
	проц     Insert_(цел ii, const Диапазон& r, бул опр);

#ifdef flagIVTEST
	проц Check(цел blki, цел offset) const;
#endif
public:
	T&       вставь(цел i)                          { return *вставь0(i, NULL); }
	T&       вставь(цел i, const T& x)              { return *вставь0(i, &x); }
	template <class Диапазон>
	проц     вставьДиапазон(цел i, const Диапазон& r)     { Insert_(i, r, false); }
	проц     вставьН(цел i, цел count)              { Insert_(i, КонстДиапазон<T>(count), true); }
	проц     удали(цел i, цел count = 1);
	template <class Диапазон>
	проц     приставьДиапазон(const Диапазон& r)            { вставьДиапазон(дайСчёт(), r); }

	const T& operator[](цел i) const;
	T&       operator[](цел i);

	T&       добавь()                                  { return вставь(дайСчёт()); }
	T&       добавь(const T& x)                        { return вставь(дайСчёт(), x); }
	проц     добавьН(цел n)                            { вставьН(дайСчёт(), n); }
	
	цел      дайСчёт() const                       { return count; }
	бул     пустой() const                        { return дайСчёт() == 0; }

	проц     обрежь(цел n)                            { удали(n, дайСчёт() - n); }
	проц     устСчёт(цел n);
	проц     очисть();

	T&       по(цел i)                              { if(i >= дайСчёт()) устСчёт(i + 1); return (*this)[i]; }

	проц     сожми();

	проц     уст(цел i, const T& x, цел count);
	T&       уст(цел i, const T& x)                 { уст(i, x, 1); return (*this)(i); }

	проц     разверни(цел i1, цел i2)                   { РНЦП::разверни((*this)[i1], (*this)[i2]); }

	проц     сбрось(цел n = 1)                        { ПРОВЕРЬ(n <= дайСчёт()); обрежь(дайСчёт() - n); }
	T&       верх()                                  { ПРОВЕРЬ(дайСчёт()); return (*this)[дайСчёт() - 1]; }
	const T& верх() const                            { ПРОВЕРЬ(дайСчёт()); return (*this)[дайСчёт() - 1]; }
	T        вынь()                                  { T h = пикуй(верх()); сбрось(); return h; }

	template <class L>
	цел найдиВерхнГран(const T& знач, const L& less) const { цел off, pos; найдиВерхнГран(знач, less, off, pos); return off + pos; }
	цел найдиВерхнГран(const T& знач) const                { return найдиВерхнГран(знач, StdLess<T>()); }

	template <class L>
	цел найдиНижнГран(const T& знач, const L& less) const { цел off, pos; найдиНижнГран(знач, less, off, pos); return off + pos; }
	цел найдиНижнГран(const T& знач) const                { return найдиНижнГран(знач, StdLess<T>()); }

	template <class L>
	цел InsertUpperBound(const T& знач, const L& less);
	цел InsertUpperBound(const T& знач)                    { return InsertUpperBound(знач, StdLess<T>()); }
	
	template <class L>
	цел найди(const T& знач, const L& less) const;
	цел найди(const T& знач) const                         { return найди(знач, StdLess<T>()); }

	КонстОбходчик    begin() const                       { КонстОбходчик it; SetBegin(it); return it; }
	КонстОбходчик    end() const                         { КонстОбходчик it; SetEnd(it); return it; }

	Обходчик         begin()                             { Обходчик it; SetBegin(it); return it; }
	Обходчик         end()                               { Обходчик it; SetEnd(it); return it; }

	InVector();
	InVector(InVector&& v)                               { подбери(пикуй(v)); }
	проц operator=(InVector&& v)                         { подбери(пикуй(v)); }
	InVector(const InVector& v, цел);
	InVector(std::initializer_list<T> init)              { иниц(); for(const auto& i : init) добавь(i); }

	проц разверни(InVector& b);

	проц     сериализуй(Поток& s)                        { StreamContainer(s, *this); }
	проц     вРяр(РярВВ& xio, кткст0 itemtag = "элт");
	проц     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
	т_хэш   дайХэшЗнач() const                        { return хэшПоСериализу(*this); }
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	friend проц разверни(InVector& a, InVector& b)      { a.разверни(b); }
	
	STL_VECTOR_COMPATIBILITY(InVector<T>)

	проц DumpIndex() const;
	
	проц резервируй(цел) {} // Does nothing, but needed for unified interface (e.g. StreamContainer)
#ifdef DEPRECATED
	typedef T        ТипЗнач;
	КонстОбходчик    дайОбх(цел pos) const         { КонстОбходчик it; SetIter(it, pos); return it; }
	Обходчик         дайОбх(цел pos)               { Обходчик it; SetIter(it, pos); return it; }
#endif
};

template <class T>
class InVector<T>::КонстОбходчик {
	const T        *укз;
	const T        *begin;
	const T        *end;
	const InVector *v;
	цел             offset;
	цел             blki;

	friend class InVector<T>;
	friend class InVector<T>::Обходчик;

	проц NextBlk();
	проц PrevBlk();

public:
	force_inline цел дайИндекс() const              { return цел(укз - begin) + offset; }

	force_inline КонстОбходчик& operator++()       { ПРОВЕРЬ(укз); if(++укз == end) NextBlk(); return *this; }
	force_inline КонстОбходчик& operator--()       { ПРОВЕРЬ(укз); if(укз == begin) PrevBlk(); --укз; return *this; }
	force_inline КонстОбходчик  operator++(цел)    { КонстОбходчик t = *this; ++*this; return t; }
	force_inline КонстОбходчик  operator--(цел)    { КонстОбходчик t = *this; --*this; return t; }

	force_inline КонстОбходчик& operator+=(цел d);
	КонстОбходчик& operator-=(цел d)               { return operator+=(-d); }

	КонстОбходчик operator+(цел d) const           { КонстОбходчик t = *this; t += d; return t; }
	КонстОбходчик operator-(цел d) const           { return operator+(-d); }

	цел  operator-(const КонстОбходчик& x) const   { return дайИндекс() - x.дайИндекс(); }

	бул operator==(const КонстОбходчик& b) const  { return укз == b.укз; }
	бул operator!=(const КонстОбходчик& b) const  { return укз != b.укз; }
	бул operator<(const КонстОбходчик& b) const   { return blki == b.blki ? укз < b.укз : blki < b.blki; }
	бул operator>(const КонстОбходчик& b) const   { return blki == b.blki ? укз > b.укз : blki > b.blki; }
	бул operator<=(const КонстОбходчик& b) const  { return blki == b.blki ? укз <= b.укз : blki <= b.blki; }
	бул operator>=(const КонстОбходчик& b) const  { return blki == b.blki ? укз >= b.укз : blki >= b.blki; }

	operator бул() const                          { return укз; }

	const T& operator*() const                     { return *укз; }
	const T *operator->() const                    { return укз; }
	const T& operator[](цел i) const               { КонстОбходчик h = *this; h += i; return *h; }

	STL_ITERATOR_COMPATIBILITY
};

template <class T>
class InVector<T>::Обходчик : public InVector<T>::КонстОбходчик {
	typedef КонстОбходчик B;
public:
	Обходчик& operator++()                         { КонстОбходчик::operator++(); return *this; }
	Обходчик& operator--()                         { КонстОбходчик::operator--(); return *this; }
	Обходчик  operator++(цел)                      { Обходчик t = *this; ++*this; return t; }
	Обходчик  operator--(цел)                      { Обходчик t = *this; --*this; return t; }

	Обходчик& operator+=(цел d)                    { КонстОбходчик::operator+=(d); return *this; }
	Обходчик& operator-=(цел d)                    { return operator+=(-d); }

	Обходчик operator+(цел d) const                { Обходчик t = *this; t += d; return t; }
	Обходчик operator-(цел d) const                { return operator+(-d); }

	цел  operator-(const Обходчик& x) const        { return B::дайИндекс() - x.дайИндекс(); }

	T& operator*()                                 { return *(T*)B::укз; }
	T *operator->()                                { return (T*)B::укз; }
	T& operator[](цел i)                           { Обходчик h = *this; h += i; return *h; }

	const T& operator*() const                     { return *B::укз; }
	const T *operator->() const                    { return B::укз; }
	const T& operator[](цел i) const               { КонстОбходчик h = *this; h += i; return *h; }
	
	typename InVector<T>::Обходчик& operator=(const B& ист) { *(B *)this = ист; return *this; }

	STL_ITERATOR_COMPATIBILITY
};

template <class T>
class InArray : public ОпцияДвижимогоИГлубКопии< InArray<T> > {
//	template <class K, class TT, class Less, class Данные> friend class SortedAMap;
	template <class TT> friend struct Slaved_InArray__;

public:
	class КонстОбходчик;
	class Обходчик;

private:
#ifdef _ОТЛАДКА
	typedef T *ТипУказатель;
#else
	typedef ук ТипУказатель;
#endif
	typedef InVector<ТипУказатель> IV;
	typedef typename InVector<ТипУказатель>::Обходчик IVIter;
	typedef typename InVector<ТипУказатель>::Обходчик IVConstIter;

	IV iv;

	T&       дай(цел i) const       { return *(T *)iv[i]; }
	проц     Delete(IVIter i, цел count);
	проц     Delete(цел i, цел count);
	проц     иниц(цел i, цел count);
	проц     освободи()                 { Delete(iv.begin(), дайСчёт()); }

	проц     SetIter(КонстОбходчик& it, цел ii) const;
	проц     SetBegin(КонстОбходчик& it) const;
	проц     SetEnd(КонстОбходчик& it) const;

	template <class L>
	struct ALess {
		const L& less;
		
		бул operator()(const ТипУказатель& a, const ТипУказатель& b) const {
			return less(*(T*)a, *(T*)b);
		}
		
		ALess(const L& less) : less(less) {}
	};

	template <class K, class V, class L> friend class SortedVectorMap;

public:
	T&       вставь(цел i, T *newt)                 { iv.вставь(i, newt); return *newt; }
	T&       вставь(цел i)                          { return вставь(i, new T); }
	T&       вставь(цел i, const T& x)              { return вставь(i, new T(x)); }
	template<class TT, class... Арги>
	TT&      InsertCreate(цел i, Арги&&... арги)    { TT *q = new TT(std::forward<Арги>(арги)...); вставь(i, q); return *q; }

	проц     вставьН(цел i, цел count);
	template <class Диапазон>
	проц     вставьДиапазон(цел i, const Диапазон& r);
	проц     удали(цел i, цел count = 1);
	
	template <class Диапазон>
	проц     приставьДиапазон(const Диапазон& r)            { вставьДиапазон(дайСчёт(), r); }

	const T& operator[](цел i) const                { return дай(i); }
	T&       operator[](цел i)                      { return дай(i); }

	T&       добавь()                                  { return вставь(дайСчёт()); }
	T&       добавь(const T& x)                        { return вставь(дайСчёт(), x); }
	проц     добавьН(цел n)                            { вставьН(дайСчёт(), n); }
	T&       добавь(T *newt)                           { вставь(дайСчёт(), newt); return *newt; }
	template<class TT, class... Арги>
	TT&      создай(Арги&&... арги)                 { TT *q = new TT(std::forward<Арги>(арги)...); добавь(q); return *q; }
	
	цел      дайСчёт() const                       { return iv.дайСчёт(); }
	бул     пустой() const                        { return дайСчёт() == 0; }

	проц     обрежь(цел n)                            { удали(n, дайСчёт() - n); }

	проц     устСчёт(цел n);
	проц     очисть();

	T&       по(цел i)                              { if(i >= дайСчёт()) устСчёт(i + 1); return (*this)[i]; }

	проц     сожми()                               { iv.сожми(); }

	проц     уст(цел i, const T& x, цел count);
	T&       уст(цел i, const T& x)                 { уст(i, x, 1); return дай(i); }

	проц     разверни(цел i1, цел i2)                   { РНЦП::разверни((*this)[i1], (*this)[i2]); }

	проц     сбрось(цел n = 1)                        { обрежь(дайСчёт() - n); }
	T&       верх()                                  { return (*this)[дайСчёт() - 1]; }
	const T& верх() const                            { return (*this)[дайСчёт() - 1]; }
	T        вынь()                                  { T h = пикуй(верх()); сбрось(); return h; }

	template <class L>
	цел найдиВерхнГран(const T& знач, const L& less) const  { return iv.найдиВерхнГран((T*)&знач, ALess<L>(less)); }
	цел найдиВерхнГран(const T& знач) const                 { return найдиВерхнГран(знач, StdLess<T>()); }

	template <class L>
	цел найдиНижнГран(const T& знач, const L& less) const  { return iv.найдиНижнГран((T*)&знач, ALess<L>(less)); }
	цел найдиНижнГран(const T& знач) const                 { return найдиНижнГран(знач, StdLess<T>()); }

	template <class L>
	цел InsertUpperBound(const T& знач, const L& lss)       { return iv.InsertUpperBound(new T(знач), ALess<L>(lss)); }
	цел InsertUpperBound(const T& знач)                     { return InsertUpperBound(знач, StdLess<T>()); }
	
	template <class L>
	цел найди(const T& знач, const L& less) const            { return iv.найди((T*)&знач, ALess<L>(less)); }
	цел найди(const T& знач) const                           { return найди(знач, StdLess<T>()); }

	КонстОбходчик    begin() const                  { КонстОбходчик it; SetBegin(it); return it; }
	КонстОбходчик    end() const                    { КонстОбходчик it; SetEnd(it); return it; }

	Обходчик         begin()                        { Обходчик it; SetBegin(it); return it; }
	Обходчик         end()                          { Обходчик it; SetEnd(it); return it; }

	InArray() {}
	InArray(InArray&& v) : iv(пикуй(v.iv))           {}
	InArray& operator=(InArray&& v)                 { if(this != &v) { освободи(); iv.operator=(пикуй(v.iv)); } return *this; }
	InArray(const InArray& v, цел);

	~InArray()                                      { освободи(); }

	InArray(std::initializer_list<T> init)          { for(const auto& i : init) добавь(i); }

	проц разверни(InArray& b)                           { iv.разверни(b.iv); }
	
	проц     сериализуй(Поток& s)                   { StreamContainer(s, *this); }
	проц     вРяр(РярВВ& xio, кткст0 itemtag = "элт");
	проц     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
	т_хэш   дайХэшЗнач() const                        { return хэшПоСериализу(*this); }
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	friend проц разверни(InArray& a, InArray& b)        { a.разверни(b); }
	
	STL_VECTOR_COMPATIBILITY(InArray<T>)

	проц резервируй(цел) {} // Does nothing, but needed for unified interface (e.g. StreamContainer)

#ifdef _ОТЛАДКА
	проц DumpIndex();
#endif

#ifdef DEPRECATED
	КонстОбходчик    дайОбх(цел pos) const         { КонстОбходчик it; SetIter(it, pos); return it; }
	Обходчик         дайОбх(цел pos)               { Обходчик it; SetIter(it, pos); return it; }
	typedef T        ТипЗнач;
#endif
};

template <class T>
class InArray<T>::КонстОбходчик {
	IVConstIter it;

	friend class InArray<T>;
	friend class InArray<T>::Обходчик;

public:
	force_inline цел дайИндекс() const              { return it.дайИндекс(); }

	force_inline КонстОбходчик& operator++()       { ++it; return *this; }
	force_inline КонстОбходчик& operator--()       { --it; return *this; }
	force_inline КонстОбходчик  operator++(цел)    { КонстОбходчик t = *this; ++*this; return t; }
	force_inline КонстОбходчик  operator--(цел)    { КонстОбходчик t = *this; --*this; return t; }

	force_inline КонстОбходчик& operator+=(цел d)  { it += d; return *this; }
	КонстОбходчик& operator-=(цел d)               { return operator+=(-d); }

	КонстОбходчик operator+(цел d) const           { КонстОбходчик t = *this; t += d; return t; }
	КонстОбходчик operator-(цел d) const           { return operator+(-d); }

	цел  operator-(const КонстОбходчик& x) const   { return дайИндекс() - x.дайИндекс(); }

	бул operator==(const КонстОбходчик& b) const  { return it == b.it; }
	бул operator!=(const КонстОбходчик& b) const  { return it != b.it; }
	бул operator<(const КонстОбходчик& b) const   { return it < b.it; }
	бул operator>(const КонстОбходчик& b) const   { return it > b.it; }
	бул operator<=(const КонстОбходчик& b) const  { return it <= b.it; }
	бул operator>=(const КонстОбходчик& b) const  { return it >= b.it; }

	operator бул() const                          { return it; }

	const T& operator*() const                     { return *(T *)*it; }
	const T *operator->() const                    { return (T *)*it; }
	const T& operator[](цел i) const               { КонстОбходчик h = *this; h += i; return *h; }

	STL_ITERATOR_COMPATIBILITY
};

template <class T>
class InArray<T>::Обходчик : public InArray<T>::КонстОбходчик {
	typedef КонстОбходчик B;

public:
	Обходчик& operator++()                         { КонстОбходчик::operator++(); return *this; }
	Обходчик& operator--()                         { КонстОбходчик::operator--(); return *this; }
	Обходчик  operator++(цел)                      { Обходчик t = *this; ++*this; return t; }
	Обходчик  operator--(цел)                      { Обходчик t = *this; --*this; return t; }

	Обходчик& operator+=(цел d)                    { КонстОбходчик::operator+=(d); return *this; }
	Обходчик& operator-=(цел d)                    { return operator+=(-d); }

	Обходчик operator+(цел d) const                { Обходчик t = *this; t += d; return t; }
	Обходчик operator-(цел d) const                { return operator+(-d); }

	цел  operator-(const Обходчик& x) const        { return B::дайИндекс() - x.дайИндекс(); }

	T& operator*()                                 { return *(T *)*B::it; }
	T *operator->()                                { return (T *)*B::it; }
	T& operator[](цел i)                           { Обходчик h = *this; h += i; return *h; }

	const T& operator*() const                     { return *(T *)*B::it; }
	const T *operator->() const                    { return (T *)*B::it; }
	const T& operator[](цел i) const               { Обходчик h = *this; h += i; return *h; }

	STL_ITERATOR_COMPATIBILITY
};

template <class T, class Less = std::less<T> >
class SortedIndex : ОпцияДвижимогоИГлубКопии< SortedIndex<T, Less> > {
	InVector<T> iv;
	
	template <class K, class TT, class Lss> friend class SortedVectorMap;
	template <class K, class TT, class Lss, class Данные> friend class SortedAMap;

public:
	цел           добавь(const T& x)                  { return iv.InsertUpperBound(x, Less()); }
	цел           найдиДобавь(const T& ключ);
	SortedIndex&  operator<<(const T& x)           { добавь(x); return *this; }
	
	цел           найдиНижнГран(const T& x) const { return iv.найдиНижнГран(x, Less()); }
	цел           найдиВерхнГран(const T& x) const { return iv.найдиВерхнГран(x, Less()); }

	цел           найди(const T& x) const           { return iv.найди(x, Less()); }
	цел           найдиСледщ(цел i) const;
	цел           найдиПоследн(const T& x) const;
	цел           найдиПредш(цел i) const;

	проц          удали(цел i)                    { iv.удали(i); }
	проц          удали(цел i, цел count)         { iv.удали(i, count); }
	цел           удалиКлюч(const T& x);

	const T&      operator[](цел i) const          { return iv[i]; }
	цел           дайСчёт() const                 { return iv.дайСчёт(); }
	бул          пустой() const                  { return iv.пустой(); }
	проц          очисть()                          { iv.очисть(); }

	проц          обрежь(цел n)                      { return iv.обрежь(n); }
	проц          сбрось(цел n = 1)                  { iv.сбрось(n); }
	const T&      верх() const                      { return iv.верх(); }

	проц          сожми()                         { iv.сожми(); }
	
	typedef typename InVector<T>::КонстОбходчик КонстОбходчик;

	КонстОбходчик    begin() const                  { return iv.begin(); }
	КонстОбходчик    end() const                    { return iv.end(); }
	
	const InVector<T>& дайКлючи()  const             { return iv; }

	SortedIndex()                                        {}
	SortedIndex(const SortedIndex& s, цел) : iv(s.iv, 1) {}
	
	проц разверни(SortedIndex& a)                        { iv.разверни(a.iv); }

	проц     сериализуй(Поток& s)                               { iv.сериализуй(s); }
	проц     вРяр(РярВВ& xio, кткст0 itemtag = "ключ")    { iv.вРяр(xio, itemtag); }
	проц     вДжейсон(ДжейсонВВ& jio)                               { iv.вДжейсон(jio); }
	Ткст   вТкст() const;
	т_хэш   дайХэшЗнач() const                        { return хэшПоСериализу(*this); }
	
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	friend проц разверни(SortedIndex& a, SortedIndex& b){ a.разверни(b); }

#ifdef DEPRECATED
	КонстОбходчик    дайОбх(цел pos) const         { return iv.дайОбх(pos); }
	
	typedef T        ТипЗнач;

	typedef T             value_type;
	typedef КонстОбходчик const_iterator;
	typedef const T&      const_reference;
	typedef цел           size_type;
	typedef цел           difference_type;
	const_iterator        старт() const          { return begin(); }
	const_iterator        стоп() const            { return end(); }
	проц                  clear()                { очисть(); }
	size_type             size()                 { return дайСчёт(); }
	бул                  empty() const          { return пустой(); }
#endif
};

template <class K, class T, class Less, class Данные>
class SortedAMap {
protected:
	SortedIndex<K, Less> ключ;
	Данные значение;

	проц     SetSlave()                             { ключ.iv.SetSlave(&значение); }
	T&       по(цел i) const                        { return (T&)значение.данные[i]; }

public:
	цел      найдиНижнГран(const K& k) const       { return ключ.найдиНижнГран(k); }
	цел      найдиВерхнГран(const K& k) const       { return ключ.найдиВерхнГран(k); }

	цел      найди(const K& k) const                 { return ключ.найди(k); }
	цел      найдиСледщ(цел i) const                  { return ключ.найдиСледщ(i); }
	цел      найдиПоследн(const K& k) const             { return ключ.найдиПоследн(k); }
	цел      найдиПредш(цел i) const                  { return ключ.найдиПредш(i); }

	T&       дай(const K& k)                        { return по(найди(k)); }
	const T& дай(const K& k) const                  { return по(найди(k)); }
	const T& дай(const K& k, const T& d) const      { цел i = найди(k); return i >= 0 ? по(i) : d; }

	T       *найдиУк(const K& k)                    { цел i = найди(k); return i >= 0 ? &по(i) : NULL; }
	const T *найдиУк(const K& k) const              { цел i = найди(k); return i >= 0 ? &по(i) : NULL; }

	const T& operator[](цел i) const                { return по(i); }
	T&       operator[](цел i)                      { return по(i); }

	const K& дайКлюч(цел i) const                    { return ключ[i]; }

	цел      дайСчёт() const                       { return ключ.дайСчёт(); }
	бул     пустой() const                        { return ключ.пустой(); }
	проц     очисть()                                { ключ.очисть(); }
	проц     сожми()                               { ключ.сожми(); значение.сожми(); }

	проц     удали(цел i)                          { ключ.удали(i); }
	проц     удали(цел i, цел count)               { ключ.удали(i, count); }
	цел      удалиКлюч(const K& k)                  { return ключ.удалиКлюч(k); }

	const SortedIndex<K>& дайИндекс() const          { return ключ; }
	const InVector<K>& дайКлючи() const              { return ключ.дайКлючи(); }

	Ткст   вТкст() const;
	бул     operator==(const SortedAMap& b) const  { return равныМапы(*this, b); }
	бул     operator!=(const SortedAMap& b) const  { return !operator==(b); }
	цел      сравни(const SortedAMap& b) const     { return сравниМап(*this, b); }
	бул     operator<=(const SortedAMap& x) const  { return сравни(x) <= 0; }
	бул     operator>=(const SortedAMap& x) const  { return сравни(x) >= 0; }
	бул     operator<(const SortedAMap& x) const   { return сравни(x) < 0; }
	бул     operator>(const SortedAMap& x) const   { return сравни(x) > 0; }

#ifdef DEPRECATED
	typedef K        ТипКлюча;

	typedef typename SortedIndex<K, Less>::КонстОбходчик КонстОбходчикКлючей;

	КонстОбходчикКлючей стартКлючи() const                             { return ключ.begin(); }
	КонстОбходчикКлючей стопКлючи() const                               { return ключ.end(); }
	КонстОбходчикКлючей дайОбхКлючей(цел pos) const                    { return ключ.дайОбх(pos); }
#endif
};

template <class T>
struct Slaved_InVector__ : InVectorSlave__ {
	typedef InVector<T> Type;
	InVector<T> данные;
	T *res;
      
	virtual проц очисть()                          { данные.очисть(); }
	virtual проц счёт(цел n)                     { данные.count += n; }
	virtual проц вставь(цел blki, цел pos);
	virtual проц разбей(цел blki, цел nextsize);
	virtual проц AddFirst();
	virtual проц RemoveBlk(цел blki, цел n)       { данные.данные.удали(blki, n); }
	virtual проц Join(цел blki)                   { данные.Join(blki); }
	virtual проц удали(цел blki, цел pos, цел n) { данные.данные[blki].удали(pos, n); }
	virtual проц Индекс(цел blki, цел n)           { данные.Индекс(blki, n); }
	virtual проц реиндексируй()                        { данные.реиндексируй(); }
//	virtual проц сериализуй(Поток& s)             { данные.сериализуй(s); }
	virtual проц сожми()                         { данные.сожми(); }

	T& дай(цел blki, цел i) const                 { return *(T*)&данные.данные[blki][i]; }
};

template <class K, class T, class Less = StdLess<K> >
class SortedVectorMap : public SortedAMap<K, T, Less, Slaved_InVector__<T> >,
                        public ОпцияДвижимогоИГлубКопии<SortedVectorMap<K, T, Less> > {
	typedef Slaved_InVector__<T> Данные;
    typedef SortedAMap<K, T, Less, Данные>  B;
    
public:
	T&       добавь(const K& k)                        { B::ключ.добавь(k); return *B::значение.res; }
	T&       добавь(const K& k, const T& x)            { B::ключ.добавь(k); *B::значение.res = клонируй(x); return *B::значение.res; }
	T&       добавь(const K& k, T&& x)                 { B::ключ.добавь(k); *B::значение.res = пикуй(x); return *B::значение.res; }

	цел      найдиДобавь(const K& k)                    { return B::ключ.найдиДобавь(k); }
	цел      найдиДобавь(const K& k, const T& init);

	T&       дайДобавь(const K& k)                     { return B::по(найдиДобавь(k)); }
	T&       дайДобавь(const K& k, const T& x)         { return B::по(найдиДобавь(k, x)); }

	SortedVectorMap& operator()(const K& k, const T& v) { добавь(k, v); return *this; }

	SortedVectorMap()                               { B::SetSlave(); }
	SortedVectorMap(SortedVectorMap&&);
	SortedVectorMap& operator=(SortedVectorMap&&);
	SortedVectorMap(const SortedVectorMap& s, цел);
	
	SortedVectorMap(std::initializer_list<std::pair<K, T>> init) : SortedVectorMap() { for(const auto& i : init) добавь(i.first, i.second); }

	проц     разверни(SortedVectorMap& x);

#ifdef РНЦП
	проц     сериализуй(Поток& s);
	проц     вРяр(РярВВ& xio);
	проц     вДжейсон(ДжейсонВВ& jio);
	т_хэш   дайХэшЗнач() const                   { return хэшПоСериализу(*this); }
#endif

	const InVector<T>& дайЗначения() const            { return B::значение.данные; }

	friend проц    разверни(SortedVectorMap& a, SortedVectorMap& b) { a.разверни(b); }

	typedef typename Данные::Type::КонстОбходчик  КонстОбходчик;
	typedef typename Данные::Type::Обходчик       Обходчик;

	Обходчик         begin()                        { return B::значение.данные.begin(); }
	Обходчик         end()                          { return B::значение.данные.end(); }
	КонстОбходчик    begin() const                  { return B::значение.данные.begin(); }
	КонстОбходчик    end() const                    { return B::значение.данные.end(); }

	STL_SORTED_MAP_COMPATIBILITY(SortedVectorMap<K _cm_ T _cm_ Less>)

#ifdef DEPRECATED
	typedef T                                       ТипЗнач;
	Обходчик         дайОбх(цел pos)               { return B::значение.данные.дайОбх(pos); }
	КонстОбходчик    дайОбх(цел pos) const         { return B::значение.данные.дайОбх(pos); }
#endif
};

template <class T>
struct Slaved_InArray__ : InVectorSlave__ {
	typedef InArray<T> Type;
	InArray<T> данные;
	T         *res;
      
	virtual проц очисть()                          { данные.очисть(); }
	virtual проц счёт(цел n)                     { данные.iv.count += n; }
	virtual проц вставь(цел blki, цел pos);
	virtual проц разбей(цел blki, цел nextsize);
	virtual проц AddFirst();
	virtual проц RemoveBlk(цел blki, цел n)       { данные.iv.данные.удали(blki, n); }
	virtual проц Join(цел blki)                   { данные.iv.Join(blki); }
	virtual проц удали(цел blki, цел pos, цел n);
	virtual проц Индекс(цел blki, цел n)           { данные.iv.Индекс(blki, n); }
	virtual проц реиндексируй()                        { данные.iv.реиндексируй(); }
//	virtual проц сериализуй(Поток& s)             { данные.iv.сериализуй(s); }
	virtual проц сожми()                         { данные.iv.сожми(); }

	T& дай(цел blki, цел i) const                 { return *(T*)данные.iv.данные[blki][i]; }
	T *открепи(цел i)                              { T *x = данные.iv[i]; данные.iv[i] = NULL; return x; }
};

template <class K, class T, class Less = StdLess<K> >
class SortedArrayMap : public ОпцияДвижимогоИГлубКопии<SortedArrayMap<K, T, Less> >,
                        public SortedAMap<K, T, Less, Slaved_InArray__<T> > {
	typedef Slaved_InArray__<T> Данные;
    typedef SortedAMap<K, T, Less, Данные> B;

public:
	T&       добавь(const K& k, const T& x)          { B::значение.res = new T(клонируй(x)); B::ключ.добавь(k); return *(T*)B::значение.res; }
	T&       добавь(const K& k, T&& x)               { B::значение.res = new T(пикуй(x)); B::ключ.добавь(k); return *(T*)B::значение.res; }
	T&       добавь(const K& k)                      { B::значение.res = NULL; B::ключ.добавь(k); return *(T*)B::значение.res; }
	T&       добавь(const K& k, T *newt)             { B::значение.res = newt; B::ключ.добавь(k); return *newt; }
	template <class TT, class... Арги>
	TT&      создай(const K& k, Арги&&... арги)   { TT *q = new TT(std::forward<Арги>(арги)...); B::значение.res = q; B::ключ.добавь(k); return *q; }

	цел      найдиДобавь(const K& k)                  { B::значение.res = NULL; return B::ключ.найдиДобавь(k); }
	цел      найдиДобавь(const K& k, const T& init);

	T&       дайДобавь(const K& k)                   { return B::по(найдиДобавь(k)); }
	T&       дайДобавь(const K& k, const T& x)       { return B::по(найдиДобавь(k, x)); }

	T       *открепи(цел i)                        { T *x = B::значение.открепи(i); B::удали(i); return x; }
	T       *выньОткрепи()                          { return открепи(B::дайСчёт() - 1); }

	const InArray<T>& дайЗначения() const           { return B::значение.данные; }

	SortedArrayMap& operator()(const K& k, const T& v) { добавь(k, v); return *this; }

	SortedArrayMap()                              { B::SetSlave(); }
	SortedArrayMap(SortedArrayMap&&);
	SortedArrayMap& operator=(SortedArrayMap&&);
	SortedArrayMap(const SortedArrayMap& s, цел);
	SortedArrayMap(std::initializer_list<std::pair<K, T>> init) : SortedArrayMap() { for(const auto& i : init) добавь(i.first, i.second); }

#ifdef РНЦП
	проц     сериализуй(Поток& s);
	проц     вРяр(РярВВ& xio);
	проц     вДжейсон(ДжейсонВВ& jio);
	т_хэш   дайХэшЗнач() const                   { return хэшПоСериализу(*this); }
#endif

	проц     разверни(SortedArrayMap& x);

	friend проц    разверни(SortedArrayMap& a, SortedArrayMap& b) { a.разверни(b); }

	typedef typename Данные::Type::КонстОбходчик  КонстОбходчик;
	typedef typename Данные::Type::Обходчик       Обходчик;

	Обходчик         begin()                        { return B::значение.данные.begin(); }
	Обходчик         end()                          { return B::значение.данные.end(); }
	КонстОбходчик    begin() const                  { return B::значение.данные.begin(); }
	КонстОбходчик    end() const                    { return B::значение.данные.end(); }

	STL_SORTED_MAP_COMPATIBILITY(SortedArrayMap<K _cm_ T _cm_ HashFn>)

#ifdef DEPRECATED
	typedef T                                   ТипЗнач;

	Обходчик         дайОбх(цел pos)               { return B::значение.данные.дайОбх(pos); }
	КонстОбходчик    дайОбх(цел pos) const         { return B::значение.данные.дайОбх(pos); }
#endif
};
