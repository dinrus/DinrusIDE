template <class T> struct Slaved_InVector__;
template <class T> struct Slaved_InArray__;
template <class K, class TT, class Lss, class Данные> class SortedAMap;
template <class K, class TT, class Less> class SortedVectorMap;

struct InVectorSlave__ {
	virtual void очисть() = 0;
	virtual void счёт(int n) = 0;
	virtual void разбей(int blki, int nextsize) = 0;
	virtual void AddFirst() = 0;
	virtual void вставь(int blki, int pos) = 0;
	virtual void Join(int blki) = 0;
	virtual void удали(int blki, int pos, int n) = 0;
	virtual void RemoveBlk(int blki, int n) = 0;
	virtual void Индекс(int blki, int n) = 0;
	virtual void реиндексируй() = 0;
	virtual void сожми() = 0;
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
	Вектор< Вектор<int> > индекс;

	int    count;
	int    hcount;
	int64  serial;
	uint16 blk_high;
	uint16 blk_low;
	uint16 slave;

	void подбери(InVector&& b);

	InVectorSlave__ *Slave()          { return (InVectorSlave__ *)((byte *)this + slave); }
	void SetSlave(InVectorSlave__ *s) { slave = (uint16)((byte *)s - (byte *)this); }

	void SetCache(int blki, int offset) const;
	void очистьКэш() const;
	int  FindBlock0(int& pos, int& off) const;
	int  FindBlock(int& pos, int& off) const;
	int  FindBlock(int& pos) const;
	void SetBlkPar();
	
	template <class L>
	int  найдиВерхнГран(const T& val, const L& less, int& off, int& pos) const;

	template <class L>
	int  найдиНижнГран(const T& val, const L& less, int& off, int& pos) const;

	void реиндексируй();
	void Индекс(int q, int n);
	void счёт(int n)                               { count += n; }
	void Join(int blki);

	bool JoinSmall(int blki);
	T   *вставь0(int ii, int blki, int pos, int off, const T *val);
	T   *вставь0(int ii, const T *val);
	
	void переустанов();

	void SetIter(КонстОбходчик& it, int ii) const;
	void SetBegin(КонстОбходчик& it) const;
	void SetEnd(КонстОбходчик& it) const;

	void     иниц();

	template <class Диапазон>
	void     Insert_(int ii, const Диапазон& r, bool опр);

#ifdef flagIVTEST
	void Check(int blki, int offset) const;
#endif
public:
	T&       вставь(int i)                          { return *вставь0(i, NULL); }
	T&       вставь(int i, const T& x)              { return *вставь0(i, &x); }
	template <class Диапазон>
	void     вставьДиапазон(int i, const Диапазон& r)     { Insert_(i, r, false); }
	void     вставьН(int i, int count)              { Insert_(i, ConstRange<T>(count), true); }
	void     удали(int i, int count = 1);
	template <class Диапазон>
	void     приставьДиапазон(const Диапазон& r)            { вставьДиапазон(дайСчёт(), r); }

	const T& operator[](int i) const;
	T&       operator[](int i);

	T&       добавь()                                  { return вставь(дайСчёт()); }
	T&       добавь(const T& x)                        { return вставь(дайСчёт(), x); }
	void     добавьН(int n)                            { вставьН(дайСчёт(), n); }
	
	int      дайСчёт() const                       { return count; }
	bool     пустой() const                        { return дайСчёт() == 0; }

	void     обрежь(int n)                            { удали(n, дайСчёт() - n); }
	void     устСчёт(int n);
	void     очисть();

	T&       по(int i)                              { if(i >= дайСчёт()) устСчёт(i + 1); return (*this)[i]; }

	void     сожми();

	void     уст(int i, const T& x, int count);
	T&       уст(int i, const T& x)                 { уст(i, x, 1); return (*this)(i); }

	void     разверни(int i1, int i2)                   { РНЦП::разверни((*this)[i1], (*this)[i2]); }

	void     сбрось(int n = 1)                        { ПРОВЕРЬ(n <= дайСчёт()); обрежь(дайСчёт() - n); }
	T&       верх()                                  { ПРОВЕРЬ(дайСчёт()); return (*this)[дайСчёт() - 1]; }
	const T& верх() const                            { ПРОВЕРЬ(дайСчёт()); return (*this)[дайСчёт() - 1]; }
	T        вынь()                                  { T h = pick(верх()); сбрось(); return h; }

	template <class L>
	int найдиВерхнГран(const T& val, const L& less) const { int off, pos; найдиВерхнГран(val, less, off, pos); return off + pos; }
	int найдиВерхнГран(const T& val) const                { return найдиВерхнГран(val, StdLess<T>()); }

	template <class L>
	int найдиНижнГран(const T& val, const L& less) const { int off, pos; найдиНижнГран(val, less, off, pos); return off + pos; }
	int найдиНижнГран(const T& val) const                { return найдиНижнГран(val, StdLess<T>()); }

	template <class L>
	int InsertUpperBound(const T& val, const L& less);
	int InsertUpperBound(const T& val)                    { return InsertUpperBound(val, StdLess<T>()); }
	
	template <class L>
	int найди(const T& val, const L& less) const;
	int найди(const T& val) const                         { return найди(val, StdLess<T>()); }

	КонстОбходчик    begin() const                       { КонстОбходчик it; SetBegin(it); return it; }
	КонстОбходчик    end() const                         { КонстОбходчик it; SetEnd(it); return it; }

	Обходчик         begin()                             { Обходчик it; SetBegin(it); return it; }
	Обходчик         end()                               { Обходчик it; SetEnd(it); return it; }

	InVector();
	InVector(InVector&& v)                               { подбери(pick(v)); }
	void operator=(InVector&& v)                         { подбери(pick(v)); }
	InVector(const InVector& v, int);
	InVector(std::initializer_list<T> init)              { иниц(); for(const auto& i : init) добавь(i); }

	void разверни(InVector& b);

	void     сериализуй(Поток& s)                        { StreamContainer(s, *this); }
	void     вРяр(РярВВ& xio, const char *itemtag = "элт");
	void     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
	hash_t   дайХэшЗнач() const                        { return хэшПоСериализу(*this); }
	template <class B> bool operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
	template <class B> int  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> bool operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return сравни(x) > 0; }

	friend void разверни(InVector& a, InVector& b)      { a.разверни(b); }
	
	STL_VECTOR_COMPATIBILITY(InVector<T>)

	void DumpIndex() const;
	
	void резервируй(int) {} // Does nothing, but needed for unified interface (e.g. StreamContainer)
#ifdef DEPRECATED
	typedef T        ТипЗнач;
	КонстОбходчик    дайОбх(int pos) const         { КонстОбходчик it; SetIter(it, pos); return it; }
	Обходчик         дайОбх(int pos)               { Обходчик it; SetIter(it, pos); return it; }
#endif
};

template <class T>
class InVector<T>::КонстОбходчик {
	const T        *ptr;
	const T        *begin;
	const T        *end;
	const InVector *v;
	int             offset;
	int             blki;

	friend class InVector<T>;
	friend class InVector<T>::Обходчик;

	void NextBlk();
	void PrevBlk();

public:
	force_inline int дайИндекс() const              { return int(ptr - begin) + offset; }

	force_inline КонстОбходчик& operator++()       { ПРОВЕРЬ(ptr); if(++ptr == end) NextBlk(); return *this; }
	force_inline КонстОбходчик& operator--()       { ПРОВЕРЬ(ptr); if(ptr == begin) PrevBlk(); --ptr; return *this; }
	force_inline КонстОбходчик  operator++(int)    { КонстОбходчик t = *this; ++*this; return t; }
	force_inline КонстОбходчик  operator--(int)    { КонстОбходчик t = *this; --*this; return t; }

	force_inline КонстОбходчик& operator+=(int d);
	КонстОбходчик& operator-=(int d)               { return operator+=(-d); }

	КонстОбходчик operator+(int d) const           { КонстОбходчик t = *this; t += d; return t; }
	КонстОбходчик operator-(int d) const           { return operator+(-d); }

	int  operator-(const КонстОбходчик& x) const   { return дайИндекс() - x.дайИндекс(); }

	bool operator==(const КонстОбходчик& b) const  { return ptr == b.ptr; }
	bool operator!=(const КонстОбходчик& b) const  { return ptr != b.ptr; }
	bool operator<(const КонстОбходчик& b) const   { return blki == b.blki ? ptr < b.ptr : blki < b.blki; }
	bool operator>(const КонстОбходчик& b) const   { return blki == b.blki ? ptr > b.ptr : blki > b.blki; }
	bool operator<=(const КонстОбходчик& b) const  { return blki == b.blki ? ptr <= b.ptr : blki <= b.blki; }
	bool operator>=(const КонстОбходчик& b) const  { return blki == b.blki ? ptr >= b.ptr : blki >= b.blki; }

	operator bool() const                          { return ptr; }

	const T& operator*() const                     { return *ptr; }
	const T *operator->() const                    { return ptr; }
	const T& operator[](int i) const               { КонстОбходчик h = *this; h += i; return *h; }

	STL_ITERATOR_COMPATIBILITY
};

template <class T>
class InVector<T>::Обходчик : public InVector<T>::КонстОбходчик {
	typedef КонстОбходчик B;
public:
	Обходчик& operator++()                         { КонстОбходчик::operator++(); return *this; }
	Обходчик& operator--()                         { КонстОбходчик::operator--(); return *this; }
	Обходчик  operator++(int)                      { Обходчик t = *this; ++*this; return t; }
	Обходчик  operator--(int)                      { Обходчик t = *this; --*this; return t; }

	Обходчик& operator+=(int d)                    { КонстОбходчик::operator+=(d); return *this; }
	Обходчик& operator-=(int d)                    { return operator+=(-d); }

	Обходчик operator+(int d) const                { Обходчик t = *this; t += d; return t; }
	Обходчик operator-(int d) const                { return operator+(-d); }

	int  operator-(const Обходчик& x) const        { return B::дайИндекс() - x.дайИндекс(); }

	T& operator*()                                 { return *(T*)B::ptr; }
	T *operator->()                                { return (T*)B::ptr; }
	T& operator[](int i)                           { Обходчик h = *this; h += i; return *h; }

	const T& operator*() const                     { return *B::ptr; }
	const T *operator->() const                    { return B::ptr; }
	const T& operator[](int i) const               { КонстОбходчик h = *this; h += i; return *h; }
	
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
	typedef void *ТипУказатель;
#endif
	typedef InVector<ТипУказатель> IV;
	typedef typename InVector<ТипУказатель>::Обходчик IVIter;
	typedef typename InVector<ТипУказатель>::Обходчик IVConstIter;

	IV iv;

	T&       дай(int i) const       { return *(T *)iv[i]; }
	void     Delete(IVIter i, int count);
	void     Delete(int i, int count);
	void     иниц(int i, int count);
	void     освободи()                 { Delete(iv.begin(), дайСчёт()); }

	void     SetIter(КонстОбходчик& it, int ii) const;
	void     SetBegin(КонстОбходчик& it) const;
	void     SetEnd(КонстОбходчик& it) const;

	template <class L>
	struct ALess {
		const L& less;
		
		bool operator()(const ТипУказатель& a, const ТипУказатель& b) const {
			return less(*(T*)a, *(T*)b);
		}
		
		ALess(const L& less) : less(less) {}
	};

	template <class K, class V, class L> friend class SortedVectorMap;

public:
	T&       вставь(int i, T *newt)                 { iv.вставь(i, newt); return *newt; }
	T&       вставь(int i)                          { return вставь(i, new T); }
	T&       вставь(int i, const T& x)              { return вставь(i, new T(x)); }
	template<class TT, class... Арги>
	TT&      InsertCreate(int i, Арги&&... арги)    { TT *q = new TT(std::forward<Арги>(арги)...); вставь(i, q); return *q; }

	void     вставьН(int i, int count);
	template <class Диапазон>
	void     вставьДиапазон(int i, const Диапазон& r);
	void     удали(int i, int count = 1);
	
	template <class Диапазон>
	void     приставьДиапазон(const Диапазон& r)            { вставьДиапазон(дайСчёт(), r); }

	const T& operator[](int i) const                { return дай(i); }
	T&       operator[](int i)                      { return дай(i); }

	T&       добавь()                                  { return вставь(дайСчёт()); }
	T&       добавь(const T& x)                        { return вставь(дайСчёт(), x); }
	void     добавьН(int n)                            { вставьН(дайСчёт(), n); }
	T&       добавь(T *newt)                           { вставь(дайСчёт(), newt); return *newt; }
	template<class TT, class... Арги>
	TT&      создай(Арги&&... арги)                 { TT *q = new TT(std::forward<Арги>(арги)...); добавь(q); return *q; }
	
	int      дайСчёт() const                       { return iv.дайСчёт(); }
	bool     пустой() const                        { return дайСчёт() == 0; }

	void     обрежь(int n)                            { удали(n, дайСчёт() - n); }

	void     устСчёт(int n);
	void     очисть();

	T&       по(int i)                              { if(i >= дайСчёт()) устСчёт(i + 1); return (*this)[i]; }

	void     сожми()                               { iv.сожми(); }

	void     уст(int i, const T& x, int count);
	T&       уст(int i, const T& x)                 { уст(i, x, 1); return дай(i); }

	void     разверни(int i1, int i2)                   { РНЦП::разверни((*this)[i1], (*this)[i2]); }

	void     сбрось(int n = 1)                        { обрежь(дайСчёт() - n); }
	T&       верх()                                  { return (*this)[дайСчёт() - 1]; }
	const T& верх() const                            { return (*this)[дайСчёт() - 1]; }
	T        вынь()                                  { T h = pick(верх()); сбрось(); return h; }

	template <class L>
	int найдиВерхнГран(const T& val, const L& less) const  { return iv.найдиВерхнГран((T*)&val, ALess<L>(less)); }
	int найдиВерхнГран(const T& val) const                 { return найдиВерхнГран(val, StdLess<T>()); }

	template <class L>
	int найдиНижнГран(const T& val, const L& less) const  { return iv.найдиНижнГран((T*)&val, ALess<L>(less)); }
	int найдиНижнГран(const T& val) const                 { return найдиНижнГран(val, StdLess<T>()); }

	template <class L>
	int InsertUpperBound(const T& val, const L& lss)       { return iv.InsertUpperBound(new T(val), ALess<L>(lss)); }
	int InsertUpperBound(const T& val)                     { return InsertUpperBound(val, StdLess<T>()); }
	
	template <class L>
	int найди(const T& val, const L& less) const            { return iv.найди((T*)&val, ALess<L>(less)); }
	int найди(const T& val) const                           { return найди(val, StdLess<T>()); }

	КонстОбходчик    begin() const                  { КонстОбходчик it; SetBegin(it); return it; }
	КонстОбходчик    end() const                    { КонстОбходчик it; SetEnd(it); return it; }

	Обходчик         begin()                        { Обходчик it; SetBegin(it); return it; }
	Обходчик         end()                          { Обходчик it; SetEnd(it); return it; }

	InArray() {}
	InArray(InArray&& v) : iv(pick(v.iv))           {}
	InArray& operator=(InArray&& v)                 { if(this != &v) { освободи(); iv.operator=(pick(v.iv)); } return *this; }
	InArray(const InArray& v, int);

	~InArray()                                      { освободи(); }

	InArray(std::initializer_list<T> init)          { for(const auto& i : init) добавь(i); }

	void разверни(InArray& b)                           { iv.разверни(b.iv); }
	
	void     сериализуй(Поток& s)                   { StreamContainer(s, *this); }
	void     вРяр(РярВВ& xio, const char *itemtag = "элт");
	void     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
	hash_t   дайХэшЗнач() const                        { return хэшПоСериализу(*this); }
	template <class B> bool operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
	template <class B> int  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> bool operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return сравни(x) > 0; }

	friend void разверни(InArray& a, InArray& b)        { a.разверни(b); }
	
	STL_VECTOR_COMPATIBILITY(InArray<T>)

	void резервируй(int) {} // Does nothing, but needed for unified interface (e.g. StreamContainer)

#ifdef _ОТЛАДКА
	void DumpIndex();
#endif

#ifdef DEPRECATED
	КонстОбходчик    дайОбх(int pos) const         { КонстОбходчик it; SetIter(it, pos); return it; }
	Обходчик         дайОбх(int pos)               { Обходчик it; SetIter(it, pos); return it; }
	typedef T        ТипЗнач;
#endif
};

template <class T>
class InArray<T>::КонстОбходчик {
	IVConstIter it;

	friend class InArray<T>;
	friend class InArray<T>::Обходчик;

public:
	force_inline int дайИндекс() const              { return it.дайИндекс(); }

	force_inline КонстОбходчик& operator++()       { ++it; return *this; }
	force_inline КонстОбходчик& operator--()       { --it; return *this; }
	force_inline КонстОбходчик  operator++(int)    { КонстОбходчик t = *this; ++*this; return t; }
	force_inline КонстОбходчик  operator--(int)    { КонстОбходчик t = *this; --*this; return t; }

	force_inline КонстОбходчик& operator+=(int d)  { it += d; return *this; }
	КонстОбходчик& operator-=(int d)               { return operator+=(-d); }

	КонстОбходчик operator+(int d) const           { КонстОбходчик t = *this; t += d; return t; }
	КонстОбходчик operator-(int d) const           { return operator+(-d); }

	int  operator-(const КонстОбходчик& x) const   { return дайИндекс() - x.дайИндекс(); }

	bool operator==(const КонстОбходчик& b) const  { return it == b.it; }
	bool operator!=(const КонстОбходчик& b) const  { return it != b.it; }
	bool operator<(const КонстОбходчик& b) const   { return it < b.it; }
	bool operator>(const КонстОбходчик& b) const   { return it > b.it; }
	bool operator<=(const КонстОбходчик& b) const  { return it <= b.it; }
	bool operator>=(const КонстОбходчик& b) const  { return it >= b.it; }

	operator bool() const                          { return it; }

	const T& operator*() const                     { return *(T *)*it; }
	const T *operator->() const                    { return (T *)*it; }
	const T& operator[](int i) const               { КонстОбходчик h = *this; h += i; return *h; }

	STL_ITERATOR_COMPATIBILITY
};

template <class T>
class InArray<T>::Обходчик : public InArray<T>::КонстОбходчик {
	typedef КонстОбходчик B;

public:
	Обходчик& operator++()                         { КонстОбходчик::operator++(); return *this; }
	Обходчик& operator--()                         { КонстОбходчик::operator--(); return *this; }
	Обходчик  operator++(int)                      { Обходчик t = *this; ++*this; return t; }
	Обходчик  operator--(int)                      { Обходчик t = *this; --*this; return t; }

	Обходчик& operator+=(int d)                    { КонстОбходчик::operator+=(d); return *this; }
	Обходчик& operator-=(int d)                    { return operator+=(-d); }

	Обходчик operator+(int d) const                { Обходчик t = *this; t += d; return t; }
	Обходчик operator-(int d) const                { return operator+(-d); }

	int  operator-(const Обходчик& x) const        { return B::дайИндекс() - x.дайИндекс(); }

	T& operator*()                                 { return *(T *)*B::it; }
	T *operator->()                                { return (T *)*B::it; }
	T& operator[](int i)                           { Обходчик h = *this; h += i; return *h; }

	const T& operator*() const                     { return *(T *)*B::it; }
	const T *operator->() const                    { return (T *)*B::it; }
	const T& operator[](int i) const               { Обходчик h = *this; h += i; return *h; }

	STL_ITERATOR_COMPATIBILITY
};

template <class T, class Less = std::less<T> >
class SortedIndex : ОпцияДвижимогоИГлубКопии< SortedIndex<T, Less> > {
	InVector<T> iv;
	
	template <class K, class TT, class Lss> friend class SortedVectorMap;
	template <class K, class TT, class Lss, class Данные> friend class SortedAMap;

public:
	int           добавь(const T& x)                  { return iv.InsertUpperBound(x, Less()); }
	int           найдиДобавь(const T& ключ);
	SortedIndex&  operator<<(const T& x)           { добавь(x); return *this; }
	
	int           найдиНижнГран(const T& x) const { return iv.найдиНижнГран(x, Less()); }
	int           найдиВерхнГран(const T& x) const { return iv.найдиВерхнГран(x, Less()); }

	int           найди(const T& x) const           { return iv.найди(x, Less()); }
	int           найдиСледщ(int i) const;
	int           найдиПоследн(const T& x) const;
	int           найдиПредш(int i) const;

	void          удали(int i)                    { iv.удали(i); }
	void          удали(int i, int count)         { iv.удали(i, count); }
	int           удалиКлюч(const T& x);

	const T&      operator[](int i) const          { return iv[i]; }
	int           дайСчёт() const                 { return iv.дайСчёт(); }
	bool          пустой() const                  { return iv.пустой(); }
	void          очисть()                          { iv.очисть(); }

	void          обрежь(int n)                      { return iv.обрежь(n); }
	void          сбрось(int n = 1)                  { iv.сбрось(n); }
	const T&      верх() const                      { return iv.верх(); }

	void          сожми()                         { iv.сожми(); }
	
	typedef typename InVector<T>::КонстОбходчик КонстОбходчик;

	КонстОбходчик    begin() const                  { return iv.begin(); }
	КонстОбходчик    end() const                    { return iv.end(); }
	
	const InVector<T>& дайКлючи()  const             { return iv; }

	SortedIndex()                                        {}
	SortedIndex(const SortedIndex& s, int) : iv(s.iv, 1) {}
	
	void разверни(SortedIndex& a)                        { iv.разверни(a.iv); }

	void     сериализуй(Поток& s)                               { iv.сериализуй(s); }
	void     вРяр(РярВВ& xio, const char *itemtag = "ключ")    { iv.вРяр(xio, itemtag); }
	void     вДжейсон(ДжейсонВВ& jio)                               { iv.вДжейсон(jio); }
	Ткст   вТкст() const;
	hash_t   дайХэшЗнач() const                        { return хэшПоСериализу(*this); }
	
	template <class B> bool operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
	template <class B> int  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> bool operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return сравни(x) > 0; }

	friend void разверни(SortedIndex& a, SortedIndex& b){ a.разверни(b); }

#ifdef DEPRECATED
	КонстОбходчик    дайОбх(int pos) const         { return iv.дайОбх(pos); }
	
	typedef T        ТипЗнач;

	typedef T             value_type;
	typedef КонстОбходчик const_iterator;
	typedef const T&      const_reference;
	typedef int           size_type;
	typedef int           difference_type;
	const_iterator        старт() const          { return begin(); }
	const_iterator        стоп() const            { return end(); }
	void                  clear()                { очисть(); }
	size_type             size()                 { return дайСчёт(); }
	bool                  empty() const          { return пустой(); }
#endif
};

template <class K, class T, class Less, class Данные>
class SortedAMap {
protected:
	SortedIndex<K, Less> ключ;
	Данные значение;

	void     SetSlave()                             { ключ.iv.SetSlave(&значение); }
	T&       по(int i) const                        { return (T&)значение.данные[i]; }

public:
	int      найдиНижнГран(const K& k) const       { return ключ.найдиНижнГран(k); }
	int      найдиВерхнГран(const K& k) const       { return ключ.найдиВерхнГран(k); }

	int      найди(const K& k) const                 { return ключ.найди(k); }
	int      найдиСледщ(int i) const                  { return ключ.найдиСледщ(i); }
	int      найдиПоследн(const K& k) const             { return ключ.найдиПоследн(k); }
	int      найдиПредш(int i) const                  { return ключ.найдиПредш(i); }

	T&       дай(const K& k)                        { return по(найди(k)); }
	const T& дай(const K& k) const                  { return по(найди(k)); }
	const T& дай(const K& k, const T& d) const      { int i = найди(k); return i >= 0 ? по(i) : d; }

	T       *найдиУк(const K& k)                    { int i = найди(k); return i >= 0 ? &по(i) : NULL; }
	const T *найдиУк(const K& k) const              { int i = найди(k); return i >= 0 ? &по(i) : NULL; }

	const T& operator[](int i) const                { return по(i); }
	T&       operator[](int i)                      { return по(i); }

	const K& дайКлюч(int i) const                    { return ключ[i]; }

	int      дайСчёт() const                       { return ключ.дайСчёт(); }
	bool     пустой() const                        { return ключ.пустой(); }
	void     очисть()                                { ключ.очисть(); }
	void     сожми()                               { ключ.сожми(); значение.сожми(); }

	void     удали(int i)                          { ключ.удали(i); }
	void     удали(int i, int count)               { ключ.удали(i, count); }
	int      удалиКлюч(const K& k)                  { return ключ.удалиКлюч(k); }

	const SortedIndex<K>& дайИндекс() const          { return ключ; }
	const InVector<K>& дайКлючи() const              { return ключ.дайКлючи(); }

	Ткст   вТкст() const;
	bool     operator==(const SortedAMap& b) const  { return IsEqualMap(*this, b); }
	bool     operator!=(const SortedAMap& b) const  { return !operator==(b); }
	int      сравни(const SortedAMap& b) const     { return CompareMap(*this, b); }
	bool     operator<=(const SortedAMap& x) const  { return сравни(x) <= 0; }
	bool     operator>=(const SortedAMap& x) const  { return сравни(x) >= 0; }
	bool     operator<(const SortedAMap& x) const   { return сравни(x) < 0; }
	bool     operator>(const SortedAMap& x) const   { return сравни(x) > 0; }

#ifdef DEPRECATED
	typedef K        ТипКлюча;

	typedef typename SortedIndex<K, Less>::КонстОбходчик КонстОбходчикКлючей;

	КонстОбходчикКлючей стартКлючи() const                             { return ключ.begin(); }
	КонстОбходчикКлючей стопКлючи() const                               { return ключ.end(); }
	КонстОбходчикКлючей дайОбхКлючей(int pos) const                    { return ключ.дайОбх(pos); }
#endif
};

template <class T>
struct Slaved_InVector__ : InVectorSlave__ {
	typedef InVector<T> Type;
	InVector<T> данные;
	T *res;
      
	virtual void очисть()                          { данные.очисть(); }
	virtual void счёт(int n)                     { данные.count += n; }
	virtual void вставь(int blki, int pos);
	virtual void разбей(int blki, int nextsize);
	virtual void AddFirst();
	virtual void RemoveBlk(int blki, int n)       { данные.данные.удали(blki, n); }
	virtual void Join(int blki)                   { данные.Join(blki); }
	virtual void удали(int blki, int pos, int n) { данные.данные[blki].удали(pos, n); }
	virtual void Индекс(int blki, int n)           { данные.Индекс(blki, n); }
	virtual void реиндексируй()                        { данные.реиндексируй(); }
//	virtual void сериализуй(Поток& s)             { данные.сериализуй(s); }
	virtual void сожми()                         { данные.сожми(); }

	T& дай(int blki, int i) const                 { return *(T*)&данные.данные[blki][i]; }
};

template <class K, class T, class Less = StdLess<K> >
class SortedVectorMap : public SortedAMap<K, T, Less, Slaved_InVector__<T> >,
                        public ОпцияДвижимогоИГлубКопии<SortedVectorMap<K, T, Less> > {
	typedef Slaved_InVector__<T> Данные;
    typedef SortedAMap<K, T, Less, Данные>  B;
    
public:
	T&       добавь(const K& k)                        { B::ключ.добавь(k); return *B::значение.res; }
	T&       добавь(const K& k, const T& x)            { B::ключ.добавь(k); *B::значение.res = clone(x); return *B::значение.res; }
	T&       добавь(const K& k, T&& x)                 { B::ключ.добавь(k); *B::значение.res = pick(x); return *B::значение.res; }

	int      найдиДобавь(const K& k)                    { return B::ключ.найдиДобавь(k); }
	int      найдиДобавь(const K& k, const T& init);

	T&       дайДобавь(const K& k)                     { return B::по(найдиДобавь(k)); }
	T&       дайДобавь(const K& k, const T& x)         { return B::по(найдиДобавь(k, x)); }

	SortedVectorMap& operator()(const K& k, const T& v) { добавь(k, v); return *this; }

	SortedVectorMap()                               { B::SetSlave(); }
	SortedVectorMap(SortedVectorMap&&);
	SortedVectorMap& operator=(SortedVectorMap&&);
	SortedVectorMap(const SortedVectorMap& s, int);
	
	SortedVectorMap(std::initializer_list<std::pair<K, T>> init) : SortedVectorMap() { for(const auto& i : init) добавь(i.first, i.second); }

	void     разверни(SortedVectorMap& x);

#ifdef РНЦП
	void     сериализуй(Поток& s);
	void     вРяр(РярВВ& xio);
	void     вДжейсон(ДжейсонВВ& jio);
	hash_t   дайХэшЗнач() const                   { return хэшПоСериализу(*this); }
#endif

	const InVector<T>& дайЗначения() const            { return B::значение.данные; }

	friend void    разверни(SortedVectorMap& a, SortedVectorMap& b) { a.разверни(b); }

	typedef typename Данные::Type::КонстОбходчик  КонстОбходчик;
	typedef typename Данные::Type::Обходчик       Обходчик;

	Обходчик         begin()                        { return B::значение.данные.begin(); }
	Обходчик         end()                          { return B::значение.данные.end(); }
	КонстОбходчик    begin() const                  { return B::значение.данные.begin(); }
	КонстОбходчик    end() const                    { return B::значение.данные.end(); }

	STL_SORTED_MAP_COMPATIBILITY(SortedVectorMap<K _cm_ T _cm_ Less>)

#ifdef DEPRECATED
	typedef T                                       ТипЗнач;
	Обходчик         дайОбх(int pos)               { return B::значение.данные.дайОбх(pos); }
	КонстОбходчик    дайОбх(int pos) const         { return B::значение.данные.дайОбх(pos); }
#endif
};

template <class T>
struct Slaved_InArray__ : InVectorSlave__ {
	typedef InArray<T> Type;
	InArray<T> данные;
	T         *res;
      
	virtual void очисть()                          { данные.очисть(); }
	virtual void счёт(int n)                     { данные.iv.count += n; }
	virtual void вставь(int blki, int pos);
	virtual void разбей(int blki, int nextsize);
	virtual void AddFirst();
	virtual void RemoveBlk(int blki, int n)       { данные.iv.данные.удали(blki, n); }
	virtual void Join(int blki)                   { данные.iv.Join(blki); }
	virtual void удали(int blki, int pos, int n);
	virtual void Индекс(int blki, int n)           { данные.iv.Индекс(blki, n); }
	virtual void реиндексируй()                        { данные.iv.реиндексируй(); }
//	virtual void сериализуй(Поток& s)             { данные.iv.сериализуй(s); }
	virtual void сожми()                         { данные.iv.сожми(); }

	T& дай(int blki, int i) const                 { return *(T*)данные.iv.данные[blki][i]; }
	T *открепи(int i)                              { T *x = данные.iv[i]; данные.iv[i] = NULL; return x; }
};

template <class K, class T, class Less = StdLess<K> >
class SortedArrayMap : public ОпцияДвижимогоИГлубКопии<SortedArrayMap<K, T, Less> >,
                        public SortedAMap<K, T, Less, Slaved_InArray__<T> > {
	typedef Slaved_InArray__<T> Данные;
    typedef SortedAMap<K, T, Less, Данные> B;

public:
	T&       добавь(const K& k, const T& x)          { B::значение.res = new T(clone(x)); B::ключ.добавь(k); return *(T*)B::значение.res; }
	T&       добавь(const K& k, T&& x)               { B::значение.res = new T(pick(x)); B::ключ.добавь(k); return *(T*)B::значение.res; }
	T&       добавь(const K& k)                      { B::значение.res = NULL; B::ключ.добавь(k); return *(T*)B::значение.res; }
	T&       добавь(const K& k, T *newt)             { B::значение.res = newt; B::ключ.добавь(k); return *newt; }
	template <class TT, class... Арги>
	TT&      создай(const K& k, Арги&&... арги)   { TT *q = new TT(std::forward<Арги>(арги)...); B::значение.res = q; B::ключ.добавь(k); return *q; }

	int      найдиДобавь(const K& k)                  { B::значение.res = NULL; return B::ключ.найдиДобавь(k); }
	int      найдиДобавь(const K& k, const T& init);

	T&       дайДобавь(const K& k)                   { return B::по(найдиДобавь(k)); }
	T&       дайДобавь(const K& k, const T& x)       { return B::по(найдиДобавь(k, x)); }

	T       *открепи(int i)                        { T *x = B::значение.открепи(i); B::удали(i); return x; }
	T       *выньОткрепи()                          { return открепи(B::дайСчёт() - 1); }

	const InArray<T>& дайЗначения() const           { return B::значение.данные; }

	SortedArrayMap& operator()(const K& k, const T& v) { добавь(k, v); return *this; }

	SortedArrayMap()                              { B::SetSlave(); }
	SortedArrayMap(SortedArrayMap&&);
	SortedArrayMap& operator=(SortedArrayMap&&);
	SortedArrayMap(const SortedArrayMap& s, int);
	SortedArrayMap(std::initializer_list<std::pair<K, T>> init) : SortedArrayMap() { for(const auto& i : init) добавь(i.first, i.second); }

#ifdef РНЦП
	void     сериализуй(Поток& s);
	void     вРяр(РярВВ& xio);
	void     вДжейсон(ДжейсонВВ& jio);
	hash_t   дайХэшЗнач() const                   { return хэшПоСериализу(*this); }
#endif

	void     разверни(SortedArrayMap& x);

	friend void    разверни(SortedArrayMap& a, SortedArrayMap& b) { a.разверни(b); }

	typedef typename Данные::Type::КонстОбходчик  КонстОбходчик;
	typedef typename Данные::Type::Обходчик       Обходчик;

	Обходчик         begin()                        { return B::значение.данные.begin(); }
	Обходчик         end()                          { return B::значение.данные.end(); }
	КонстОбходчик    begin() const                  { return B::значение.данные.begin(); }
	КонстОбходчик    end() const                    { return B::значение.данные.end(); }

	STL_SORTED_MAP_COMPATIBILITY(SortedArrayMap<K _cm_ T _cm_ HashFn>)

#ifdef DEPRECATED
	typedef T                                   ТипЗнач;

	Обходчик         дайОбх(int pos)               { return B::значение.данные.дайОбх(pos); }
	КонстОбходчик    дайОбх(int pos) const         { return B::значение.данные.дайОбх(pos); }
#endif
};
