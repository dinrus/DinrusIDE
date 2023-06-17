#ifndef _DinrusPro_Tpl_h_
#define _DinrusPro_Tpl_h_

#include <DinrusPro/DinrusPro.h>
#include <DinrusPro/TplFuncs.h>

class Ткст0 : Движ<Ткст0> {
	enum { //
		РОД = 14,    // chr[РОД] is Ткст tier flag, 0 - small, 31 - medium, 32..254 ref alloc, 255 - read alloc from Реф
		МДЛИН = 15,    // chr[МДЛИН] stores the length of small tier strings (up to 14 bytes)
		БДЛИН = 2,     // chr[БДЛИН] stores the length of medium (< 32) and large tier strings
		СПЕЦ = 13, // chr[СПЕЦ]: stores Значение тип
	};
	enum { // chr[РОД] predefined values, if > СРЕДНИЙ, it is Rc тип; if <255, also stores alloc
		МАЛЫЙ = 0, // SVO ending zero, also has to be 0 because of дайСпец, also flag of small
	    СРЕДНИЙ = 31 // medium, укз points to 32 bytes block on heap
	};

	struct Rc {
		Атомар refcount;
		цел    alloc;

		сим *дайУк() const  { return (сим*)(this + 1); }
	};

	union {
		сим   chr[16];
		ткст0 укз; // medium and large tier, in large tier (Rc *)укз - 1 points to Rc
		бцел *wptr;
		дим *qptr;
		бкрат   v[8];
		бцел  w[4];
		дим  q[2];
	};


#if defined(_ОТЛАДКА) && defined(COMPILER_GCC)
	цел          len;
	кткст0 s;
#endif

#ifdef _ОТЛАДКА
	проц Dsyn();
#else
	проц Dsyn() {}
#endif

	сим&  мДлин()                { return chr[МДЛИН]; }
	сим   мДлин() const          { return chr[МДЛИН]; }
	бцел& бДлин()                { return w[БДЛИН]; }
	бцел  бДлин() const          { return w[БДЛИН]; }
	бул   малый() const       { return chr[РОД] == МАЛЫЙ; }
	бул   большой() const       { return chr[РОД] != МАЛЫЙ; }
	бул   средний() const      { return chr[РОД] == СРЕДНИЙ; }
	бул   реф_ли() const         { return (ббайт)chr[РОД] > СРЕДНИЙ; }
	Rc    *Реф() const           { return (Rc *)укз - 1; }
	бул   совместный() const      { return Реф()->refcount != 1; }
	бул   шарссыл_ли() const   { return реф_ли() && совместный(); }
	цел    бРазмести() const        { цел b = (ббайт)chr[РОД]; return b == 255 ? Реф()->alloc : b; }
	бцел  бРавен(const Ткст0& s) const;

	проц бУст(const Ткст0& s);
	проц бОсвободи();
	проц бКат(цел c);
	т_хэш бЗначХэша() const;

	проц разшарь();
	проц устМДлин(цел l);

	сим *Ук()                   { return малый() ? chr : укз; }
	сим *размести_(цел count, сим& kind);
	сим *размести(цел count, сим& kind);

	static Ткст0::Rc voidptr[2];

	проц разверни(Ткст0& b);

	// интерфейс к Значение
	static бцел StW(ббайт st)     { return СДЕЛАЙ4Б(0, st, 0, 0); }
	проц устСпец0(ббайт st)     { w[3] = StW(st); }
	проц устСпец(ббайт st)      { ПРОВЕРЬ(малый() && дайСчёт() == 0); устСпец0(st); }
	ббайт дайСпец() const       { return (chr[МДЛИН] | chr[РОД]) == 0 ? chr[СПЕЦ] : 0; }
	ббайт GetSt() const            { return chr[СПЕЦ]; }
	бцел GetStW() const          { return w[3]; }
	бул спец_ли() const        { return !v[7] && v[6]; }
	бул спец_ли(ббайт st) const { return w[3] == StW(st); }

	friend class Ткст;
	friend class ТкстБуф;
	friend class Значение;
	friend class ТекстКтрл;

protected:
	проц обнули()                     { q[0] = q[1] = 0; Dsyn(); }
	проц устМалый(const Ткст0& s) { q[0] = s.q[0]; q[1] = s.q[1]; }
	проц освободи()                     { if(большой()) бОсвободи(); }
	проц пикуй0(Ткст0&& s) {
		устМалый(s);
		s.обнули();
	}
	проц уст0(const Ткст0& s) {
		if(s.малый()) устМалый(s); else бУст(s);
		Dsyn();
	}
	проц присвой(const Ткст0& s) {
		if(s.малый()) {
			освободи();
			устМалый(s);
		}
		else
			if(this != &s) {
				освободи();
				бУст(s);
			}
		Dsyn();
	}
	проц  уст0(кткст0 s, цел len);
	проц  устБ(кткст0 s, цел len);
	сим *вставь(цел pos, цел count, кткст0 str);

	typedef сим         tchar;
	typedef ббайт         bchar;
	typedef ТкстБуф Буфер;
	typedef Ткст  Ткст;

	цел    сравниБ(const Ткст0& s) const;

public:
	бул бРавн(const Ткст0& s) const;
	бул равен(const Ткст0& s) const {
		бдол q1 = q[1];
		бдол sq1 = s.q[1];
		return q1 == sq1 && q[0] == s.q[0] || ((q1 | sq1) & СДЕЛАЙ8Б(0,0,0,0,0,0,255,0)) && бРавн(s);
	}
	бул равен(кткст0 s) const;

	цел    сравни(const Ткст0& s) const;

	т_хэш дайХэшЗнач() const {
#ifdef ХЭШ64
		return chr[РОД] ? бЗначХэша() : (т_хэш)КомбХэш(q[0], q[1]);
#else
		return chr[РОД] ? бЗначХэша() : (т_хэш)КомбХэш(w[0], w[1], w[2], w[3]);
#endif
	}

	проц кат(цел c) {
		if(мДлин() < 14)
			chr[цел(мДлин()++)] = c;
		else
			бКат(c);
		Dsyn();
	}

	проц кат(кткст0 s, цел len);
	проц уст(кткст0 s, цел len);

	проц уст(цел i, цел chr);
	проц обрежь(цел pos);

	кткст0 старт() const   { return малый() ? chr : укз; }
	кткст0 begin() const   { return старт(); }
	кткст0 стоп() const     { return старт() + дайДлину(); }
	кткст0 end() const     { return стоп(); }

	цел operator[](цел i) const { ПРОВЕРЬ(i >= 0 && i <= дайСчёт()); return старт()[i]; }

	operator кткст0 () const   { return старт(); }
	кткст0 operator~() const   { return старт(); }

	проц удали(цел pos, цел count = 1);
	проц очисть()                { освободи(); обнули(); }

	цел дайСчёт() const        { return малый() ? chr[МДЛИН] : w[БДЛИН]; }
	цел дайДлину() const       { return дайСчёт(); }
	цел дайРазмест() const        { return малый() ? 14 : бРазмести(); }

	проц резервируй(цел r);

	Ткст0()                   {}
	~Ткст0()                  { освободи(); }
};

class Ткст : public Движ<Ткст, АТкст<Ткст0> > {
	проц разверни(Ткст& b)                                   { Ткст0::разверни(b); }

#ifdef _ОТЛАДКА
#ifndef COMPILER_GCC
	цел          len;
	кткст0 s;
#endif
	friend class Ткст0;
#endif

	проц присвойДлин(кткст0 s, цел МДЛИН);

	enum SSPECIAL { СПЕЦ };

	template <class T>
	Ткст(const T& x, ббайт st, SSPECIAL) {
		*(T*)chr = x;
		устСпец0(st);
	}
	Ткст(SSPECIAL) {}

	friend class Значение;

public:
	const Ткст& operator+=(сим c)                       { кат(c); return *this; }
	const Ткст& operator+=(кткст0 s)                { кат(s); return *this; }
	const Ткст& operator+=(const Ткст& s)              { кат(s); return *this; }

	Ткст& operator=(кткст0 s);
	Ткст& operator=(const Ткст& s)                     { Ткст0::присвой(s); return *this; }
	Ткст& operator=(Ткст&& s)                          { if(this != &s) { освободи(); пикуй0(пикуй(s)); } return *this; }
	Ткст& operator=(ТкстБуф& b)                     { *this = Ткст(b); return *this; }

	проц   сожми()                                        { *this = Ткст(старт(), дайДлину()); }
	цел    дайСчётСим() const;

	Ткст()                                               { обнули(); }
	Ткст(const Обнул&)                                  { обнули(); }
	Ткст(const Ткст& s)                                { Ткст0::уст0(s); }
	Ткст(Ткст&& s)                                     { Ткст0::пикуй0(пикуй(s)); }
	Ткст(кткст0 s);
	Ткст(const Ткст& s, цел n)                         { ПРОВЕРЬ(n >= 0 && n <= s.дайДлину()); Ткст0::уст0(~s, n); }
	Ткст(кткст0 s, цел n)                           { Ткст0::уст0(s, n); }
	Ткст(const ббайт *s, цел n)                           { Ткст0::уст0((кткст0 )s, n); }
	Ткст(кткст0 s, кткст0 lim)                 { Ткст0::уст0(s, (цел)(lim - s)); }
	Ткст(цел chr, цел count)                             { Ткст0::обнули(); кат(chr, count); }
	Ткст(ТкстБуф& b);


	Ткст(сим16 *s);
	Ткст(шим *s);

	ШТкст вШТкст() const;
	const Ткст& вТкст() const                         { return *this; }

	static Ткст дайПроц();
	бул   проц_ли() const;

	friend проц разверни(Ткст& a, Ткст& b)                 { a.разверни(b); }

	Ткст(const std::string& s)                           { Ткст0::уст0(s.c_str(), (цел)s.length()); }
	std::string вСтд() const                              { return std::string(старт(), стоп()); }

	template <class Делец>
	static Ткст сделай(цел alloc, Делец m);
};

class ШТкст : public Движ<ШТкст, АТкст<ШТкст0> >
{
	проц разверни(ШТкст& b)                                   { ШТкст0::разверни(b); }

#ifdef _ОТЛАДКА
	цел          len;
	const шим *s;
	friend class ШТкст0;
#endif

public:
	const ШТкст& operator+=(шим c)                      { кат(c); return *this; }
	const ШТкст& operator+=(const шим *s)               { кат(s); return *this; }
	const ШТкст& operator+=(const ШТкст& s)             { кат(s); return *this; }

	ШТкст& operator<<(шим c)                            { кат(c); return *this; }
	ШТкст& operator<<(const ШТкст& s)                   { кат(s); return *this; }
	ШТкст& operator<<(const шим *s)                     { кат(s); return *this; }

	ШТкст& operator=(const шим *s);
	ШТкст& operator=(const ШТкст& s)                    { if(this != &s) { ШТкст0::FFree(); ШТкст0::уст0(s); } return *this; }
	ШТкст& operator=(ШТкст&& s)                         { if(this != &s) { ШТкст0::FFree(); ШТкст0::пикуй0(пикуй(s)); } return *this; }
	ШТкст& operator=(ШТкстБуф& b)                    { *this = ШТкст(b); return *this; }
	ШТкст& operator<<=(const ШТкст& s)                  { if(this != &s) { ШТкст0::освободи(); ШТкст0::уст0(s, s.дайСчёт()); } return *this; }

	проц   сожми()                                         { *this = ШТкст(старт(), дайДлину()); }

	ШТкст()                                               {}
	ШТкст(const Обнул&)                                  {}
	ШТкст(const ШТкст& s)                               { ШТкст0::уст0(s); }
	ШТкст(ШТкст&& s)                                    { ШТкст0::пикуй0(пикуй(s)); }
	ШТкст(const шим *s)                                 { ШТкст0::уст0(s, длинтекс__(s)); }
	ШТкст(const ШТкст& s, цел n)                        { ПРОВЕРЬ(n >= 0 && n <= s.дайДлину()); ШТкст0::уст0(~s, n); }
	ШТкст(const шим *s, цел n)                          { ШТкст0::уст0(s, n); }
	ШТкст(const шим *s, const шим *lim)               { ШТкст0::уст0(s, (цел)(lim - s)); }
	ШТкст(цел chr, цел count)                             { ШТкст0::обнули(); кат(chr, count); }
	ШТкст(ШТкстБуф& b);

	ШТкст(кткст0 s);
	ШТкст(кткст0 s, цел n);
	ШТкст(кткст0 s, кткст0 lim);

	ШТкст(const сим16 *s);

	static ШТкст дайПроц();
	бул   проц_ли() const                                   { return alloc < 0; }

	friend проц разверни(ШТкст& a, ШТкст& b)                { a.разверни(b); }
	friend ШТкст operator+(const ШТкст& a, сим b)      { ШТкст c(a); c += b; return c; }
	friend ШТкст operator+(ШТкст&& a, сим b)           { ШТкст c(пикуй(a)); c += b; return c; }
	friend ШТкст operator+(сим a, const ШТкст& b)      { ШТкст c(a, 1); c += b; return c; }

    ::Ткст вТкст() const                                   { return изЮникода(*this, ДЕФНАБСИМ);}

#ifndef _HAVE_NO_STDWSTRING
	ШТкст(const std::wstring& s);
	operator std::wstring() const                           { return вСтд(); }
	std::wstring вСтд() const;
#endif
};


template <цел size>
struct Data_S_ : Движ< Data_S_<size> >
{
	ббайт filler[size];
};

template <class T>
class Буфер : Движ< Буфер<T> > {
	T *укз;

	проц разместпам(т_мера size) {
		if(std::is_trivially_destructible<T>::value)
			укз = (T *)разместиПам(size * sizeof(T));
		else {
			ук p = разместиПам(size * sizeof(T) + 16);
			*(т_мера *)p = size;
			укз = (T *)((ббайт *)p + 16);
		}
	}
	проц нов(т_мера size) {
		разместпам(size);
		строй(укз, укз + size);
	}
	проц нов(т_мера size, const T& in) {
		разместпам(size);
		стройЗаполниГлубКопию(укз, укз + size, in);
	}
	проц освободи() {
		if(укз) {
			if(std::is_trivially_destructible<T>::value)
				освободиПам(укз);
			else {
				ук p = (ббайт *)укз - 16;
				т_мера size = *(т_мера *)p;
				разрушь(укз, укз + size);
				освободиПам(p);
			}
		}
	}

public:
	operator T*()                        { return укз; }
	operator const T*() const            { return укз; }
	T *operator~()                       { return укз; }
	const T *operator~() const           { return укз; }
	T          *дай()                    { return укз; }
	const T    *дай() const              { return укз; }
	T          *begin()                  { return укз; }
	const T    *begin() const            { return укз; }

	проц размести(т_мера size)              { очисть(); нов(size); }
	проц размести(т_мера size, const T& in) { очисть(); нов(size, in); }

	проц очисть()                         { освободи(); укз = NULL; }
	бул пустой() const                 { return укз == NULL; }

	Буфер()                             { укз = NULL; }
	Буфер(т_мера size)                  { нов(size); }
	Буфер(т_мера size, const T& init)   { нов(size, init); }
	~Буфер()                            { освободи(); }

	проц operator=(Буфер&& v)           { if(&v != this) { очисть(); укз = v.укз; v.укз = NULL; } }
	Буфер(Буфер&& v)                   { укз = v.укз; v.укз = NULL; }

	Буфер(т_мера size, std::initializer_list<T> init) {
		разместпам(size); T *t = укз; for(const auto& i : init) new (t++) T(i);
	}
	Буфер(std::initializer_list<T> init) : Буфер(init.size(), init) {}
};

template <class T>
class Один : ОпцияДвижимогоИГлубКопии< Один<T> > {
	mutable T  *укз;

	проц        освободи()                     { if(укз && укз != (T*)1) delete укз; }
	template <class TT>
	проц        подбери(Один<TT>&& данные)       { укз = данные.открепи(); }

public:
	проц        прикрепи(T *данные)            { освободи(); укз = данные; }
	T          *открепи()                   { T *t = укз; укз = NULL; return t; }
	проц        очисть()                    { освободи(); укз = NULL; }

	проц        operator=(T *данные)         { прикрепи(данные); }

	template <class TT>
	проц        operator=(Один<TT>&& d)     { if((проц *)this != (проц *)&d) { освободи(); подбери(пикуй(d)); }}

	const T    *operator->() const         { ПРОВЕРЬ(укз); return укз; }
	T          *operator->()               { ПРОВЕРЬ(укз); return укз; }
	const T    *operator~() const          { return укз; }
	T          *operator~()                { return укз; }
	const T    *дай() const                { return укз; }
	T          *дай()                      { return укз; }
	const T&    operator*() const          { ПРОВЕРЬ(укз); return *укз; }
	T&          operator*()                { ПРОВЕРЬ(укз); return *укз; }

	template <class TT, class... Арги>
	TT&         создай(Арги&&... арги)     { TT *q = new TT(std::forward<Арги>(арги)...); прикрепи(q); return *q; }
	template <class TT> // with C++ conforming compiler, this would not be needed - GCC bug workaround
	TT&         создай()                   { TT *q = new TT; прикрепи(q); return *q; }
	template <class... Арги>
	T&          создай(Арги&&... арги)     { T *q = new T(std::forward<Арги>(арги)...); прикрепи(q); return *q; }
	T&          создай()                   { T *q = new T; прикрепи(q); return *q; }

	template <class TT>
	бул        является() const                 { return dynamic_cast<const TT *>(укз); }

	бул        пустой() const            { return !укз; }

	operator бул() const                  { return укз; }

	Ткст вТкст() const                { return укз ? какТкст(*укз) : "<empty>"; }

	Один()                                  { укз = NULL; }
	Один(T *newt)                           { укз = newt; }
	template <class TT>
	Один(Один<TT>&& p)                       { подбери(пикуй(p)); }
	Один(const Один<T>& p, цел)              { укз = p.пустой() ? NULL : new T(клонируй(*p)); }
	Один(const Один<T>& p) = delete;
	проц operator=(const Один<T>& p) = delete;
	~Один()                                 { освободи(); }
};

template <class T, class... Арги>
Один<T> сделайОдин(Арги&&... арги) {
	Один<T> r;
	r.создай(std::forward<Арги>(арги)...);
	return r;
}

template <class T>
class Вектор : public ОпцияДвижимогоИГлубКопии< Вектор<T> > {
	T       *vector;
	цел      items;
	цел      alloc;

	static проц    освободиРяд(T *укз)            { if(укз) освободиПам(укз); }
	static T      *разместиРяд(цел& n);

	проц     обнули()                          { vector = NULL; items = alloc = 0; }
	проц     подбери(Вектор<T>&& v);

	T       *Rdd()                           { return vector + items++; }

	проц     освободи();
	проц     __глубКопия(const Вектор& ист);
	T&       дай(цел i) const                { ПРОВЕРЬ(i >= 0 && i < items); return vector[i]; }
	бул     перемест(цел alloc);
	проц     ReAllocF(цел alloc);
	бул     GrowSz();
	проц     GrowF();
	T&       нарастиДобавь(const T& x);
	T&       нарастиДобавь(T&& x);
	проц     вставьРяд(цел q, цел count);

	template <class U> friend class Индекс;

public:
	T&       добавь()                           { if(items >= alloc) GrowF(); return *(::new(Rdd()) T); }
	T&       добавь(const T& x)                 { return items < alloc ? *(new(Rdd()) T(x)) : нарастиДобавь(x); }
	T&       добавь(T&& x)                      { return items < alloc ? *(::new(Rdd()) T(пикуй(x))) : нарастиДобавь(пикуй(x)); }
	template <class... Арги>
	T&       создай(Арги&&... арги)          { if(items >= alloc) GrowF(); return *(::new(Rdd()) T(std::forward<Арги>(арги)...)); }
	проц     добавьН(цел n);
	const T& operator[](цел i) const         { return дай(i); }
	T&       operator[](цел i)               { return дай(i); }
	const T& дай(цел i, const T& опр) const  { return i >= 0 && i < дайСчёт() ? дай(i) : опр; }
	T&       дай(цел i, T& опр)              { return i >= 0 && i < дайСчёт() ? дай(i) : опр; }
	цел      дайСчёт() const                { return items; }
	бул     пустой() const                 { return items == 0; }
	проц     обрежь(цел n);
	проц     устСчёт(цел n);
	проц     устСчёт(цел n, const T& init);
	проц     устСчётР(цел n);
	проц     устСчётР(цел n, const T& init);
	проц     очисть();

	T&       по(цел i)                  { if(i >= items) устСчётР(i + 1); return (*this)[i]; }
	T&       по(цел i, const T& x)      { if(i >= items) устСчётР(i + 1, x); return (*this)[i]; }

	проц     сожми()                   { if(items != alloc) ReAllocF(items); }
	проц     резервируй(цел n);
	цел      дайРазмест() const           { return alloc; }

	проц     уст(цел i, const T& x, цел count);
	T&       уст(цел i, const T& x)     { уст(i, x, 1); return дай(i); }
	T&       уст(цел i, T&& x)          { return по(i) = пикуй(x); }
	template <class Диапазон>
	проц     устДиапазон(цел i, const Диапазон& r);

	проц     удали(цел i, цел count = 1);
	проц     удали(const цел *sorted_list, цел n);
	проц     удали(const Вектор<цел>& sorted_list);
	template <class Условие>
	проц     удалиЕсли(Условие c);

	проц     вставьН(цел i, цел count = 1);
	T&       вставь(цел i)              { вставьН(i); return дай(i); }
	проц     вставь(цел i, const T& x, цел count);
	T&       вставь(цел i, const T& x)  { вставь(i, x, 1); return дай(i); }
	T&       вставь(цел i, T&& x);
	проц     вставь(цел i, const Вектор& x);
	проц     вставь(цел i, const Вектор& x, цел offset, цел count);
	проц     вставь(цел i, Вектор&& x);
	template <class Диапазон>
	проц     вставьДиапазон(цел i, const Диапазон& r);
	проц     приставь(const Вектор& x)               { вставь(дайСчёт(), x); }
	проц     приставь(const Вектор& x, цел o, цел c) { вставь(дайСчёт(), x, o, c); }
	проц     приставь(Вектор&& x)                    { вставь(дайСчёт(), пикуй(x)); }
	template <class Диапазон>
	проц     приставьДиапазон(const Диапазон& r)           { вставьДиапазон(дайСчёт(), r); }

	проц     вставьРазбей(цел i, Вектор<T>& v, цел from);

	проц     разверни(цел i1, цел i2)    { разверни(дай(i1), дай(i2)); }

	проц     сбрось(цел n = 1)         { ПРОВЕРЬ(n <= дайСчёт()); обрежь(items - n); }
	T&       верх()                   { ПРОВЕРЬ(дайСчёт()); return дай(items - 1); }
	const T& верх() const             { ПРОВЕРЬ(дайСчёт()); return дай(items - 1); }
	T        вынь()                   { T h = пикуй(верх()); сбрось(); return h; }

	operator T*()                    { return (T*)vector; }
	operator const T*() const        { return (T*)vector; }

	Вектор&  operator<<(const T& x)  { добавь(x); return *this; }
	Вектор&  operator<<(T&& x)            { добавь(пикуй(x)); return *this; }

#ifdef РНЦП
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
#endif

	Вектор()                                  { обнули(); }
	explicit Вектор(цел n)                    { items = n; vector = разместиРяд(n); alloc = n; строй(vector, vector + n); }
	explicit Вектор(цел n, const T& init)     { items = n; vector = разместиРяд(n); alloc = n; стройЗаполниГлубКопию(vector, vector + n, init); }
	~Вектор() {
		освободи();
		return; // Constraints:
		проверьДвиж((T *)0);  // T must be moveable
	}

// подбери assignment & copy. Picked source can only do очисть(), ~Вектор(), operator=, operator <<=
	Вектор(Вектор&& v)               { подбери(пикуй(v)); }
	проц operator=(Вектор&& v)       { if(this != &v) { освободи(); подбери(пикуй(v)); } }

	проц     вставь(цел i, std::initializer_list<T> init);
	проц     приставь(std::initializer_list<T> init) { вставь(дайСчёт(), init); }
	Вектор(std::initializer_list<T> init) { vector = NULL; items = alloc = 0; вставь(0, init); }

// Deep copy
	Вектор(const Вектор& v, цел)     { __глубКопия(v); }

// Standard container interface
	const T         *begin() const          { return (T*)vector; }
	const T         *end() const            { return (T*)vector + items; }
	T               *begin()                { return (T*)vector; }
	T               *end()                  { return (T*)vector + items; }

// Optimizations
	friend проц разверни(Вектор& a, Вектор& b)  { ::разверни(a.items, b.items); ::разверни(a.alloc, b.alloc); ::разверни(a.vector, b.vector); }

#ifdef DEPRECATED
	T&       индексируй(цел i)             { return по(i); }
	T&       индексируй(цел i, const T& x) { return по(i, x); }
	T&       добавьПодбор(T&& x)             { return items < alloc ? *(::new(Rdd()) T(пикуй(x))) : нарастиДобавь(пикуй(x)); }
	цел      дайИндекс(const T& элт) const;
	T&       вставьПодбор(цел i, T&& x)   { return вставь(i, пикуй(x)); }
	проц     вставьПодбор(цел i, Вектор&& x) { вставь(i, пикуй(x)); }
	проц     приставьПодбор(Вектор&& x)                { вставьПодбор(дайСчёт(), пикуй(x)); }
	typedef T       *Обходчик;
	typedef const T *КонстОбходчик;
	КонстОбходчик    дайОбх(цел i) const   { ПРОВЕРЬ(i >= 0 && i <= items); return begin() + i; }
	Обходчик         дайОбх(цел i)         { ПРОВЕРЬ(i >= 0 && i <= items); return begin() + i; }
	typedef T        ТипЗнач;
	friend проц приставь(Вектор& dst, const Вектор& ист)         { dst.приставь(ист); }

	СОВМЕСТИМОСТЬ_С_ВЕКТОРОМ_СТЛ(Вектор<T>)
#endif
};

template <class T>
class Индекс : ОпцияДвижимогоИГлубКопии<Индекс<T>>, ИндексОбщее {
	Вектор<T> ключ;

	static бцел Smear(const T& k)   { return ИндексОбщее::Smear(дайХэшЗнач(k)); }

	цел  найдиОт(цел i, бцел sh, const T& k, цел end) const;
	цел  FindBack(цел i, бцел sh, const T& k, цел end) const;


	проц переразместиХэш(цел n);
	template <typename U> проц нарастиДобавь(U&& k, бцел sh);
	template <typename U> проц AddS(цел& m, U&& k, бцел sh);
	template <typename U> проц AddS(U&& k, бцел sh);

	template <class OP, class U> цел найдиДобавь(U&& k, OP add_op);
	template <class U> цел FindPut0(U&& k);

	template <typename U> цел Put0(U&& k, бцел sh);
	template <typename U> проц уст0(цел i, U&& k);

	template <typename, typename, typename> friend class АМап;

	проц        фиксируйХэш(бул makemap = true);

public:
	проц        добавь(const T& k)             { AddS(k, Smear(k)); }
	проц        добавь(T&& k)                  { AddS(пикуй(k), Smear(k)); }
	Индекс&      operator<<(const T& x)      { добавь(x); return *this; }
	Индекс&      operator<<(T&& x)           { добавь(пикуй(x)); return *this; }

	цел         найди(const T& k) const;
	цел         найдиСледщ(цел i) const;
	цел         найдиПоследн(const T& k) const;
	цел         найдиПредш(цел i) const;

	цел         найдиДобавь(const T& k)         { return найдиДобавь(k, []{}); }
	цел         найдиДобавь(T&& k)              { return найдиДобавь(пикуй(k), []{}); }

	цел         помести(const T& k)             { return Put0(k, Smear(k)); }
	цел         помести(T&& k)                  { return Put0(пикуй(k), Smear(k)); }
	цел         найдиПомести(const T& k)         { return FindPut0(k); }
	цел         найдиПомести(T&& k)              { return FindPut0(пикуй(k)); }

	проц        отлинкуй(цел i);
	цел         отлинкуйКлюч(const T& k);
	бул        отлинкован(цел i) const      { return hash[i].hash == 0; }
	бул        HasUnlinked() const          { return unlinked >= 0; }
	Вектор<цел> дайОтлинкованно() const          { return ИндексОбщее::дайОтлинкованно(); }

	проц        смети();

	проц        уст(цел i, const T& k)       { уст0(i, k); }
	проц        уст(цел i, T&& k)            { уст0(i, пикуй(k)); }

	const T&    operator[](цел i) const      { return ключ[i]; }
	цел         дайСчёт() const             { return ключ.дайСчёт(); }
	бул        пустой() const              { return ключ.пустой(); }

	проц        очисть()                      { ключ.очисть(); ИндексОбщее::очисть(); }

	проц        обрежь(цел n = 0)              { ИндексОбщее::обрежь(n, дайСчёт()); ключ.обрежь(n); }
	проц        сбрось(цел n = 1)              { обрежь(дайСчёт() - 1); }
	const T&    верх() const                  { return ключ.верх(); }
	T           вынь()                        { T x = пикуй(верх()); сбрось(); return x; }

	проц        резервируй(цел n);
	проц        сожми();
	цел         дайРазмест() const             { return ключ.дайРазмест(); }

	Вектор<T>        подбериКлючи()              { Вектор<T> r = пикуй(ключ); очисть(); return r; }
	const Вектор<T>& дайКлючи() const         { return ключ; }

	проц     удали(const цел *sorted_list, цел count);
	проц     удали(const Вектор<цел>& sorted_list)         { удали(sorted_list, sorted_list.дайСчёт()); }
	template <typename Предикат> проц удалиЕсли(Предикат p)          { удали(найдиВсеи(ключ, p)); }

	Индекс()                                                 {}
	Индекс(Индекс&& s) : ключ(пикуй(s.ключ))                     { ИндексОбщее::подбери(s); }
	Индекс(const Индекс& s, цел) : ключ(s.ключ, 0)              { переразместиХэш(0); ИндексОбщее::копируй(s, ключ.дайСчёт()); } // TODO: Unlinked!
	explicit Индекс(Вектор<T>&& s) : ключ(пикуй(s))            { фиксируйХэш(); }
	Индекс(const Вектор<T>& s, цел) : ключ(s, 0)              { фиксируйХэш(); }

	Индекс& operator=(Вектор<T>&& x)                         { ключ = пикуй(x); фиксируйХэш(); return *this; }
	Индекс& operator=(Индекс<T>&& x)                          { ключ = пикуй(x.ключ); ИндексОбщее::подбери(x); return *this; }

	Индекс(std::initializer_list<T> init) : ключ(init)        { фиксируйХэш(); }

	проц     сериализуй(Поток& s);
	проц     вРяр(РярВВ& xio, кткст0 itemtag = "ключ");
	проц     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

// Standard container interface
	typedef КонстОбходчикУ<Вектор<T>> КонстОбходчик;
	КонстОбходчик begin() const                             { return ключ.begin(); }
	КонстОбходчик end() const                               { return ключ.end(); }

	friend проц разверни(Индекс& a, Индекс& b)                    { a.ИндексОбщее::разверни(b); разверни(a.ключ, b.ключ); }

// deprecated:
#ifdef DEPRECATED
	T&       вставь(цел i, const T& k)                      { ключ.вставь(i, k); фиксируйХэш(); return ключ[i]; }
	проц     удали(цел i, цел count)                       { ключ.удали(i, count); фиксируйХэш(); }
	проц     удали(цел i)                                  { удали(i, 1); }
	цел      удалиКлюч(const T& k)                          { цел i = найди(k); if(i >= 0) удали(i); return i; }

	unsigned дайХэш(цел i) const                           { return hash[i].hash; }

	Индекс& operator<<=(const Вектор<T>& s)                  { *this = клонируй(s); return *this; }
	typedef T                ТипЗнач;
	typedef Вектор<T>        КонтейнерЗначений;
	КонстОбходчик  дайОбх(цел pos) const                   { return ключ.дайОбх(pos); }

	проц     зачистьИндекс()                    {}
	проц     реиндексируй(цел n)                  {}
	проц     реиндексируй()                       {}

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

#ifdef _ОТЛАДКА
	Ткст дамп() const;
#endif
};

template <class T>
class Массив : public ОпцияДвижимогоИГлубКопии< Массив<T> > {
protected:
#ifdef _ОТЛАДКА
	typedef T *ТипУказатель;
#else
	typedef ук ТипУказатель;
#endif
	Вектор<ТипУказатель> vector;

	проц     освободи();
	проц     __глубКопия(const Массив& v);
	T&       дай(цел i) const                           { return *(T *)vector[i]; }
	T      **дайУк(цел i) const                        { return (T **)vector.begin() + i; }

	проц     уд(ТипУказатель *укз, ТипУказатель *lim)              { while(укз < lim) delete (T *) *укз++; }
	проц     иниц(ТипУказатель *укз, ТипУказатель *lim)             { while(укз < lim) *укз++ = new T; }
	проц     иниц(ТипУказатель *укз, ТипУказатель *lim, const T& x) { while(укз < lim) *укз++ = new T(клонируй(x)); }

public:
	T&       добавь()                           { T *q = new T; vector.добавь(q); return *q; }
	T&       добавь(const T& x)                 { T *q = new T(x); vector.добавь(q); return *q; }
	T&       добавь(T&& x)                      { T *q = new T(пикуй(x)); vector.добавь(q); return *q; }
	T&       добавь(T *newt)                    { vector.добавь(newt); return *newt; }
	T&       добавь(Один<T>&& one)               { ПРОВЕРЬ(one); return добавь(one.открепи()); }
	template<class... Арги>
	T&       создай(Арги&&... арги)          { T *q = new T(std::forward<Арги>(арги)...); добавь(q); return *q; }
	template<class TT, class... Арги>
	TT&      создай(Арги&&... арги)          { TT *q = new TT(std::forward<Арги>(арги)...); добавь(q); return *q; }
	const T& operator[](цел i) const         { return дай(i); }
	T&       operator[](цел i)               { return дай(i); }
	const T& дай(цел i, const T& опр) const  { return i >= 0 && i < дайСчёт() ? дай(i) : опр; }
	T&       дай(цел i, T& опр)              { return i >= 0 && i < дайСчёт() ? дай(i) : опр; }
	цел      дайСчёт() const                { return vector.дайСчёт(); }
	бул     пустой() const                 { return vector.пустой(); }
	проц     обрежь(цел n);
	проц     устСчёт(цел n);
	проц     устСчёт(цел n, const T& init);
	проц     устСчётР(цел n);
	проц     устСчётР(цел n, const T& init);
	проц     очисть()                    { освободи(); vector.очисть(); }

	T&       по(цел i)                  { if(i >= дайСчёт()) устСчётР(i + 1); return дай(i); }
	T&       по(цел i, const T& x)      { if(i >= дайСчёт()) устСчётР(i + 1, x); return дай(i); }

	проц     сожми()                   { vector.сожми(); }
	проц     резервируй(цел xtra)          { vector.резервируй(xtra); }
	цел      дайРазмест() const           { return vector.дайРазмест(); }

	проц     уст(цел i, const T& x, цел count);
	T&       уст(цел i, const T& x)     { уст(i, x, 1); return дай(i); }
	T&       уст(цел i, T&& x)          { return по(i) = пикуй(x); }
	проц     удали(цел i, цел count = 1);
	проц     удали(const цел *sorted_list, цел n);
	проц     удали(const Вектор<цел>& sorted_list);
	template <class Условие>
	проц     удалиЕсли(Условие c);
	проц     вставьН(цел i, цел count = 1);
	T&       вставь(цел i)              { вставьН(i); return дай(i); }
	проц     вставь(цел i, const T& x, цел count);
	T&       вставь(цел i, const T& x)  { вставь(i, x, 1); return дай(i); }
	T&       вставь(цел i, T&& x);
	проц     вставь(цел i, const Массив& x);
	проц     вставь(цел i, const Массив& x, цел offset, цел count);
	template <class Диапазон>
	проц     вставьДиапазон(цел i, const Диапазон& r);
	проц     вставь(цел i, Массив&& x)             { vector.вставь(i, пикуй(x.vector)); }
	проц     приставь(const Массив& x)               { вставь(дайСчёт(), x); }
	проц     приставь(const Массив& x, цел o, цел c) { вставь(дайСчёт(), x, o, c); }
	проц     приставь(Массив&& x)                    { вставьПодбор(дайСчёт(), пикуй(x)); }
	template <class Диапазон>
	проц     приставьДиапазон(const Диапазон& r)          { вставьДиапазон(дайСчёт(), r); }

	проц     разверни(цел i1, цел i2)                 { разверни(vector[i1], vector[i2]); }
	проц     перемести(цел i1, цел i2);

	T&       уст(цел i, T *newt)        { delete (T *)vector[i]; vector[i] = newt; return *newt; }
	T&       вставь(цел i, T *newt);

	проц     сбрось(цел n = 1)            { обрежь(дайСчёт() - n); }
	T&       верх()                      { return дай(дайСчёт() - 1); }
	const T& верх() const                { return дай(дайСчёт() - 1); }

	T       *открепи(цел i)              { T *t = &дай(i); vector.удали(i); return t; }
	T       *разверни(цел i, T *newt)       { T *tmp = (T*)vector[i]; vector[i] = newt; return tmp; }
	T       *выньОткрепи()                { return (T *) vector.вынь(); }

	проц     разверни(Массив& b)             { разверни(vector, b.vector); }

	Массив& operator<<(const T& x)       { добавь(x); return *this; }
	Массив& operator<<(T&& x)            { добавь(пикуй(x)); return *this; }
	Массив& operator<<(T *newt)          { добавь(newt); return *this; }

	проц     сериализуй(Поток& s)       { StreamContainer(s, *this); }
	проц     вРяр(РярВВ& xio, кткст0 itemtag = "элт");
	проц     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
	т_хэш   дайХэшЗнач() const       { return хэшПоСериализу(*this); }

	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	Массив()                                          {}
	explicit Массив(цел n) : vector(n)                { иниц(vector.begin(), vector.end()); }
	explicit Массив(цел n, const T& init) : vector(n) { иниц(vector.begin(), vector.end(), init); }
	~Массив()                                         { освободи(); }

// подбери assignment & copy. Picked source can only очисть(), ~Вектор(), operator=, operator<<=
	Массив(Массив&& v) : vector(пикуй(v.vector))  {}
	проц operator=(Массив&& v)           { if(this != &v) { освободи(); vector = пикуй(v.vector); } }

// Deep copy
	Массив(const Массив& v, цел)          { __глубКопия(v); }

	проц     вставь(цел i, std::initializer_list<T> init);
	проц     приставь(std::initializer_list<T> init) { вставь(дайСчёт(), init); }
	Массив(std::initializer_list<T> init) { вставь(0, init); }

	class Обходчик;

	class КонстОбходчик {
	protected:
		T **укз;
		КонстОбходчик(T **p)                    { укз = p; }

		friend class Массив<T>;
		struct NP { цел dummy; };

	public:
		const T& operator*() const              { return **укз; }
		const T *operator->() const             { return *укз; }
		const T& operator[](цел i) const        { return *укз[i]; }

		КонстОбходчик& operator++()             { укз++; return *this; }
		КонстОбходчик& operator--()             { укз--; return *this; }
		КонстОбходчик  operator++(цел)          { КонстОбходчик t = *this; ++*this; return t; }
		КонстОбходчик  operator--(цел)          { КонстОбходчик t = *this; --*this; return t; }

		КонстОбходчик& operator+=(цел i)        { укз += i; return *this; }
		КонстОбходчик& operator-=(цел i)        { укз -= i; return *this; }

		КонстОбходчик operator+(цел i) const    { return укз + i; }
		КонстОбходчик operator-(цел i) const    { return укз - i; }

		цел  operator-(КонстОбходчик x) const   { return (цел)(укз - x.укз); }

		бул operator==(КонстОбходчик x) const  { return укз == x.укз; }
		бул operator!=(КонстОбходчик x) const  { return укз != x.укз; }
		бул operator<(КонстОбходчик x) const   { return укз < x.укз; }
		бул operator>(КонстОбходчик x) const   { return укз > x.укз; }
		бул operator<=(КонстОбходчик x) const  { return укз <= x.укз; }
		бул operator>=(КонстОбходчик x) const  { return укз >= x.укз; }

		operator бул() const                   { return укз; }

		КонстОбходчик()                         {}
		КонстОбходчик(NP *null)                 { ПРОВЕРЬ(null == NULL); укз = NULL; }

		СОВМЕСТИМОСТЬ_С_ОБХОДЧИКОМ_СТЛ
	};

	class Обходчик : public КонстОбходчик {
		friend class Массив<T>;
		Обходчик(T **p) : КонстОбходчик(p)      {}
		typedef КонстОбходчик B;
		struct NP { цел dummy; };

	public:
		T& operator*()                          { return **B::укз; }
		T *operator->()                         { return *B::укз; }
		T& operator[](цел i)                    { return *B::укз[i]; }

		const T& operator*() const              { return **B::укз; }
		const T *operator->() const             { return *B::укз; }
		const T& operator[](цел i) const        { return *B::укз[i]; }

		Обходчик& operator++()                  { B::укз++; return *this; }
		Обходчик& operator--()                  { B::укз--; return *this; }
		Обходчик  operator++(цел)               { Обходчик t = *this; ++*this; return t; }
		Обходчик  operator--(цел)               { Обходчик t = *this; --*this; return t; }

		Обходчик& operator+=(цел i)             { B::укз += i; return *this; }
		Обходчик& operator-=(цел i)             { B::укз -= i; return *this; }

		Обходчик operator+(цел i) const         { return B::укз + i; }
		Обходчик operator-(цел i) const         { return B::укз - i; }

		цел      operator-(Обходчик x) const    { return B::operator-(x); }

		Обходчик()                               {}
		Обходчик(NP *null) : КонстОбходчик(null) {}

		СОВМЕСТИМОСТЬ_С_ОБХОДЧИКОМ_СТЛ
	};

// Standard container interface
	Обходчик         begin()                    { return (T **)vector.begin(); }
	Обходчик         end()                      { return (T **)vector.end(); }
	КонстОбходчик    begin() const              { return (T **)vector.begin(); }
	КонстОбходчик    end() const                { return (T **)vector.end(); }

// Optimalization
	friend проц разверни(Массив& a, Массив& b)                   { ::разверни(a.vector, b.vector); }
	//GCC forced move from Обходчик, ugly workaround
private:
	static проц IterSwap0(Обходчик a, Обходчик b)          { ::разверни(*a.укз, *b.укз); }
public:
	friend проц IterSwap(Обходчик a, Обходчик b)           { Массив<T>::IterSwap0(a, b); }

#ifdef DEPRECATED
	цел      дайИндекс(const T& элт) const;
	проц     вставьПодбор(цел i, Массив&& x)       { vector.вставьПодбор(i, пикуй(x.vector)); }
	проц     приставьПодбор(Массив&& x)              { вставьПодбор(дайСчёт(), пикуй(x)); }
	T&       индексируй(цел i)                     { return по(i); }
	T&       индексируй(цел i, const T& x)         { return по(i, x); }
	T&       добавьПодбор(T&& x)                     { T *q = new T(пикуй(x)); vector.добавь(q); return *q; }
	T&       вставьПодбор(цел i, T&& x)           { return вставь(i, пикуй(x)); }
	typedef T        ТипЗнач;
	Обходчик         дайОбх(цел pos)           { return (T **)vector.дайОбх(pos); }
	КонстОбходчик    дайОбх(цел pos) const     { return (T **)vector.дайОбх(pos); }
#endif
	СОВМЕСТИМОСТЬ_С_ВЕКТОРОМ_СТЛ(Массив<T>)
};

template <class T>
class БиВектор : ОпцияДвижимогоИГлубКопии< БиВектор<T> > {
protected:
	T       *vector;
	цел      start;
	цел      items;
	цел      alloc;

	цел      Ix(цел i) const         { return i + start < alloc ? i + start : i + start - alloc; }

	цел      EI() const              { return Ix(items - 1); }
	проц     перемест(цел newalloc);
	проц     добавь0();
	проц     глубокаяКопия0(const БиВектор& ист);
	T       *добавьГолову0()              { проверьДвиж<T>(); добавь0(); return &vector[start = Ix(alloc - 1)/*(start + alloc - 1) % alloc*/]; }
	T       *добавьХвост0()              { проверьДвиж<T>(); добавь0(); return &vector[EI()]; }
	проц     обнули()                  { start = items = alloc = 0; vector = NULL; }
	проц     освободи();
	проц     подбери(БиВектор&& x)      { vector = пикуй(x.vector); start = x.start; items = x.items;
	                                   alloc = x.alloc; ((БиВектор&)x).items = -1; }
	проц     копируй(T *dst, цел start, цел count) const;

public:
	цел      дайСчёт() const        { return items; }
	бул     пустой() const         { return items == 0; }
	проц     очисть();

	T&       добавьГолову()               { return *new(добавьГолову0()) T; }
	T&       добавьХвост()               { return *new(добавьХвост0()) T; }
	проц     добавьГолову(const T& x)     { new(добавьГолову0()) T(x); }
	проц     добавьХвост(const T& x)     { new(добавьХвост0()) T(x); }
	проц     добавьГолову(T&& x)          { new(добавьГолову0()) T(пикуй(x)); }
	проц     добавьХвост(T&& x)          { new(добавьХвост0()) T(пикуй(x)); }
	T&       дайГолову()                  { ПРОВЕРЬ(items > 0); return vector[start]; }
	T&       дайХвост()                  { ПРОВЕРЬ(items > 0); return vector[EI()]; }
	const T& дайГолову() const            { ПРОВЕРЬ(items > 0); return vector[start]; }
	const T& дайХвост() const            { ПРОВЕРЬ(items > 0); return vector[EI()]; }
	проц     сбросьГолову()              { (&дайГолову())->~T(); items--; start = Ix(1); }
	проц     сбросьХвост()              { (&дайХвост())->~T(); items--; }
	T        выньГолову()               { T x = дайГолову(); сбросьГолову(); return x; }
	T        выньХвост()               { T x = дайХвост(); сбросьХвост(); return x; }
	проц     сбросьГолову(цел n)         { while(n-- > 0) БиВектор<T>::сбросьГолову(); }
	проц     сбросьХвост(цел n)         { while(n-- > 0) БиВектор<T>::сбросьХвост(); }
	const T& operator[](цел i) const { ПРОВЕРЬ(i >= 0 && i < items); return vector[Ix(i)]; }
	T&       operator[](цел i)       { ПРОВЕРЬ(i >= 0 && i < items); return vector[Ix(i)]; }
	проц     сожми();
	проц     резервируй(цел n);
	цел      дайРазмест() const        { return alloc; }

	проц     сериализуй(Поток& s);
	Ткст   вТкст() const;
	т_хэш   дайХэшЗнач() const    { return хэшПоСериализу(*this); }
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	БиВектор(const БиВектор& ист, цел)          { глубокаяКопия0(ист); }
	БиВектор(БиВектор&& ист)                    { подбери(пикуй(ист)); }
	проц operator=(БиВектор&& ист)              { if(this != &ист) { освободи(); подбери(пикуй(ист)); } }
	БиВектор()                                  { обнули(); }
	~БиВектор()                                 { освободи(); } // gcc4.0 workaround!!

	БиВектор(std::initializer_list<T> init);

	typedef ОбходчикКонстИ<БиВектор> КонстОбходчик;
	typedef ОбходчикИ<БиВектор>      Обходчик;

	КонстОбходчик    begin() const              { return КонстОбходчик(*this, 0); }
	КонстОбходчик    end() const                { return КонстОбходчик(*this, дайСчёт()); }
	Обходчик         begin()                    { return Обходчик(*this, 0); }
	Обходчик         end()                      { return Обходчик(*this, дайСчёт()); }

	friend проц разверни(БиВектор& a, БиВектор& b)  { разверни(a.vector, b.vector);
	                                              разверни(a.start, b.start);
	                                              разверни(a.items, b.items);
	                                              разверни(a.alloc, b.alloc); }
};

template <class T>
class БиМассив : ОпцияДвижимогоИГлубКопии< БиМассив<T> > {
protected:
	БиВектор<проц *> bv;

	проц     освободи();
	проц     глубокаяКопия0(const БиМассив<T>& v);

public:
	цел      дайСчёт() const              { return bv.дайСчёт(); }
	бул     пустой() const               { return дайСчёт() == 0; }
	проц     очисть()                       { освободи(); bv.очисть(); }

	T&       добавьГолову()                     { T *q = new T; bv.добавьГолову(q); return *q; }
	T&       добавьХвост()                     { T *q = new T; bv.добавьХвост(q); return *q; }
	проц     добавьГолову(const T& x)           { bv.добавьГолову(new T(x)); }
	проц     добавьХвост(const T& x)           { bv.добавьХвост(new T(x)); }
	T&       добавьГолову(T *newt)              { bv.добавьГолову(newt); return *newt; }
	T&       добавьХвост(T *newt)              { bv.добавьХвост(newt); return *newt; }
	template <class TT> TT& создайГоолову()   { TT *q = new TT; bv.добавьГолову(q); return *q; }
	template <class TT> TT& создайХвост()   { TT *q = new TT; bv.добавьХвост(q); return *q; }
	T&       добавьГолову(Один<T>&& one)         { ПРОВЕРЬ(one); return добавьГолову(one.открепи()); }
	T&       добавьХвост(Один<T>&& one)         { ПРОВЕРЬ(one); return добавьХвост(one.открепи()); }
	T&       дайГолову()                        { return *(T *) bv.дайГолову(); }
	T&       дайХвост()                        { return *(T *) bv.дайХвост(); }
	const T& дайГолову() const                  { return *(const T *) bv.дайГолову(); }
	const T& дайХвост() const                  { return *(const T *) bv.дайХвост(); }
	проц     сбросьГолову()                    { delete (T*) bv.дайГолову(); bv.сбросьГолову(); }
	проц     сбросьХвост()                    { delete (T*) bv.дайХвост(); bv.сбросьХвост(); }
	T       *открепиГолову()                  { T *q = (T*) bv.дайГолову(); bv.сбросьГолову(); return q; }
	T       *открепиХвост()                  { T *q = (T*) bv.дайХвост(); bv.сбросьХвост(); return q; }

	T&       operator[](цел i)             { return *(T *) bv[i]; }
	const T& operator[](цел i) const       { return *(const T *) bv[i]; }

	проц     сожми()                      { bv.сожми(); }
	проц     резервируй(цел n)                { bv.резервируй(n); }
	цел      дайРазмест() const              { return bv.дайРазмест(); }

	проц     сериализуй(Поток& s);
	Ткст   вТкст() const;
	т_хэш   дайХэшЗнач() const    { return хэшПоСериализу(*this); }
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	БиМассив(const БиМассив& v, цел)           { глубокаяКопия0(v); }

	БиМассив(БиМассив&& ист) : bv(пикуй(ист.bv)){}
	проц operator=(БиМассив&& ист)            { if(this != &ист) { освободи(); bv = пикуй(ист.bv); } }
	БиМассив()                                {}
	~БиМассив()                               { освободи(); }

	БиМассив(std::initializer_list<T> init);

	typedef ОбходчикКонстИ<БиМассив> КонстОбходчик;
	typedef ОбходчикИ<БиМассив>      Обходчик;

	КонстОбходчик    begin() const              { return КонстОбходчик(*this, 0); }
	КонстОбходчик    end() const                { return КонстОбходчик(*this, дайСчёт()); }
	Обходчик         begin()                    { return Обходчик(*this, 0); }
	Обходчик         end()                      { return Обходчик(*this, дайСчёт()); }

	friend проц разверни(БиМассив& a, БиМассив& b)    { разверни(a.bv, b.bv); }
};

template <class Диапазон>
using ТипЗначУ = typename std::remove_reference<decltype(*ДеклУкз__<typename std::remove_reference<Диапазон>::type>()->begin())>::type;

template <class Диапазон>
using ОбходчикУ = decltype(ДеклУкз__<typename std::remove_reference<Диапазон>::type>()->begin());

template <class Диапазон>
using КонстОбходчикУ = decltype(ДеклУкз__<const typename std::remove_reference<Диапазон>::type>()->begin());

template <class I>
class КлассСубДиапазон {
	I   l;
	цел count;

public:
	typedef typename std::remove_reference<decltype(*l)>::type value_type;

	цел дайСчёт() const { return count; }

	КлассСубДиапазон& пиши()                   { return *this; }

	value_type& operator[](цел i) const { ПРОВЕРЬ(i >= 0 && i < count); return l[i]; }
	I  begin() const { return l; }
	I  end() const { return l + count; }

	Ткст   вТкст() const                            { return AsStringArray(*this); }
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	КлассСубДиапазон(I begin, цел count) : l(begin), count(count)   {}
	КлассСубДиапазон(I begin, I end) : l(begin), count(цел(end - begin)) {}
	КлассСубДиапазон() {} // MSC bug workaround
};

template <class C> using СубДиапазонУ = decltype(СубДиапазон(((C *)0)->begin(), ((C *)0)->end()));

template <class T>
struct КлассКонстДиапазон {
	T   значение;
	цел count;

	typedef T value_type;
	typedef value_type ТипЗнач;

	const value_type& operator[](цел i) const { return значение; }
	цел дайСчёт() const                      { return count; }

	typedef ОбходчикКонстИ<КлассКонстДиапазон> Обходчик;

	Обходчик begin() const { return Обходчик(*this, 0); }
	Обходчик end() const { return Обходчик(*this, count); }

	Ткст   вТкст() const                            { return AsStringArray(*this); }
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	КлассКонстДиапазон(const T& значение, цел count) : значение(значение), count(count) {}
	КлассКонстДиапазон(цел count) : count(count) {}
	КлассКонстДиапазон() {} // MSC bug workaround
};

template <class ДиапазонОснова>
struct КлассРеверсДиапазон {
	typename std::remove_reference<ДиапазонОснова>::type& r;

	typedef ТипЗначУ<ДиапазонОснова>                       value_type;
	typedef value_type                                   ТипЗнач;

	const value_type& operator[](цел i) const            { return r[r.дайСчёт() - i - 1]; }
	value_type& operator[](цел i)                        { return r[r.дайСчёт() - i - 1]; }
	цел дайСчёт() const                                 { return r.дайСчёт(); }

	typedef ОбходчикИ<КлассРеверсДиапазон>                  Обходчик;
	typedef ОбходчикКонстИ<КлассРеверсДиапазон>             КонстОбходчик;

	КлассРеверсДиапазон& пиши()                           { return *this; }

	КонстОбходчик begin() const                          { return КонстОбходчик(*this, 0); }
	КонстОбходчик end() const                            { return КонстОбходчик(*this, r.дайСчёт()); }

	Обходчик begin()                                     { return Обходчик(*this, 0); }
	Обходчик end()                                       { return Обходчик(*this, r.дайСчёт()); }

	Ткст   вТкст() const                            { return AsStringArray(*this); }
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	КлассРеверсДиапазон(ДиапазонОснова& r) : r(r) {}
	КлассРеверсДиапазон() {} // MSC bug workaround
};

template <class ДиапазонОснова>
struct КлассДиапазонОбзора {
	typename std::remove_reference<ДиапазонОснова>::тип *r;
	Вектор<цел> ndx;

	typedef ТипЗначУ<ДиапазонОснова> value_type;
	typedef value_type ТипЗнач;

	const value_type& operator[](цел i) const { return (*r)[ndx[i]]; }
	value_type& operator[](цел i)             { return (*r)[ndx[i]]; }
	цел дайСчёт() const                      { return ndx.дайСчёт(); }

	typedef ОбходчикИ<КлассДиапазонОбзора> Обходчик;
	typedef ОбходчикКонстИ<КлассДиапазонОбзора> КонстОбходчик;

	КлассДиапазонОбзора& пиши()                   { return *this; }

	КонстОбходчик begin() const { return КонстОбходчик(*this, 0); }
	КонстОбходчик end() const  { return КонстОбходчик(*this, ndx.дайСчёт()); }

	Обходчик begin() { return Обходчик(*this, 0); }
	Обходчик end()   { return Обходчик(*this, ndx.дайСчёт()); }

	Ткст   вТкст() const                            { return AsStringArray(*this); }
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	КлассДиапазонОбзора(ДиапазонОснова& r, Вектор<цел>&& ndx) : r(&r), ndx(пикуй(ndx)) {}
	КлассДиапазонОбзора() {} // MSC bug workaround
};

//# System dependent
template <class T>
class Митор : Движ< Митор<T> > {
	union {
		mutable unsigned   count;
		mutable Вектор<T> *vector;
	};
	ббайт elem0[sizeof(T)];

	T&        дай(цел i) const;
	проц      подбери(Митор&& m);
	проц      копируй(const Митор& m);
	проц      проверь() const               { ПРОВЕРЬ(count != 2); }

public:
	T&        operator[](цел i)         { return дай(i); }
	const T&  operator[](цел i) const   { return дай(i); }
	цел       дайСчёт() const;
	T&        добавь();
	проц      добавь(const T& x);
	проц      очисть();
	проц      сожми();

	Митор(Митор&& m)                    { подбери(пикуй(m)); }
	проц operator=(Митор&& m)           { if(this != &m) { очисть(); подбери(пикуй(m)); } }

	Митор(Митор& m, цел)                { копируй(m); }

	Митор()                             { count = 0; }
	~Митор()                            { очисть(); }
};

template <class T, цел N = 1>
struct Линк {
	T *link_prev[N];
	T *link_next[N];

protected:
	проц LPN(цел i)                      { link_prev[i]->link_next[i] = link_next[i]->link_prev[i] = (T *)this; }

public:
	НЕСАННЕОПР	T *дайУк()                  { return (T *) this; }
	const T *дайУк() const              { return (const T *) this; }
	T       *дайСледщ(цел i = 0)          { return link_next[i]; }
	T       *дайПредш(цел i = 0)          { return link_prev[i]; }
	const T *дайСледщ(цел i = 0) const    { return link_next[i]; }
	const T *дайПредш(цел i = 0) const    { return link_prev[i]; }

	НЕСАННЕОПР	проц линкуйся(цел i = 0)     { link_next[i] = link_prev[i] = (T *)this; }
	проц линкуйсяВесь()                   { for(цел i = 0; i < N; i++) линкуйся(i); }
	проц отлинкуй(цел i = 0)               { link_next[i]->link_prev[i] = link_prev[i]; link_prev[i]->link_next[i] = link_next[i];
	                                       линкуйся(i); }
	проц отлинкуйВсе()                     { for(цел i = 0; i < N; i++) отлинкуй(i); }
	НЕСАННЕОПР	проц линкПеред(Линк *n, цел i = 0)  { link_next[i] = (T *)n; link_prev[i] = link_next[i]->link_prev[i]; LPN(i); }
	НЕСАННЕОПР	проц линкПосле(Линк *p, цел i = 0)   { link_prev[i] = (T *)p; link_next[i] = link_prev[i]->link_next[i]; LPN(i); }

	T   *вставьСледщ(цел i = 0)           { T *x = new T; x->линкПосле(this, i); return x; }
	T   *вставьПредш(цел i = 0)           { T *x = new T; x->линкПеред(this, i); return x; }

	проц удалиСписок(цел i = 0)           { while(link_next[i] != дайУк()) delete link_next[i]; }

	бул вСписке(цел i = 0) const         { return link_next[i] != дайУк(); }
	бул пустой(цел i = 0) const        { return !вСписке(i); }

	Линк()                               { линкуйсяВесь(); }
	~Линк()                              { отлинкуйВсе(); }

private:
	Линк(const Линк&);
	проц operator=(const Линк&);

};

template <class T, цел N = 1>
class ЛинкВлад : public Линк<T, N> {
public:
	~ЛинкВлад()                         { Линк<T, N>::удалиСписок(); }
};

template <class T>
class СоРесурсыТрудяги {
	цел          workercount;
	Буфер<T>    res;

public:
	цел дайСчёт() const  { return workercount + 1; }
	T& operator[](цел i)  { return res[i]; }

	T& дай()              { цел i = СоРабота::дайИндексТрудяги(); return res[i < 0 ? workercount : i]; }
	T& operator~()        { return дай(); }

	T *begin()            { return ~res; }
	T *end()              { return ~res + дайСчёт(); }

	СоРесурсыТрудяги()   { workercount = СоРабота::дайРазмерПула(); res.размести(дайСчёт()); }

	СоРесурсыТрудяги(Событие<T&> initializer) : СоРесурсыТрудяги() {
		for(цел i = 0; i < дайСчёт(); i++)
			initializer(res[i]);
	}
};

template <class Ret>
class АсинхРабота {
	template <class Ret2>
	struct Imp {
		СоРабота co;
		Ret2   ret;

		template<class Функция, class... Арги>
		проц        делай(Функция&& f, Арги&&... арги) { co.делай([=]() { ret = f(арги...); }); }
		const Ret2& дай()                            { return ret; }
		Ret2        подбери()                           { return пикуй(ret); }
	};

	struct ImpVoid {
		СоРабота co;

		template<class Функция, class... Арги>
		проц        делай(Функция&& f, Арги&&... арги) { co.делай([=]() { f(арги...); }); }
		проц        дай()                            {}
		проц        подбери()                           {}
	};

	using ImpType = typename std::conditional<std::is_void<Ret>::значение, ImpVoid, Imp<Ret>>::тип;

	Один<ImpType> imp;

public:
	template< class Функция, class... Арги>
	проц  делай(Функция&& f, Арги&&... арги)          { imp.создай().делай(f, арги...); }

	проц        отмена()                            { if(imp) imp->co.отмена(); }
	static бул отменён()                        { return СоРабота::отменён(); }
	бул        финишировал()                        { return imp && imp->co.финишировал(); }
	Ret         дай()                               { ПРОВЕРЬ(imp); imp->co.финиш(); return imp->дай(); }
	Ret         operator~()                         { return дай(); }
	Ret         подбери()                              { ПРОВЕРЬ(imp); imp->co.финиш(); return imp->подбери(); }

	АсинхРабота& operator=(АсинхРабота&&) = default;
	АсинхРабота(АсинхРабота&&) = default;

	АсинхРабота()                                     {}
	~АсинхРабота()                                    { if(imp) imp->co.отмена(); }
};

template <class K, class V>
struct КлючЗначРеф {
	const K& ключ;
	V&       значение;

	КлючЗначРеф(const K& ключ, V& значение) : ключ(ключ), значение(значение) {}
};

template <class вКарту, class K, class V>
struct ДиапазонКЗМап {
	вКарту& map;

	struct Обходчик {
		вКарту& map;
		цел  ii;

		проц пропустиОтлинк()               { while(ii < map.дайСчёт() && map.отлинкован(ii)) ii++; }

		проц operator++()                 { ++ii; пропустиОтлинк(); }
		КлючЗначРеф<K, V> operator*()     { return КлючЗначРеф<K, V>(map.дайКлюч(ii), map[ii]); }
		бул operator!=(Обходчик b) const { return ii != b.ii; }

		Обходчик(вКарту& map, цел ii) : map(map), ii(ii) { пропустиОтлинк(); }
	};

	Обходчик begin() const             { return Обходчик(map, 0); }
	Обходчик end() const               { return Обходчик(map, map.дайСчёт()); }

	ДиапазонКЗМап(вКарту& map) : map(map) {}
};

template <class K, class T, class V>
class АМап {
protected:
	Индекс<K> ключ;
	V        значение;

	template <class KK>           цел FindAdd_(KK&& k);
	template <class KK, class TT> цел FindAdd_(KK&& k, TT&& init);
	template <class KK>           T&  Put_(KK&& k);
	template <class KK, class TT> цел Put_(KK&& k, TT&& x);
	template <class KK>           цел PutDefault_(KK&& k);
	template <class KK>           цел FindPut_(KK&& k);
	template <class KK, class TT> цел FindPut_(KK&& k, TT&& init);
	template <class KK>           T&  GetAdd_(KK&& k);
	template <class KK, class TT> T&  GetAdd_(KK&& k, TT&& init);
	template <class KK>           T&  GetPut_(KK&& k);
	template <class KK, class TT> T&  GetPut_(KK&& k, TT&& init);

public:
	T&       добавь(const K& k, const T& x)            { ключ.добавь(k); return значение.добавь(x); }
	T&       добавь(const K& k, T&& x)                 { ключ.добавь(k); return значение.добавь(пикуй(x)); }
	T&       добавь(const K& k)                        { ключ.добавь(k); return значение.добавь(); }
	T&       добавь(K&& k, const T& x)                 { ключ.добавь(пикуй(k)); return значение.добавь(x); }
	T&       добавь(K&& k, T&& x)                      { ключ.добавь(пикуй(k)); return значение.добавь(пикуй(x)); }
	T&       добавь(K&& k)                             { ключ.добавь(пикуй(k)); return значение.добавь(); }

	цел      найди(const K& k) const                 { return ключ.найди(k); }
	цел      найдиСледщ(цел i) const                  { return ключ.найдиСледщ(i); }
	цел      найдиПоследн(const K& k) const             { return ключ.найдиПоследн(k); }
	цел      найдиПредш(цел i) const                  { return ключ.найдиПредш(i); }

	цел      найдиДобавь(const K& k)                    { return FindAdd_(k); }
	цел      найдиДобавь(const K& k, const T& init)     { return FindAdd_(k, init); }
	цел      найдиДобавь(const K& k, T&& init)          { return FindAdd_(k, пикуй(init)); }
	цел      найдиДобавь(K&& k)                         { return FindAdd_(пикуй(k)); }
	цел      найдиДобавь(K&& k, const T& init)          { return FindAdd_(пикуй(k), init); }
	цел      найдиДобавь(K&& k, T&& init)               { return FindAdd_(пикуй(k), пикуй(init)); }

	T&       помести(const K& k)                        { return Put_(k); }
	цел      помести(const K& k, const T& x)            { return Put_(k, x); }
	цел      помести(const K& k, T&& x)                 { return Put_(k, пикуй(x)); }
	T&       помести(K&& k)                             { return Put_(пикуй(k)); }
	цел      помести(K&& k, const T& x)                 { return Put_(пикуй(k), x); }
	цел      помести(K&& k, T&& x)                      { return Put_(пикуй(k), пикуй(x)); }

	цел      поместиДефолт(const K& k)                 { return PutDefault_(k); }
	цел      поместиДефолт(K&& k)                      { return PutDefault_(пикуй(k)); }

	цел      найдиПомести(const K& k)                    { return FindPut_(k); }
	цел      найдиПомести(const K& k, const T& init)     { return FindPut_(k, init); }
	цел      найдиПомести(const K& k, T&& init)          { return FindPut_(k, пикуй(init)); }
	цел      найдиПомести(K&& k)                         { return FindPut_(пикуй(k)); }
	цел      найдиПомести(K&& k, const T& init)          { return FindPut_(пикуй(k), init); }
	цел      найдиПомести(K&& k, T&& init)               { return FindPut_(пикуй(k), пикуй(init)); }

	T&       дай(const K& k)                        { return значение[найди(k)]; }
	const T& дай(const K& k) const                  { return значение[найди(k)]; }
	const T& дай(const K& k, const T& d) const      { цел i = найди(k); return i >= 0 ? значение[i] : d; }

	T&       дайДобавь(const K& k)                     { return GetAdd_(k); }
	T&       дайДобавь(const K& k, const T& x)         { return GetAdd_(k, x); }
	T&       дайДобавь(const K& k, T&& x)              { return GetAdd_(k, пикуй(x)); }
	T&       дайДобавь(K&& k)                          { return GetAdd_(пикуй(k)); }
	T&       дайДобавь(K&& k, const T& x)              { return GetAdd_(пикуй(k), x); }
	T&       дайДобавь(K&& k, T&& x)                   { return GetAdd_(пикуй(k), пикуй(x)); }

	T&       дайПомести(const K& k)                     { return GetPut_(k); }
	T&       дайПомести(const K& k, const T& x)         { return GetPut_(k, x); }
	T&       дайПомести(const K& k, T&& x)              { return GetPut_(k, пикуй(x)); }
	T&       дайПомести(K&& k)                          { return GetPut_(пикуй(k)); }
	T&       дайПомести(K&& k, const T& x)              { return GetPut_(пикуй(k), x); }
	T&       дайПомести(K&& k, T&& x)                   { return GetPut_(пикуй(k), пикуй(x)); }

	проц     устКлюч(цел i, const K& k)              { ключ.уст(i, k); }
	проц     устКлюч(цел i, K&& k)                   { ключ.уст(i, пикуй(k)); }

	T       *найдиУк(const K& k)                    { цел i = найди(k); return i >= 0 ? &значение[i] : NULL; }
	const T *найдиУк(const K& k) const              { цел i = найди(k); return i >= 0 ? &значение[i] : NULL; }

	T       *найдиПоследнУк(const K& k)                { цел i = найдиПоследн(k); return i >= 0 ? &значение[i] : NULL; }
	const T *найдиПоследнУк(const K& k) const          { цел i = найдиПоследн(k); return i >= 0 ? &значение[i] : NULL; }

	проц     отлинкуй(цел i)                          { ключ.отлинкуй(i); }
	цел      отлинкуйКлюч(const K& k, unsigned h)      { return ключ.отлинкуйКлюч(k, h); }
	цел      отлинкуйКлюч(const K& k)                  { return ключ.отлинкуйКлюч(k); }
	бул     отлинкован(цел i) const                { return ключ.отлинкован(i); }
	проц     смети();
	бул     HasUnlinked() const                    { return ключ.HasUnlinked(); }

	const T& operator[](цел i) const                { return значение[i]; }
	T&       operator[](цел i)                      { return значение[i]; }
	цел      дайСчёт() const                       { return значение.дайСчёт(); }
	бул     пустой() const                        { return значение.пустой(); }
	проц     очисть()                                { ключ.очисть(); значение.очисть(); }
	проц     сожми()                               { значение.сожми(); ключ.сожми(); }
	проц     резервируй(цел xtra)                      { значение.резервируй(xtra); ключ.резервируй(xtra); }
	цел      дайРазмест() const                       { return значение.дайРазмест(); }

	unsigned дайХэш(цел i) const                   { return ключ.дайХэш(i); }

	проц     сбрось(цел n = 1)                        { ключ.сбрось(n); значение.сбрось(n); }
	T&       верх()                                  { return значение.верх(); }
	const T& верх() const                            { return значение.верх(); }
	const K& TopKey() const                         { return ключ.верх(); }
//	T        вынь()                                  { T h = верх(); сбрось(); return h; }
	K        PopKey()                               { K h = TopKey(); сбрось(); return h; }
	проц     обрежь(цел n)                            { ключ.обрежь(n); значение.обрежь(n); }

	const K& дайКлюч(цел i) const                    { return ключ[i]; }

	проц     удали(const цел *sl, цел n)           { ключ.удали(sl, n); значение.удали(sl, n); }
	проц     удали(const Вектор<цел>& sl)          { удали(sl, sl.дайСчёт()); }
	template <typename P> проц удалиЕсли(P p)        { удали(найдиВсеи(p)); }

	проц     сериализуй(Поток& s);
	проц     вРяр(РярВВ& xio);
	проц     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
	т_хэш   дайХэшЗнач() const                   { return хэшПоСериализу(*this); }
	бул     operator==(const АМап& b) const        { ПРОВЕРЬ(!HasUnlinked()); return равныМапы(*this, b); }
	бул     operator!=(const АМап& b) const        { return !operator==(b); }
	цел      сравни(const АМап& b) const           { ПРОВЕРЬ(!HasUnlinked()); return сравниМап(*this, b); }
	бул     operator<=(const АМап& x) const        { return сравни(x) <= 0; }
	бул     operator>=(const АМап& x) const        { return сравни(x) >= 0; }
	бул     operator<(const АМап& x) const         { return сравни(x) < 0; }
	бул     operator>(const АМап& x) const         { return сравни(x) > 0; }

	проц     разверни(АМап& x)                          { разверни(значение, x.значение); разверни(ключ, x.ключ); }
	const Индекс<K>&  дайИндекс() const               { return ключ; }
	Индекс<K>         PickIndex()                    { return пикуй(ключ); }

	const Вектор<K>& дайКлючи() const                { return ключ.дайКлючи(); }
	Вектор<K>        подбериКлючи()                     { return ключ.подбериКлючи(); }

	const V&         дайЗначения() const              { return значение; }
	V&               дайЗначения()                    { return значение; }
	V                подбериЗначения()                   { return пикуй(значение); }

	ДиапазонКЗМап<АМап, K, T> operator~()                   { return ДиапазонКЗМап<АМап, K, T>(*this); }
	ДиапазонКЗМап<const АМап, K, const T> operator~() const { return ДиапазонКЗМап<const АМап, K, const T>(*this); }

	АМап& operator()(const K& k, const T& v)        { добавь(k, v); return *this; }

	АМап()                                          {}
	АМап(const АМап& s, цел) : ключ(s.ключ, 0), значение(s.значение, 0) {}
	АМап(Индекс<K>&& ndx, V&& знач) : ключ(пикуй(ndx)), значение(пикуй(знач)) {}
	АМап(Вектор<K>&& ndx, V&& знач) : ключ(пикуй(ndx)), значение(пикуй(знач)) {}
	АМап(std::initializer_list<std::pair<K, T>> init) { for(const auto& i : init) добавь(клонируй(i.first), клонируй(i.second)); }

	typedef ОбходчикУ<V>           Обходчик;
	typedef КонстОбходчикУ<V>      КонстОбходчик;

	Обходчик         begin()                        { return значение.begin(); }
	Обходчик         end()                          { return значение.end(); }
	КонстОбходчик    begin() const                  { return значение.begin(); }
	КонстОбходчик    end() const                    { return значение.end(); }

#ifdef DEPRECATED
	typedef V                          КонтейнерЗначений;
	typedef T                          ТипЗнач;

	typedef Вектор<K> КонтейнерКлючей;
	typedef K         ТипКлюча;
	typedef КонстОбходчикУ<Индекс<K>> КонстОбходчикКлючей;

	friend цел     дайСчёт(const АМап& v)                        { return v.дайСчёт(); }
	цел      PutPick(const K& k, T&& x)                           { return помести(k, пикуй(x)); }
	T&       добавьПодбор(const K& k, T&& x)                           { return добавь(k, пикуй(x)); }
	цел      FindAddPick(const K& k, T&& init)                    { return найдиДобавь(k, пикуй(init)); }
	цел      FindPutPick(const K& k, T&& init)                    { return найдиПомести(k, пикуй(init)); }
	T&       GetAddPick(const K& k, T&& x)                        { return дайДобавь(k, пикуй(x)); }
	T&       GetPutPick(const K& k, T&& x)                        { return дайПомести(k, пикуй(x)); }

	КонстОбходчикКлючей дайОбхКлючей(цел pos) const                    { return ключ.дайОбх(pos); }
	Обходчик         дайОбх(цел pos)                             { return значение.дайОбх(pos); }
	КонстОбходчик    дайОбх(цел pos) const                       { return значение.дайОбх(pos); }

	КонстОбходчикКлючей стартКлючи() const                             { return ключ.begin(); }
	КонстОбходчикКлючей стопКлючи() const                               { return ключ.end(); }

	T&       вставь(цел i, const K& k)             { ключ.вставь(i, k); return значение.вставь(i); }
	T&       вставь(цел i, const K& k, const T& x) { ключ.вставь(i, k); return значение.вставь(i, x); }
	T&       вставь(цел i, const K& k, T&& x)      { ключ.вставь(i, k); return значение.вставь(i, пикуй(x)); }
	T&       вставь(цел i, K&& k)                  { ключ.вставь(i, пикуй(k)); return значение.вставь(i); }
	T&       вставь(цел i, K&& k, const T& x)      { ключ.вставь(i, пикуй(k)); return значение.вставь(i, x); }
	T&       вставь(цел i, K&& k, T&& x)           { ключ.вставь(i, пикуй(k)); return значение.вставь(i, пикуй(x)); }

	проц     удали(цел i)                         { ключ.удали(i); значение.удали(i); }
	проц     удали(цел i, цел count)              { ключ.удали(i, count); значение.удали(i, count); }
	цел      удалиКлюч(const K& k);
#endif
};

template <class K, class T>
class ВекторМап : public ОпцияДвижимогоИГлубКопии<ВекторМап<K, T>>,
                  public АМап<K, T, Вектор<T>> {
    typedef АМап<K, T, Вектор<T>> B;
public:
	T        вынь()                            { T h = пикуй(B::верх()); B::сбрось(); return h; }

	ВекторМап(const ВекторМап& s, цел) : АМап<K, T, Вектор<T>>(s, 1) {}
	ВекторМап(Индекс<K>&& ndx, Вектор<T>&& знач) : АМап<K, T, Вектор<T>>(пикуй(ndx), пикуй(знач)) {}
	ВекторМап(Вектор<K>&& ndx, Вектор<T>&& знач) : АМап<K, T, Вектор<T>>(пикуй(ndx), пикуй(знач)) {}
	ВекторМап()                                                       {}
	ВекторМап(std::initializer_list<std::pair<K, T>> init) : B::АМап(init) {}

	template <class KK, class TT>
	ВекторМап& operator()(KK&& k, TT&& v)                { B::добавь(std::forward<KK>(k), std::forward<TT>(v)); return *this; }

	friend проц    разверни(ВекторМап& a, ВекторМап& b)      { a.B::разверни(b); }

	typedef typename АМап< K, T, Вектор<T>>::КонстОбходчик КонстОбходчик;
	typedef typename АМап< K, T, Вектор<T>>::Обходчик      Обходчик;
	СОВМЕСТИМОСТЬ_С_МАП_СТЛ(ВекторМап<K _cm_ T _cm_ HashFn>)
};

template <class K, class T>
class МассивМап : public ОпцияДвижимогоИГлубКопии<МассивМап<K, T>>,
                 public АМап<K, T, Массив<T>> {
	typedef АМап<K, T, Массив<T>> B;
public:
	T&        добавь(const K& k, const T& x)          { return B::добавь(k, x); }
	T&        добавь(const K& k, T&& x)               { return B::добавь(k, пикуй(x)); }
	T&        добавь(const K& k)                      { return B::добавь(k); }
	T&        добавь(const K& k, T *newt)             { B::ключ.добавь(k); return B::значение.добавь(newt); }
	T&        добавь(K&& k, const T& x)               { return B::добавь(пикуй(k), x); }
	T&        добавь(K&& k, T&& x)                    { return B::добавь(пикуй(k), пикуй(x)); }
	T&        добавь(K&& k)                           { return B::добавь(пикуй(k)); }
	T&        добавь(K&& k, T *newt)                  { B::ключ.добавь(пикуй(k)); return B::значение.добавь(newt); }

	T&        вставь(цел i, const K& k, T *newt)   { B::ключ.вставь(i, k); return B::значение.вставь(i, newt); }
	T&        вставь(цел i, K&& k, T *newt)        { B::ключ.вставь(i, пикуй(k)); return B::значение.вставь(i, newt); }
	using B::вставь;

	template <class TT, class... Арги>
	TT& создай(const K& k, Арги&&... арги)         { TT *q = new TT(std::forward<Арги>(арги)...); B::ключ.добавь(k); return static_cast<TT&>(B::значение.добавь(q)); }
	template <class TT, class... Арги>
	TT& создай(K&& k, Арги&&... арги)              { TT *q = new TT(std::forward<Арги>(арги)...); B::ключ.добавь(пикуй(k)); return static_cast<TT&>(B::значение.добавь(q)); }

	T&        уст(цел i, T *укз)                   { return B::значение.уст(i, укз); }
	T        *выньОткрепи()                          { B::ключ.сбрось(); return B::значение.выньОткрепи(); }
	T        *открепи(цел i)                        { B::ключ.удали(i); return B::значение.открепи(i); }
	T        *разверни(цел i, T *newt)                 { return B::значение.разверни(i, newt); }

	МассивМап(const МассивМап& s, цел) : АМап<K, T, Массив<T>>(s, 1) {}
	МассивМап(Индекс<K>&& ndx, Массив<T>&& знач) : АМап<K, T, Массив<T>>(пикуй(ndx), пикуй(знач)) {}
	МассивМап(Вектор<K>&& ndx, Массив<T>&& знач) : АМап<K, T, Массив<T>>(пикуй(ndx), пикуй(знач)) {}
	МассивМап() {}

	МассивМап(std::initializer_list<std::pair<K, T>> init) : B::АМап(init) {}

	template <class KK, class TT>
	МассивМап& operator()(KK&& k, TT&& v)           { добавь(std::forward<KK>(k), std::forward<TT>(v)); return *this; }

	friend проц    разверни(МассивМап& a, МассивМап& b)        { a.B::разверни(b); }

	typedef typename АМап< K, T, Массив<T>>::КонстОбходчик КонстОбходчик;
	typedef typename АМап< K, T, Массив<T>>::Обходчик      Обходчик;
	СОВМЕСТИМОСТЬ_С_МАП_СТЛ(МассивМап<K _cm_ T _cm_ HashFn>)
};

template <class T>
class Pte : public PteBase {
	friend class Ук<T>;
};

template <class T>
class Ук : public УкОснова, Движ< Ук<T> > {
	T   *дай() const                          { return prec ? static_cast<T *>(prec->укз) : NULL; }

public:
	T       *operator->() const               { return дай(); }
	T       *operator~() const                { return дай(); }
	operator T*() const                       { return дай(); }

	Ук& operator=(T *укз)                    { присвой(укз); return *this; }
	Ук& operator=(const Ук& укз)            { присвой(укз.дай()); return *this; }

	Ук()                                     { prec = NULL; }
	Ук(T *укз)                               { уст(укз); }
	Ук(const Ук& укз)                       { уст(укз.дай()); }

	Ткст вТкст() const;

	friend бул operator==(const Ук& a, const T *b)   { return a.дай() == b; }
	friend бул operator==(const T *a, const Ук& b)   { return a == b.дай(); }
	friend бул operator==(const Ук& a, const Ук& b) { return a.prec == b.prec; }

	friend бул operator==(const Ук& a, T *b)         { return a.дай() == b; }
	friend бул operator==(T *a, const Ук& b)         { return a == b.дай(); }

	friend бул operator!=(const Ук& a, const T *b)   { return a.дай() != b; }
	friend бул operator!=(const T *a, const Ук& b)   { return a != b.дай(); }
	friend бул operator!=(const Ук& a, const Ук& b) { return a.prec != b.prec; }

	friend бул operator!=(const Ук& a, T *b)         { return a.дай() != b; }
	friend бул operator!=(T *a, const Ук& b)         { return a != b.дай(); }
};

template <class T, бцел тип = UNKNOWN_V, class B = ПустойКласс>
class ТипЗнач : public B {
public:
	static бцел номерТипаЗнач()                      { return тип == UNKNOWN_V ? СтатичНомТипа<T>() + 0x8000000 : тип; }
	friend бцел номерТипаЗнач(const T*)              { return T::номерТипаЗнач(); }

	бул     экзПусто_ли() const                 { return false; }
	проц     сериализуй(Поток& s)                   { НИКОГДА(); }
	проц     вРяр(РярВВ& xio)                     { НИКОГДА(); }
	проц     вДжейсон(ДжейсонВВ& jio)                   { НИКОГДА(); }
	т_хэш   дайХэшЗнач() const                   { return 0; }
	бул     operator==(const T&) const             { НИКОГДА(); return false; }
	Ткст   вТкст() const                       { return typeid(T).name(); }
	цел      сравни(const T&) const                { НИКОГДА(); return 0; }
	цел      полиСравни(const Значение&) const        { НИКОГДА(); return 0; }

	operator РефНаТипЗнач();
};

template <class T, бцел тип, class B = ПустойКласс> // Backward compatiblity
class ПрисвойНомТипаЗнач : public ТипЗнач<T, тип, B> {};



template <class T, class K = Ткст>
class КэшЛРУ {
public:
	struct Делец {
		virtual K      Ключ() const = 0;
		virtual цел    сделай(T& object) const = 0;
		virtual ~Делец() {}
	};

private:
	struct Элемент : Движ<Элемент> {
		цел    prev, next;
		цел    size;
		Один<T> данные;
		бул   flag;
	};

	struct Ключ : Движ<Ключ> {
		K            ключ;
		Ткст       тип;

		бул operator==(const Ключ& b) const { return ключ == b.ключ && тип == b.тип; }
		т_хэш дайХэшЗнач() const { return КомбХэш(ключ, тип); }
	};

	Индекс<Ключ>   ключ;
	Вектор<Элемент> данные;
	цел  head;

	цел  size;
	цел  count;

	цел  foundsize;
	цел  newsize;
	бул flag = false;

	const цел InternalSize = 3 * (sizeof(Элемент) + sizeof(Ключ) + 24) / 2;

	проц отлинкуй(цел i);
	проц линкуйГолову(цел i);

public:
	цел  дайРазм() const            { return size; }
	цел  дайСчёт() const           { return count; }

	template <class P> проц настройРазм(P getsize);

	T&       дайЛРУ();
	const K& дайКлючЛРУ();
	проц     сбросьЛРУ();
	проц     сожми(цел maxsize, цел maxcount = 30000);
	проц     сожмиСчёт(цел maxcount = 30000)          { сожми(INT_MAX, maxcount); }

	template <class P> цел  удали(P predicate);
	template <class P> бул удалиОдин(P predicate);

	T&   дай(const Делец& m);

	проц очисть();

	проц очистьСчётчики();
	цел  дайНайденРазм() const       { return foundsize; }
	цел  дайНовРазм() const         { return newsize; }

	КэшЛРУ() { head = -1; size = 0; count = 0; очистьСчётчики(); }
};

template <class T>
struct ЭлтСтабСорта__ {
	const T& значение;
	цел      индекс;

	ЭлтСтабСорта__(const T& значение, цел индекс) : значение(значение), индекс(индекс) {}
};

template <class II, class T>
struct ОбходчикСтабСорта__ {
	II          ii;
	цел        *vi;

	typedef ОбходчикСтабСорта__<II, T> Обх;

	Обх&       operator ++ ()               { ++ii; ++vi; return *this; }
	Обх&       operator -- ()               { --ii; --vi; return *this; }
	Обх        operator +  (цел i) const    { return Обх(ii + i, vi + i); }
	Обх        operator -  (цел i) const    { return Обх(ii - i, vi - i); }
	цел         operator -  (Обх b) const   { return (цел)(ii - b.ii); }
	бул        operator == (Обх b) const   { return ii == b.ii; }
	бул        operator != (Обх b) const   { return ii != b.ii; }
	бул        operator <  (Обх b) const   { return ii <  b.ii; }
	бул        operator <= (Обх b) const   { return ii <=  b.ii; }

	ЭлтСтабСорта__<T> operator*() const    { return ЭлтСтабСорта__<T>(*ii, *vi); }

	friend проц IterSwap(Обх a, Обх b)     { IterSwap(a.ii, b.ii); IterSwap(a.vi, b.vi); }

	ОбходчикСтабСорта__(II ii, цел *vi) : ii(ii), vi(vi) {}
};

template <class T, class Less>
struct СтабСортМеньшее__ {
	const Less& less;

	бул operator()(const ЭлтСтабСорта__<T>& a, const ЭлтСтабСорта__<T>& b) const {
		if(less(a.значение, b.значение)) return true;
		return less(b.значение, a.значение) ? false : a.индекс < b.индекс;
	}

	СтабСортМеньшее__(const Less& less) : less(less) {}
};

template <class II, class VI, class K>
struct ИндексСортОбходчик__
{
	typedef ИндексСортОбходчик__<II, VI, K> Обх;

	ИндексСортОбходчик__(II ii, VI vi) : ii(ii), vi(vi) {}

	Обх&       operator ++ ()               { ++ii; ++vi; return *this; }
	Обх&       operator -- ()               { --ii; --vi; return *this; }
	const K&    operator *  () const         { return *ii; }
	Обх        operator +  (цел i) const    { return Обх(ii + i, vi + i); }
	Обх        operator -  (цел i) const    { return Обх(ii - i, vi - i); }
	цел         operator -  (Обх b) const   { return (цел)(ii - b.ii); }
	бул        operator == (Обх b) const   { return ii == b.ii; }
	бул        operator != (Обх b) const   { return ii != b.ii; }
	бул        operator <  (Обх b) const   { return ii <  b.ii; }
	бул        operator <= (Обх b) const   { return ii <=  b.ii; }
	friend проц IterSwap    (Обх a, Обх b) { IterSwap(a.ii, b.ii); IterSwap(a.vi, b.vi); }

	II          ii;
	VI          vi;
};

template <class II, class VI, class WI, class XI, class K>
struct ИндексСорт3Обходчик__
{
	typedef ИндексСорт3Обходчик__<II, VI, WI, XI, K> Обх;

	ИндексСорт3Обходчик__(II ii, VI vi, WI wi, XI xi) : ii(ii), vi(vi), wi(wi), xi(xi) {}

	Обх&       operator ++ ()               { ++ii; ++vi; ++wi; ++xi; return *this; }
	Обх&       operator -- ()               { --ii; --vi; --wi; --xi; return *this; }
	const K&    operator *  () const         { return *ii; }
	Обх        operator +  (цел i) const    { return Обх(ii + i, vi + i, wi + i, xi + i); }
	Обх        operator -  (цел i) const    { return Обх(ii - i, vi - i, wi - i, xi - i); }
	цел         operator -  (Обх b) const   { return (цел)(ii - b.ii); }
	бул        operator == (Обх b) const   { return ii == b.ii; }
	бул        operator != (Обх b) const   { return ii != b.ii; }
	бул        operator <  (Обх b) const   { return ii <  b.ii; }
	бул        operator <= (Обх b) const   { return ii <=  b.ii; }
	friend проц IterSwap    (Обх a, Обх b) { IterSwap(a.ii, b.ii); IterSwap(a.vi, b.vi); IterSwap(a.wi, b.wi); IterSwap(a.xi, b.xi); }

	II          ii;
	VI          vi;
	WI          wi;
	XI          xi;
};

template <class I, class V>
struct ОбходчикПорядкаСорта__ : ОпыПостфикс< ОбходчикПорядкаСорта__<I, V> >
{
	typedef ОбходчикПорядкаСорта__<I, V> Обх;

	ОбходчикПорядкаСорта__(цел *ii, I vi) : ii(ii), vi(vi) {}

	Обх&       operator ++ ()               { ++ii; return *this; }
	Обх&       operator -- ()               { --ii; return *this; }
	const V&    operator *  () const         { return *(vi + *ii); }
	Обх        operator +  (цел i) const    { return Обх(ii + i, vi); }
	Обх        operator -  (цел i) const    { return Обх(ii - i, vi); }
	цел         operator -  (Обх b) const   { return цел(ii - b.ii); }
	бул        operator == (Обх b) const   { return ii == b.ii; }
	бул        operator != (Обх b) const   { return ii != b.ii; }
	бул        operator <  (Обх b) const   { return ii < b.ii; }
	бул        operator <= (Обх b) const   { return ii <=  b.ii; }
	friend проц IterSwap    (Обх a, Обх b) { IterSwap(a.ii, b.ii); }

	цел        *ii;
	I           vi;
};

template <class I, class T>
struct ОбходчикПорядкаСтабСорта__ : ОпыПостфикс< ОбходчикПорядкаСтабСорта__<I, T> >
{
	typedef ОбходчикПорядкаСтабСорта__<I, T> Обх;

	ОбходчикПорядкаСтабСорта__(цел *ii, I vi) : ii(ii), vi(vi) {}

	Обх&       operator ++ ()               { ++ii; return *this; }
	Обх&       operator -- ()               { --ii; return *this; }
	Обх        operator +  (цел i) const    { return Обх(ii + i, vi); }
	Обх        operator -  (цел i) const    { return Обх(ii - i, vi); }
	цел         operator -  (Обх b) const   { return цел(ii - b.ii); }
	бул        operator == (Обх b) const   { return ii == b.ii; }
	бул        operator != (Обх b) const   { return ii != b.ii; }
	бул        operator <  (Обх b) const   { return ii < b.ii; }
	бул        operator <= (Обх b) const   { return ii <=  b.ii; }
	friend проц IterSwap    (Обх a, Обх b) { IterSwap(a.ii, b.ii); }

	ЭлтСтабСорта__<T> operator*() const    { return ЭлтСтабСорта__<T>(*(vi + *ii), *ii); }

	цел        *ii;
	I           vi;
};

template <class T, class B = ПустойКласс>
class ОпыРеляций : public B
{
public:
	friend бул operator >  (const T& a, const T& b)  { return b < a; }
	friend бул operator != (const T& a, const T& b)  { return !(a == b); }
	friend бул operator <= (const T& a, const T& b)  { return !(b < a); }
	friend бул operator >= (const T& a, const T& b)  { return !(a < b); }
};

template <class U, class V, class B = ПустойКласс>
class ОпыСложения : public B
{
public:
	friend U& operator -= (U& a, const V& b)          { a += -b; return a; }
	friend U  operator +  (const U& a, const V& b)    { U x(a); x += b; return x; }
	friend U  operator -  (const U& a, const V& b)    { U x(a); x += -b; return x; }
};

template <class T, class B = ПустойКласс>
class ОпыПостфикс : public B
{
public:
	friend T operator ++ (T& i, цел)                  { T x = i; ++i; return x; }
	friend T operator -- (T& i, цел)                  { T x = i; --i; return x; }
};

template <class T, цел (*сравни)(T a, T b), class B = ПустойКласс>
class ОпыРелСравнения : public B
{
public:
	friend бул operator <  (T a, T b) { return (*сравни)(a, b) <  0; }
	friend бул operator >  (T a, T b) { return (*сравни)(a, b) >  0; }
	friend бул operator == (T a, T b) { return (*сравни)(a, b) == 0; }
	friend бул operator != (T a, T b) { return (*сравни)(a, b) != 0; }
	friend бул operator <= (T a, T b) { return (*сравни)(a, b) <= 0; }
	friend бул operator >= (T a, T b) { return (*сравни)(a, b) >= 0; }
};

template <class T, class B = ПустойКласс>
struct Сравнимое : public B
{
public:
	friend бул operator <  (const T& a, const T& b) { return a.сравни(b) < 0; }
	friend бул operator >  (const T& a, const T& b) { return a.сравни(b) > 0; }
	friend бул operator == (const T& a, const T& b) { return a.сравни(b) == 0; }
	friend бул operator != (const T& a, const T& b) { return a.сравни(b) != 0; }
	friend бул operator <= (const T& a, const T& b) { return a.сравни(b) <= 0; }
	friend бул operator >= (const T& a, const T& b) { return a.сравни(b) >= 0; }
	friend цел  сравниЗнак(const T& a, const T& b)   { return a.сравни(b); }
};

template <class II, class VI, class WI, class K>
struct ИндексСорт2Обходчик__
{
	typedef ИндексСорт2Обходчик__<II, VI, WI, K> Обх;

	ИндексСорт2Обходчик__(II ii, VI vi, WI wi) : ii(ii), vi(vi), wi(wi) {}

	Обх&       operator ++ ()               { ++ii; ++vi; ++wi; return *this; }
	Обх&       operator -- ()               { --ii; --vi; --wi; return *this; }
	const K&    operator *  () const         { return *ii; }
	Обх        operator +  (цел i) const    { return Обх(ii + i, vi + i, wi + i); }
	Обх        operator -  (цел i) const    { return Обх(ii - i, vi - i, wi - i); }
	цел         operator -  (Обх b) const   { return (цел)(ii - b.ii); }
	бул        operator == (Обх b) const   { return ii == b.ii; }
	бул        operator != (Обх b) const   { return ii != b.ii; }
	бул        operator <  (Обх b) const   { return ii <  b.ii; }
	бул        operator <= (Обх b) const   { return ii <=  b.ii; }
	friend проц IterSwap    (Обх a, Обх b) { IterSwap(a.ii, b.ii); IterSwap(a.vi, b.vi); IterSwap(a.wi, b.wi); }

	II          ii;
	VI          vi;
	WI          wi;
};

template <class T, class B = ПустойКласс>
class СКлоном : public B {
public:
	friend T клонируй(const T& ист) { T c(ист, 1); return c; }
};

template <class T, class B = ПустойКласс>
class ОпцияГлубокойКопии : public B {
public:
#ifdef DEPRECATED
	friend T& operator<<=(T& приёмник, const T& ист)
	{ if(&приёмник != &ист) { (&приёмник)->~T(); ::new(&приёмник) T(ист, 1); } return приёмник; }
#endif
	friend T  клонируй(const T& ист) { T c(ист, 1); return c; }
};

template <class T, class B = ПустойКласс>
class ОпцияДвижимогоИГлубКопии : public B {
	friend проц подтвердиДвиж0(T *) {}
#ifdef DEPRECATED
	friend T& operator<<=(T& приёмник, const T& ист)
	{ if(&приёмник != &ист) { (&приёмник)->~T(); ::new(&приёмник) T(ист, 1); } return приёмник; }
#endif
	friend T  клонируй(const T& ист) { T c(ист, 1); return c; }
};

template <class T>
class СГлубокойКопией : public T {
public:
	СГлубокойКопией(const T& a) : T(a, 1)                 {}
	СГлубокойКопией(const T& a, цел) : T(a, 1)            {}

	СГлубокойКопией(const СГлубокойКопией& a) : T(a, 1)      {}
	СГлубокойКопией& operator=(const СГлубокойКопией& a)     { (T&)*this = пикуй(T(a, 1)); return *this; }

	СГлубокойКопией(T&& a) : T(пикуй(a))                   {}
	СГлубокойКопией& operator=(T&& a)                     { (T&)*this = пикуй(a); return *this; }

	СГлубокойКопией(СГлубокойКопией&& a) : T(пикуй(a))        {}
	СГлубокойКопией& operator=(СГлубокойКопией&& a)          { (T&)*this = пикуй(a); return *this; }

	СГлубокойКопией()                                     {}
};

template<typename Рез, typename... ТипыАрг>
class Функция<Рез(ТипыАрг...)> : Движ<Функция<Рез(ТипыАрг...)>> {
	
	
	struct ОбёрткаОснова {
		Атомар  refcount;

		virtual Рез выполни(ТипыАрг... арги) = 0;
		
		ОбёрткаОснова() { refcount = 1; }
		virtual ~ОбёрткаОснова() {}
	};

	template <class F>
	struct Обёртка : ОбёрткаОснова {
		F фн;
		virtual Рез выполни(ТипыАрг... арги) { return фн(арги...); }

		Обёртка(F&& фн) : фн(пикуй(фн)) {}
	};

	template <class F>
	struct Обёртка2 : ОбёрткаОснова {
		Функция l;
		F        фн;

		virtual Рез выполни(ТипыАрг... арги) { l(арги...); return фн(арги...); }

		Обёртка2(const Функция& l, F&& фн) : l(l), фн(пикуй(фн)) {}
		Обёртка2(const Функция& l, const F& фн) : l(l), фн(фн) {}
	};

	ОбёрткаОснова *укз;
	
	static проц освободи(ОбёрткаОснова *укз) {
		if(укз && атомнДек(укз->refcount) == 0)
			delete укз;
	}
	
	проц копируй(const Функция& a) {
		укз = a.укз;
		if(укз)
			атомнИнк(укз->refcount);
	}
	
	проц подбери(Функция&& ист) {
		укз = ист.укз;
		ист.укз = NULL;
	}

public:
	Функция()                                 { укз = NULL; }

	Функция(const Обнул&)                    { укз = NULL; }
	
	template <class F> Функция(F фн)          { укз = new Обёртка<F>(пикуй(фн)); }
	
	Функция(const Функция& ист)              { копируй(ист); }
	Функция& operator=(const Функция& ист)   { auto b = укз; копируй(ист); освободи(b); return *this; }

	Функция(Функция&& ист)                   { подбери(пикуй(ист)); }
	Функция& operator=(Функция&& ист)        { if(&ист != this) { освободи(укз); укз = ист.укз; ист.укз = NULL; } return *this; }
	
	Функция прокси() const                     { return [=] (ТипыАрг... арги) { return (*this)(арги...); }; }

	template <class F>
	Функция& operator<<(F фн)                 { if(!укз) { подбери(пикуй(фн)); return *this; }
	                                             ОбёрткаОснова *b = укз; укз = new Обёртка2<F>(*this, пикуй(фн)); освободи(b); return *this; }

	Функция& operator<<(const Функция& фн)   { if(!укз) { копируй(фн); return *this; }
	                                             ОбёрткаОснова *b = укз; укз = new Обёртка2<Функция>(*this, фн); освободи(b); return *this; }

	Функция& operator<<(Функция&& фн)        { if(!укз) { подбери(пикуй(фн)); return *this; }
	                                             ОбёрткаОснова *b = укз; укз = new Обёртка2<Функция>(*this, пикуй(фн)); освободи(b); return *this; }

	Рез operator()(ТипыАрг... арги) const     { return укз ? укз->выполни(арги...) : Рез(); }
	
	operator бул() const                      { return укз; }
	проц очисть()                               { освободи(укз); укз = NULL; }

	~Функция()                                { освободи(укз); }

	friend Функция прокси(const Функция& a)   { return a.прокси(); }
	friend проц разверни(Функция& a, Функция& b) { разверни(a.укз, b.укз); }
};

template <typename... ТипыАрг>
using Событие = Функция<проц (ТипыАрг...)>;

template <typename... ТипыАрг>
using Врата = Функция<бул (ТипыАрг...)>;

template <class Диапазон>
using ТипЗначенияМассива = typename std::remove_reference<decltype((*ДеклУкз__<Диапазон>())[0])>::type;

template <class V>
class ОбходчикКонстИ {
	typedef ТипЗначенияМассива<V> T;

	const V       *cont;
	цел            ii;
	struct NP { цел dummy; };

public:
	const T&       operator*() const       { return (*cont)[ii]; }
	const T       *operator->() const      { return &(*cont)[ii]; }
	const T&       operator[](цел i) const { return (*cont)[ii + i]; }

	ОбходчикКонстИ& operator++()           { ++ii; return *this; }
	ОбходчикКонстИ& operator--()           { --ii; return *this; }
	ОбходчикКонстИ  operator++(цел)        { ОбходчикКонстИ t = *this; ++ii; return t; }
	ОбходчикКонстИ  operator--(цел)        { ОбходчикКонстИ t = *this; --ii; return t; }

	ОбходчикКонстИ& operator+=(цел d)      { ii += d; return *this; }
	ОбходчикКонстИ& operator-=(цел d)      { ii -= d; return *this; }

	ОбходчикКонстИ  operator+(цел d) const { return ОбходчикКонстИ(*cont, ii + d); }
	ОбходчикКонстИ  operator-(цел d) const { return ОбходчикКонстИ(*cont, ii - d); }

	цел  operator-(const ОбходчикКонстИ& b) const   { return ii - b.ii; }

	бул operator==(const ОбходчикКонстИ& b) const  { return ii == b.ii; }
	бул operator!=(const ОбходчикКонстИ& b) const  { return ii != b.ii; }
	бул operator<(const ОбходчикКонстИ& b) const   { return ii < b.ii; }
	бул operator>(const ОбходчикКонстИ& b) const   { return ii > b.ii; }
	бул operator<=(const ОбходчикКонстИ& b) const  { return ii <= b.ii; }
	бул operator>=(const ОбходчикКонстИ& b) const  { return ii >= b.ii; }

	operator бул() const     { return ii < 0; }

	ОбходчикКонстИ()          {}
	ОбходчикКонстИ(NP *null)  { ПРОВЕРЬ(null == NULL); ii = -1; }
	ОбходчикКонстИ(const V& _cont, цел ii) : cont(&_cont), ii(ii) {}

	СОВМЕСТИМОСТЬ_С_ОБХОДЧИКОМ_СТЛ
};

template <class V>
class ОбходчикИ {
	typedef ТипЗначенияМассива<V> T;

	V             *cont;
	цел            ii;
	struct NP { цел dummy; };

public:
	T&       operator*()              { return (*cont)[ii]; }
	T       *operator->()             { return &(*cont)[ii]; }
	T&       operator[](цел i)        { return (*cont)[ii + i]; }

	const T& operator*() const        { return (*cont)[ii]; }
	const T *operator->() const       { return &(*cont)[ii]; }
	const T& operator[](цел i) const  { return (*cont)[ii + i]; }

	ОбходчикИ& operator++()           { ++ii; return *this; }
	ОбходчикИ& operator--()           { --ii; return *this; }
	ОбходчикИ  operator++(цел)        { ОбходчикИ t = *this; ++ii; return t; }
	ОбходчикИ  operator--(цел)        { ОбходчикИ t = *this; --ii; return t; }

	ОбходчикИ& operator+=(цел d)      { ii += d; return *this; }
	ОбходчикИ& operator-=(цел d)      { ii -= d; return *this; }

	ОбходчикИ  operator+(цел d) const { return ОбходчикИ(*cont, ii + d); }
	ОбходчикИ  operator-(цел d) const { return ОбходчикИ(*cont, ii - d); }

	цел  operator-(const ОбходчикИ& b) const   { return ii - b.ii; }

	бул operator==(const ОбходчикИ& b) const  { return ii == b.ii; }
	бул operator!=(const ОбходчикИ& b) const  { return ii != b.ii; }
	бул operator<(const ОбходчикИ& b) const   { return ii < b.ii; }
	бул operator>(const ОбходчикИ& b) const   { return ii > b.ii; }
	бул operator<=(const ОбходчикИ& b) const  { return ii <= b.ii; }
	бул operator>=(const ОбходчикИ& b) const  { return ii >= b.ii; }

	operator бул() const                      { return ii < 0; }

	ОбходчикИ()          {}
	ОбходчикИ(NP *null)  { ПРОВЕРЬ(null == NULL); ii = -1; }
	ОбходчикИ(V& _cont, цел ii) : cont(&_cont), ii(ii) {}

	СОВМЕСТИМОСТЬ_С_ОБХОДЧИКОМ_СТЛ
};

//Range.h
template <class Диапазон>
using ТипЗначУ = typename std::remove_reference<decltype(*ДеклУкз__<typename std::remove_reference<Диапазон>::type>()->begin())>::type;

template <class Диапазон>
using ОбходчикУ = decltype(ДеклУкз__<typename std::remove_reference<Диапазон>::type>()->begin());

template <class Диапазон>
using КонстОбходчикУ = decltype(ДеклУкз__<const typename std::remove_reference<Диапазон>::type>()->begin());

template <class I>
class КлассСубДиапазон {
	I   l;
	цел count;

public:
	typedef typename std::remove_reference<decltype(*l)>::type value_type;

	цел дайСчёт() const { return count; }

	КлассСубДиапазон& пиши()                   { return *this; }

	value_type& operator[](цел i) const { ПРОВЕРЬ(i >= 0 && i < count); return l[i]; }
	I  begin() const { return l; }
	I  end() const { return l + count; }

	Ткст   вТкст() const                            { return AsStringArray(*this); }
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	КлассСубДиапазон(I begin, цел count) : l(begin), count(count)   {}
	КлассСубДиапазон(I begin, I end) : l(begin), count(цел(end - begin)) {}
	КлассСубДиапазон() {} // MSC bug workaround
};

template <class I>
КлассСубДиапазон<I> СубДиапазон(I l, I h)
{
	return КлассСубДиапазон<I>(l, h);
}

template <class I>
КлассСубДиапазон<I> СубДиапазон(I l, цел count)
{
	return КлассСубДиапазон<I>(l, count);
}

template <class C>
auto СубДиапазон(C&& c, цел pos, цел count) -> decltype(СубДиапазон(c.begin() + pos, count))
{
	return СубДиапазон(c.begin() + pos, count);
}

template <class C> using СубДиапазонУ = decltype(СубДиапазон(((C *)0)->begin(), ((C *)0)->end()));

template <class T>
struct КлассКонстДиапазон {
	T   значение;
	цел count;

	typedef T value_type;
	typedef value_type ТипЗнач;

	const value_type& operator[](цел i) const { return значение; }
	цел дайСчёт() const                      { return count; }

	typedef ОбходчикКонстИ<КлассКонстДиапазон> Обходчик;

	Обходчик begin() const { return Обходчик(*this, 0); }
	Обходчик end() const { return Обходчик(*this, count); }

	Ткст   вТкст() const                            { return AsStringArray(*this); }
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	КлассКонстДиапазон(const T& значение, цел count) : значение(значение), count(count) {}
	КлассКонстДиапазон(цел count) : count(count) {}
	КлассКонстДиапазон() {} // MSC bug workaround
};

template <class T>
КлассКонстДиапазон<T> КонстДиапазон(const T& значение, цел count)
{
	return КлассКонстДиапазон<T>(значение, count);
}

template <class T>
КлассКонстДиапазон<T> КонстДиапазон(цел count)
{
	return КлассКонстДиапазон<T>(count);
}

template <class ДиапазонОснова>
struct КлассРеверсДиапазон {
	typename std::remove_reference<ДиапазонОснова>::type& r;

	typedef ТипЗначУ<ДиапазонОснова>                       value_type;
	typedef value_type                                   ТипЗнач;

	const value_type& operator[](цел i) const            { return r[r.дайСчёт() - i - 1]; }
	value_type& operator[](цел i)                        { return r[r.дайСчёт() - i - 1]; }
	цел дайСчёт() const                                 { return r.дайСчёт(); }

	typedef ОбходчикИ<КлассРеверсДиапазон>                  Обходчик;
	typedef ОбходчикКонстИ<КлассРеверсДиапазон>             КонстОбходчик;

	КлассРеверсДиапазон& пиши()                           { return *this; }

	КонстОбходчик begin() const                          { return КонстОбходчик(*this, 0); }
	КонстОбходчик end() const                            { return КонстОбходчик(*this, r.дайСчёт()); }

	Обходчик begin()                                     { return Обходчик(*this, 0); }
	Обходчик end()                                       { return Обходчик(*this, r.дайСчёт()); }

	Ткст   вТкст() const                            { return AsStringArray(*this); }
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	КлассРеверсДиапазон(ДиапазонОснова& r) : r(r) {}
	КлассРеверсДиапазон() {} // MSC bug workaround
};

template <class ДиапазонОснова>
КлассРеверсДиапазон<ДиапазонОснова> РеверсДиапазон(ДиапазонОснова&& r)
{
	return КлассРеверсДиапазон<ДиапазонОснова>(r);
}

template <class ДиапазонОснова>
struct КлассДиапазонОбзора {
	typename std::remove_reference<ДиапазонОснова>::тип *r;
	Вектор<цел> ndx;

	typedef ТипЗначУ<ДиапазонОснова> value_type;
	typedef value_type ТипЗнач;

	const value_type& operator[](цел i) const { return (*r)[ndx[i]]; }
	value_type& operator[](цел i)             { return (*r)[ndx[i]]; }
	цел дайСчёт() const                      { return ndx.дайСчёт(); }

	typedef ОбходчикИ<КлассДиапазонОбзора> Обходчик;
	typedef ОбходчикКонстИ<КлассДиапазонОбзора> КонстОбходчик;

	КлассДиапазонОбзора& пиши()                   { return *this; }

	КонстОбходчик begin() const { return КонстОбходчик(*this, 0); }
	КонстОбходчик end() const  { return КонстОбходчик(*this, ndx.дайСчёт()); }

	Обходчик begin() { return Обходчик(*this, 0); }
	Обходчик end()   { return Обходчик(*this, ndx.дайСчёт()); }

	Ткст   вТкст() const                            { return AsStringArray(*this); }
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	КлассДиапазонОбзора(ДиапазонОснова& r, Вектор<цел>&& ndx) : r(&r), ndx(пикуй(ndx)) {}
	КлассДиапазонОбзора() {} // MSC bug workaround
};

template <class ДиапазонОснова>
КлассДиапазонОбзора<ДиапазонОснова> ДиапазонОбзора(ДиапазонОснова&& r, Вектор<цел>&& ndx)
{
	return КлассДиапазонОбзора<ДиапазонОснова>(r, пикуй(ndx));
}

template <class ДиапазонОснова, class Предикат>
КлассДиапазонОбзора<ДиапазонОснова> ДиапазонФильтр(ДиапазонОснова&& r, Предикат p)
{
	return КлассДиапазонОбзора<ДиапазонОснова>(r, найдиВсе(r, p));
}

template <class ДиапазонОснова, class Предикат>
КлассДиапазонОбзора<ДиапазонОснова> СортированныйДиапазон(ДиапазонОснова&& r, Предикат p)
{
	return КлассДиапазонОбзора<ДиапазонОснова>(r, дайСортПорядок(r, p));
}

template <class ДиапазонОснова>
КлассДиапазонОбзора<ДиапазонОснова> СортированныйДиапазон(ДиапазонОснова&& r)
{
	return КлассДиапазонОбзора<ДиапазонОснова>(r, дайСортПорядок(r));
}
////////////////////

template <class T, бцел тип = UNKNOWN_V, class B = ПустойКласс>
class ТипЗнач : public B {
public:
	static бцел номерТипаЗнач()                      { return тип == UNKNOWN_V ? СтатичНомТипа<T>() + 0x8000000 : тип; }
	friend бцел номерТипаЗнач(const T*)              { return T::номерТипаЗнач(); }
	
	бул     экзПусто_ли() const                 { return false; }
	проц     сериализуй(Поток& s)                   { НИКОГДА(); }
	проц     вРяр(РярВВ& xio)                     { НИКОГДА(); }
	проц     вДжейсон(ДжейсонВВ& jio)                   { НИКОГДА(); }
	т_хэш   дайХэшЗнач() const                   { return 0; }
	бул     operator==(const T&) const             { НИКОГДА(); return false; }
	//Ткст   вТкст() const                       { return typeid(T).name(); }
	цел      сравни(const T&) const                { НИКОГДА(); return 0; }
	цел      полиСравни(const Значение&) const        { НИКОГДА(); return 0; }
	
	operator РефНаТипЗнач();
};

template <class T, бцел тип, class B = ПустойКласс> // Backward compatiblity
class ПрисвойНомТипаЗнач : public ТипЗнач<T, тип, B> {};

template <class T>
struct СтдРеф : public РефМенеджер {
	virtual проц  устЗначение(ук p, const Значение& v) { *(T *) p = (T)v; }
	virtual Значение дайЗначение(кук p)           { return *(const T *) p; }
	virtual цел   дайТип()                         { return дайНомТипаЗнач<T>(); }
	virtual бул  пусто_ли(кук p)             { return пусто_ли(*(T *) p); }
	virtual проц  устПусто(ук p)                  { устПусто(*(T *)p); }
	virtual ~СтдРеф() {}
};

template <typename F, typename S, class R>
struct КлассЛямбдаПреобр : Преобр {
	Функция<Значение(const Значение& w)> формат;
	Функция<Значение(const Значение& text)> scan;
	Функция<цел(цел)> фильтр;

	virtual Значение фмт(const Значение& q) const { return формат(q); }
	virtual Значение скан(const Значение& text) const { return scan(text); }
	virtual цел фильтруй(цел chr) const { return фильтр(chr); }
	
	КлассЛямбдаПреобр(F формат, S scan, R фильтр) : формат(формат), scan(scan), фильтр(фильтр) {}
};

template <class IZE>
struct LambdaIzeVar {
	IZE& ize;

	проц вДжейсон(ДжейсонВВ& io) { ize(io); }
	проц вРяр(РярВВ& io) { ize(io); }

	LambdaIzeVar(IZE& ize) : ize(ize) {}
};


template <class K, class T, class V, class Less>
class ФиксАМап {
protected:
	Вектор<K> ключ;
	V значение;

public:
	T&       добавь(const K& k, const T& x)       { ключ.добавь(k); return значение.добавь(x); }
	T&       добавьПодбор(const K& k, T&& x)        { ключ.добавь(k); return значение.добавьПодбор(пикуй(x)); }
	T&       добавь(const K& k)                   { ключ.добавь(k); return значение.добавь(); }
	
	проц     финиш()                          { индексСортируй(ключ, значение, Less()); сожми(); }

	цел      найди(const K& k) const            { return найдиБинар(ключ, k, Less()); }
	цел      найдиСледщ(цел i) const             { return i + 1 < ключ.дайСчёт() && ключ[i] == ключ[i + 1] ? i + 1 : -1; }

	T&       дай(const K& k)                   { return значение[найди(k)]; }
	const T& дай(const K& k) const             { return значение[найди(k)]; }
	const T& дай(const K& k, const T& d) const { цел i = найди(k); return i >= 0 ? значение[i] : d; }

	T       *найдиУк(const K& k)               { цел i = найди(k); return i >= 0 ? &значение[i] : NULL; }
	const T *найдиУк(const K& k) const         { цел i = найди(k); return i >= 0 ? &значение[i] : NULL; }

	const T& operator[](цел i) const           { return значение[i]; }
	T&       operator[](цел i)                 { return значение[i]; }
	цел      дайСчёт() const                  { return значение.дайСчёт(); }
	бул     пустой() const                   { return значение.пустой(); }
	проц     очисть()                           { ключ.очисть(); значение.очисть(); }
	проц     сожми()                          { значение.сожми(); ключ.сожми(); }
	проц     резервируй(цел xtra)                 { значение.резервируй(xtra); ключ.резервируй(xtra); }
	цел      дайРазмест() const                  { return значение.дайРазмест(); }

	const K& дайКлюч(цел i) const               { return ключ[i]; }

#ifdef РНЦП
	проц     сериализуй(Поток& s);
	проц     вРяр(РярВВ& xio);
	проц     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
#endif

	проц     разверни(ФиксАМап& x)                { разверни(значение, x.значение); разверни(ключ, x.ключ); }

	const Вектор<K>& дайКлючи() const           { return ключ; }
	Вектор<K>        подбериКлючи()                { return пикуй(ключ); }

	const V&         дайЗначения() const         { return значение; }
	V&               дайЗначения()               { return значение; }
	V                подбериЗначения()              { return пикуй(значение); }
	
	ФиксАМап& operator()(const K& k, const T& v)       { добавь(k, v); return *this; }

	ФиксАМап()                                         {}
	ФиксАМап(const ФиксАМап& s, цел) : ключ(s.ключ, 0), значение(s.значение, 0) {}
	ФиксАМап(Вектор<K>&& ключ, V&& знач) : ключ(пикуй(ключ)), значение(пикуй(знач)) {}

	typedef КонстОбходчикУ<V>  КонстОбходчик;
	typedef ОбходчикУ<V>       Обходчик;

	Обходчик         begin()                                      { return значение.begin(); }
	Обходчик         end()                                        { return значение.end(); }
	КонстОбходчик    begin() const                                { return значение.begin(); }
	КонстОбходчик    end() const                                  { return значение.end(); }

#ifdef DEPRECATED
	typedef V                          КонтейнерЗначений;
	typedef T                          ТипЗнач;

	typedef Вектор<K> КонтейнерКлючей;
	typedef K         ТипКлюча;
	typedef КонстОбходчикУ<Вектор<K>> КонстОбходчикКлючей;

	КонстОбходчикКлючей стартКлючи() const                             { return ключ.старт(); }
	КонстОбходчикКлючей стопКлючи() const                               { return ключ.стоп(); }
	КонстОбходчикКлючей дайОбхКлючей(цел pos) const                    { return ключ.дайОбх(pos); }

	Обходчик         дайОбх(цел pos)                             { return значение.дайОбх(pos); }
	КонстОбходчик    дайОбх(цел pos) const                       { return значение.дайОбх(pos); }
#endif
};

template <class K, class T, class Less = StdLess<K> >
class ФиксированнВекторМап : public ОпцияДвижимогоИГлубКопии<ФиксированнВекторМап<K, T, Less> >,
                       public ФиксАМап< K, T, Вектор<T>, Less > {
    typedef ФиксАМап< K, T, Вектор<T>, Less > B;
public:
	ФиксированнВекторМап(const ФиксированнВекторМап& s, цел) : ФиксАМап<K, T, Вектор<T>, Less>(s, 1) {}
	ФиксированнВекторМап(Вектор<K>&& ключ, Вектор<T>&& знач) : ФиксАМап<K, T, Вектор<T>, Less>(пикуй(ключ), пикуй(знач)) {}
	ФиксированнВекторМап()                                                       {}

	friend проц    разверни(ФиксированнВекторМап& a, ФиксированнВекторМап& b)      { a.B::разверни(b); }

	typedef typename ФиксАМап< K, T, Вектор<T>, Less >::КонстОбходчик КонстОбходчик; // GCC bug (?)
	typedef typename ФиксАМап< K, T, Вектор<T>, Less >::Обходчик      Обходчик; // GCC bug (?)
	СОВМЕСТИМОСТЬ_С_МАП_СТЛ(ФиксированнВекторМап<K _cm_ T _cm_ Less>)
};

template <class K, class T, class Less = StdLess<K> >
class ФиксированнМассивМап : public ОпцияДвижимогоИГлубКопии< ФиксированнМассивМап<K, T, Less> >,
                      public ФиксАМап< K, T, Массив<T>, Less > {
	typedef ФиксАМап< K, T, Массив<T>, Less > B;
public:
	T&        добавь(const K& k, const T& x)          { return B::добавь(k, x); }
	T&        добавь(const K& k)                      { return B::добавь(k); }
	T&        добавь(const K& k, T *newt)             { B::ключ.добавь(k); return B::значение.добавь(newt); }
	template <class TT, class... Арги>
	TT&       создай(const K& k, Арги&&... арги)   { TT *q = new TT(std::forward<Арги>(арги)...); B::ключ.добавь(k); return static_cast<TT&>(B::значение.добавь(q)); }

	ФиксированнМассивМап(const ФиксированнМассивМап& s, цел) : ФиксАМап<K, T, Массив<T>, Less>(s, 1) {}
	ФиксированнМассивМап(Вектор<K>&& ndx, Массив<T>&& знач) : ФиксАМап<K, T, Массив<T>, Less>(пикуй(ndx), пикуй(знач)) {}
	ФиксированнМассивМап() {}

	friend проц    разверни(ФиксированнМассивМап& a, ФиксированнМассивМап& b)        { a.B::разверни(b); }

	typedef typename ФиксАМап< K, T, Массив<T>, Less >::КонстОбходчик КонстОбходчик; // GCC bug (?)
	typedef typename ФиксАМап< K, T, Массив<T>, Less >::Обходчик      Обходчик; // GCC bug (?)
	СОВМЕСТИМОСТЬ_С_МАП_СТЛ(ФиксированнМассивМап<K _cm_ T _cm_ Less>)
};

template <class T> struct Slaved_InVector__;
template <class T> struct Slaved_InArray__;
template <class K, class TT, class Lss, class Данные> class ОтсортАМап;
template <class K, class TT, class Less> class ОтсортВекторМап;

struct InVectorSlave__ {
	virtual проц очисть() = 0;
	virtual проц счёт(цел n) = 0;
	virtual проц разбей(цел blki, цел nextsize) = 0;
	virtual проц добавьПервый() = 0;
	virtual проц вставь(цел blki, цел pos) = 0;
	virtual проц соедини(цел blki) = 0;
	virtual проц удали(цел blki, цел pos, цел n) = 0;
	virtual проц удалиБлок(цел blki, цел n) = 0;
	virtual проц Индекс(цел blki, цел n) = 0;
	virtual проц реиндексируй() = 0;
	virtual проц сожми() = 0;
};

template <class T>
class ВхоВектор : public ОпцияДвижимогоИГлубКопии< ВхоВектор<T> > {
public:
	class КонстОбходчик;
	class Обходчик;

	template <class K, class TT, class Lss, class Данные> friend class ОтсортАМап;
	template <class K, class TT, class Less> friend class ОтсортВекторМап;
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

	проц подбери(ВхоВектор&& b);

	InVectorSlave__ *Slave()          { return (InVectorSlave__ *)((ббайт *)this + slave); }
	проц SetSlave(InVectorSlave__ *s) { slave = (бкрат)((ббайт *)s - (ббайт *)this); }

	проц устКэш(цел blki, цел offset) const;
	проц очистьКэш() const;
	цел  найдиБлок0(цел& pos, цел& off) const;
	цел  найдиБлок(цел& pos, цел& off) const;
	цел  найдиБлок(цел& pos) const;
	проц устПарБлк();
	
	template <class L>
	цел  найдиВерхнГран(const T& знач, const L& less, цел& off, цел& pos) const;

	template <class L>
	цел  найдиНижнГран(const T& знач, const L& less, цел& off, цел& pos) const;

	проц реиндексируй();
	проц Индекс(цел q, цел n);
	проц счёт(цел n)                               { count += n; }
	проц соедини(цел blki);

	бул JoinSmall(цел blki);
	T   *вставь0(цел ii, цел blki, цел pos, цел off, const T *знач);
	T   *вставь0(цел ii, const T *знач);
	
	проц переустанов();

	проц устОбх(КонстОбходчик& it, цел ii) const;
	проц устНач(КонстОбходчик& it) const;
	проц устКон(КонстОбходчик& it) const;

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

	проц     разверни(цел i1, цел i2)                   { разверни((*this)[i1], (*this)[i2]); }

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
	цел вставьВерхнГран(const T& знач, const L& less);
	цел вставьВерхнГран(const T& знач)                    { return вставьВерхнГран(знач, StdLess<T>()); }
	
	template <class L>
	цел найди(const T& знач, const L& less) const;
	цел найди(const T& знач) const                         { return найди(знач, StdLess<T>()); }

	КонстОбходчик    begin() const                       { КонстОбходчик it; устНач(it); return it; }
	КонстОбходчик    end() const                         { КонстОбходчик it; устКон(it); return it; }

	Обходчик         begin()                             { Обходчик it; устНач(it); return it; }
	Обходчик         end()                               { Обходчик it; устКон(it); return it; }

	ВхоВектор();
	ВхоВектор(ВхоВектор&& v)                               { подбери(пикуй(v)); }
	проц operator=(ВхоВектор&& v)                         { подбери(пикуй(v)); }
	ВхоВектор(const ВхоВектор& v, цел);
	ВхоВектор(std::initializer_list<T> init)              { иниц(); for(const auto& i : init) добавь(i); }

	проц разверни(ВхоВектор& b);

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

	friend проц разверни(ВхоВектор& a, ВхоВектор& b)      { a.разверни(b); }
	
	СОВМЕСТИМОСТЬ_С_ВЕКТОРОМ_СТЛ(ВхоВектор<T>)

	проц DumpIndex() const;
	
	проц резервируй(цел) {} // Does nothing, but needed for unified interface (e.g. StreamContainer)
#ifdef DEPRECATED
	typedef T        ТипЗнач;
	КонстОбходчик    дайОбх(цел pos) const         { КонстОбходчик it; устОбх(it, pos); return it; }
	Обходчик         дайОбх(цел pos)               { Обходчик it; устОбх(it, pos); return it; }
#endif
};

template <class T>
class ВхоВектор<T>::КонстОбходчик {
	const T        *укз;
	const T        *begin;
	const T        *end;
	const ВхоВектор *v;
	цел             offset;
	цел             blki;

	friend class ВхоВектор<T>;
	friend class ВхоВектор<T>::Обходчик;

	проц следщБлк();
	проц предшБлк();

public:
	форс_инлайн цел дайИндекс() const              { return цел(укз - begin) + offset; }

	форс_инлайн КонстОбходчик& operator++()       { ПРОВЕРЬ(укз); if(++укз == end) следщБлк(); return *this; }
	форс_инлайн КонстОбходчик& operator--()       { ПРОВЕРЬ(укз); if(укз == begin) предшБлк(); --укз; return *this; }
	форс_инлайн КонстОбходчик  operator++(цел)    { КонстОбходчик t = *this; ++*this; return t; }
	форс_инлайн КонстОбходчик  operator--(цел)    { КонстОбходчик t = *this; --*this; return t; }

	форс_инлайн КонстОбходчик& operator+=(цел d);
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

	СОВМЕСТИМОСТЬ_С_ОБХОДЧИКОМ_СТЛ
};

template <class T>
class ВхоВектор<T>::Обходчик : public ВхоВектор<T>::КонстОбходчик {
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
	
	typename ВхоВектор<T>::Обходчик& operator=(const B& ист) { *(B *)this = ист; return *this; }

	СОВМЕСТИМОСТЬ_С_ОБХОДЧИКОМ_СТЛ
};

template <class T>
class ВхоМассив : public ОпцияДвижимогоИГлубКопии< ВхоМассив<T> > {
//	template <class K, class TT, class Less, class Данные> friend class ОтсортАМап;
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
	typedef ВхоВектор<ТипУказатель> IV;
	typedef typename ВхоВектор<ТипУказатель>::Обходчик IVIter;
	typedef typename ВхоВектор<ТипУказатель>::Обходчик IVConstIter;

	IV iv;

	T&       дай(цел i) const       { return *(T *)iv[i]; }
	проц     Delete(IVIter i, цел count);
	проц     Delete(цел i, цел count);
	проц     иниц(цел i, цел count);
	проц     освободи()                 { Delete(iv.begin(), дайСчёт()); }

	проц     устОбх(КонстОбходчик& it, цел ii) const;
	проц     устНач(КонстОбходчик& it) const;
	проц     устКон(КонстОбходчик& it) const;

	template <class L>
	struct ALess {
		const L& less;
		
		бул operator()(const ТипУказатель& a, const ТипУказатель& b) const {
			return less(*(T*)a, *(T*)b);
		}
		
		ALess(const L& less) : less(less) {}
	};

	template <class K, class V, class L> friend class ОтсортВекторМап;

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

	проц     разверни(цел i1, цел i2)                   { разверни((*this)[i1], (*this)[i2]); }

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
	цел вставьВерхнГран(const T& знач, const L& lss)       { return iv.вставьВерхнГран(new T(знач), ALess<L>(lss)); }
	цел вставьВерхнГран(const T& знач)                     { return вставьВерхнГран(знач, StdLess<T>()); }
	
	template <class L>
	цел найди(const T& знач, const L& less) const            { return iv.найди((T*)&знач, ALess<L>(less)); }
	цел найди(const T& знач) const                           { return найди(знач, StdLess<T>()); }

	КонстОбходчик    begin() const                  { КонстОбходчик it; устНач(it); return it; }
	КонстОбходчик    end() const                    { КонстОбходчик it; устКон(it); return it; }

	Обходчик         begin()                        { Обходчик it; устНач(it); return it; }
	Обходчик         end()                          { Обходчик it; устКон(it); return it; }

	ВхоМассив() {}
	ВхоМассив(ВхоМассив&& v) : iv(пикуй(v.iv))           {}
	ВхоМассив& operator=(ВхоМассив&& v)                 { if(this != &v) { освободи(); iv.operator=(пикуй(v.iv)); } return *this; }
	ВхоМассив(const ВхоМассив& v, цел);

	~ВхоМассив()                                      { освободи(); }

	ВхоМассив(std::initializer_list<T> init)          { for(const auto& i : init) добавь(i); }

	проц разверни(ВхоМассив& b)                           { iv.разверни(b.iv); }
	
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

	friend проц разверни(ВхоМассив& a, ВхоМассив& b)        { a.разверни(b); }
	
	СОВМЕСТИМОСТЬ_С_ВЕКТОРОМ_СТЛ(ВхоМассив<T>)

	проц резервируй(цел) {} // Does nothing, but needed for unified interface (e.g. StreamContainer)

#ifdef _ОТЛАДКА
	проц DumpIndex();
#endif

#ifdef DEPRECATED
	КонстОбходчик    дайОбх(цел pos) const         { КонстОбходчик it; устОбх(it, pos); return it; }
	Обходчик         дайОбх(цел pos)               { Обходчик it; устОбх(it, pos); return it; }
	typedef T        ТипЗнач;
#endif
};

template <class T>
class ВхоМассив<T>::КонстОбходчик {
	IVConstIter it;

	friend class ВхоМассив<T>;
	friend class ВхоМассив<T>::Обходчик;

public:
	форс_инлайн цел дайИндекс() const              { return it.дайИндекс(); }

	форс_инлайн КонстОбходчик& operator++()       { ++it; return *this; }
	форс_инлайн КонстОбходчик& operator--()       { --it; return *this; }
	форс_инлайн КонстОбходчик  operator++(цел)    { КонстОбходчик t = *this; ++*this; return t; }
	форс_инлайн КонстОбходчик  operator--(цел)    { КонстОбходчик t = *this; --*this; return t; }

	форс_инлайн КонстОбходчик& operator+=(цел d)  { it += d; return *this; }
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

	СОВМЕСТИМОСТЬ_С_ОБХОДЧИКОМ_СТЛ
};

template <class T>
class ВхоМассив<T>::Обходчик : public ВхоМассив<T>::КонстОбходчик {
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

	СОВМЕСТИМОСТЬ_С_ОБХОДЧИКОМ_СТЛ
};

template <class T, class Less = std::less<T> >
class ОтсортИндекс : ОпцияДвижимогоИГлубКопии< ОтсортИндекс<T, Less> > {
	ВхоВектор<T> iv;
	
	template <class K, class TT, class Lss> friend class ОтсортВекторМап;
	template <class K, class TT, class Lss, class Данные> friend class ОтсортАМап;

public:
	цел           добавь(const T& x)                  { return iv.вставьВерхнГран(x, Less()); }
	цел           найдиДобавь(const T& ключ);
	ОтсортИндекс&  operator<<(const T& x)           { добавь(x); return *this; }
	
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
	
	typedef typename ВхоВектор<T>::КонстОбходчик КонстОбходчик;

	КонстОбходчик    begin() const                  { return iv.begin(); }
	КонстОбходчик    end() const                    { return iv.end(); }
	
	const ВхоВектор<T>& дайКлючи()  const             { return iv; }

	ОтсортИндекс()                                        {}
	ОтсортИндекс(const ОтсортИндекс& s, цел) : iv(s.iv, 1) {}
	
	проц разверни(ОтсортИндекс& a)                        { iv.разверни(a.iv); }

	проц     сериализуй(Поток& s)                               { iv.сериализуй(s); }
	проц     вРяр(РярВВ& xio, кткст0 itemtag = "key")    { iv.вРяр(xio, itemtag); }
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

	friend проц разверни(ОтсортИндекс& a, ОтсортИндекс& b){ a.разверни(b); }

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
class ОтсортАМап {
protected:
	ОтсортИндекс<K, Less> ключ;
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

	const ОтсортИндекс<K>& дайИндекс() const          { return ключ; }
	const ВхоВектор<K>& дайКлючи() const              { return ключ.дайКлючи(); }

	Ткст   вТкст() const;
	бул     operator==(const ОтсортАМап& b) const  { return равныМапы(*this, b); }
	бул     operator!=(const ОтсортАМап& b) const  { return !operator==(b); }
	цел      сравни(const ОтсортАМап& b) const     { return сравниМап(*this, b); }
	бул     operator<=(const ОтсортАМап& x) const  { return сравни(x) <= 0; }
	бул     operator>=(const ОтсортАМап& x) const  { return сравни(x) >= 0; }
	бул     operator<(const ОтсортАМап& x) const   { return сравни(x) < 0; }
	бул     operator>(const ОтсортАМап& x) const   { return сравни(x) > 0; }

#ifdef DEPRECATED
	typedef K        ТипКлюча;

	typedef typename ОтсортИндекс<K, Less>::КонстОбходчик КонстОбходчикКлючей;

	КонстОбходчикКлючей стартКлючи() const                             { return ключ.begin(); }
	КонстОбходчикКлючей стопКлючи() const                               { return ключ.end(); }
	КонстОбходчикКлючей дайОбхКлючей(цел pos) const                    { return ключ.дайОбх(pos); }
#endif
};

template <class T>
struct Slaved_InVector__ : InVectorSlave__ {
	typedef ВхоВектор<T> Type;
	ВхоВектор<T> данные;
	T *res;
      
	virtual проц очисть()                          { данные.очисть(); }
	virtual проц счёт(цел n)                     { данные.count += n; }
	virtual проц вставь(цел blki, цел pos);
	virtual проц разбей(цел blki, цел nextsize);
	virtual проц добавьПервый();
	virtual проц удалиБлок(цел blki, цел n)       { данные.данные.удали(blki, n); }
	virtual проц соедини(цел blki)                   { данные.соедини(blki); }
	virtual проц удали(цел blki, цел pos, цел n) { данные.данные[blki].удали(pos, n); }
	virtual проц Индекс(цел blki, цел n)           { данные.Индекс(blki, n); }
	virtual проц реиндексируй()                        { данные.реиндексируй(); }
//	virtual проц сериализуй(Поток& s)             { данные.сериализуй(s); }
	virtual проц сожми()                         { данные.сожми(); }

	T& дай(цел blki, цел i) const                 { return *(T*)&данные.данные[blki][i]; }
};

template <class K, class T, class Less = StdLess<K> >
class ОтсортВекторМап : public ОтсортАМап<K, T, Less, Slaved_InVector__<T> >,
                        public ОпцияДвижимогоИГлубКопии<ОтсортВекторМап<K, T, Less> > {
	typedef Slaved_InVector__<T> Данные;
    typedef ОтсортАМап<K, T, Less, Данные>  B;
    
public:
	T&       добавь(const K& k)                        { B::ключ.добавь(k); return *B::значение.res; }
	T&       добавь(const K& k, const T& x)            { B::ключ.добавь(k); *B::значение.res = клонируй(x); return *B::значение.res; }
	T&       добавь(const K& k, T&& x)                 { B::ключ.добавь(k); *B::значение.res = пикуй(x); return *B::значение.res; }

	цел      найдиДобавь(const K& k)                    { return B::ключ.найдиДобавь(k); }
	цел      найдиДобавь(const K& k, const T& init);

	T&       дайДобавь(const K& k)                     { return B::по(найдиДобавь(k)); }
	T&       дайДобавь(const K& k, const T& x)         { return B::по(найдиДобавь(k, x)); }

	ОтсортВекторМап& operator()(const K& k, const T& v) { добавь(k, v); return *this; }

	ОтсортВекторМап()                               { B::SetSlave(); }
	ОтсортВекторМап(ОтсортВекторМап&&);
	ОтсортВекторМап& operator=(ОтсортВекторМап&&);
	ОтсортВекторМап(const ОтсортВекторМап& s, цел);
	
	ОтсортВекторМап(std::initializer_list<std::pair<K, T>> init) : ОтсортВекторМап() { for(const auto& i : init) добавь(i.first, i.second); }

	проц     разверни(ОтсортВекторМап& x);

#ifdef РНЦП
	проц     сериализуй(Поток& s);
	проц     вРяр(РярВВ& xio);
	проц     вДжейсон(ДжейсонВВ& jio);
	т_хэш   дайХэшЗнач() const                   { return хэшПоСериализу(*this); }
#endif

	const ВхоВектор<T>& дайЗначения() const            { return B::значение.данные; }

	friend проц    разверни(ОтсортВекторМап& a, ОтсортВекторМап& b) { a.разверни(b); }

	typedef typename Данные::Type::КонстОбходчик  КонстОбходчик;
	typedef typename Данные::Type::Обходчик       Обходчик;

	Обходчик         begin()                        { return B::значение.данные.begin(); }
	Обходчик         end()                          { return B::значение.данные.end(); }
	КонстОбходчик    begin() const                  { return B::значение.данные.begin(); }
	КонстОбходчик    end() const                    { return B::значение.данные.end(); }

	СОВМЕСТИМОСТЬ_С_ОТСОРТМАП_СТЛ(ОтсортВекторМап<K _cm_ T _cm_ Less>)

#ifdef DEPRECATED
	typedef T                                       ТипЗнач;
	Обходчик         дайОбх(цел pos)               { return B::значение.данные.дайОбх(pos); }
	КонстОбходчик    дайОбх(цел pos) const         { return B::значение.данные.дайОбх(pos); }
#endif
};

template <class T>
struct Slaved_InArray__ : InVectorSlave__ {
	typedef ВхоМассив<T> Type;
	ВхоМассив<T> данные;
	T         *res;
      
	virtual проц очисть()                          { данные.очисть(); }
	virtual проц счёт(цел n)                     { данные.iv.count += n; }
	virtual проц вставь(цел blki, цел pos);
	virtual проц разбей(цел blki, цел nextsize);
	virtual проц добавьПервый();
	virtual проц удалиБлок(цел blki, цел n)       { данные.iv.данные.удали(blki, n); }
	virtual проц соедини(цел blki)                   { данные.iv.соедини(blki); }
	virtual проц удали(цел blki, цел pos, цел n);
	virtual проц Индекс(цел blki, цел n)           { данные.iv.Индекс(blki, n); }
	virtual проц реиндексируй()                        { данные.iv.реиндексируй(); }
//	virtual проц сериализуй(Поток& s)             { данные.iv.сериализуй(s); }
	virtual проц сожми()                         { данные.iv.сожми(); }

	T& дай(цел blki, цел i) const                 { return *(T*)данные.iv.данные[blki][i]; }
	T *открепи(цел i)                              { T *x = данные.iv[i]; данные.iv[i] = NULL; return x; }
};

template <class K, class T, class Less = StdLess<K> >
class ОтсортМассивМап : public ОпцияДвижимогоИГлубКопии<ОтсортМассивМап<K, T, Less> >,
                        public ОтсортАМап<K, T, Less, Slaved_InArray__<T> > {
	typedef Slaved_InArray__<T> Данные;
    typedef ОтсортАМап<K, T, Less, Данные> B;

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

	const ВхоМассив<T>& дайЗначения() const           { return B::значение.данные; }

	ОтсортМассивМап& operator()(const K& k, const T& v) { добавь(k, v); return *this; }

	ОтсортМассивМап()                              { B::SetSlave(); }
	ОтсортМассивМап(ОтсортМассивМап&&);
	ОтсортМассивМап& operator=(ОтсортМассивМап&&);
	ОтсортМассивМап(const ОтсортМассивМап& s, цел);
	ОтсортМассивМап(std::initializer_list<std::pair<K, T>> init) : ОтсортМассивМап() { for(const auto& i : init) добавь(i.first, i.second); }

#ifdef РНЦП
	проц     сериализуй(Поток& s);
	проц     вРяр(РярВВ& xio);
	проц     вДжейсон(ДжейсонВВ& jio);
	т_хэш   дайХэшЗнач() const                   { return хэшПоСериализу(*this); }
#endif

	проц     разверни(ОтсортМассивМап& x);

	friend проц    разверни(ОтсортМассивМап& a, ОтсортМассивМап& b) { a.разверни(b); }

	typedef typename Данные::Type::КонстОбходчик  КонстОбходчик;
	typedef typename Данные::Type::Обходчик       Обходчик;

	Обходчик         begin()                        { return B::значение.данные.begin(); }
	Обходчик         end()                          { return B::значение.данные.end(); }
	КонстОбходчик    begin() const                  { return B::значение.данные.begin(); }
	КонстОбходчик    end() const                    { return B::значение.данные.end(); }

	СОВМЕСТИМОСТЬ_С_ОТСОРТМАП_СТЛ(ОтсортМассивМап<K _cm_ T _cm_ HashFn>)

#ifdef DEPRECATED
	typedef T                                   ТипЗнач;

	Обходчик         дайОбх(цел pos)               { return B::значение.данные.дайОбх(pos); }
	КонстОбходчик    дайОбх(цел pos) const         { return B::значение.данные.дайОбх(pos); }
#endif
};



////////
#endif