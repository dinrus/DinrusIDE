#ifndef _DinrusPro_NonTpl_h_
#define _DinrusPro_NonTpl_h_
////////////////////////////
#include <DinrusPro/DinrusPro.h>
#include <DinrusPro/Tpl.h>
#include <DinrusPro/TplFuncs.h>
#include <DinrusPro/Funcs.h>
////////////////////////////
class МассивДжейсон;
class Хьюдж;
class ТкстБуф;
class ШТкстБуф;
//class Ткст0;
class ШТкст0;
class Ткст;
class ШТкст;
class Поток;

template <class T> проц разверни(T& a, T& b);// { T tmp = пикуй(a); a = пикуй(b); b = пикуй(tmp); }


struct КомбХэш {
	т_хэш hash;

	template <class T> КомбХэш& делай(const T& x)                  { помести(дайХэшЗнач(x)); return *this; }

public:
	КомбХэш& помести(т_хэш h)                                      { hash = ХЭШ_КОНСТ2 * hash + h; return *this; }

	operator т_хэш() const                                         { return hash; }

	КомбХэш()                                                   { hash = ХЭШ_КОНСТ1; }
	template <class T>
	КомбХэш(const T& h1)                                        { hash = ХЭШ_КОНСТ1; делай(h1); }
	template <class T, class U>
	КомбХэш(const T& h1, const U& h2)                           { hash = ХЭШ_КОНСТ1; делай(h1); делай(h2); }
	template <class T, class U, class V>
	КомбХэш(const T& h1, const U& h2, const V& h3)              { hash = ХЭШ_КОНСТ1; делай(h1); делай(h2); делай(h3); }
	template <class T, class U, class V, class W>
	КомбХэш(const T& h1, const U& h2, const V& h3, const W& h4)	{ hash = ХЭШ_КОНСТ1; делай(h1); делай(h2); делай(h3); делай(h4); }

	template <class T> КомбХэш& operator<<(const T& x)          { делай(x); return *this; }
};

проц начниИницБлок__(кткст0 фн, цел line);
проц завершиИницБлок__(кткст0 фн, цел line);
/////////////////////////////
struct ИницВызов {
	ИницВызов(проц (*фн)(), кткст0 cpp, цел line) { начниИницБлок__(cpp, line); фн(); завершиИницБлок__(cpp, line); }
	ИницВызов(проц (*фн)())                            { фн(); }
};

struct ВыходИзВызова {
	ВыходИзВызова(проц (*фн)()) { atexit(фн); }
};

struct КомбСравни {
	цел result;

	template <class T>
	КомбСравни& operator()(const T& a, const T& b) { if(!result) result = сравниЗнак(a, b); return *this; }

	operator цел() const                               { return result; }

	КомбСравни()                                   { result = 0; }

	template <class T>
	КомбСравни(const T& a, const T& b)             { result = 0; operator()(a, b); }
};

class ШТкст0 {
	enum { МАЛЫЙ = 23 };

	шим *укз;
	цел    length;
	цел    alloc;

#ifdef _ОТЛАДКА
	проц Dsyn();
#else
	проц Dsyn() {}
#endif

	static Атомар voidptr[2];

	бул    IsRc() const  { return alloc > МАЛЫЙ; }
	Атомар& Rc()          { return *((Атомар *)укз - 1); }
	бул    совместный()    { return IsRc() && Rc() > 1; }

	шим  *размести(цел& count);
	проц    бКат(цел c);
	проц    разшарь();

	friend class ШТкстБуф;
	friend class ШТкст;

protected:
	typedef шим         tchar;
	typedef крат         bchar;
	typedef ШТкстБуф Буфер;
	typedef ШТкст       Ткст;

	проц    обнули()                       { static шим e[2]; length = alloc = 0; укз = e; Dsyn(); ПРОВЕРЬ(*укз == 0); }
	проц    уст0(const шим *s, цел length);
	проц    уст0(const ШТкст0& s);
	проц    пикуй0(ШТкст0&& s)          { укз = s.укз; length = s.length; alloc = s.alloc; s.обнули(); Dsyn(); }
	проц    освободи();
	проц    FFree()                      { if(alloc > 0) освободи(); }
	проц    разверни(ШТкст0& b)            { ::разверни(укз, b.укз); ::разверни(length, b.length); ::разверни(alloc, b.alloc); Dsyn(); b.Dsyn(); }
	шим  *вставь(цел pos, цел count, const шим *данные);

public:
	const шим *старт() const           { return укз; }
	const шим *стоп() const             { return старт() + дайДлину(); }
	const шим *begin() const           { return старт(); }
	const шим *end() const             { return стоп(); }
	цел   operator[](цел i) const        { return укз[i]; }

	operator const шим *() const       { return старт(); }
	const шим *operator~() const       { return старт(); }

	проц кат(цел c)                      { if(!IsRc() && length < alloc) { укз[length++] = c; укз[length] = 0; } else бКат(c); Dsyn(); }
	проц кат(const шим *s, цел length);
	проц уст(const шим *s, цел length);

	цел  дайСчёт() const                { return length; }
	цел  дайДлину() const               { return length; }
	цел  дайРазмест() const                { return alloc; }

	т_хэш   дайХэшЗнач() const             { return хэшпам(укз, length * sizeof(шим)); }
	бул     равен(const ШТкст0& s) const { return s.length == length && т_равнпам(укз, s.укз, length); }
	бул     равен(const шим *s) const    { цел l = длинтекс__(s); return l == дайСчёт() && т_равнпам(begin(), s, l); }
	цел      сравни(const ШТкст0& s) const;

	проц удали(цел pos, цел count = 1);
	проц вставь(цел pos, const шим *s, цел count);
	проц очисть()                         { освободи(); обнули(); }

	проц уст(цел pos, цел ch);
	проц обрежь(цел pos);

	ШТкст0()                           { обнули(); }
	~ШТкст0()                          { освободи(); }

//	ШТкст0& operator=(const ШТкст0& s) { освободи(); уст0(s); return *this; }
};

class ТкстБуф : БезКопий {
	сим   *pbegin;
	сим   *pend;
	сим   *limit;
	сим    буфер[256];

	friend class Ткст;

	typedef Ткст0::Rc Rc;

	сим *размести(цел len, цел& alloc);
	проц  переразмести(бцел n, кткст0 cat = NULL, цел l = 0);
	проц  переразместиД(кткст0 s, цел l);
	проц  расширь();
	проц  обнули()                    { pbegin = pend = буфер; limit = pbegin + 255; }
	проц  освободи();
	проц  уст(Ткст& s);

public:
	сим *старт()                   { *pend = '\0'; return pbegin; }
	сим *begin()                   { return старт(); }
	сим *стоп()                     { *pend = '\0'; return pend; }
	сим *end()                     { return стоп(); }

	сим& operator*()               { return *старт(); }
	сим& operator[](цел i)         { return старт()[i]; }
	operator сим*()                { return старт(); }
	operator ббайт*()                { return (ббайт *)старт(); }
	operator ук()                { return старт(); }
	сим *operator~()               { return старт(); }

	проц устДлину(цел l);
	проц устСчёт(цел l)            { устДлину(l); }
	цел  дайДлину() const          { return (цел)(pend - pbegin); }
	цел  дайСчёт() const           { return дайДлину(); }
	проц длинтекс();
	проц очисть()                    { освободи(); обнули(); }
	проц резервируй(цел r)             { цел l = дайДлину(); устДлину(l + r); устДлину(l); }
	проц сожми();

	проц кат(цел c)                        { if(pend >= limit) расширь(); *pend++ = c; }
	проц кат(цел c, цел count);
	проц кат(кткст0 s, цел l);
	проц кат(кткст0 s, кткст0 e) { кат(s, цел(e - s)); }
	проц кат(кткст0 s);
	проц кат(const Ткст& s)              { кат(s, s.дайДлину()); }

	цел  дайРазмест() const           { return (цел)(limit - pbegin); }

	проц operator=(Ткст& s)       { освободи(); уст(s); }

	ТкстБуф()                  { обнули(); }
	ТкстБуф(Ткст& s)         { уст(s); }
	ТкстБуф(цел len)           { обнули(); устДлину(len); }
	~ТкстБуф()                 { if(pbegin != буфер) освободи(); }
};

class ШТкстБуф : БезКопий {
	шим   *pbegin;
	шим   *pend;
	шим   *limit;

	friend class ШТкст;

	шим *размести(цел len, цел& alloc);
	проц   расширь(бцел n, const шим *cat = NULL, цел l = 0);
	проц   расширь();
	проц   обнули();
	проц   освободи();
	проц   уст(ШТкст& s);

public:
	шим *старт()                   { *pend = '\0'; return pbegin; }
	шим *begin()                   { return старт(); }
	шим *стоп()                     { *pend = '\0'; return pend; }
	шим *end()                     { return стоп(); }

	шим& operator*()               { return *старт(); }
	шим& operator[](цел i)         { return старт()[i]; }
	operator шим*()                { return старт(); }
	operator крат*()                { return (крат *)старт(); }
	operator проц*()                 { return старт(); }
	шим *operator~()               { return старт(); }

	проц  устДлину(цел l);
	проц  устСчёт(цел l)            { устДлину(l); }
	цел   дайДлину() const          { return (цел)(pend - pbegin); }
	цел   дайСчёт() const           { return дайДлину(); }
	проц  длинтекс()                   { устДлину(длинтекс__(pbegin)); }
	проц  очисть()                    { освободи(); обнули(); }
	проц  резервируй(цел r)             { цел l = дайДлину(); устДлину(l + r); устДлину(l); }

	проц  кат(цел c)                          { if(pend >= limit) расширь(); *pend++ = c; }
	проц  кат(цел c, цел count);
	проц  кат(const шим *s, цел l);
	проц  кат(const шим *s, const шим *e) { кат(s, цел(e - s)); }
	проц  кат(const шим *s)                 { кат(s, длинтекс__(s)); }
	проц  кат(const ШТкст& s)               { кат(s, s.дайДлину()); }
	проц  кат(кткст0 s)                  { кат(ШТкст(s)); }

	цел   дайРазмест() const           { return (цел)(limit - pbegin); }

	проц operator=(ШТкст& s)       { освободи(); уст(s); }

	ШТкстБуф()                  { обнули(); }
	ШТкстБуф(ШТкст& s)        { уст(s); }
	ШТкстБуф(цел len)           { обнули(); устДлину(len); }
	~ШТкстБуф()                 { освободи(); }
};

class Любое : Движ<Любое> {
	struct ДанныеОснова {
		цел      typeno;

		virtual ~ДанныеОснова() {}
	};

	template <class T>
	struct Данные : ДанныеОснова {
		T        данные;

		template <class... Арги>
		Данные(Арги&&... арги) : данные(std::forward<Арги>(арги)...) { typeno = СтатичНомТипа<T>(); }
	};

	ДанныеОснова *укз;

	проц проверь() const                              { ПРОВЕРЬ(укз != (проц *)1); }
	проц подбери(Любое&& s)                            { укз = s.укз; const_cast<Любое&>(s).укз = NULL; }

public:
	template <class T, class... Арги> T& создай(Арги&&... арги) { очисть(); Данные<T> *x = new Данные<T>(std::forward<Арги>(арги)...); укз = x; return x->данные; }
	template <class T> бул является() const            { return укз && укз->typeno == СтатичНомТипа<T>(); }
	template <class T> T& дай()                   { ПРОВЕРЬ(является<T>()); проверь(); return ((Данные<T>*)укз)->данные; }
	template <class T> const T& дай() const       { ПРОВЕРЬ(является<T>()); проверь(); return ((Данные<T>*)укз)->данные; }

	проц очисть()                                  { if(укз) delete укз; укз = NULL; }

	бул пустой() const                          { return укз == NULL; }

	проц operator=(Любое&& s)                       { if(this != &s) { очисть(); подбери(пикуй(s)); } }
	Любое(Любое&& s)                                  { подбери(пикуй(s)); }

	Любое(const Любое& s) = delete;
	проц operator=(const Любое& s) = delete;

	Любое()                                         { укз = NULL; }
	~Любое()                                        { очисть(); }
};

class Биты : Движ<Биты> {
	цел         alloc;
	бцел      *bp;

	проц расширь(цел q);
	проц переразмести(цел nalloc);
	цел  дайПоследн() const;

public:
	проц   очисть();
	проц   уст(цел i, бул b = true) { ПРОВЕРЬ(i >= 0 && alloc >= 0); цел q = i >> 5; if(q >= alloc) расширь(q);
	                                   i &= 31; bp[q] = (bp[q] & ~(1 << i)) | (b << i); }
	бул   дай(цел i) const        { ПРОВЕРЬ(i >= 0 && alloc >= 0); цел q = i >> 5;
	                                 return q < alloc ? bp[q] & (1 << (i & 31)) : false; }
	бул   operator[](цел i) const { return дай(i); }

	проц   уст(цел i, бцел bits, цел count);
	бцел  дай(цел i, цел count);
	проц   уст64(цел i, бдол bits, цел count);
	бдол дай64(цел i, цел count);

	проц   устН(цел i, бул b, цел count);
	проц   устН(цел i, цел count)         { устН(i, true, count); }

	проц   резервируй(цел nbits);
	проц   сожми();

	Ткст вТкст() const;

	бцел       *создайРяд(цел n_dwords);
	const бцел *дайРяд(цел& n_dwords) const { n_dwords = alloc; return bp; }
	бцел       *дайРяд(цел& n_dwords)       { n_dwords = alloc; return bp; }

	проц         сериализуй(Поток& s);

	Биты()                                { bp = NULL; alloc = 0; }
	~Биты()                               { очисть(); }

	Биты(Биты&& b)                        { alloc = b.alloc; bp = b.bp; b.bp = NULL; }
	проц operator=(Биты&& b)              { if(this != &b) { очисть(); alloc = b.alloc; bp = b.bp; b.bp = NULL; } }

	Биты(const Биты&) = delete;
	проц operator=(const Биты&) = delete;
};

class СоРабота : БезКопий {
	struct МРабота : Движ<МРабота>, Линк<МРабота, 2> {
		Функция<проц ()> фн;
		СоРабота           *work = NULL;
		бул              looper = false;
	};

	enum { SCHEDULED_MAX = 2048 };

public:
	struct Пул {
		МРабота             *free;
		Линк<МРабота, 2>     jobs;
		МРабота              slot[SCHEDULED_MAX];
		цел               waiting_threads;
		Массив<Нить>     threads;
		бул              quit;

		Стопор             lock;
		ПеременнаяУсловия waitforjob;

		проц              освободи(МРабота& m);
		проц              работай(МРабота& m);
		проц              суньРаботу(Функция<проц ()>&& фн, СоРабота *work, бул looper = false);

		проц              иницНити(цел nthreads);
		проц              покиньНити();

		Пул();
		~Пул();

		static thread_local бул    финблок;

		бул работай();
		static проц пускНити(цел tno);
	};

	friend struct Пул;

	static Пул& дайПул();

	static thread_local цел индекс_трудяги;
	static thread_local СоРабота *текущ;

	ПеременнаяУсловия  waitforfinish;
	Линк<МРабота, 2>      jobs; // global stack and СоРабота stack as дво-linked lists
	цел                todo;
	бул               canceled;
	std::exception_ptr exc = nullptr; // workaround for sanitizer bug(?)
	Функция<проц ()>  looper_fn;
	цел                looper_count;

	проц делай0(Функция<проц ()>&& фн, бул looper);

	проц отмени0();
	проц финишируй0();

	Атомар             индекс;

// experimental pipe support
	Стопор stepmutex;
	Массив<БиВектор<Функция<проц ()>>> step;
	Вектор<бул> steprunning;

public:
	static бул пробуйПлан(Функция<проц ()>&& фн);
	static бул пробуйПлан(const Функция<проц ()>& фн)      { return пробуйПлан(клонируй(фн)); }
	static проц планируй(Функция<проц ()>&& фн);
	static проц планируй(const Функция<проц ()>& фн)         { return планируй(клонируй(фн)); }

	проц     делай(Функция<проц ()>&& фн)                       { делай0(пикуй(фн), false); }
	проц     делай(const Функция<проц ()>& фн)                  { делай(клонируй(фн)); }

	СоРабота&  operator&(const Функция<проц ()>& фн)           { делай(фн); return *this; }
	СоРабота&  operator&(Функция<проц ()>&& фн)                { делай(пикуй(фн)); return *this; }

	проц     цикл(Функция<проц ()>&& фн);
	проц     цикл(const Функция<проц ()>& фн)                { цикл(клонируй(фн)); }

	СоРабота&  operator*(const Функция<проц ()>& фн)           { цикл(фн); return *this; }
	СоРабота&  operator*(Функция<проц ()>&& фн)                { цикл(пикуй(фн)); return *this; }

	цел      следщ()                                           { return ++индекс - 1; }

	цел  дайСчётПланируемых() const                            { return todo; }
	проц пайп(цел stepi, Функция<проц ()>&& lambda); // experimental

	static проц финБлок();

	проц отмена();
	static бул отменён();

	проц финиш();

	бул финишировал();

	проц переустанов();

	static бул трудяга_ли()                                    { return дайИндексТрудяги() >= 0; }
	static цел  дайИндексТрудяги();
	static цел  дайРазмерПула();
	static проц устРазмерПула(цел n);

	СоРабота();
	~СоРабота() noexcept(false);
};

struct СоРаботаБИ : СоРабота {
	~СоРаботаБИ() noexcept(true) {}
};

struct ОшПотока {};
struct ОшЗагрузки : ОшПотока {};

struct Форматирование
{
	цел    язык;
	Значение  арг;
	Ткст формат;
	Ткст ид;
};


class Поток {
protected:
	дол  pos;
	ббайт  *буфер;
	ббайт  *укз;
	ббайт  *rdlim;
	ббайт  *wrlim;

	unsigned style:6;
	unsigned errorcode:16;

	цел      версия = 0;

	enum {
		BEGINOFLINE = 0x02,
	};

	virtual   проц  _помести(цел w);
	virtual   цел   _прекрати();
	virtual   цел   _получи();
	virtual   проц  _помести(кук данные, бцел size);
	virtual   бцел _получи(ук данные, бцел size);

private:
	цел       _получи8();
	цел       _получи16();
	цел       _получи32();
	дол     _получи64();

public:
	virtual   проц  перейди(дол pos);
	virtual   дол дайРазм() const;
	virtual   проц  устРазм(дол size);
	virtual   проц  слей();
	virtual   проц  закрой();
	virtual   бул  открыт() const = 0;

	Поток();
	virtual  ~Поток();

	бкрат      дайСтиль() const       { return style; }

	проц      устВерсию(цел ver)    { версия = ver; }
	цел       дайВерсию() const     { return версия; }

	бул      ошибка_ли() const        { return style & STRM_ERROR; }
	бул      ок_ли() const           { return !ошибка_ли(); }
	проц      устОш(цел c = 0)    { style |= STRM_ERROR; errorcode = c; }
#ifdef PLATFORM_WIN32
	проц      устПоследнОш()         { устОш(GetLastError()); }
#endif
#ifdef PLATFORM_POSIX
	проц      устПоследнОш()         { устОш(errno); }
#endif
	цел       дайОш() const       { return errorcode; }
	Ткст    дайТекстОш() const;
	проц      сотриОш()           { style = style & ~STRM_ERROR; errorcode = 0; }

	дол     дайПоз() const         { return бцел(укз - буфер) + pos; }
	дол     GetLeft() const        { return дайРазм() - дайПоз(); }
	проц      SeekEnd(дол rel = 0) { перейди(дайРазм() + rel); }
	проц      SeekCur(дол rel)     { перейди(дайПоз() + rel); }

	бул      кф_ли()                { return укз >= rdlim && _прекрати() < 0; }

	проц      помести(цел c)             { if(укз < wrlim) *укз++ = c; else _помести(c); }
	цел       прекрати()                 { return укз < rdlim ? *укз : _прекрати(); }
	цел       подбери()                 { return прекрати(); }
	цел       дай()                  { return укз < rdlim ? *укз++ : _получи(); }

	const ббайт *подбериУк(цел size = 1){ ПРОВЕРЬ(size > 0); return укз + size <= rdlim ? укз : NULL; }
	const ббайт *дайУк(цел size = 1) { ПРОВЕРЬ(size > 0); if(укз + size <= rdlim) { ббайт *p = укз; укз += size; return p; }; return NULL; }
	ббайт       *поместиУк(цел size = 1) { ПРОВЕРЬ(size > 0); if(укз + size <= wrlim) { ббайт *p = укз; укз += size; return p; }; return NULL; }
	const ббайт *GetSzPtr(цел& size)  { прекрати(); size = цел(rdlim - укз); ббайт *p = укз; укз += size; return p; }

	проц      помести(кук данные, цел size)  { ПРОВЕРЬ(size >= 0); if(size) { if(укз + size <= wrlim) { копирпам8(укз, данные, size); укз += size; } else _помести(данные, size); } }
	цел       дай(ук данные, цел size)        { ПРОВЕРЬ(size >= 0); if(укз + size <= rdlim) { копирпам8(данные, укз, size); укз += size; return size; } return _получи(данные, size); }

	проц      помести(const Ткст& s)   { помести((кук) *s, s.дайДлину()); }
	Ткст      дай(цел size);
	Ткст      дайВсе(цел size);

	цел       пропусти(цел size);

	проц      LoadThrowing()         { style |= STRM_THROW; }
	проц      загрузиОш();

	бул      дайВсе(ук данные, цел size);

	проц      помести64(кук данные, дол size);
	дол       дай64(ук данные, дол size);
	бул      GetAll64(ук данные, дол size);

	т_мера    дай(Хьюдж& h, т_мера size);
	бул       дайВсе(Хьюдж& h, т_мера size);

	цел       дай8()                 { return укз < rdlim ? *укз++ : _получи8(); }
	цел       дай16()                { if(укз + 1 >= rdlim) return _получи16(); цел q = подбери16(укз); укз += 2; return q; }
	цел       дай32()                { if(укз + 3 >= rdlim) return _получи32(); цел q = подбери32(укз); укз += 4; return q; }
	дол       дай64()                { if(укз + 7 >= rdlim) return _получи64(); дол q = подбери64(укз); укз += 8; return q; }

	цел       дайУтф8();

	Ткст      дайСтроку();

	проц      помести16(бкрат q)          { if(укз + 1 < wrlim) { ::помести16(укз, q); укз += 2; } else помести(&q, 2); }
	проц      помести32(бцел q)         { if(укз + 3 < wrlim) { ::помести32(укз, q); укз += 4; } else помести(&q, 4); }
	проц      помести64(дол q)         { if(укз + 7 < wrlim) { ::помести64(укз, q); укз += 8; } else помести(&q, 8); }

#ifdef CPU_LE
	цел       Get16le()              { return дай16(); }
	цел       Get32le()              { return дай32(); }
	дол       Get64le()              { return дай64(); }
	цел       Get16be()              { return эндианРазворот16(дай16()); }
	цел       Get32be()              { return эндианРазворот32(дай32()); }
	дол       Get64be()              { return эндианРазворот64(дай64()); }

	проц      Put16le(бкрат q)        { помести16(q); }
	проц      Put32le(бцел q)       { помести32(q); }
	проц      Put64le(дол q)       { помести64(q); }
	проц      Put16be(бкрат q)        { помести16(эндианРазворот16(q)); }
	проц      Put32be(бцел q)       { помести32(эндианРазворот32(q)); }
	проц      Put64be(дол q)       { помести64(эндианРазворот64(q)); }
#else
	цел       Get16le()              { return эндианРазворот16(дай16()); }
	цел       Get32le()              { return эндианРазворот32(дай32()); }
	дол       Get64le()              { return эндианРазворот64(дай64()); }
	цел       Get16be()              { return дай16(); }
	цел       Get32be()              { return дай32(); }
	дол       Get64be()              { return дай64(); }

	проц      Put16le(бкрат q)        { помести16(эндианРазворот16(q)); }
	проц      Put32le(бцел q)       { помести32(эндианРазворот32(q)); }
	проц      Put64le(дол q)       { помести64(эндианРазворот64(q)); }
	проц      Put16be(бкрат q)        { помести16(q); }
	проц      Put32be(бцел q)       { помести32(q); }
	проц      Put64be(дол q)       { помести64(q); }
#endif

	проц      PutUtf8(цел c);

	проц      помести(кткст0 s);
	проц      помести(цел c, цел count);
	проц      Put0(цел count)        { помести(0, count); }

	проц      PutCrLf()              { помести16(MAKEWORD('\r', '\n')); }
#ifdef PLATFORM_WIN32
	проц      PutEol()               { PutCrLf(); }
#else
	проц      PutEol()               { помести('\n'); }
#endif

	Поток&   operator<<(EOLenum)    { PutEol(); return *this; }

	проц      PutLine(кткст0 s);
	проц      PutLine(const Ткст& s);

	проц      помести(Поток& s, дол size = ЦЕЛ64_МАКС, бцел click = 4096);

//  Поток как сериализатор
	проц      SetLoading()                 { ПРОВЕРЬ(style & STRM_READ); style |= STRM_LOADING; }
	проц      SetStoring()                 { ПРОВЕРЬ(style & STRM_WRITE); style &= ~STRM_LOADING; }
	бул      грузится() const            { return style & STRM_LOADING; }
	бул      сохраняется() const            { return !грузится(); }

	проц      SerializeRaw(ббайт *данные, дол count);
	проц      SerializeRaw(бкрат *данные, дол count);
	проц      SerializeRaw(бцел *данные, дол count);
	проц      SerializeRaw(бдол *данные, дол count);

	Ткст    GetAllRLE(цел size);
	проц      SerializeRLE(ббайт *данные, цел count);

	Поток&   SerializeRaw(ббайт *данные)     { if(грузится()) *данные = дай(); else помести(*данные); return *this; }
	Поток&   SerializeRaw(бкрат *данные)     { if(грузится()) *данные = Get16le(); else Put16le(*данные); return *this; }
	Поток&   SerializeRaw(бцел *данные)    { if(грузится()) *данные = Get32le(); else Put32le(*данные); return *this; }
	Поток&   SerializeRaw(бдол *данные)   { if(грузится()) *данные = Get64le(); else Put64le(*данные); return *this; }

	Поток&   operator%(бул& d)           { ббайт b; if(сохраняется()) b = d; SerializeRaw(&b); d = b; return *this; }
	Поток&   operator%(сим& d)           { return SerializeRaw((ббайт *)&d); }
	Поток&   operator%(байт& d)    { return SerializeRaw((ббайт *)&d); }
	Поток&   operator%(ббайт& d)  { return SerializeRaw((ббайт *)&d); }
	Поток&   operator%(крат& d)          { return SerializeRaw((бкрат *)&d); }
	Поток&   operator%(бкрат& d) { return SerializeRaw((бкрат *)&d); }
	Поток&   operator%(цел& d)            { return SerializeRaw((бцел *)&d); }
	Поток&   operator%(бцел& d)   { return SerializeRaw((бцел *)&d); }
	Поток&   operator%(long& d)           { return SerializeRaw((бцел *)&d); }
	Поток&   operator%(unsigned long& d)  { return SerializeRaw((бцел *)&d); }
	Поток&   operator%(плав& d)          { return SerializeRaw((бцел *)&d); }
	Поток&   operator%(дво& d)         { return SerializeRaw((бдол *)&d); }
	Поток&   operator%(дол& d)          { return SerializeRaw((бдол *)&d); }
	Поток&   operator%(бдол& d)         { return SerializeRaw((бдол *)&d); }

	Поток&   operator%(Ткст& s);
	Поток&   operator/(Ткст& s);

	Поток&   operator%(ШТкст& s);
	Поток&   operator/(ШТкст& s);

	проц      Pack(бцел& i);
	Поток&   operator/(цел& i);
	Поток&   operator/(бцел& i);
	Поток&   operator/(long& i);
	Поток&   operator/(unsigned long& i);

	проц      Magic(бцел magic = 0x7d674d7b);

	проц      Pack(бул& a, бул& b, бул& c, бул& d, бул& e, бул& f, бул& g, бул& h);
	проц      Pack(бул& a, бул& b, бул& c, бул& d, бул& e, бул& f, бул& g);
	проц      Pack(бул& a, бул& b, бул& c, бул& d, бул& e, бул& f);
	проц      Pack(бул& a, бул& b, бул& c, бул& d, бул& e);
	проц      Pack(бул& a, бул& b, бул& c, бул& d);
	проц      Pack(бул& a, бул& b, бул& c);
	проц      Pack(бул& a, бул& b);

#ifdef DEPRECATED
	цел       дайШ()                 { return дай16(); }
	цел       GetL()                 { return дай32(); }
	цел       GetIW()                { return Get16le(); }
	цел       GetIL()                { return Get32le(); }
	цел       GetMW()                { return Get16be(); }
	цел       GetML()                { return Get32be(); }
	проц      PutW(цел c)            { помести16(c); }
	проц      PutL(цел c)            { помести32(c); }
	проц      PutIW(цел c)           { Put16le(c); }
	проц      PutIL(цел c)           { Put32le(c); }
	проц      PutMW(цел c)           { Put16be(c); }
	проц      PutML(цел c)           { Put32be(c); }
	проц      PutW(const сим16 *s, цел count)       { помести(s, count * 2); }
	бцел     дайШ(сим16 *s, цел count)             { return дай(s, count * 2) / 2; }
	бул      GetAllW(сим16 *s, цел count)          { return дайВсе(s, count * 2); }
#endif
private: // No copy
	Поток(const Поток& s);
	проц operator=(const Поток& s);
};

class ТкстПоток : public Поток {
protected:
	virtual  проц  _помести(цел w);
	virtual  цел   _прекрати();
	virtual  цел   _получи();
	virtual  проц  _помести(кук данные, бцел size);
	virtual  бцел _получи(ук данные, бцел size);

public:
	virtual  проц  перейди(дол pos);
	virtual  дол дайРазм() const;
	virtual  проц  устРазм(дол size);
	virtual  бул  открыт() const;

protected:
	бул           writemode;
	Ткст         данные;
	ТкстБуф   wdata;
	бцел          size;
	цел            limit = INT_MAX;

	проц           иницРежимЧтен();
	проц           SetWriteBuffer();
	проц           SetReadMode();
	проц           SetWriteMode();

public:
	проц        открой(const Ткст& данные);
	проц        создай();
	проц        резервируй(цел n);

	Ткст      дайРез();
	operator    Ткст()                              { return дайРез(); }

	проц        Limit(цел sz)                         { limit = sz; }

	struct LimitExc : public ОшПотока {};

	ТкстПоток()                           { создай(); }
	ТкстПоток(const Ткст& данные)         { открой(данные); }
};

class ПамПоток : public Поток {
protected:
	virtual   проц  _помести(кук данные, бцел size);
	virtual   бцел _получи(ук данные, бцел size);

public:
	virtual   проц  перейди(дол pos);
	virtual   дол дайРазм() const;
	virtual   бул  открыт() const;

public:
	проц создай(ук данные, дол size);

	ПамПоток();
	ПамПоток(ук данные, дол size);
};

class ПамЧтенПоток : public ПамПоток {
public:
	проц создай(кук данные, дол size);

	ПамЧтенПоток(кук данные, дол size);
	ПамЧтенПоток();
};

class БлокПоток : public Поток {
protected:
	virtual  проц  _помести(цел w);
	virtual  цел   _прекрати();
	virtual  цел   _получи();
	virtual  проц  _помести(кук данные, бцел size);
	virtual  бцел _получи(ук данные, бцел size);

public:
	virtual  проц  перейди(дол pos);
	virtual  дол дайРазм() const;
	virtual  проц  устРазм(дол size);
	virtual  проц  слей();

private:
	цел           pagesize;
	дол         pagemask;
	дол         pagepos;
	бул          pagedirty;

	дол         streamsize;

	проц          устПоз(дол p);
	проц          синхРазм();
	бул          синхСтраницу();
	бул          синхПоз();
	проц          ReadData(ук данные, дол at, цел size);

protected:
	virtual  бцел читай(дол at, ук укз, бцел size);
	virtual  проц  пиши(дол at, кук данные, бцел size);
	virtual  проц  устРазмПотока(дол size);

public:
	enum {
		READ, CREATE, APPEND, READWRITE,

		NOWRITESHARE = 0x10,
		SHAREMASK = 0x70,
#ifdef DEPRECATED
		DELETESHARE = 0x20,
		NOREADSHARE = 0x40,
#endif
	};

	бцел     дайРазмБуф() const           { return pagesize; }
	проц      устРазмБуф(бцел newsize);
	дол     дайРазмПотока() const           { return streamsize; }

	БлокПоток();
	virtual ~БлокПоток();

protected:
	проц     иницОткр(бцел mode, дол file_size);
};

class ФайлПоток : public БлокПоток {
protected:
	virtual  проц  устРазмПотока(дол size);
	virtual  бцел читай(дол at, ук укз, бцел size);
	virtual  проц  пиши(дол at, кук данные, бцел size);

public:
	virtual  проц  закрой();
	virtual  бул  открыт() const;

protected:
#ifdef PLATFORM_WIN32
	HANDLE    handle;
#endif
#ifdef PLATFORM_POSIX
	цел       handle;
#endif

	проц      устПоз(дол pos);
	проц      иниц(дол size);

public:
	operator  бул() const                 { return открыт(); }
	ФВремя  дайВремя() const;

#ifdef PLATFORM_WIN32
	проц      устВремя(const ФВремя& tm);
	бул      открой(кткст0 имяф, бцел mode);

	ФайлПоток(кткст0 имяф, бцел mode);
#endif

#ifdef PLATFORM_POSIX
	бул      открой(кткст0 имяф, бцел mode, mode_t acm = 0644);
	ФайлПоток(кткст0 имяф, бцел mode, mode_t acm = 0644);
	ФайлПоток(цел std_handle);
#endif

	ФайлПоток();
	~ФайлПоток();

#ifdef PLATFORM_WIN32
	HANDLE    дайУк() const            { return handle; }
#endif
#ifdef PLATFORM_POSIX
	цел       дайУк() const            { return handle; }
#endif
};

class ФайлВывод : public ФайлПоток {
public:
#ifdef PLATFORM_POSIX
	бул открой(кткст0 фн, mode_t acm = 0644);
#endif
#ifdef PLATFORM_WIN32
	бул открой(кткст0 фн);
#endif

	ФайлВывод(кткст0 фн)                { открой(фн); }
	ФайлВывод()                              {}
};

class ФайлДоп : public ФайлПоток {
public:
	бул открой(кткст0 фн)         { return ФайлПоток::открой(фн, ФайлПоток::APPEND|ФайлПоток::NOWRITESHARE); }

	ФайлДоп(кткст0 фн)        { открой(фн); }
	ФайлДоп()                      {}
};

class ФайлВвод : public ФайлПоток {
public:
	бул открой(кткст0 фн)         { return ФайлПоток::открой(фн, ФайлПоток::READ); }

	ФайлВвод(кткст0 фн)            { открой(фн); }
	ФайлВвод()                          {}
};

class РазмПоток : public Поток {
protected:
	virtual проц  _помести(цел w);
	virtual проц  _помести(кук данные, бцел size);

public:
	virtual дол дайРазм() const;
	virtual бул  открыт() const;

protected:
	ббайт    h[256];

public:
	operator дол() const            { return дайРазм(); }

	проц     открой()                   { укз = буфер; сотриОш(); }

	РазмПоток();
};

class СравнПоток : public Поток {
protected:
	virtual  проц  _помести(цел w);
	virtual  проц  _помести(кук данные, бцел size);

public:
	virtual  проц  перейди(дол pos);
	virtual  дол дайРазм() const;
	virtual  проц  устРазм(дол size);
	virtual  проц  открой();
	virtual  бул  открыт() const;
	virtual  проц  слей();

private:
	Поток  *stream;
	бул     equal;
	дол    size;
	ббайт     h[128];

	проц     сравни(дол pos, кук данные, цел size);

public:
	проц     открой(Поток& aStream);

	бул     равен()                         { слей(); return equal; }
	operator бул()                            { слей(); return equal; }

	СравнПоток();
	СравнПоток(Поток& aStream);
};

class ПотокВывода : public Поток {
	ббайт *h;

protected:
	virtual  проц  _помести(цел w);
	virtual  проц  _помести(кук данные, бцел size);
	virtual  бул  открыт() const;

	virtual  проц  выведи(кук данные, бцел size) = 0;

public:
	virtual  проц  открой();

	проц     слей();

	ПотокВывода();
	~ПотокВывода();
};

class TeeStream : public ПотокВывода {
protected:
	virtual  проц  выведи(кук данные, бцел size);

private:
	Поток& a;
	Поток& b;

public:
	TeeStream(Поток& a, Поток& b) : a(a), b(b) {}
	~TeeStream()                                 { открой(); }
};

class КартФайл
{
public:
	КартФайл(кткст0 file = NULL, бул delete_share = false);
	~КартФайл() { открой(); }

	бул        открой(кткст0 file, бул delete_share = false);
	бул        создай(кткст0 file, дол filesize, бул delete_share = false);
	бул        расширь(дол filesize);
	бул        вКарту(дол offset, т_мера len);
	бул        изКарты();
	бул        открой();

	бул        открыт() const            { return hfile != INVALID_HANDLE_VALUE; }

	дол       дайРазмФайла() const       { return filesize; }
	Время        дайВремя() const;
	Ткст      дайДанные(дол offset, цел len);

	дол       дайСмещ() const         { return offset; }
	т_мера      дайСчёт() const          { return size; }

	дол       GetRawOffset() const      { return rawoffset; }
	т_мера      GetRawCount() const       { return rawsize; }

	const ббайт *operator ~ () const       { ПРОВЕРЬ(открыт()); return base; }
	const ббайт *старт() const             { ПРОВЕРЬ(открыт()); return base; }
	const ббайт *стоп() const               { ПРОВЕРЬ(открыт()); return base + size; }
	const ббайт *дайОбх(цел i) const      { ПРОВЕРЬ(открыт() && i >= 0 && (т_мера)i <= size); return base + i; }
	const ббайт& operator [] (цел i) const { ПРОВЕРЬ(открыт() && i >= 0 && (т_мера)i < size); return base[i]; }

	ббайт       *operator ~ ()             { ПРОВЕРЬ(открыт()); return base; }
	ббайт       *старт()                   { ПРОВЕРЬ(открыт()); return base; }
	ббайт       *стоп()                     { ПРОВЕРЬ(открыт()); return base + size; }
	ббайт       *дайОбх(цел i)            { ПРОВЕРЬ(открыт() && i >= 0 && (т_мера)i <= size); return base + i; }
	ббайт&       operator [] (цел i)       { ПРОВЕРЬ(открыт() && i >= 0 && (т_мера)i < size); return base[i]; }

private:
#ifdef PLATFORM_WIN32
	HANDLE      hfile;
	HANDLE      hmap;
#endif
#ifdef PLATFORM_POSIX
	enum { INVALID_HANDLE_VALUE = -1 };
	цел	        hfile;
	struct stat hfstat;
#endif
	ббайт       *base;
	ббайт       *rawbase;
	дол       filesize;
	дол       offset;
	дол       rawoffset;
	т_мера      size;
	т_мера      rawsize;
	бул        write;
};

/* мд5 context. */
typedef struct {
	бцел state[4];                                   /* state (ABCD) */
	бцел count[2];        /* number of bits, modulo 2^64 (lsb first) */
	ббайт буфер[64];                         /* input буфер */
} РНЦП_МД5_КОНТЕКСТ;

class ПотокМд5 : public ПотокВывода {
	РНЦП_МД5_КОНТЕКСТ context;

	virtual  проц  выведи(кук данные, бцел size);

public:
	проц   финиш(ббайт *hash16);
	Ткст завершиТкст();
	Ткст завершиПТкст();
	проц   переустанов();

	ПотокМд5();
	~ПотокМд5();
};

typedef struct {
    uint32_t state[5];
    uint32_t count[2];
    ббайт буфер[64];
} РНЦП_ША1_КОНТЕКСТ;

class ПотокШа1 : public ПотокВывода {
	РНЦП_ША1_КОНТЕКСТ ctx[1];

	virtual  проц  выведи(кук данные, бцел size);

	проц  зачисть()                      { memset(ctx, 0, sizeof(ctx)); }

public:
	проц   финиш(ббайт *hash20);
	Ткст завершиТкст();
	Ткст завершиПТкст();

	проц   переустанов();
	проц   нов()                         { переустанов(); }

	ПотокШа1();
	~ПотокШа1();
};

class ПотокШа256 : public ПотокВывода {
	ббайт  буфер[128];

	virtual  проц  выведи(кук данные, бцел size);

	проц  зачисть();

public:
	проц   финиш(ббайт *hash32);
	Ткст завершиТкст();
	Ткст завершиПТкст();

	проц   переустанов();
	проц   нов()                         { переустанов(); }

	ПотокШа256();
	~ПотокШа256();
};

class ххХэшПоток : public ПотокВывода {
	ббайт context[8 * 8];

	virtual  проц  выведи(кук данные, бцел size);

public:
	цел финиш();

	проц переустанов(бцел seed = 0);

	ххХэшПоток(бцел seed = 0);
};

class ххХэш64Поток : public ПотокВывода {
	ббайт context[12 * 8];

	virtual  проц  выведи(кук данные, бцел size);

public:
	дол финиш();

	проц переустанов(бцел seed = 0);

	ххХэш64Поток(бцел seed = 0);
};

struct Ууид : ПрисвойНомТипаЗнач<Ууид, 50, Движ<Ууид> > {
	бдол v[2];

	проц сериализуй(Поток& s);
	проц вРяр(РярВВ& xio);
	проц вДжейсон(ДжейсонВВ& jio);
	бул экзПусто_ли() const   { return (v[0] | v[1]) == 0; }
	проц устПусто()                { v[0] = v[1] = 0; }

	operator Значение() const        { return богатыйВЗнач(*this); }
	Ууид(const Значение& q)          { *this = q.дай<Ууид>(); }
	Ууид(const Обнул&)           { устПусто(); }
	Ууид()                        {}

	т_хэш   дайХэшЗнач() const { return КомбХэш(v[0], v[1]); }
	Ткст   вТкст() const;
	Ткст   ToStringWithDashes() const;
	
	проц     нов();

	static Ууид создай()          { Ууид uuid; uuid.нов(); return uuid; }
};


class PteBase {
protected:
	struct Prec {
		PteBase *укз;
		Атомар   n;
	};

	volatile Prec  *prec;

	Prec           *добавьУк();
	static проц     отпустиУк(Prec *prec);
	static Prec    *добавьУк(const Ууид& uuid);

	PteBase();
	~PteBase();

	friend class УкОснова;
};

class УкОснова {
protected:
	PteBase::Prec *prec;
	проц уст(PteBase *p);
	проц отпусти();
	проц присвой(PteBase *p);

public:
	~УкОснова();
};

class Искл : public Ткст {
public:
	Искл() {}
	Искл(const Ткст& desc) : Ткст(desc) {}
};

struct DebugLogBlock
{
	DebugLogBlock(кткст0 имя) : имя(имя) { VppLog() << имя << EOL << LOG_BEGIN; }
	~DebugLogBlock()                             { VppLog() << LOG_END << "//" << имя << EOL; }
	кткст0 имя;
};

class СиПарсер {
protected:
	кткст0 term;
	кткст0 wspc;
	кткст0 lineptr;
	цел         line;
	Ткст      фн;
	бул        skipspaces;
	бул        skipcomments;
	бул        nestcomments;
	бул        uescape;

	бул        пробелы0();
	кткст0 ид0_ли(кткст0 s) const;
	бул        ид0(кткст0 ид);
	проц        сделайПробелы()                    { if(skipspaces) пробелы(); }
	бцел       читайГекс();
	бул        читайГекс(бцел& hex, цел n);

public:
	struct Ошибка : public Искл                 { Ошибка(кткст0 s) : Искл(s) {} };

	проц   выведиОш(кткст0 s);
	проц   выведиОш()                       { выведиОш(""); }

	бул   пробелы()                           { wspc = term; return ((ббайт)*term <= ' ' || *term == '/') && пробелы0(); }
	сим   подбериСим() const                   { return *term; }
	сим   дайСим();

	бул   сим_ли(сим c) const               { return *term == c; }
	бул   сим2_ли(сим c1, сим c2) const    { return term[0] == c1 && term[1] == c2; }
	бул   сим3_ли(сим c1, сим c2, сим c3) const { return term[0] == c1 && term[1] == c2 && term[2] == c3; }
	бул   Сим(сим c);
	бул   сим2(сим c1, сим c2);
	бул   сим3(сим c1, сим c2, сим c3);
	проц   передайСим(сим c);
	проц   передайСим2(сим c1, сим c2);
	проц   передайСим3(сим c1, сим c2, сим c3);
	бул   ид(кткст0 s)                  { return term[0] == s[0] && (s[1] == 0 || term[1] == s[1]) && ид0(s); }
	проц   передайИд(кткст0 s);
	бул   ид_ли() const                       { return iscib(*term); }
	бул   ид_ли(кткст0 s) const          { return term[0] == s[0] && (s[1] == 0 || term[1] == s[1]) && ид0_ли(s); }
	Ткст читайИд();
	Ткст ReadIdt();
	бул   цел_ли() const;
	цел    знак();
	цел    читайЦел();
	цел    читайЦел(цел мин, цел макс);
	дол  читайЦел64();
	дол  читайЦел64(дол мин, дол макс);
	бул   число_ли() const                   { return цифра_ли(*term); }
	бул   число_ли(цел base) const;
	бцел читайЧисло(цел base = 10);
	бдол читайЧисло64(цел base = 10);
	бул   дво_ли() const                   { return цел_ли(); }
	бул   дво2_ли() const;
	дво читайДво();
	бул   ткст_ли() const                   { return сим_ли('\"'); };
	Ткст читай1Ткст(бул chkend = true);
	Ткст читайТкст(бул chkend = true);
	Ткст читай1Ткст(цел delim, бул chkend = true);
	Ткст читайТкст(цел delim, бул chkend = true);

	проц   пропусти();
	проц   пропустиТерм()                         { пропусти(); }

	struct Поз {
		кткст0 укз;
		кткст0 wspc;
		кткст0 lineptr;
		цел         line;
		Ткст      фн;

		цел дайКолонку(цел tabsize = 4) const;

		Поз(кткст0 укз = NULL, цел line = 1, Ткст фн = Null) : укз(укз), line(line), фн(фн) {}
	};

	кткст0 дайУк() const                { return (кткст0 )term; }
	кткст0 дайПбелУк() const           { return (кткст0 )wspc; }

	Поз         дайПоз() const;
	проц        устПоз(const Поз& pos);

	бул   кф_ли() const                      { return *term == '\0'; }
	operator бул() const                     { return *term; }

	цел    дайСтроку() const                    { return line; }
	цел    дайКолонку(цел tabsize = 4) const;
	Ткст дайИмяф() const                { return фн; }

	static Ткст LineInfoComment(const Ткст& имяф, цел line = 1, цел column = 1);
	Ткст GetLineInfoComment(цел tabsize = 4) const;
	enum { LINEINFO_ESC = '\2' };

	проц   уст(кткст0 укз, кткст0 фн, цел line = 1);
	проц   уст(кткст0 укз);

	СиПарсер& пропустиПробелы(бул b = true)        { skipspaces = b; return *this; }
	СиПарсер& безПропускаПробелов()                   { skipspaces = false; return *this; }
	СиПарсер& искейпЮникод(бул b = true)     { uescape = b; return *this; }
	СиПарсер& пропустиКомменты(бул b = true);
	СиПарсер& безПропускаКомментов()                 { return пропустиКомменты(false); }
	СиПарсер& гнездиКомменты(бул b = true);
	СиПарсер& безГнездКомментов()                 { return гнездиКомменты(false); }

	СиПарсер(кткст0 укз);
	СиПарсер(кткст0 укз, кткст0 фн, цел line = 1);
	СиПарсер();
};

class Джейсон {
	Ткст text;

public:
	Джейсон& CatRaw(кткст0 ключ, const Ткст& знач);

	Ткст вТкст() const                                     { return "{" + text + "}"; }
	Ткст operator~() const                                    { return вТкст(); }
	operator Ткст() const                                     { return вТкст(); }

	operator бул() const                                       { return text.дайСчёт(); }

	Джейсон& operator()(кткст0 ключ, const Значение& значение)       { return CatRaw(ключ, какДжейсон(значение)); }
	Джейсон& operator()(кткст0 ключ, цел i)                    { return CatRaw(ключ, какДжейсон(i)); }
	Джейсон& operator()(кткст0 ключ, дво n)                 { return CatRaw(ключ, какДжейсон(n)); }
	Джейсон& operator()(кткст0 ключ, бул b)                   { return CatRaw(ключ, какДжейсон(b)); }
	Джейсон& operator()(кткст0 ключ, Дата d)                   { return CatRaw(ключ, какДжейсон(d)); }
	Джейсон& operator()(кткст0 ключ, Время t)                   { return CatRaw(ключ, какДжейсон(t)); }
	Джейсон& operator()(кткст0 ключ, const Ткст& s)          { return CatRaw(ключ, какДжейсон(s)); }
	Джейсон& operator()(кткст0 ключ, const ШТкст& s)         { return CatRaw(ключ, какДжейсон(s)); }
	Джейсон& operator()(кткст0 ключ, кткст0 s)            { return CatRaw(ключ, какДжейсон(s)); }
	Джейсон& operator()(кткст0 ключ, const Джейсон& object)       { return CatRaw(ключ, ~object); }
	Джейсон& operator()(кткст0 ключ, const МассивДжейсон& array);

	Джейсон() {}
	Джейсон(кткст0 ключ, const Значение& значение)                   { CatRaw(ключ, какДжейсон(значение)); }
	Джейсон(кткст0 ключ, цел i)                                { CatRaw(ключ, какДжейсон(i)); }
	Джейсон(кткст0 ключ, дво n)                             { CatRaw(ключ, какДжейсон(n)); }
	Джейсон(кткст0 ключ, бул b)                               { CatRaw(ключ, какДжейсон(b)); }
	Джейсон(кткст0 ключ, Дата d)                               { CatRaw(ключ, какДжейсон(d)); }
	Джейсон(кткст0 ключ, Время t)                               { CatRaw(ключ, какДжейсон(t)); }
	Джейсон(кткст0 ключ, const Ткст& s)                      { CatRaw(ключ, какДжейсон(s)); }
	Джейсон(кткст0 ключ, const ШТкст& s)                     { CatRaw(ключ, какДжейсон(s)); }
	Джейсон(кткст0 ключ, кткст0 s)                        { CatRaw(ключ, какДжейсон(s)); }
	Джейсон(кткст0 ключ, const Джейсон& object)                   { CatRaw(ключ, ~object); }
	Джейсон(кткст0 ключ, const МассивДжейсон& array)               { operator()(ключ, array); }
};

class МассивДжейсон {
	Ткст text;

public:
	МассивДжейсон& CatRaw(const Ткст& знач);

	Ткст вТкст() const                                     { return "[" + text + "]"; }
	Ткст operator~() const                                    { return вТкст(); }
	operator Ткст() const                                     { return вТкст(); }

	operator бул() const                                       { return text.дайСчёт(); }

	МассивДжейсон& operator<<(const Значение& значение)                   { return CatRaw(какДжейсон(значение)); }
	МассивДжейсон& operator<<(цел i)                                { return CatRaw(какДжейсон(i)); }
	МассивДжейсон& operator<<(дво n)                             { return CatRaw(какДжейсон(n)); }
	МассивДжейсон& operator<<(бул b)                               { return CatRaw(какДжейсон(b)); }
	МассивДжейсон& operator<<(Дата d)                               { return CatRaw(какДжейсон(d)); }
	МассивДжейсон& operator<<(Время t)                               { return CatRaw(какДжейсон(t)); }
	МассивДжейсон& operator<<(const Ткст& s)                      { return CatRaw(какДжейсон(s)); }
	МассивДжейсон& operator<<(const ШТкст& s)                     { return CatRaw(какДжейсон(s)); }
	МассивДжейсон& operator<<(кткст0 s)                        { return CatRaw(какДжейсон(s)); }
	МассивДжейсон& operator<<(const Джейсон& object)                   { return CatRaw(~object); }
	МассивДжейсон& operator<<(const МассивДжейсон& array)               { return CatRaw(~array); }

	МассивДжейсон() {}
};


class ДжейсонВВ {
	const Значение   *ист;
	Один<МапЗнач>  map;
	Значение          tgt;

public:
	бул грузится() const                       { return ист; }
	бул сохраняется() const                       { return !ист; }

	const Значение& дай() const                     { ПРОВЕРЬ(грузится()); return *ист; }
	проц         уст(const Значение& v)             { ПРОВЕРЬ(сохраняется() && !map); tgt = v; }

	Значение        дай(кткст0 ключ)            { ПРОВЕРЬ(грузится()); return (*ист)[ключ]; }
	проц         уст(кткст0 ключ, const Значение& v);

	проц         помести(Значение& v)                   { ПРОВЕРЬ(сохраняется()); if(map) v = *map; else v = tgt; }
	Значение        дайРез() const               { ПРОВЕРЬ(сохраняется()); return map ? Значение(*map) : tgt; }

	template <class T>
	ДжейсонВВ& operator()(кткст0 ключ, T& значение);

	template <class T>
	ДжейсонВВ& operator()(кткст0 ключ, T& значение, const T& defvalue);

	template <class T>
	ДжейсонВВ& Список(кткст0 ключ, кткст0 , T& var) { return operator()(ключ, var); }

	template <class T, class X>
	ДжейсонВВ& Var(кткст0 ключ, T& значение, X item_jsonize);

	template <class T, class X>
	ДжейсонВВ& Массив(кткст0 ключ, T& значение, X item_jsonize, кткст0  = NULL);

	ДжейсонВВ(const Значение& ист) : ист(&ист)         {}
	ДжейсонВВ()                                     { ист = NULL; }
};

struct ОшДжейсонизации : Искл {
	ОшДжейсонизации(const Ткст& s) : Искл(s) {}
};

class Значение : Движ_<Значение> {
public:
	class Проц {
	protected:
		Атомар  refcount;

	public:
		проц               Retain()                    { атомнИнк(refcount); }
		проц               отпусти()                   { if(атомнДек(refcount) == 0) delete this; }
		цел                GetRefCount() const         { return refcount; }

		virtual бул       пусто_ли() const              { return true; }
		virtual проц       сериализуй(Поток& s)        {}
		virtual проц       вРяр(РярВВ& xio)          {}
		virtual проц       вДжейсон(ДжейсонВВ& jio)        {}
		virtual т_хэш     дайХэшЗнач() const        { return 0; }
		virtual бул       равен(const Проц *p)      { return false; }
		virtual бул       полиРавны(const Значение& v) { return false; }
		virtual Ткст     какТкст() const            { return ""; }
		virtual цел        сравни(const Проц *p)      { return 0; }
		virtual цел        полиСравни(const Значение& p) { return 0; }

		Проц()                                         { refcount = 1; }
		virtual ~Проц()                                {}

		friend class Значение;
	};

	struct Sval {
		бул       (*пусто_ли)(кук p);
		проц       (*сериализуй)(ук p, Поток& s);
		проц       (*вРяр)(ук p, РярВВ& xio);
		проц       (*вДжейсон)(ук p, ДжейсонВВ& jio);
		т_хэш     (*дайХэшЗнач)(кук p);
		бул       (*равен)(кук p1, кук p2);
		бул       (*полиРавны)(кук p, const Значение& v);
		Ткст     (*какТкст)(кук p);
		цел        (*сравни)(кук p1, кук p2);
		цел        (*полиСравни)(кук p1, const Значение& p2);
	};

protected:
	enum { STRING = 0, REF = 255, VOIDV = 3 };

	static ВекторМап<бцел, Проц* (*)()>& Typemap();
	static Sval *svo[256];
	static Индекс<Ткст>& индексИмени();
	static Индекс<бцел>&  индексТипа();

	static проц   добавьИмя(бцел тип, кткст0 имя);
	static цел    дайТип(кткст0 имя);
	static Ткст дайИмя(бцел тип);
	static проц   регистрируйСтд();

	friend проц ValueRegisterHelper();

	Ткст   данные;
	Проц    *&укз()                  { ПРОВЕРЬ(реф_ли()); return *(Проц **)&данные; }
	Проц     *укз() const            { ПРОВЕРЬ(реф_ли()); return *(Проц **)&данные; }

	проц     устРефТип(бцел тип)  { ПРОВЕРЬ(реф_ли()); ((цел *)&данные)[2] = тип; }
	бцел    дайРефТип() const      { ПРОВЕРЬ(реф_ли()); return ((цел *)&данные)[2]; }

	бул     ткст_ли() const          { return !данные.спец_ли(); }
	бул     является(ббайт v) const          { return данные.спец_ли(v); }
	бул     реф_ли() const             { return является(REF); }
	проц     иницРеф(Проц *p, бцел t) { данные.устСпец(REF); укз() = p; устРефТип(t); }
	проц     отпустиРеф();
	проц     RefRetain();
	проц     освободиРеф()                 { if(реф_ли()) отпустиРеф(); }
	проц     освободи()                    { освободиРеф(); данные.очисть(); }
	проц     устЛардж(const Значение& v);

	template <class T>
	проц     иницСмолл(const T& init);
	template <class T>
	T&       дайСыройСмолл() const;
	template <class T>
	T&       дайСмолл() const;

	цел      дайДрЦел() const;
	дол    дайДрЦел64() const;
	дво   дайДрДво() const;
	бул     дайДрБул() const;
	Дата     дайДрДату() const;
	Время     дайДрВремя() const;
	Ткст   дайДрТкст() const;
	т_хэш   дайДрЗначХэш() const;

	бул     полиРавны(const Значение& v) const;

	enum VSMALL { МАЛЫЙ };

	template <class T>
	Значение(const T& значение, VSMALL);

	template <class T> friend Значение своВЗнач(const T& x);

	Ткст  дайИмя() const;

	цел     полиСравни(const Значение& v) const;
	цел     сравни2(const Значение& v) const;

	Вектор<Значение>&  разшарьArray();

	const Вектор<Значение>& GetVA() const;

#if defined(_ОТЛАДКА) && defined(COMPILER_GCC)
	бцел  magic[4];
	проц    Magic()               { magic[0] = 0xc436d851; magic[1] = 0x72f67c76; magic[2] = 0x3e5e10fd; magic[3] = 0xc90d370b; }
	проц    ClearMagic()          { magic[0] = magic[1] = magic[2] = magic[3] = 0; }
#else
	проц    Magic()               {}
	проц    ClearMagic()          {}
#endif

#ifndef DEPRECATED
	static  проц регистрируй(бцел w, Проц* (*c)(), кткст0 имя = NULL);
#endif

	Значение(Проц *p) = delete;

public:
	template <class T>
	static  проц регистрируй(кткст0 имя = NULL);
	template <class T>
	static  проц SvoRegister(кткст0 имя = NULL);

	бцел    дайТип() const;
	бул     ошибка_ли() const         { return дайТип() == ERROR_V; }
	бул     проц_ли() const          { return является(VOIDV) || ошибка_ли(); }
	бул     пусто_ли() const;

	template <class T>
	бул     является() const;
	template <class T>
	const T& To() const;
	template <class T>
	const T& дай() const;

	operator Ткст() const          { return ткст_ли() ? данные : дайДрТкст(); }
	operator ШТкст() const;
	operator Дата() const            { return является(DATE_V) ? дайСыройСмолл<Дата>() : дайДрДату(); }
	operator Время() const            { return является(TIME_V) ? дайСыройСмолл<Время>() : дайДрВремя(); }
	operator дво() const          { return является(DOUBLE_V) ? дайСыройСмолл<дво>() : дайДрДво(); }
	operator цел() const             { return является(INT_V) ? дайСыройСмолл<цел>() : дайДрЦел(); }
	operator дол() const           { return является(INT64_V) ? дайСыройСмолл<дол>() : дайДрЦел64(); }
	operator бул() const            { return является(BOOL_V) ? дайСыройСмолл<бул>() : дайДрБул(); }
	std::string  вСтд() const       { return operator Ткст().вСтд(); }
	std::wstring вСтдШ() const      { return operator ШТкст().вСтд(); }

	Значение(const Ткст& s) : данные(s) { Magic(); }
	Значение(const ШТкст& s);
	Значение(кткст0 s) : данные(s)   { Magic(); }
	Значение(цел i)                     : данные(i, INT_V, Ткст::СПЕЦ) { Magic(); }
	Значение(дол i)                   : данные(i, INT64_V, Ткст::СПЕЦ) { Magic(); }
	Значение(дво d)                  : данные(d, DOUBLE_V, Ткст::СПЕЦ) { Magic(); }
	Значение(бул b)                    : данные(b, BOOL_V, Ткст::СПЕЦ) { Magic(); }
	Значение(Дата d)                    : данные(d, DATE_V, Ткст::СПЕЦ) { Magic(); }
	Значение(Время t)                    : данные(t, TIME_V, Ткст::СПЕЦ) { Magic(); }
	Значение(const Обнул&)             : данные((цел)Null, INT_V, Ткст::СПЕЦ) { Magic(); }
	Значение(const std::string& s) : Значение(Ткст(s)) {}
	Значение(const std::wstring& s) : Значение(ШТкст(s)) {}

	бул operator==(const Значение& v) const;
	бул operator!=(const Значение& v) const { return !operator==(v); }
	бул одинаково(const Значение& v) const;

	цел  сравни(const Значение& v) const;
	бул operator<=(const Значение& x) const { return сравни(x) <= 0; }
	бул operator>=(const Значение& x) const { return сравни(x) >= 0; }
	бул operator<(const Значение& x) const  { return сравни(x) < 0; }
	бул operator>(const Значение& x) const  { return сравни(x) > 0; }

	Ткст вТкст() const;
	Ткст operator ~() const             { return вТкст(); }
	Ткст дайИмяТипа() const            { return дайИмя(); }

	проц  сериализуй(Поток& s);
	проц  вРяр(РярВВ& xio);
	проц  вДжейсон(ДжейсонВВ& jio);

	т_хэш дайХэшЗнач() const;

	Значение& operator=(const Значение& v);
	Значение(const Значение& v);

	цел   дайСчёт() const;
	const Значение& operator[](цел i) const;
	const Значение& operator[](const Ткст& ключ) const;
	const Значение& operator[](кткст0 ключ) const;
	const Значение& operator[](const Ид& ключ) const;

	Значение& по(цел i);
	Значение& operator()(цел i)              { return по(i); }
	проц   добавь(const Значение& ист);
	template <typename T>
	Значение& operator<<(const T& ист)       { добавь(ист); return *this; }

	Значение& дайДобавь(const Значение& ключ);
	Значение& operator()(const Ткст& ключ);
	Значение& operator()(кткст0 ключ);
	Значение& operator()(const Ид& ключ);

	Значение()                               : данные((цел)Null, VOIDV, Ткст::СПЕЦ) { Magic(); }
	~Значение()                              { ClearMagic(); if(реф_ли()) отпустиРеф(); }

	Значение(Проц *p, бцел тип)            { иницРеф(p, тип); Magic(); }
	const Проц *дайПроцУк() const        { ПРОВЕРЬ(реф_ли()); return укз(); }

	friend проц разверни(Значение& a, Значение& b)  { разверни(a.данные, b.данные); }

	typedef КонстОбходчикУ<Вектор<Значение>> const_iterator;

	const_iterator         begin() const                      { return GetVA().begin(); }
	const_iterator         end() const                        { return GetVA().end(); }

#ifdef DEPRECATED
	typedef Значение          value_type;
	typedef const_iterator КонстОбходчик;
	typedef const Значение&   const_reference;
	typedef цел            size_type;
	typedef цел            difference_type;

	static  проц регистрируй(бцел w, Проц* (*c)(), кткст0 имя = NULL);
#endif
};

struct ОшибкаТипаЗначения : Искл {
	Значение ист;
	цел   target;

	ОшибкаТипаЗначения(const Ткст& text, const Значение& ист, цел target);
};

struct ИндексОбщее {
	enum { HIBIT = 0x80000000 };

	struct Хэш : Движ<Хэш> {
		цел   next; // also link for unlinked items...
		бцел hash;
		цел   prev;
	};

	цел         *map;
	Хэш        *hash;
	бцел        mask;
	цел          unlinked;


	static цел empty[1];

	static бцел Smear(т_хэш h)          { return складиХэш(h) | HIBIT; }

	проц линкуй(цел& m, Хэш& hh, цел ii);
	проц линкуй(цел ii, бцел sh);
	проц уд(цел& m, Хэш& hh, цел ii);
	проц Ins(цел& m, Хэш& hh, цел ii);

	проц сделайМап(цел count);
	проц ремапируй(цел count);
	проц реиндексируй(цел count);
	проц нарастиМап(цел count);
	проц освободиМап();
	проц освободи();

	проц уст(цел ii, бцел h);

	Вектор<цел> дайОтлинкованно() const;

	проц очисть();
	проц обрежь(цел n, цел count);
	проц смети(цел n);
	проц резервируй(цел n);
	проц сожми();
	проц AdjustMap(цел count, цел alloc);

	проц копируй(const ИндексОбщее& b, цел count);
	проц подбери(ИндексОбщее& b);
	проц разверни(ИндексОбщее& b);

	ИндексОбщее();
	~ИндексОбщее();
};


class Реф : Движ<Реф> {
protected:
	РефМенеджер *m;
	проц       *укз;
	struct      ValueRef;

public:
	бцел    дайТип() const           { return m ? m->дайТип() : VOID_V; }
	бул     пусто_ли() const            { return m ? m->пусто_ли(укз) : true; }

	проц       *дайПроцУк() const     { return укз; }
	РефМенеджер *GetManager() const     { return m; }

	template <class T>
	бул     является() const                { return дайТип() == дайНомТипаЗнач<T>(); } // VALUE_V!!!
	template <class T>
	T&       дай() const               { ПРОВЕРЬ(дайНомТипаЗнач<T>() == дайТип()); return *(T *)дайПроцУк(); }

	проц     устПусто()                 { if(m) m->устПусто(укз); }
	Значение    дайЗначение() const          { return m ? m->дайЗначение(укз) : Значение(); }
	проц     устЗначение(const Значение& v)  { ПРОВЕРЬ(m); m->устЗначение(укз, v); }

	operator Значение() const             { return дайЗначение(); }
	Значение    operator~() const         { return дайЗначение(); }
	Реф&     operator=(const Значение& v) { устЗначение(v); return *this; }

	Реф(Ткст& s);
	Реф(ШТкст& s);
	Реф(цел& i);
	Реф(дол& i);
	Реф(дво& d);
	Реф(бул& b);
	Реф(Дата& d);
	Реф(Время& t);
	Реф(Значение& v);
	Реф(ук _ptr, РефМенеджер *_m)    { укз = _ptr, m = _m; }
	Реф(const РефНаТипЗнач& r);
	Реф()                              { укз = m = NULL; }
};

struct РефНаТипЗнач {
	РефМенеджер *m;
	проц       *укз;
};

class ГенЗнач {
public:
	virtual Значение  дай() = 0;
	Значение operator++()  { return дай(); }
	virtual ~ГенЗнач() {}
};

class МассивЗнач : public ТипЗнач<МассивЗнач, VALUEARRAY_V, Движ<МассивЗнач> > {
	struct Данные : Значение::Проц {
		virtual бул         пусто_ли() const;
		virtual проц         сериализуй(Поток& s);
		virtual проц         вРяр(РярВВ& xio);
		virtual проц         вДжейсон(ДжейсонВВ& jio);
		virtual т_хэш       дайХэшЗнач() const;
		virtual бул         равен(const Значение::Проц *p);
		virtual Ткст       какТкст() const;
		virtual цел          сравни(const Значение::Проц *p);

		цел GetRefCount() const     { return refcount; }
		Вектор<Значение>& клонируй();

		Вектор<Значение> данные;
	};
	struct ДанныеПусто : Данные {};
	Данные *данные;
	
	static Вектор<Значение> VoidData;

	Вектор<Значение>& создай();
	Вектор<Значение>& клонируй();
	проц  иниц0();

	friend Значение::Проц *создайДанМассивЗач();
	friend class Значение;
	friend class МапЗнач;

public:
	МассивЗнач()                              { иниц0(); }
	МассивЗнач(const МассивЗнач& v);
	МассивЗнач(МассивЗнач&& v);
	МассивЗнач(Вектор<Значение>&& values);
	explicit МассивЗнач(const Вектор<Значение>& values, цел deep);
	МассивЗнач(std::initializer_list<Значение> init)    { иниц0(); for(const auto& i : init) { добавь(i); }}
	~МассивЗнач();

	МассивЗнач& operator=(const МассивЗнач& v);
	МассивЗнач& operator=(Вектор<Значение>&& values)  { *this = МассивЗнач(пикуй(values)); return *this; }

	operator Значение() const;
	МассивЗнач(const Значение& ист);

	МассивЗнач(const Обнул&)                 { иниц0(); }
	бул экзПусто_ли() const               { return пустой(); }
	
	проц очисть();
	проц устСчёт(цел n);
	проц устСчёт(цел n, const Значение& v);
	цел  дайСчёт() const                     { return данные->данные.дайСчёт(); }
	бул пустой() const                      { return данные->данные.пустой(); }

	проц добавь(const Значение& v);
	МассивЗнач& operator<<(const Значение& v)    { добавь(v); return *this; }
	проц уст(цел i, const Значение& v);
	const Значение& дай(цел i) const;
	Значение дайИОчисть(цел i);
	const Вектор<Значение>& дай() const          { return данные ? данные->данные : VoidData; }
	Вектор<Значение> подбери();

	проц удали(цел i, цел count = 1);
	проц удали(const Вектор<цел>& ii);
	проц вставь(цел i, const МассивЗнач& va);
	проц приставь(const МассивЗнач& va)         { вставь(дайСчёт(), va); }

	const Значение& operator[](цел i) const      { return дай(i); }
	
	Значение& по(цел i);

	т_хэш   дайХэшЗнач() const             { return данные->дайХэшЗнач(); }
	проц     сериализуй(Поток& s);
	проц     вДжейсон(ДжейсонВВ& jio);
	проц     вРяр(РярВВ& xio);
	Ткст   вТкст() const;

	бул     operator==(const МассивЗнач& v) const;
	бул     operator!=(const МассивЗнач& v) const  { return !operator==(v); }
	
	цел      сравни(const МассивЗнач& b) const;
	бул     operator<=(const МассивЗнач& x) const { return сравни(x) <= 0; }
	бул     operator>=(const МассивЗнач& x) const { return сравни(x) >= 0; }
	бул     operator<(const МассивЗнач& x) const  { return сравни(x) < 0; }
	бул     operator>(const МассивЗнач& x) const  { return сравни(x) > 0; }

	typedef КонстОбходчикУ<Вектор<Значение>> const_iterator;

#ifdef DEPRECATED
	typedef Значение          value_type;
	typedef const_iterator КонстОбходчик;
	typedef const Значение&   const_reference;
	typedef цел            size_type;
	typedef цел            difference_type;
#endif

	const_iterator         begin() const                      { return дай().begin(); }
	const_iterator         end() const                        { return дай().end(); }
};
class МапЗнач : public ТипЗнач<МапЗнач, VALUEMAP_V, Движ<МапЗнач> >{
	struct Данные : Значение::Проц {
		virtual бул       пусто_ли() const;
		virtual проц       сериализуй(Поток& s);
		virtual проц       вРяр(РярВВ& xio);
		virtual проц       вДжейсон(ДжейсонВВ& jio);
		virtual т_хэш     дайХэшЗнач() const;
		virtual бул       равен(const Значение::Проц *p);
		virtual Ткст     какТкст() const;
		virtual цел        сравни(const Значение::Проц *p);

		const Значение& дай(const Значение& k) const {
			цел q = ключ.найди(k);
			return q >= 0 ? значение[q] : значОш();
		}
		Значение& дайДобавь(const Значение& k) {
			цел i = ключ.найди(k);
			if(i < 0) {
				i = значение.дайСчёт();
				ключ.добавь(k);
			}
			return значение.по(i);
		}
		Значение& по(цел i) {
			ПРОВЕРЬ(i < значение.дайСчёт());
			return значение.по(i);
		}

		Индекс<Значение> ключ;
		МассивЗнач   значение;
	};

	struct ДанныеПусто : Данные {};
	Данные *данные;

	Данные& создай();
	static проц клонируй(Данные *&укз);
	форс_инлайн
	static МапЗнач::Данные& разшарь(МапЗнач::Данные *&укз) { if(укз->GetRefCount() != 1) клонируй(укз); return *укз; }
	Данные& разшарь() { return разшарь(данные); }
	проц  иниц0();
	проц  изМассива(const МассивЗнач& va);

	friend Значение::Проц *создайДанМапЗач();
	friend class Значение;

public:
	МапЗнач()                                      { иниц0(); }
	МапЗнач(const МапЗнач& v);
	МапЗнач(const МассивЗнач& va)                  { изМассива(va); }
	МапЗнач(Индекс<Значение>&& k, Вектор<Значение>&& v);
	МапЗнач(ВекторМап<Значение, Значение>&& m);
	МапЗнач(const Индекс<Значение>& k, const Вектор<Значение>& v, цел deep);
	МапЗнач(const ВекторМап<Значение, Значение>& m, цел deep);
	МапЗнач(std::initializer_list<std::pair<Значение, Значение>> init) { иниц0(); for(const auto& i : init) { добавь(i.first, i.second); }}
	~МапЗнач();

	МапЗнач& operator=(const МапЗнач& v);
	МапЗнач& operator=(ВекторМап<Значение, Значение>&& m) { *this = МапЗнач(пикуй(m)); return *this; }

	operator Значение() const;
	МапЗнач(const Значение& ист);

	МапЗнач(const Обнул&)                         { иниц0(); }
	бул экзПусто_ли() const                     { return пустой(); }

	проц очисть();
	цел  дайСчёт() const                           { return данные->значение.дайСчёт(); }
	бул пустой() const                            { return данные->значение.пустой(); }
	const Значение& дайКлюч(цел i) const                { return данные->ключ[i]; }
	const Значение& дайЗначение(цел i) const              { return данные->значение[i]; }
	цел  найди(const Значение& ключ) const               { return данные ? данные->ключ.найди(ключ) : -1; }
	цел  найдиСледщ(цел ii) const                     { return данные ? данные->ключ.найдиСледщ(ii) : -1; }

	проц добавь(const Значение& ключ, const Значение& значение);
	проц добавь(const Ткст& ключ, const Значение& значение) { добавь(Значение(ключ), значение); }
	проц добавь(кткст0 ключ, const Значение& значение)   { добавь(Значение(ключ), значение); }
	проц добавь(цел ключ, const Значение& значение)           { добавь(Значение(ключ), значение); }
	проц добавь(Ид ключ, const Значение& значение)            { добавь(Значение(ключ.вТкст()), значение); }
	
	МапЗнач& operator()(const Значение& ключ, const Значение& значение)  { добавь(ключ, значение); return *this; }
	МапЗнач& operator()(const Ткст& ключ, const Значение& значение) { добавь(Значение(ключ), значение); return *this; }
	МапЗнач& operator()(кткст0 ключ, const Значение& значение)   { добавь(Значение(ключ), значение); return *this; }
	МапЗнач& operator()(цел ключ, const Значение& значение)           { добавь(Значение(ключ), значение); return *this; }
	МапЗнач& operator()(Ид ключ, const Значение& значение)            { добавь(Значение(ключ.вТкст()), значение); return *this; }

	проц уст(const Значение& ключ, const Значение& значение);
	проц уст(const Ткст& ключ, const Значение& значение) { уст(Значение(ключ), значение); }
	проц уст(кткст0 ключ, const Значение& значение)   { уст(Значение(ключ), значение); }
	проц уст(цел ключ, const Значение& значение)           { уст(Значение(ключ), значение); }
	проц уст(Ид ключ, const Значение& значение)            { уст(Значение(ключ.вТкст()), значение); }

	проц устПо(цел i, const Значение& v);
	проц устКлюч(цел i, const Значение& ключ);
	проц устКлюч(цел i, const Ткст& ключ)           { устКлюч(i, Значение(ключ)); }
	проц устКлюч(цел i, const сим* ключ)             { устКлюч(i, Значение(ключ)); }
	проц устКлюч(цел i, цел ключ)                     { устКлюч(i, Значение(ключ)); }
	проц устКлюч(цел i, Ид ключ)                      { устКлюч(i, Значение(ключ.вТкст())); }

	цел  удалиКлюч(const Значение& ключ);
	цел  удалиКлюч(const Ткст& ключ)               { return удалиКлюч(Значение(ключ)); }
	цел  удалиКлюч(const сим* ключ)                 { return удалиКлюч(Значение(ключ)); }
	цел  удалиКлюч(цел ключ)                         { return удалиКлюч(Значение(ключ)); }
	цел  удалиКлюч(Ид ключ)                          { return удалиКлюч(Значение(ключ.вТкст())); }
	проц удали(цел i);

	const Индекс<Значение>& дайКлючи() const             { return данные->ключ; }
	МассивЗнач дайЗначения() const                    { return данные->значение; }

	operator МассивЗнач() const                     { return дайЗначения(); }
	
	ВекторМап<Значение, Значение> подбери();

	const Значение& operator[](const Значение& ключ) const  { return данные->дай(ключ); }
	const Значение& operator[](const Ткст& ключ) const { return operator[](Значение(ключ)); }
	const Значение& operator[](кткст0 ключ) const   { return operator[](Значение(ключ)); }
	const Значение& operator[](const цел ключ) const     { return operator[](Значение(ключ)); }
	const Значение& operator[](const Ид& ключ) const     { return operator[](Значение(ключ.вТкст())); }

	Значение& дайДобавь(const Значение& ключ)                  { return разшарь().дайДобавь(ключ); }
	Значение& operator()(const Значение& ключ)              { return дайДобавь(ключ); }
	Значение& operator()(const Ткст& ключ)             { return operator()(Значение(ключ)); }
	Значение& operator()(кткст0 ключ)               { return operator()(Значение(ключ)); }
	Значение& operator()(const цел ключ)                 { return operator()(Значение(ключ)); }
	Значение& operator()(const Ид& ключ)                 { return operator()(Значение(ключ.вТкст())); }
	Значение& по(цел i)                                 { return разшарь().по(i); }
	
	Значение дайИОчисть(const Значение& ключ);

	т_хэш   дайХэшЗнач() const                   { return данные->дайХэшЗнач(); }
	проц     сериализуй(Поток& s);
	проц     вДжейсон(ДжейсонВВ& jio);
	проц     вРяр(РярВВ& xio);
	Ткст   вТкст() const                       { return данные->какТкст(); }

	бул operator==(const МапЗнач& v) const;
	бул operator!=(const МапЗнач& v) const        { return !operator==(v); }

	цел      сравни(const МапЗнач& b) const;
	бул     operator<=(const МапЗнач& x) const    { return сравни(x) <= 0; }
	бул     operator>=(const МапЗнач& x) const    { return сравни(x) >= 0; }
	бул     operator<(const МапЗнач& x) const     { return сравни(x) < 0; }
	бул     operator>(const МапЗнач& x) const     { return сравни(x) > 0; }

	бул     одинаково(const МапЗнач& b) const;
};

struct ПорядокЗнач {
	virtual бул operator()(const Значение& a, const Значение& b) const = 0;
	virtual ~ПорядокЗнач() {}
};

struct StdValueOrder : ПорядокЗнач {
	цел язык;

	virtual бул operator()(const Значение& a, const Значение& b) const { return сравниСтдЗнач(a, b, язык) < 0; }

	StdValueOrder(цел l = -1) : язык(l) {}
};

struct FnValueOrder : ПорядокЗнач {
	цел (*фн)(const Значение& a, const Значение& b);

	virtual бул operator()(const Значение& a, const Значение& b) const { return (*фн)(a, b) < 0; }

	FnValueOrder(цел (*фн)(const Значение& a, const Значение& b)) : фн(фн) {}
};

struct ValuePairOrder {
	virtual бул operator()(const Значение& keya, const Значение& valuea, const Значение& keyb, const Значение& valueb) const = 0;
	virtual ~ValuePairOrder() {}
};

struct StdValuePairOrder : ValuePairOrder {
	цел язык;

	virtual бул operator()(const Значение& keya, const Значение& valuea, const Значение& keyb, const Значение& valueb) const;

	StdValuePairOrder(цел l = -1);
};

struct FnValuePairOrder : ValuePairOrder {
	цел (*фн)(const Значение& k1, const Значение& v1, const Значение& k2, const Значение& v2);

	virtual бул operator()(const Значение& keya, const Значение& valuea, const Значение& keyb, const Значение& valueb) const;

	FnValuePairOrder(цел (*фн)(const Значение& k1, const Значение& v1, const Значение& k2, const Значение& v2)) : фн(фн) {}
};

class Ид : Движ<Ид> {
	Ткст ид;

public:
	const Ткст&  вТкст() const              { return ид; }
	т_хэш         дайХэшЗнач() const          { return дайХэшЗнач(ид); }
	бул           пусто_ли() const                { return пусто_ли(ид); }

	operator const Ткст&() const               { return вТкст(); }
	const Ткст&  operator~() const             { return вТкст(); }
	бул           operator==(const Ид& b) const { return ид == b.ид; }
	бул           operator!=(const Ид& b) const { return ид != b.ид; }

	operator бул() const                        { return ид.дайСчёт(); }

	Ид()                                         {}
	Ид(const Ткст& s)                          { ид = s; }
	Ид(кткст0 s)                            { ид = s; }
};

struct РефМенеджер {
	virtual цел   дайТип() = 0;
	virtual Значение дайЗначение(const проц *)            { return Null; }
	virtual бул  пусто_ли(const проц *)              { return false; }
	virtual проц  устЗначение(проц *, const Значение& v)  { НИКОГДА(); }
	virtual проц  устПусто(проц *)                   { НИКОГДА(); }
	virtual ~РефМенеджер() {}
};

class Преобр {
public:
	Преобр();
	virtual ~Преобр();

	virtual Значение  фмт(const Значение& q) const;
	virtual Значение  скан(const Значение& text) const;
	virtual цел    фильтруй(цел chr) const;

	Значение operator()(const Значение& q) const              { return фмт(q); }
};

class ПреобрЦел : public Преобр {
public:
	virtual Значение скан(const Значение& text) const;
	virtual цел   фильтруй(цел chr) const;

protected:
	дол минзнач, максзнач;
	бул  notnull;

public:
	ПреобрЦел& минмакс(цел _min, цел _max)        { минзнач = _min; максзнач = _max; return *this; }
	ПреобрЦел& мин(цел _min)                     { минзнач = _min; return *this; }
	ПреобрЦел& макс(цел _max)                     { максзнач = _max; return *this; }
	ПреобрЦел& неПусто(бул b = true)            { notnull = b; return *this; }
	ПреобрЦел& безНеПусто()                       { return неПусто(false); }
	цел         дайМин() const                    { return (цел)минзнач; }
	цел         дайМакс() const                    { return (цел)максзнач; }
	бул        неПусто_ли() const                 { return notnull; }
	
	static цел  дайДефМин()                   { return -INT_MAX; }
	static цел  дайДефМакс()                   { return INT_MAX; }

	ПреобрЦел(цел минзнач = -INT_MAX, цел максзнач = INT_MAX, бул notnull = false)
		: минзнач(минзнач), максзнач(максзнач), notnull(notnull) {}
};

struct ПреобрЦел64 : public ПреобрЦел {
	ПреобрЦел64& минмакс(дол _min, дол _max)    { минзнач = _min; максзнач = _max; return *this; }
	ПреобрЦел64& мин(дол _min)                   { минзнач = _min; return *this; }
	ПреобрЦел64& макс(дол _max)                   { максзнач = _max; return *this; }
	дол         дайМин() const                    { return минзнач; }
	дол         дайМакс() const                    { return максзнач; }

	static дол  дайДефМин()                   { return -ЦЕЛ64_МАКС; }
	static дол  дайДефМакс()                   { return ЦЕЛ64_МАКС; }

	ПреобрЦел64(дол минзнач = -ЦЕЛ64_МАКС, дол максзнач = ЦЕЛ64_МАКС, бул notnull = false) {
		минмакс(минзнач, максзнач); неПусто(notnull);
	}
};

class ПреобрДво : public Преобр {
public:
	virtual Значение фмт(const Значение& q) const;
	virtual Значение скан(const Значение& text) const;
	virtual цел   фильтруй(цел chr) const;

protected:
	дво      минзнач, максзнач;
	бул        notnull, comma;
	Ткст      pattern;

public:
	ПреобрДво& образец(кткст0 p);
	ПреобрДво& минмакс(дво _min, дво _max)  { минзнач = _min; максзнач = _max; return *this; }
	ПреобрДво& мин(дво _min)                  { минзнач = _min; return *this; }
	ПреобрДво& макс(дво _max)                  { максзнач = _max; return *this; }
	ПреобрДво& неПусто(бул b = true)            { notnull = b; return *this; }
	ПреобрДво& безНеПусто()                       { return неПусто(false); }
	дво         дайМин() const                    { return минзнач; }
	дво         дайМакс() const                    { return максзнач; }
	бул           неПусто_ли() const                 { return notnull; }

	static дво  дайДефМин()                   { return -std::numeric_limits<дво>::макс(); }
	static дво  дайДефМакс()                   { return std::numeric_limits<дво>::макс(); }

	ПреобрДво(дво минзнач = дайДефМин(), дво максзнач = дайДефМакс(),
		          бул notnull = false);
};

class ПреобрДату : public Преобр {
public:
	virtual Значение фмт(const Значение& q) const;
	virtual Значение скан(const Значение& text) const;
	virtual цел   фильтруй(цел chr) const;

protected:
	Дата минзнач, максзнач, defaultval;
	бул notnull;
	
	static Дата& default_min();
	static Дата& default_max();

public:
	ПреобрДату& минмакс(Дата _min, Дата _max)      { минзнач = _min; максзнач = _max; return *this; }
	ПреобрДату& мин(Дата _min)                    { минзнач = _min; return *this; }
	ПреобрДату& макс(Дата _max)                    { максзнач = _max; return *this; }
	ПреобрДату& неПусто(бул b = true)            { notnull = b; return *this; }
	ПреобрДату& безНеПусто()                       { return неПусто(false); }
	ПреобрДату& дефолт(Дата d)                   { defaultval = d; return *this; }
	бул         неПусто_ли() const                 { return notnull; }
	Дата         дайМин() const                    { return макс(дайДефМин(), минзнач); }
	Дата         дайМакс() const                    { return мин(дайДефМакс(), максзнач); }

	static проц  устДефМинМакс(Дата мин, Дата макс);
	static Дата  дайДефМин()                   { return default_min(); }
	static Дата  дайДефМакс()                   { return default_max(); }

	ПреобрДату(Дата минзнач = Дата::наименьш(), Дата максзнач = Дата::наибольш(), бул notnull = false);
};

class ПреобрВремя : public Преобр {
public:
	virtual Значение скан(const Значение& text) const;
	virtual цел   фильтруй(цел chr) const;
	virtual Значение фмт(const Значение& q) const;

protected:
	Время минзнач, максзнач, defaultval;
	бул notnull;
	бул seconds;
	бул timealways;
	бул dayend;

public:
	ПреобрВремя& минмакс(Время _min, Время _max)      { минзнач = _min; максзнач = _max; return *this; }
	ПреобрВремя& мин(Время _min)                    { минзнач = _min; return *this; }
	ПреобрВремя& макс(Время _max)                    { максзнач = _max; return *this; }
	ПреобрВремя& неПусто(бул b = true)            { notnull = b; return *this; }
	ПреобрВремя& безНеПусто()                       { return неПусто(false); }
	ПреобрВремя& секунды(бул b = true)            { seconds = b; return *this; }
	ПреобрВремя& безСекунд()                       { return секунды(false); }
	бул         секунды_ли() const                 { return seconds; }
	ПреобрВремя& времяВсегда(бул b = true)         { timealways = b; return *this; }
	бул         времяВсегда_ли() const              { return timealways; }
	ПреобрВремя& конецДня(бул b = true)             { dayend = b; return *this; }
	бул         конецДня_ли() const                  { return dayend; }
	ПреобрВремя& дефолт(Время d)                   { defaultval = d; return *this; }
	бул         неПусто_ли() const                 { return notnull; }

	Время         дайМин() const                    { return макс(минзнач, дайДефМин()); }
	Время         дайМакс() const                    { return мин(максзнач, дайДефМакс()); }

	static Время  дайДефМин()                   { return воВремя(ПреобрДату::дайДефМин()); }
	static Время  дайДефМакс()                   { return воВремя(ПреобрДату::дайДефМакс()); }

	ПреобрВремя(Время минзнач = воВремя(Дата::наименьш()), Время максзнач = воВремя(Дата::наибольш()), бул notnull = false);
	virtual ~ПреобрВремя();
};

class ПреобрТкст : public Преобр {
public:
	virtual Значение скан(const Значение& text) const;

protected:
	цел  maxlen;
	бул notnull, trimleft, trimright;

public:
	ПреобрТкст& максдлин(цел _maxlen)             { maxlen = _maxlen; return *this; }
	цел            дайМаксДлин() const            { return maxlen; }
	ПреобрТкст& неПусто(бул b = true)          { notnull = b; return *this; }
	ПреобрТкст& безНеПусто()                     { return неПусто(false); }
	бул           неПусто_ли() const               { return notnull; }
	ПреобрТкст& обрежьЛево(бул b = true)         { trimleft = b; return *this; }
	ПреобрТкст& обрежьПраво(бул b = true)        { trimright = b; return *this; }
	ПреобрТкст& обрежьОба(бул b = true)         { return обрежьЛево(b).обрежьПраво(b); }
	бул           обрежьЛево_ли() const              { return trimleft; }
	бул           обрежьПраво_ли() const             { return trimright; }

	ПреобрТкст(цел maxlen = INT_MAX, бул notnull = false)
		: maxlen(maxlen), notnull(notnull) { trimleft = trimright = false; }
};

class КлассНеПреобр : public Преобр {
public:
	КлассНеПреобр();

	virtual Значение  фмт(const Значение& q) const;
};

class КлассОшибкаПреобр : public Преобр {
public:
	Значение скан(const Значение& v) const;
};

class МапПреобр : public Преобр {
public:
	virtual Значение  фмт(const Значение& q) const;

protected:
	ВекторМап<Значение, Значение> map;
	Значение                   default_value;

public:
	проц         очисть()                                 { map.очисть(); }
	МапПреобр&  добавь(const Значение& a, const Значение& b)     { map.добавь(a, b); return *this; }
	МапПреобр&  дефолт(const Значение& v)                 { default_value = v; return *this; }

	цел          дайСчёт() const                        { return map.дайСчёт(); }
	цел          найди(const Значение& v) const              { return map.найди(v); }
	const Значение& дайКлюч(цел i) const                     { return map.дайКлюч(i); }
	const Значение& дайЗначение(цел i) const                   { return map[i]; }
	const Значение& operator[](цел i) const                 { return map[i]; }

	virtual ~МапПреобр() {}
};

class ПреобрСоед : public Преобр {
public:
	virtual Значение фмт(const Значение& v) const;

protected:
	struct Элемент {
		цел            pos;
		const Преобр *convert;
		Ткст         text;
	};
	Массив<Элемент> элт;

	цел следщПоз() const;

public:
	ПреобрСоед& добавь(кткст0 text);
	ПреобрСоед& добавь(цел pos, const Преобр& cv);
	ПреобрСоед& добавь(цел pos);
	ПреобрСоед& добавь(const Преобр& cv);
	ПреобрСоед& добавь();
};

class ПреобрФормата : public Преобр {
public:
	virtual Значение фмт(const Значение& v) const;

private:
	Ткст формат;

public:
	проц   устФормат(кткст0 fmt)           { формат = fmt; }
};

class Цвет : public ТипЗнач<Цвет, COLOR_V, Движ<Цвет> > {
protected:
	бцел    color;

	бцел дай() const;

public:
	бцел    дайСырой() const            { return color; }

	цел      дайК() const              { return дайЗнК(дай()); }
	цел      дайЗ() const              { return дайЗнЗ(дай()); }
	цел      дайС() const              { return дайЗнС(дай()); }

	проц     устПусто()                 { color = 0xffffffff; }
	бул     экзПусто_ли() const    { return color == 0xffffffff; }
	т_хэш   дайХэшЗнач() const      { return color; }
	бул     operator==(Цвет c) const { return color == c.color; }
	бул     operator!=(Цвет c) const { return color != c.color; }

	проц     сериализуй(Поток& s)      { s % color; }
	проц     вДжейсон(ДжейсонВВ& jio);
	проц     вРяр(РярВВ& xio);

	Цвет()                            { устПусто(); }
	Цвет(цел r, цел g, цел b)         { color = дайКЗС(r, g, b); }

	Цвет(const Обнул&)               { устПусто(); }

	operator Значение() const             { return своВЗнач(*this); }
	Цвет(const Значение& q)              { color = q.дай<Цвет>().color; }

	operator КЗСА() const;
	Цвет(КЗСА rgba);

	Цвет(Цвет (*фн)())               { color = (*фн)().color; }

	static Цвет изСырого(бцел co)     { Цвет c; c.color = co; return c; }
	static Цвет особый(цел n)        { Цвет c; c.color = 0x80000000 | n; return c; }

	цел  дайСпец() const            { return color & 0x80000000 ? color & 0x7fffffff : -1; }

#ifdef PLATFORM_WIN32
	operator COLORREF() const          { return (COLORREF) дай(); }
	static  Цвет FromCR(COLORREF cr)  { Цвет c; c.color = (бцел)cr; return c; }
#else
	operator бцел() const             { return дай(); }
#endif

private:
	Цвет(цел);
};

#define VALUE_COMPARE_V(T, VT) \
inline бул operator==(const Значение& v, T x)   { return v.является<VT>() ? (VT)v == x : v == Значение(x); } \
inline бул operator==(T x, const Значение& v)   { return v.является<VT>() ? (VT)v == x : v == Значение(x); } \
inline бул operator!=(const Значение& v, T x)   { return v.является<VT>() ? (VT)v != x : v != Значение(x); } \
inline бул operator!=(T x, const Значение& v)   { return v.является<VT>() ? (VT)v != x : v != Значение(x); } \

#define СРАВНИ_ЗНАЧЕНИЕ(T) VALUE_COMPARE_V(T, T)

СРАВНИ_ЗНАЧЕНИЕ(цел)
СРАВНИ_ЗНАЧЕНИЕ(дол)
СРАВНИ_ЗНАЧЕНИЕ(дво)
СРАВНИ_ЗНАЧЕНИЕ(бул)
СРАВНИ_ЗНАЧЕНИЕ(Дата)
СРАВНИ_ЗНАЧЕНИЕ(Время)
СРАВНИ_ЗНАЧЕНИЕ(Ткст)
СРАВНИ_ЗНАЧЕНИЕ(ШТкст)
VALUE_COMPARE_V(кткст0 , Ткст)
VALUE_COMPARE_V(const шим *, ШТкст)

struct Комплекс : std::complex<дво>
{
	typedef std::complex<дво> C;

	Комплекс()                               {}
	Комплекс(дво r) : C(r)                {}
	Комплекс(дво r, дво i) : C(r,i)    {}
	Комплекс(const C& c) : C(c)              {}

	Комплекс(const Обнул&) : C(ДВО_НУЛЛ, ДВО_НУЛЛ) {}
	operator Значение() const                  { return богатыйВЗнач(*this); }
	Комплекс(const Значение& v) : C(число_ли(v) ? Комплекс((дво)v) : v.дай<Комплекс>()) {}

	operator Реф()                          { return какРеф(*this); }

	бул operator==(const Комплекс& c) const { return (const C&)(*this) == (const C&)c; }
	бул operator!=(const Комплекс& c) const { return (const C&)(*this) != (const C&)c; }

	цел  сравни(const Комплекс& c) const    { НИКОГДА(); return 0; }

	проц сериализуй(Поток& s);
	проц вРяр(РярВВ& xio);
	проц вДжейсон(ДжейсонВВ& jio);
};

СРАВНИ_ЗНАЧЕНИЕ(Комплекс)
NTL_MOVEABLE(Комплекс)

class ТаймСтоп : Движ<ТаймСтоп> {
	дво starttime;

public:
	дво прошло() const           { return usecs() - starttime; }
	дво секунды() const           { return (дво)прошло() / 1000000; }
	Ткст вТкст() const;
	проц   переустанов();

	ТаймСтоп();
};

struct ТаймСтопор {
	кткст0 имя;
	ТаймСтоп tm;
	
	ТаймСтопор(кткст0 имя) : имя(имя) {}
	~ТаймСтопор() { RLOG(имя << " " << tm); }
};

#define RTIMESTOP(имя) ТаймСтопор КОМБИНИРУЙ(sTmStop, __LINE__)(имя);

struct ИниТкст {
// "private":
	const сим   *ид;
	Ткст      (*опр)();
	Ткст&     (*ссыл_фн)();
	цел           версия;

// "public:"
	operator Ткст();
	Ткст   operator=(const Ткст& s);
	Ткст   вТкст() const;
};

struct ИниЦел {
// "private":
	кткст0 ид;
	цел       (*опр)();
	цел         версия;
	цел         значение;
	цел         грузи();

// "public:"
	operator    цел()             { цел h = значение; if(иниИзменён__(версия)) return грузи(); return h; }
	цел         operator=(цел b);
	Ткст      вТкст() const;
};

struct ИниЦел64 {
// "private":
	кткст0 ид;
	дол     (*опр)();
	цел         версия;
	дол       значение;

// "public:"
	operator    дол();
	дол       operator=(дол b);
	Ткст      вТкст() const;
};

struct ИниДво {
// "private":
	кткст0 ид;
	дво    (*опр)();
	цел         версия;
	дво      значение;
	дво      грузи();

// "public:"
	operator    дво()           { дво h = значение; if(иниИзменён__(версия)) return грузи(); return h; }
	дво      operator=(дво b);
	Ткст      вТкст() const;
};

struct ИниБул {
// "private":
	кткст0 ид;
	бул      (*опр)();
	цел         версия;
	бул        значение;
	бул        грузи();

// "public:"
	operator     бул()            { бул h = значение; if(иниИзменён__(версия)) return грузи(); return h; }
	бул         operator=(бул b);
	Ткст       вТкст() const;
};

class ИнфОЯз {
	проц уст(цел язык);

	friend const ИнфОЯз& дайИнфОЯз(цел lang);

public:
	цел             язык;
	Ткст          english_name;
	Ткст          native_name;

	Ткст          thousand_separator, decimal_point;
	Ткст          date_format, time_format; // (?)
	
	Ткст          month[12], smonth[12], day[7], sday[7];

	цел     (*сравни)(const шим *a, цел alen, const шим *b, цел blen, цел lang);
	ШТкст (*getindexletter)(const шим *text, цел lang);

	Ткст          фмтЦел(цел значение) const;
	Ткст          фмтДво(дво значение, цел digits, цел FD_flags = 0, цел fill_exp = 0) const;
	Ткст          фмтДата(Дата date) const;
	Ткст          фмтВремя(Время time) const;

	ШТкст         GetIndexLetter(const шим *text) const                            { return (*getindexletter)(text, язык); }
	цел             сравни(const шим *a, цел alen, const шим *b, цел blen) const  { return (*сравни)(a, alen, b, blen, язык); }

	цел             сравни(const шим *a, const шим *b) const    { return сравни(a, длинтекс__(a), b, длинтекс__(b)); }
	цел             сравни(ШТкст a, ШТкст b) const              { return сравни(a, a.дайДлину(), b, b.дайДлину()); }
	цел             сравни(кткст0 a, кткст0 b) const      { return сравни(вУтф32(a), вУтф32(b)); }
	цел             сравни(Ткст a, Ткст b) const                { return сравни(a.вШТкст(), b.вШТкст()); }

	бул            operator()(const шим *a, const шим *b) const { return сравни(a, b) < 0; }
	бул            operator()(ШТкст a, ШТкст b) const           { return сравни(a, b) < 0; }
	бул            operator()(кткст0 a, кткст0 b) const   { return сравни(a, b) < 0; }
	бул            operator()(Ткст a, Ткст b) const             { return сравни(a, b) < 0; }

	Ткст          вТкст() const;
	
	ИнфОЯз();

//BWC
	ШТкст         month_names[12], short_month_names[12];
	ШТкст         day_names[7], short_day_names[7];
	цел             сравни(const шим *a, const шим *b, цел alen, цел blen) const { return сравни(a, alen, b, blen); }
};

// ------ Language internals ----------------

struct LangModuleRecord {
	кткст0 ид;
	кткст0 *укз;
};

struct LangTextRecord {
	кткст0 *укз;
	кткст0 text;
};

struct ИниИнфо {
	Ткст ид;
	Ткст инфо;
	Ткст (*текущ)();
	Ткст (*опр)();
};

#define INI_TYPE(var, опр, инфо, тип, decl, ref)\
тип DefIni_##var() { return опр; }\
decl var = { #var, DefIni_##var, ref };\
Ткст AsStringIniCurrent_##var() { return какТкст(var); } \
Ткст AsStringIniDefault_##var() { return какТкст(DefIni_##var()); } \
ИНИЦБЛОК { добавьИнфоИни(#var, AsStringIniCurrent_##var, AsStringIniDefault_##var, инфо); }

#define INI_BOOL(var, опр, инфо)   INI_TYPE(var, опр, инфо, бул, ИниБул, 0);
#define INI_INT(var, опр, инфо)    INI_TYPE(var, опр, инфо, цел, ИниЦел, 0);
#define INI_INT64(var, опр, инфо)  INI_TYPE(var, опр, инфо, дол, ИниЦел64, 0);
#define INI_DOUBLE(var, опр, инфо) INI_TYPE(var, опр, инфо, дво, ИниДво, 0);

#define INI_STRING(var, опр, инфо) Ткст& DefRef_##var() { static Ткст x; return x; }\
                                   INI_TYPE(var, опр, инфо, Ткст, ИниТкст, DefRef_##var);

//# System dependent !
class БитИУк {
	uintptr_t bap;

public:
	проц  устБит(бул b)  { bap = (~1 & bap) | (uintptr_t)b; }
	проц  устУк(ук p) { bap = (1 & bap) | (uintptr_t)p; }

	бул  дайБит() const  { return bap & 1; }
	ук дайУк() const  { return (проц *) (bap & ~1); }

	проц  уст0(ук укз) { bap = (uintptr_t)укз; }
	проц  уст1(ук укз) { bap = (uintptr_t)укз | 1; }

	БитИУк()           { bap = 0; }
};

class ПрервиИскл : public Искл {
public:
	ПрервиИскл();
};

// ------------------- настройки текста в стиле Linux -------------
class НастройкиТекста {
	ВекторМап< Ткст, ВекторМап< Ткст, Ткст > > настройки;

public:
	Ткст дай(кткст0 группа, кткст0 ключ) const;
	Ткст дай(кткст0 ключ) const                            { return дай("", ключ); }
	Ткст дай(цел индексГруппы, кткст0 ключ) const;
	Ткст дай(цел индексГруппы, цел индексКлюча) const;
	
	Ткст operator()(кткст0 группа, кткст0 ключ) const  { return дай(группа, ключ); }
	Ткст operator()(кткст0 ключ) const                     { return дай(ключ); }

	проц очисть()                                                 { настройки.очисть(); }
	проц грузи(кткст0 filename);
	
	цел дайСчётГрупп()                                          { return настройки.дайСчёт(); }
	цел дайСчётКлючей(цел группа)                                   { return настройки[группа].дайСчёт(); }
	
	Ткст дайИмяГруппы(цел индексГруппы)                          { return настройки.дайКлюч(индексГруппы); }
	Ткст дайКлюч(цел индексГруппы, цел индексКлюча)                  { return настройки[индексГруппы].дайКлюч(индексКлюча); }
};

struct Дата : ОпыРеляций< Дата, Движ<Дата> > {
	ббайт   day;
	ббайт   month;
	крат  year;

	проц     сериализуй(Поток& s);

	бул     пригоден() const;
	проц     уст(цел scalar);
	цел      дай() const;

	static Дата наименьш()                    { return Дата(-4000, 1, 1); }
	static Дата наибольш()                   { return Дата(4000, 1, 1); }

	цел      сравни(Дата b) const;

	Дата& operator++()                   { if(day < 28) day++; else уст(дай() + 1); return *this; }
	Дата& operator--()                   { if(day > 1) day--; else уст(дай() - 1); return *this; }

	Дата  operator++(цел)                { Дата d = *this; operator++(); return d; }
	Дата  operator--(цел)                { Дата d = *this; operator--(); return d; }

	Дата()                               { year = -32768; day = month = 0; }
	Дата(const Обнул&)                  { year = -32768; day = month = 0; }
	Дата(цел y, цел m, цел d)            { day = d; month = m; year = y; }
};

struct Время : Дата, ОпыРеляций< Время, Движ<Время> > {
	ббайт   hour;
	ббайт   minute;
	ббайт   second;

	проц     сериализуй(Поток& s);

	static Время наибольш()       { return Время(4000, 1, 1); }
	static Время наименьш()        { return Время(-4000, 1, 1); }

	проц   уст(дол scalar);
	дол  дай() const;

	бул   пригоден() const;

	цел    сравни(Время b) const;

	Время()                   { hour = minute = second = 0; }
	Время(const Обнул&)      { hour = minute = second = 0; }
	Время(цел y, цел m, цел d, цел h = 0, цел n = 0, цел s = 0)
		{ day = d; month = m; year = y; hour = h; minute = n; second = s; }

	Время(ФВремя filetime);
	ФВремя какФВремя() const;
};

#ifdef PLATFORM_MACOS
struct КЗСА : Движ<КЗСА> {
        ббайт a, r, g, b;
};
#else
struct КЗСА : Движ<КЗСА> {
	ббайт b, g, r, a;
};
#endif

typedef Цвет (*ColorF)();

#ifdef PLATFORM_WIN32

struct ФВремя : FILETIME, ОпыРелСравнения<const ФВремя&, &сравни_ФВремя> {
	ФВремя()                          {}
	ФВремя(const FILETIME& ft)        { dwLowDateTime = ft.dwLowDateTime;
	                                      dwHighDateTime = ft.dwHighDateTime; }
};

class  ФайлПоиск : БезКопий {
	WIN32_FIND_DATAW  данные[1];
	HANDLE            handle;
	Ткст            pattern;
	Ткст            path;

	бул        Next0();
	проц        открой();

public:
	бул        ищи(кткст0 path);
	бул        следщ();

	бцел       дайАтры() const      { return данные->dwFileAttributes; }
	Ткст      дайИмя() const;
	Ткст      дайПуть() const;
	дол       дайДлину() const;
	ФВремя    дайВремяСозд() const    { return данные->ftCreationTime; }
	ФВремя    дайВремяПоследнДоступа() const  { return данные->ftLastAccessTime; }
	ФВремя    дайВремяПоследнЗаписи() const   { return данные->ftLastWriteTime; }

	бул        директория_ли() const        { return дайАтры() & FILE_ATTRIBUTE_DIRECTORY; }
	бул        папка_ли() const;
	бул        файл_ли() const             { return !директория_ли(); }
	бул        симссылка_ли() const;
	бул        экзе_ли() const;

	бул        архив_ли() const          { return дайАтры() & FILE_ATTRIBUTE_ARCHIVE; }
	бул        сжат_ли() const       { return дайАтры() & FILE_ATTRIBUTE_COMPRESSED; }
	бул        скрыт_ли() const           { return дайАтры() & FILE_ATTRIBUTE_HIDDEN; }
	бул        толькочтен_ли() const         { return дайАтры() & FILE_ATTRIBUTE_READONLY; }
	бул        системн_ли() const           { return дайАтры() & FILE_ATTRIBUTE_SYSTEM; }
	бул        временен_ли() const        { return дайАтры() & FILE_ATTRIBUTE_TEMPORARY; }

	operator    бул() const               { return handle != INVALID_HANDLE_VALUE; }
	бул        operator++()               { return следщ(); }
	бул        operator++(цел)            { return следщ(); }

	struct Обходчик {
		ФайлПоиск *ff;

		проц operator++()                           { if(!ff->следщ()) ff = NULL; }
		бул operator!=(const Обходчик& b) const    { return ff != b.ff; }
		const ФайлПоиск& operator*() const           { return *ff; }
	};
	
	Обходчик begin() { Обходчик h; h.ff = *this ? this : nullptr; return h; }
	Обходчик end()   { Обходчик h; h.ff = nullptr; return h; }

	ФайлПоиск();
	ФайлПоиск(кткст0 имя);
	~ФайлПоиск();
};

#endif

#ifdef PLATFORM_POSIX

struct ФВремя : ОпыРелСравнения<const ФВремя&, &сравни_ФВремя>
{
	ФВремя() {}
	ФВремя(time_t ft) : ft(ft) {}

	operator time_t () const { return ft; }

	time_t ft;
};

inline цел сравни_ФВремя(const ФВремя& f, const ФВремя& g) { return f.ft < g.ft ? -1 : f.ft > g.ft ? 1 : 0; }

class ФайлПоиск : БезКопий {
	бул           file;
	DIR           *dir;
	mutable бул   statis;
	mutable struct stat statf;
	Ткст         path;
	Ткст         имя;
	Ткст         pattern;

	struct stat &Stat() const;
	бул CanMode(бцел usr, бцел grp, бцел oth) const;

public:
	бул        ищи(кткст0 имя);
	бул        следщ();
	проц        открой();

	бцел       дайРежим() const           { return Stat().st_mode; }
	Ткст      дайИмя() const           { return имя; }
	Ткст      дайПуть() const;
	дол       дайДлину() const         { return Stat().st_size; }
	ФВремя    дайВремяПоследнИзм() const { return Stat().st_ctime; }
	ФВремя    дайВремяПоследнДоступа() const { return Stat().st_atime; }
	ФВремя    дайВремяПоследнЗаписи() const  { return Stat().st_mtime; }

	uid_t       дайУид()                  { return Stat().st_uid; }
	gid_t       дайГид()                  { return Stat().st_gid; }

	бул        читаемый() const           { return CanMode(S_IRUSR, S_IRGRP, S_IROTH); }
	бул        записываемый() const          { return CanMode(S_IWUSR, S_IWGRP, S_IWOTH); }
	бул        исполнимый() const        { return CanMode(S_IXUSR, S_IXGRP, S_IXOTH); }

	бул        толькочтен_ли() const        { return читаемый() && !записываемый(); }

	бул        скрыт_ли() const          { return *имя == '.'; }
	бул        директория_ли() const       { return S_ISDIR(дайРежим()); }
	бул        папка_ли() const;
	бул        файл_ли() const            { return S_ISREG(дайРежим()); }
	бул        симссылка_ли() const;
	бул        экзе_ли() const;

	operator    бул() const              { return file; }
	бул        operator++()              { return следщ(); }
	бул        operator++(цел)           { return следщ(); }

	struct Обходчик {
		ФайлПоиск *ff;

		проц operator++()                           { if(!ff->следщ()) ff = NULL; }
		бул operator!=(const Обходчик& b) const    { return ff != b.ff; }
		const ФайлПоиск& operator*() const           { return *ff; }
	};
	
	Обходчик begin() { Обходчик h; h.ff = *this ? this : nullptr; return h; }
	Обходчик end()   { Обходчик h; h.ff = nullptr; return h; }

	ФайлПоиск()                            { file = false; dir = NULL; }
	ФайлПоиск(кткст0 имя);
	~ФайлПоиск()                           { открой(); }
};

// POSIX ФВремя is unfortunately long цел and clashes with Дата::operator цел()
inline бул operator==(Время a, ФВремя b) { return a == Время(b); }
inline бул operator!=(Время a, ФВремя b) { return a != Время(b); }

inline бул operator==(ФВремя a, Время b) { return Время(a) == b; }
inline бул operator!=(ФВремя a, Время b) { return Время(a) != b; }

#endif

class ИнфОФС {
public:
	enum
	{
		ROOT_UNKNOWN     = 0,
		ROOT_NO_ROOT_DIR = 1,
		ROOT_REMOVABLE   = 2,
		ROOT_FIXED       = 3,
		ROOT_REMOTE      = 4,
		ROOT_CDROM       = 5,
		ROOT_RAMDISK     = 6,
		ROOT_NETWORK     = 7,
		ROOT_COMPUTER    = 8,
	};

	enum
	{
		STYLE_WIN32      = 0x0001,
		STYLE_POSIX      = 0x0002,
	};

	struct ИнфОФайле
	{
		ИнфОФайле();

		operator бул () const { return !пусто_ли(имяф); }

		Ткст имяф;
		Ткст msdos_name;
		Ткст root_desc;
		дол  length;
		Время   last_access_time;
		Время   last_write_time;
		Время   creation_time;
		бул   read_only;
		бул   is_directory;
		бул   is_folder;
		бул   is_file;
		бул   is_symlink;
		бул   is_archive;
		бул   is_compressed;
		бул   is_hidden;
		бул   is_read_only;
		бул   is_system;
		бул   is_temporary;
		сим   root_style;
		бцел  unix_mode;
	};

	virtual цел             дайСтиль() const;
	бул                    IsWin32() const { return дайСтиль() & STYLE_WIN32; }
	бул                    IsPosix() const { return дайСтиль() & STYLE_POSIX; }

	virtual Массив<ИнфОФайле> найди(Ткст mask, цел max_count = 1000000, бул unmounted = false) const; // mask = Null -> root
	virtual бул            создайПапку(Ткст path, Ткст& Ошибка) const;

	бул                    папкаЕсть(Ткст path) const;

	virtual ~ИнфОФС() {}
};

#ifdef PLATFORM_WIN32

class УзелСети : Движ<УзелСети> {
	NETRESOURCEW net;
	Вектор<сим16> local, remote, comment, provider;

	Ткст       имя;
	Ткст       path;

	static проц           копируй(Ткст& t, сим *s);
	static Массив<УзелСети> Enum0(HANDLE hEnum);

	проц SetPtrs();

public:
	enum {
		UNKNOWN, NETWORK, GROUP, SERVER, SHARE
	};
	Ткст         дайИмя() const     { return имя; }
	Ткст         дайПуть() const     { return path; }
	цел            GetDisplayType() const;
	Ткст         GetRemote() const   { return вУтф8(remote); }
	Ткст         GetLocal() const    { return вУтф8(local); }
	Ткст         GetProvider() const { return вУтф8(provider); }
	Ткст         GetComment() const  { return вУтф8(comment); }
	Массив<УзелСети> Enum() const;

	static Массив<УзелСети> EnumRoot();
	static Массив<УзелСети> EnumRemembered();

	УзелСети();
	УзелСети(const УзелСети& s)         { *this = s; }

	УзелСети& operator=(const УзелСети& s);
};

#endif

class ВхоФильтрПоток : public Поток {
public:
	virtual   бул  открыт() const;

protected:
	virtual   цел   _прекрати();
	virtual   цел   _получи();
	virtual   бцел _получи(ук данные, бцел size);

	Вектор<ббайт> буфер;
	бул         eof;
	цел          buffersize = 4096;
	Буфер<цел>  inbuffer;
	ббайт        *t; // target pointer for block _получи
	бцел        todo; // target count

	проц   иниц();
	проц   фетч();
	проц   SetRd();

private:
	проц   устРазм(дол size)  { НИКОГДА(); } // removed
	дол  дайРазм() const      { НИКОГДА(); return 0; }

public:
	Поток                      *in;
	Событие<const проц *, цел>     фильтруй;
	Врата<>                       FilterEof;
	Событие<>                      стоп;
	Врата<>                       ещё;
	проц                         выведи(кук укз, цел size);
	
	Событие<>                      WhenOut;
	
	template <class F>
	проц уст(Поток& in_, F& фильтр) {
		иниц();
		in = &in_;
		фильтр.WhenOut = [=](кук укз, цел size) { выведи(укз, size); };
		фильтруй = [&фильтр](кук укз, цел size) { фильтр.помести(укз, size); };
		стоп = [&фильтр] { фильтр.стоп(); };
	}
	
	проц устРазмБуф(цел size) { buffersize = size; inbuffer.очисть(); }
	
	ВхоФильтрПоток();
	template <class F> ВхоФильтрПоток(Поток& in, F& фильтр) { уст(in, фильтр); }
};

class ВыхФильтрПоток : public Поток {
public:
	virtual   проц  открой();
	virtual   бул  открыт() const;

protected:
	virtual   проц  _помести(цел w);
	virtual   проц  _помести(кук данные, бцел size);

	Буфер<ббайт> буфер;
	дол        count;

	проц   излей();
	бцел  Avail()               { return бцел(4096 - (укз - ~буфер)); }
	проц   иниц();

public:
	Поток                      *out;
	Событие<const проц *, цел>     фильтруй;
	Событие<>                      стоп;
	проц                         выведи(кук укз, цел size);
	
	дол                        дайСчёт() const             { return count; }
	
	Событие<дол>                 WhenPos;

	template <class F>
	проц уст(Поток& out_, F& фильтр) {
		иниц();
		out = &out_;
		фильтр.WhenOut = обрвыз(this, &ВыхФильтрПоток::выведи);
		фильтруй = обрвыз<F, F, const проц *, цел>(&фильтр, &F::помести);
		стоп = обрвыз(&фильтр, &F::стоп);
		count = 0;
	}
	
	ВыхФильтрПоток();
	template <class F> ВыхФильтрПоток(Поток& in, F& фильтр) { уст(in, фильтр); }
	~ВыхФильтрПоток();
};


class Хьюдж {
public:
	enum { КУСОК = 1024 * 1024 };

private:
	struct Блок {
		ббайт данные[КУСОК];
	};
	
	Массив<Блок>  данные;
	т_мера        size;

public:
	ббайт  *добавьКусок();
	проц   финиш(цел last_chunk_size);
	
	т_мера дайРазм() const                              { return size; }
	проц   дай(ук t, т_мера pos, т_мера sz) const;
	проц   дай(ук t)                                 { дай(t, 0, дайРазм()); }
	Ткст дай() const;
	
	Хьюдж();
};

struct ПроцессИнк : БезКопий {
public:
	virtual проц глуши() = 0;
	virtual бул пущен() = 0;
	virtual проц пиши(Ткст s) = 0;
	virtual бул читай(Ткст& s) = 0;
	virtual бул читай2(Ткст& os, Ткст& es) { НИКОГДА(); return false; }
	virtual цел  дайКодВыхода() = 0;
	virtual Ткст дайСообВыхода() { return Ткст(); };
	virtual проц закройЧтен()        {}
	virtual проц закройЗап()       {}
	virtual проц открепи()           {};
	
	Ткст  дай()                  { Ткст x; if(читай(x)) return x; return Ткст::дайПроц(); }

	ПроцессИнк() {}
	virtual ~ПроцессИнк() {}
};

class ЛокальнПроцесс : public ПроцессИнк {
public:
	virtual проц глуши();
	virtual бул пущен();
	virtual проц пиши(Ткст s);
	virtual бул читай(Ткст& s);
	virtual бул читай2(Ткст& os, Ткст &es);
	virtual Ткст дайСообВыхода();
	virtual цел  дайКодВыхода();
	virtual проц закройЧтен();
	virtual проц закройЗап();
	virtual проц открепи();

private:
	проц         иниц();
	проц         освободи();
#ifdef PLATFORM_POSIX
	бул         DecodeExitCode(цел код_);
#endif

private:
	бул         convertcharset;

#ifdef PLATFORM_WIN32
	HANDLE       hProcess;
	HANDLE       hOutputRead;
	HANDLE       hErrorRead;
	HANDLE       hInputWrite;
	DWORD        dwProcessId;
#endif
#ifdef PLATFORM_POSIX
	pid_t        pid;
	цел          rpipe[2], wpipe[2], epipe[2];
	Ткст       exit_string;
	бул         doublefork;
#endif
	цел          exit_code;
	Ткст       wreso, wrese; // Output fetched during пиши

	typedef ЛокальнПроцесс ИМЯ_КЛАССА;

	бул DoStart(кткст0 cmdline, const Вектор<Ткст> *арг, бул spliterr, кткст0 envptr, кткст0 cd);

public:
	бул старт(кткст0 cmdline, кткст0 envptr = NULL, кткст0 cd = NULL)                         { return DoStart(cmdline, NULL, false, envptr, cd); }
	бул Start2(кткст0 cmdline, кткст0 envptr = NULL, кткст0 cd = NULL)                        { return DoStart(cmdline, NULL, true, envptr, cd); }

	бул старт(кткст0 cmd, const Вектор<Ткст>& арг, кткст0 envptr = NULL, кткст0 cd = NULL)  { return DoStart(cmd, &арг, false, envptr, cd); }
	бул Start2(кткст0 cmd, const Вектор<Ткст>& арг, кткст0 envptr = NULL, кткст0 cd = NULL) { return DoStart(cmd, &арг, true, envptr, cd); }
	
#ifdef PLATFORM_POSIX
	ЛокальнПроцесс& DoubleFork(бул b = true)                           { doublefork = b; return *this; }

	цел  GetPid()  const                                              { return pid; }
#endif

#ifdef PLATFORM_WIN32
	HANDLE  GetProcessHandle()  const                                 { return hProcess; }
#endif

	цел  финиш(Ткст& out);
		
	ЛокальнПроцесс& преобразуйНабСим(бул b = true)                       { convertcharset = b; return *this; }
	ЛокальнПроцесс& NoConvertCharset()                                  { return преобразуйНабСим(false); }

	ЛокальнПроцесс()                                                                          { иниц(); }
	ЛокальнПроцесс(кткст0 cmdline, кткст0 envptr = NULL)                            { иниц(); старт(cmdline, envptr); }
	ЛокальнПроцесс(кткст0 cmdline, const Вектор<Ткст>& арг, кткст0 envptr = NULL) { иниц(); старт(cmdline, арг, envptr); }
	virtual ~ЛокальнПроцесс()                                                                 { глуши(); }
};



/////////////
#endif
