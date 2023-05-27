#ifndef _DinrusPro_TextCore_h_
#define _DinrusPro_TextCore_h_
#include <DinrusPro/DinrusPro.h>

/////////////////////////////
ПИ_РНЦП//Пространство имён//
///////////////////////////

template <class B>
class АТкст : public B {
	typedef typename B::tchar  tchar;
	typedef typename B::bchar  bchar;
	typedef typename B::Буфер буфер;
	typedef typename B::Ткст Ткст;

public:
	проц очисть()                                              { B::освободи(); B::обнули(); }
	цел  дайДлину() const                                    { return B::дайСчёт(); }
	бул пустой() const                                      { return B::дайСчёт() == 0; }

	const tchar *стоп() const                                  { return B::старт() + дайДлину(); }
	const tchar *end() const                                  { return стоп(); }
	const tchar *последний() const                                 { return стоп() - !!B::дайСчёт(); }
	const tchar *дайОбх(цел i) const                         { ПРОВЕРЬ(i >= 0 && i <= B::дайСчёт()); return B::старт() + i; }

	цел operator*() const                                     { return *B::старт(); }
	цел operator[](цел i) const                               { ПРОВЕРЬ(i >= 0 && i <= B::дайСчёт()); return B::старт()[i]; }

	operator const tchar *() const                            { return B::старт(); }
	const tchar *operator~() const                            { return B::старт(); }
	operator const bchar *() const                            { return (bchar *)B::старт(); }
	operator const проц *() const                             { return B::старт(); }

	проц вставь(цел pos, цел c)                               { *B::вставь(pos, 1, NULL) = c; }
	проц вставь(цел pos, const tchar *s, цел count)           { B::вставь(pos, count, s); }
	проц вставь(цел pos, const Ткст& s)                     { вставь(pos, s, s.дайСчёт()); }
	проц вставь(цел pos, кткст0 s);

	проц обрежьПоследн(цел count = 1)                              { B::обрежь(B::дайСчёт() - count); }

	проц  кат(цел c)                                          { B::кат(c); }
	проц  кат(const tchar *s, цел len)                        { B::кат(s, len); }
	проц  кат(const tchar *s);
	проц  кат(const Ткст& s)                                { кат(~s, s.дайДлину()); }
	проц  кат(цел c, цел count);
	проц  кат(const tchar *s, const tchar *lim)               { ПРОВЕРЬ(s <= lim); кат(s, цел(lim - s)); }
	проц  кат(const Ткст& s, цел len)                       { B::кат(~s, len); }
	проц  кат(const bchar *s, цел len)                        { кат((const tchar *) s, len); }

	Ткст& кат()                                             { return *(Ткст *)this; }

	цел    сравни(const Ткст& s) const                     { return B::сравни(s); }
	цел    сравни(const tchar *s) const;

	бул   равен(const Ткст& s) const                     { return B::равен(s); }
	бул   равен(const tchar *s) const                      { return B::равен(s); }

	Ткст середина(цел pos, цел length) const;
	Ткст середина(цел pos) const                                 { return середина(pos, дайДлину() - pos); }
	Ткст право(цел count) const                             { return середина(дайДлину() - count); }
	Ткст лево(цел count) const                              { return середина(0, count); }

	цел    найди(цел chr, цел from = 0) const;
	цел    найдирек(цел chr, цел from) const;
	цел    найдирек(цел chr) const;

	цел    найди(цел len, const tchar *s, цел from) const      { return найди(B::старт(), B::дайСчёт(), s, len, from); }
	цел    найди(const tchar *s, цел from = 0) const           { return найди(длинтекс__(s), s, from); }
	цел    найди(const Ткст& s, цел from = 0) const          { return найди(s.дайСчёт(), ~s, from); }

	цел    найдиПосле(const tchar *s, цел from = 0) const      { цел n = длинтекс__(s); цел q = найди(n, s, from); return q < 0 ? -1 : q + n; }
	цел    найдиПосле(const Ткст& s, цел from = 0) const     { цел n = s.дайСчёт(); цел q = найди(n, ~s, from); return q < 0 ? -1 : q + n; }

	цел    найдирек(цел len, const tchar *s, цел from) const;
	цел    найдирек(const tchar *s, цел from) const;
	цел    найдирек(const Ткст& s, цел from) const       { return найдирек(s.дайСчёт(), ~s, from); }
	цел    найдирек(const tchar *s) const                  { return дайДлину() ? найдирек(s, дайДлину()-1) : -1;}
	цел    найдирек(const Ткст& s) const                 { return дайДлину() ? найдирек(s, дайДлину()-1) : -1;}

	цел    найдирекПосле(цел len, const tchar *s, цел from) const;
	цел    найдирекПосле(const tchar *s, цел from) const;
	цел    найдирекПосле(const Ткст& s, цел from) const  { return найдирекПосле(s.дайСчёт(), ~s, from); }
	цел    найдирекПосле(const tchar *s) const             { return дайДлину() ? найдирекПосле(s, дайДлину()-1) : -1;}
	цел    найдирекПосле(const Ткст& s) const            { return дайДлину() ? найдирекПосле(s, дайДлину()-1) : -1;}

	проц   замени(const tchar *найди, цел findlen, const tchar *replace, цел replacelen);
	проц   замени(const Ткст& найди, const Ткст& replace);
	проц   замени(const tchar *найди, const tchar *replace);
	проц   замени(const Ткст& найди, const tchar *replace);
	проц   замени(const tchar *найди, const Ткст& replace);

	бул   начинаетсяС(const tchar *s, цел len) const;
	бул   начинаетсяС(const tchar *s) const;
	бул   начинаетсяС(const Ткст& s) const                  { return начинаетсяС(~s, s.дайДлину()); }

	бул   обрежьСтарт(const tchar *s, цел len)                 { if(!начинаетсяС(s, len)) return false; B::удали(0, len); return true; }
	бул   обрежьСтарт(const tchar *s)                          { return обрежьСтарт(s, длинтекс__(s)); }
	бул   обрежьСтарт(const Ткст& s)                         { return обрежьСтарт(~s, s.дайДлину()); }

	бул   заканчиваетсяНа(const tchar *s, цел len) const;
	бул   заканчиваетсяНа(const tchar *s) const;
	бул   заканчиваетсяНа(const Ткст& s) const                    { return заканчиваетсяНа(~s, s.дайДлину()); }

	бул   обрежьКонец(const tchar *s, цел len)                   { if(!заканчиваетсяНа(s, len)) return false; обрежьПоследн(len); return true; }
	бул   обрежьКонец(const tchar *s)                            { return обрежьКонец(s, длинтекс__(s)); }
	бул   обрежьКонец(const Ткст& s)                           { return обрежьКонец(~s, s.дайДлину()); }

	цел    найдиПервыйИз(цел len, const tchar *set, цел from = 0) const;
	цел    найдиПервыйИз(const tchar *set, цел from = 0) const  { return найдиПервыйИз(длинтекс__(set), set, from); }
	цел    найдиПервыйИз(const Ткст& set, цел from = 0) const { return найдиПервыйИз(set.дайСчёт(), ~set, from); }

	friend бул operator<(const Ткст& a, const Ткст& b)   { return a.сравни(b) < 0; }
	friend бул operator<(const Ткст& a, const tchar *b)    { return a.сравни(b) < 0; }
	friend бул operator<(const tchar *a, const Ткст& b)    { return b.сравни(a) > 0; }

	friend бул operator<=(const Ткст& a, const Ткст& b)  { return a.сравни(b) <= 0; }
	friend бул operator<=(const Ткст& a, const tchar *b)   { return a.сравни(b) <= 0; }
	friend бул operator<=(const tchar *a, const Ткст& b)   { return b.сравни(a) >= 0; }

	friend бул operator>(const Ткст& a, const Ткст& b)   { return a.сравни(b) > 0; }
	friend бул operator>(const Ткст& a, const tchar *b)    { return a.сравни(b) > 0; }
	friend бул operator>(const tchar *a, const Ткст& b)    { return b.сравни(a) < 0; }

	friend бул operator>=(const Ткст& a, const Ткст& b)   { return a.сравни(b) >= 0; }
	friend бул operator>=(const Ткст& a, const tchar *b)    { return a.сравни(b) >= 0; }
	friend бул operator>=(const tchar *a, const Ткст& b)    { return b.сравни(a) <= 0; }

	friend бул operator==(const Ткст& a, const Ткст& b)   { return a.равен(b); }
	friend бул operator!=(const Ткст& a, const Ткст& b)   { return !a.равен(b); }
	friend бул operator==(const Ткст& a, const tchar *b)    { return a.равен(b); }
	friend бул operator==(const tchar *a, const Ткст& b)    { return b.равен(a); }
	friend бул operator!=(const Ткст& a, const tchar *b)    { return !a.равен(b); }
	friend бул operator!=(const tchar *a, const Ткст& b)    { return !b.равен(a); }

	friend Ткст operator+(const Ткст& a, const Ткст& b)  { Ткст c(a); c += b; return c; }
	friend Ткст operator+(const Ткст& a, const tchar *b)   { Ткст c(a); c += b; return c; }
	friend Ткст operator+(const Ткст& a, tchar b)          { Ткст c(a); c += b; return c; }
	friend Ткст operator+(Ткст&& a, const Ткст& b)       { Ткст c(pick(a)); c += b; return c; }
	friend Ткст operator+(Ткст&& a, const tchar *b)        { Ткст c(pick(a)); c += b; return c; }
	friend Ткст operator+(Ткст&& a, tchar b)               { Ткст c(pick(a)); c += b; return c; }
	friend Ткст operator+(const tchar *a, const Ткст& b)   { Ткст c(a); c += b; return c; }
	friend Ткст operator+(tchar a, const Ткст& b)          { Ткст c(a, 1); c += b; return c; }

};

class Ткст0 : Движ<Ткст0> {
	enum { //
		KIND = 14,    // chr[KIND] is Ткст tier flag, 0 - small, 31 - medium, 32..254 ref alloc, 255 - read alloc from Реф
		SLEN = 15,    // chr[SLEN] stores the length of small tier strings (up to 14 bytes)
		LLEN = 2,     // chr[LLEN] stores the length of medium (< 32) and large tier strings
		SPECIAL = 13, // chr[SPECIAL]: stores Значение тип
	};
	enum { // chr[KIND] predefined values, if > MEDIUM, it is Rc тип; if <255, also stores alloc
		SMALL = 0, // SVO ending zero, also has to be 0 because of дайОсобый, also flag of small
	    MEDIUM = 31 // medium, укз points to 32 bytes block on heap
	};

	struct Rc {
		Атомар refcount;
		цел    alloc;

		char *дайУк() const  { return (char*)(this + 1); }
	};

	union {
		char   chr[16];
		ткст0 укз; // medium and large tier, in large tier (Rc *)укз - 1 points to Rc
		бцел *wptr;
		qword *qptr;
		бкрат   v[8];
		бцел  w[4];
		qword  q[2];
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

	char&  SLen()                { return chr[SLEN]; }
	char   SLen() const          { return chr[SLEN]; }
	бцел& LLen()                { return w[LLEN]; }
	бцел  LLen() const          { return w[LLEN]; }
	бул   смолл_ли() const       { return chr[KIND] == SMALL; }
	бул   IsLarge() const       { return chr[KIND] != SMALL; }
	бул   IsMedium() const      { return chr[KIND] == MEDIUM; }
	бул   реф_ли() const         { return (ббайт)chr[KIND] > MEDIUM; }
	Rc    *Реф() const           { return (Rc *)укз - 1; }
	бул   IsShared() const      { return Реф()->refcount != 1; }
	бул   шарссыл_ли() const   { return реф_ли() && IsShared(); }
	цел    LAlloc() const        { цел b = (ббайт)chr[KIND]; return b == 255 ? Реф()->alloc : b; }
	бцел  дРавноual(const Ткст0& s) const;

	проц LSet(const Ткст0& s);
	проц LFree();
	проц LCat(цел c);
	т_хэш дЗначХэша() const;

	проц разшарь();
	проц устДлинТ(цел l);

	char *Ук()                   { return смолл_ли() ? chr : укз; }
	char *Alloc_(цел count, char& kind);
	char *размести(цел count, char& kind);

	static Ткст0::Rc voidptr[2];

	проц разверни(Ткст0& b);

	// интерфейс к Значение
	static бцел StW(ббайт st)     { return MAKE4B(0, st, 0, 0); }
	проц SetSpecial0(ббайт st)     { w[3] = StW(st); }
	проц устОсобо(ббайт st)      { ПРОВЕРЬ(смолл_ли() && дайСчёт() == 0); SetSpecial0(st); }
	ббайт дайОсобый() const       { return (chr[SLEN] | chr[KIND]) == 0 ? chr[SPECIAL] : 0; }
	ббайт GetSt() const            { return chr[SPECIAL]; }
	бцел GetStW() const          { return w[3]; }
	бул особый_ли() const        { return !v[7] && v[6]; }
	бул особый_ли(ббайт st) const { return w[3] == StW(st); }

	friend class Ткст;
	friend class ТкстБуф;
	friend class Значение;
	friend class ТекстКтрл;

protected:
	проц обнули()                     { q[0] = q[1] = 0; Dsyn(); }
	проц устСмолл(const Ткст0& s) { q[0] = s.q[0]; q[1] = s.q[1]; }
	проц освободи()                     { if(IsLarge()) LFree(); }
	проц подбери0(Ткст0&& s) {
		устСмолл(s);
		s.обнули();
	}
	проц уст0(const Ткст0& s) {
		if(s.смолл_ли()) устСмолл(s); else LSet(s);
		Dsyn();
	}
	проц присвой(const Ткст0& s) {
		if(s.смолл_ли()) {
			освободи();
			устСмолл(s);
		}
		else
			if(this != &s) {
				освободи();
				LSet(s);
			}
		Dsyn();
	}
	проц  уст0(кткст0 s, цел len);
	проц  SetL(кткст0 s, цел len);
	char *вставь(цел pos, цел count, кткст0 str);

	typedef char         tchar;
	typedef ббайт         bchar;
	typedef ТкстБуф Буфер;
	//typedef ДинрусРНЦП::Ткст  Ткст;

	цел    сравниД(const Ткст0& s) const;

public:
	бул дРавно(const Ткст0& s) const;
	бул равен(const Ткст0& s) const {
		бдол q1 = q[1];
		бдол sq1 = s.q[1];
		return q1 == sq1 && q[0] == s.q[0] || ((q1 | sq1) & MAKE8B(0,0,0,0,0,0,255,0)) && дРавно(s);
	}
	бул равен(кткст0 s) const;

	цел    сравни(const Ткст0& s) const;

	т_хэш дайХэшЗнач() const {
#ifdef HASH64
		return chr[KIND] ? дЗначХэша() : (т_хэш)комбинируйХэш(q[0], q[1]);
#else
		return chr[KIND] ? дЗначХэша() : (т_хэш)комбинируйХэш(w[0], w[1], w[2], w[3]);
#endif
	}

	проц кат(цел c) {
		if(SLen() < 14)
			chr[цел(SLen()++)] = c;
		else
			LCat(c);
		Dsyn();
	}

	проц кат(кткст0 s, цел len);
	проц уст(кткст0 s, цел len);

	проц уст(цел i, цел chr);
	проц обрежь(цел pos);

	кткст0 старт() const   { return смолл_ли() ? chr : укз; }
	кткст0 begin() const   { return старт(); }
	кткст0 стоп() const     { return старт() + дайДлину(); }
	кткст0 end() const     { return стоп(); }

	цел operator[](цел i) const { ПРОВЕРЬ(i >= 0 && i <= дайСчёт()); return старт()[i]; }

	operator const char *() const   { return старт(); }
	кткст0 operator~() const   { return старт(); }

	проц удали(цел pos, цел count = 1);
	проц очисть()                { освободи(); обнули(); }

	цел дайСчёт() const        { return смолл_ли() ? chr[SLEN] : w[LLEN]; }
	цел дайДлину() const       { return дайСчёт(); }
	цел дайРазмест() const        { return смолл_ли() ? 14 : LAlloc(); }

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

	проц присвойДлин(кткст0 s, цел slen);

	enum SSPECIAL { SPECIAL };

	template <class T>
	Ткст(const T& x, ббайт st, SSPECIAL) {
		*(T*)chr = x;
		SetSpecial0(st);
	}
	Ткст(SSPECIAL) {}

	friend class Значение;

public:
	const Ткст& operator+=(char c)                       { кат(c); return *this; }
	const Ткст& operator+=(кткст0 s)                { кат(s); return *this; }
	const Ткст& operator+=(const Ткст& s)              { кат(s); return *this; }

	Ткст& operator=(кткст0 s);
	Ткст& operator=(const Ткст& s)                     { Ткст0::присвой(s); return *this; }
	Ткст& operator=(Ткст&& s)                          { if(this != &s) { освободи(); подбери0(pick(s)); } return *this; }
	Ткст& operator=(ТкстБуф& b)                     { *this = Ткст(b); return *this; }

	проц   сожми()                                        { *this = Ткст(старт(), дайДлину()); }
	цел    дайСчётСим() const;

	Ткст()                                               { обнули(); }
	Ткст(const Обнул&)                                  { обнули(); }
	Ткст(const Ткст& s)                                { Ткст0::уст0(s); }
	Ткст(Ткст&& s)                                     { Ткст0::подбери0(pick(s)); }
	Ткст(кткст0 s);
	Ткст(const Ткст& s, цел n)                         { ПРОВЕРЬ(n >= 0 && n <= s.дайДлину()); Ткст0::уст0(~s, n); }
	Ткст(кткст0 s, цел n)                           { Ткст0::уст0(s, n); }
	Ткст(const ббайт *s, цел n)                           { Ткст0::уст0((const char *)s, n); }
	Ткст(кткст0 s, кткст0 lim)                 { Ткст0::уст0(s, (цел)(lim - s)); }
	Ткст(цел chr, цел count)                             { Ткст0::обнули(); кат(chr, count); }
	Ткст(ТкстБуф& b);


	Ткст(char16 *s);
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

inline std::string to_string(const Ткст& s)              { return std::string(s.старт(), s.стоп()); }

class ТкстБуф : БезКопий {
	char   *pbegin;
	char   *pend;
	char   *limit;
	char    буфер[256];

	friend class Ткст;

	typedef Ткст0::Rc Rc;

	char *размести(цел len, цел& alloc);
	проц  переразмести(бцел n, кткст0 cat = NULL, цел l = 0);
	проц  переразместиД(кткст0 s, цел l);
	проц  расширь();
	проц  обнули()                    { pbegin = pend = буфер; limit = pbegin + 255; }
	проц  освободи();
	проц  уст(Ткст& s);

public:
	char *старт()                   { *pend = '\0'; return pbegin; }
	char *begin()                   { return старт(); }
	char *стоп()                     { *pend = '\0'; return pend; }
	char *end()                     { return стоп(); }

	char& operator*()               { return *старт(); }
	char& operator[](цел i)         { return старт()[i]; }
	operator char*()                { return старт(); }
	operator ббайт*()                { return (ббайт *)старт(); }
	operator проц*()                { return старт(); }
	char *operator~()               { return старт(); }

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

inline бул  пустой(const Ткст& s)      { return s.пустой(); }

Ткст фмтУк(const ук p);

template <class T>
inline Ткст какТкст(const T& x)
{
	return x.вТкст();
}

template <class T>
inline Ткст какТкст(T *x)
{
	return фмтУк(x);
}

inline Ткст& operator<<(Ткст& s, кткст0 x)
{
	s.кат(x, длинтекс__(x));
	return s;
}

inline Ткст& operator<<(Ткст& s, char *x)
{
	s.кат(x);
	return s;
}

inline Ткст& operator<<(Ткст& s, const Ткст &x)
{
	s.кат(x);
	return s;
}

inline Ткст& operator<<(Ткст& s, char x)
{
	s.кат((цел) x);
	return s;
}

inline Ткст& operator<<(Ткст& s, const ук x)
{
	s << фмтУк(x);
	return s;
}

template <class T>
inline Ткст& operator<<(Ткст& s, const T& x)
{
	s.кат(какТкст(x));
	return s;
}

template<>
inline Ткст& operator<<(Ткст& s, кткст0  const &x)
{
	s.кат(x);
	return s;
}

template<>
inline Ткст& operator<<(Ткст& s, const Ткст &x)
{
	s.кат(x);
	return s;
}

template<>
inline Ткст& operator<<(Ткст& s, const char& x)
{
	s.кат(x);
	return s;
}

inline Ткст& operator<<(Ткст&& s, кткст0 x)
{
	s.кат(x, длинтекс__(x));
	return s;
}

inline Ткст& operator<<(Ткст&& s, char *x)
{
	s.кат(x);
	return s;
}

inline Ткст& operator<<(Ткст&& s, const Ткст &x)
{
	s.кат(x);
	return s;
}

inline Ткст& operator<<(Ткст&& s, char x)
{
	s.кат((цел) x);
	return s;
}

inline Ткст& operator<<(Ткст&& s, const ук x)
{
	s << фмтУк(x);
	return s;
}

template <class T>
inline Ткст& operator<<(Ткст&& s, const T& x)
{
	s.кат(какТкст(x));
	return s;
}

template<>
inline Ткст& operator<<(Ткст&& s, кткст0  const &x)
{
	s.кат(x);
	return s;
}

template<>
inline Ткст& operator<<(Ткст&& s, const Ткст &x)
{
	s.кат(x);
	return s;
}

template<>
inline Ткст& operator<<(Ткст&& s, const char& x)
{
	s.кат(x);
	return s;
}

template<>
inline бул  пусто_ли(const Ткст& s)       { return s.пустой(); }

template<>
inline Ткст какТкст(const Ткст& s)     { return s; }

template<>
inline т_хэш дайХэшЗнач(const Ткст& s) { return s.дайХэшЗнач(); }

цел сравнилюб(const Ткст& a, const Ткст& b);
цел сравнилюб(const Ткст& a, кткст0 b);

inline
цел сравнилюб(кткст0 a, const Ткст& b) {
	return -сравнилюб(b, a);
}

Ткст обрежьЛево(const Ткст& s);
Ткст обрежьПраво(const Ткст& s);
Ткст обрежьОба(const Ткст& s);

Ткст обрежьЛево(кткст0 prefix, цел len, const Ткст& s);
force_inline Ткст обрежьЛево(кткст0 prefix, const Ткст& s)    { return обрежьЛево(prefix, (цел)strlen(prefix), s); }
force_inline Ткст обрежьЛево(const Ткст& prefix, const Ткст& s)  { return обрежьЛево(~prefix, prefix.дайСчёт(), s); }

Ткст обрежьПраво(кткст0 suffix, цел len, const Ткст& s);
force_inline Ткст обрежьПраво(кткст0 suffix, const Ткст& s)   { return обрежьПраво(suffix, (цел)strlen(suffix), s); }
force_inline Ткст обрежьПраво(const Ткст& suffix, const Ткст& s) { return обрежьПраво(~suffix, suffix.дайСчёт(), s); }

inline ТкстБуф& operator<<(ТкстБуф& s, кткст0 x)
{
	s.кат(x);
	return s;
}

inline ТкстБуф& operator<<(ТкстБуф& s, char *x)
{
	s.кат(x);
	return s;
}

inline ТкстБуф& operator<<(ТкстБуф& s, const Ткст &x)
{
	s.кат(x);
	return s;
}

inline ТкстБуф& operator<<(ТкстБуф& s, char x)
{
	s.кат((цел) x);
	return s;
}

inline ТкстБуф& operator<<(ТкстБуф& s, const ук x)
{
	s << фмтУк(x);
	return s;
}

inline ТкстБуф& operator<<(ТкстБуф& s, ук x)
{
	s << фмтУк(x);
	return s;
}

template <class T>
inline ТкстБуф& operator<<(ТкстБуф& s, const T& x)
{
	s.кат(какТкст(x));
	return s;
}

template<>
inline ТкстБуф& operator<<(ТкстБуф& s, кткст0  const &x)
{
	s.кат(x);
	return s;
}

template<>
inline ТкстБуф& operator<<(ТкстБуф& s, const Ткст &x)
{
	s.кат(x);
	return s;
}

template<>
inline ТкстБуф& operator<<(ТкстБуф& s, const char& x)
{
	s.кат(x);
	return s;
}

template <class T>
проц RawCat(Ткст& s, const T& x)
{
	s.кат((const char *)&x, sizeof(x));
}

template <class T>
проц RawCat(ТкстБуф& s, const T& x)
{
	s.кат((const char *)&x, sizeof(x));
}

class ШТкст0 {
	enum { SMALL = 23 };

	шим *укз;
	цел    length;
	цел    alloc;

#ifdef _ОТЛАДКА
	проц Dsyn();
#else
	проц Dsyn() {}
#endif

	static Атомар voidptr[2];

	бул    IsRc() const  { return alloc > SMALL; }
	Атомар& Rc()          { return *((Атомар *)укз - 1); }
	бул    IsShared()    { return IsRc() && Rc() > 1; }

	шим  *размести(цел& count);
	проц    LCat(цел c);
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
	проц    подбери0(ШТкст0&& s)          { укз = s.укз; length = s.length; alloc = s.alloc; s.обнули(); Dsyn(); }
	проц    освободи();
	проц    FFree()                      { if(alloc > 0) освободи(); }
	проц    разверни(ШТкст0& b)            { ДинрусРНЦП::разверни(укз, b.укз); ДинрусРНЦП::разверни(length, b.length); ДинрусРНЦП::разверни(alloc, b.alloc); Dsyn(); b.Dsyn(); }
	шим  *вставь(цел pos, цел count, const шим *данные);

public:
	const шим *старт() const           { return укз; }
	const шим *стоп() const             { return старт() + дайДлину(); }
	const шим *begin() const           { return старт(); }
	const шим *end() const             { return стоп(); }
	цел   operator[](цел i) const        { return укз[i]; }

	operator const шим *() const       { return старт(); }
	const шим *operator~() const       { return старт(); }

	проц кат(цел c)                      { if(!IsRc() && length < alloc) { укз[length++] = c; укз[length] = 0; } else LCat(c); Dsyn(); }
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

class ШТкст : public Движ<ШТкст, АТкст<ШТкст0> >
{
	проц разверни(ШТкст& b)                                   { ШТкст0::разверни(b); }

#ifdef _ОТЛАДКА
	цел          len;
	const шим *s;
	friend class ШТкст0;
#endif

public:
	РНЦП::Ткст вТкст() const;

	const ШТкст& operator+=(шим c)                      { кат(c); return *this; }
	const ШТкст& operator+=(const шим *s)               { кат(s); return *this; }
	const ШТкст& operator+=(const ШТкст& s)             { кат(s); return *this; }

	ШТкст& operator<<(шим c)                            { кат(c); return *this; }
	ШТкст& operator<<(const ШТкст& s)                   { кат(s); return *this; }
	ШТкст& operator<<(const шим *s)                     { кат(s); return *this; }

	ШТкст& operator=(const шим *s);
	ШТкст& operator=(const ШТкст& s)                    { if(this != &s) { ШТкст0::FFree(); ШТкст0::уст0(s); } return *this; }
	ШТкст& operator=(ШТкст&& s)                         { if(this != &s) { ШТкст0::FFree(); ШТкст0::подбери0(pick(s)); } return *this; }
	ШТкст& operator=(ШТкстБуф& b)                    { *this = ШТкст(b); return *this; }
	ШТкст& operator<<=(const ШТкст& s)                  { if(this != &s) { ШТкст0::освободи(); ШТкст0::уст0(s, s.дайСчёт()); } return *this; }

	проц   сожми()                                         { *this = ШТкст(старт(), дайДлину()); }

	ШТкст()                                               {}
	ШТкст(const Обнул&)                                  {}
	ШТкст(const ШТкст& s)                               { ШТкст0::уст0(s); }
	ШТкст(ШТкст&& s)                                    { ШТкст0::подбери0(pick(s)); }
	ШТкст(const шим *s)                                 { ШТкст0::уст0(s, длинтекс__(s)); }
	ШТкст(const ШТкст& s, цел n)                        { ПРОВЕРЬ(n >= 0 && n <= s.дайДлину()); ШТкст0::уст0(~s, n); }
	ШТкст(const шим *s, цел n)                          { ШТкст0::уст0(s, n); }
	ШТкст(const шим *s, const шим *lim)               { ШТкст0::уст0(s, (цел)(lim - s)); }
	ШТкст(цел chr, цел count)                             { ШТкст0::обнули(); кат(chr, count); }
	ШТкст(ШТкстБуф& b);

	ШТкст(кткст0 s);
	ШТкст(кткст0 s, цел n);
	ШТкст(кткст0 s, кткст0 lim);

	ШТкст(const char16 *s);

	static ШТкст дайПроц();
	бул   проц_ли() const                                   { return alloc < 0; }

	friend проц разверни(ШТкст& a, ШТкст& b)                { a.разверни(b); }
	friend ШТкст operator+(const ШТкст& a, char b)      { ШТкст c(a); c += b; return c; }
	friend ШТкст operator+(ШТкст&& a, char b)           { ШТкст c(pick(a)); c += b; return c; }
	friend ШТкст operator+(char a, const ШТкст& b)      { ШТкст c(a, 1); c += b; return c; }

#ifndef _HAVE_NO_STDWSTRING
	ШТкст(const std::wstring& s);
	operator std::wstring() const                           { return вСтд(); }
	std::wstring вСтд() const;
#endif
};

#ifndef _HAVE_NO_STDWSTRING
inline std::wstring to_string(const ШТкст& s)             { return s.вСтд(); }
#endif

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

inline бул  пустой(const ШТкст& s)      { return s.пустой(); }

template<>
inline бул  пусто_ли(const ШТкст& s)       { return s.пустой(); }

//template<>
//inline Ткст какТкст(const ШТкст& s)     { return s; }

template<>
inline т_хэш дайХэшЗнач(const ШТкст& s) { return хэшпам(~s, 2 * s.дайДлину()); }

ШТкст обрежьЛево(const ШТкст& str);
ШТкст обрежьПраво(const ШТкст& s);

цел сравнилюб(const ШТкст& a, const ШТкст& b);
цел сравнилюб(const ШТкст& a, const шим *b);

inline
цел сравнилюб(const шим *a, const ШТкст& b) {
	return -сравнилюб(b, a);
}

template<> inline Ткст какТкст(кткст0  const &s)    { return s; }
template<> inline Ткст какТкст(char * const &s)          { return s; }
template<> inline Ткст какТкст(кткст0 s)            { return s; }
template<> inline Ткст какТкст(char *s)                  { return s; }
template<> inline Ткст какТкст(const char& a)            { return Ткст(a, 1); }
template<> inline Ткст какТкст(const signed char& a)     { return Ткст(a, 1); }
template<> inline Ткст какТкст(const unsigned char& a)   { return Ткст(a, 1); }
template<> inline Ткст какТкст(const бул& a)            { return a ? "true" : "false"; }
template<> inline Ткст какТкст(const std::string& s)     { return Ткст(s); }
template<> inline Ткст какТкст(const std::wstring& s)    { return ШТкст(s).вТкст(); }

unsigned ctoi(цел c);

typedef цел (*CharFilter)(цел);

цел CharFilterAscii(цел c);
цел CharFilterAscii128(цел c);
цел CharFilterUnicode(цел c);
цел CharFilterDigit(цел c);
цел CharFilterWhitespace(цел c);
цел CharFilterNotWhitespace(цел c);
цел CharFilterAlpha(цел c);
цел CharFilterToUpper(цел c);
цел CharFilterToLower(цел c);
цел CharFilterToUpperAscii(цел c);
цел CharFilterAlphaToUpper(цел c);
цел CharFilterAlphaToLower(цел c);
цел CharFilterInt(цел c);
цел CharFilterDouble(цел c);
цел CharFilterDefaultToUpperAscii(цел c);
цел CharFilterCrLf(цел c);
цел CharFilterNoCrLf(цел c);

Ткст фильтруй(кткст0 s, цел (*filter)(цел));
Ткст FilterWhile(кткст0 s, цел (*filter)(цел));

ШТкст фильтруй(const шим *s, цел (*filter)(цел));
ШТкст FilterWhile(const шим *s, цел (*filter)(цел));

//#include "AString.hpp"
template <class B>
force_inline
проц АТкст<B>::вставь(цел pos, кткст0 s)
{
	вставь(pos, s, длинтекс__(s));
}

template <class B>
проц АТкст<B>::кат(цел c, цел count)
{
	tchar *s = B::вставь(дайДлину(), count, NULL);
	while(count--)
		*s++ = c;
}

template <class B>
force_inline
проц АТкст<B>::кат(const tchar *s)
{
	кат(s, длинтекс__(s));
}

template <class B>
цел АТкст<B>::сравни(const tchar *b) const
{
	const tchar *a = B::старт();
	const tchar *ae = стоп();
	for(;;) {
		if(a >= ae)
			return *b == 0 ? 0 : -1;
		if(*b == 0)
			return 1;
		цел q = сравнзн__(*a++) - сравнзн__(*b++);
		if(q)
			return q;
	}
}

template <class B>
typename АТкст<B>::Ткст АТкст<B>::середина(цел from, цел count) const
{
	цел l = дайДлину();
	if(from > l) from = l;
	if(from < 0) from = 0;
	if(count < 0)
		count = 0;
	if(from + count > l)
		count = l - from;
	return Ткст(B::старт() + from, count);
}

template <class B>
цел АТкст<B>::найди(цел chr, цел from) const
{
	ПРОВЕРЬ(from >= 0 && from <= дайДлину());
	const tchar *e = стоп();
	const tchar *укз = B::старт();
	for(const tchar *s = укз + from; s < e; s++)
		if(*s == chr)
			return (цел)(s - укз);
	return -1;
}

template <class B>
цел АТкст<B>::найдирек(цел chr, цел from) const
{
	ПРОВЕРЬ(from >= 0 && from <= дайДлину());
	if(from < дайДлину()) {
		const tchar *укз = B::старт();
		for(const tchar *s = укз + from; s >= укз; s--)
			if(*s == chr)
				return (цел)(s - укз);
	}
	return -1;
}

template <class B>
цел АТкст<B>::найдирек(цел len, const tchar *s, цел from) const
{
	ПРОВЕРЬ(from >= 0 && from <= дайДлину());
	if(from < дайДлину()) {
		const tchar *укз = B::старт();
		const tchar *p = укз + from - len + 1;
		len *= sizeof(tchar);
		while(p >= укз) {
			if(*s == *p && memcmp(s, p, len) == 0)
				return (цел)(p - укз);
			p--;
		}
	}
	return -1;
}

template <class B>
цел АТкст<B>::найдирекПосле(цел len, const tchar *s, цел from) const
{
	цел q = найдирек(len, s, from);
	return q >= 0 ? q + len : -1;
}

template <class B>
проц АТкст<B>::замени(const tchar *найди, цел findlen, const tchar *replace, цел replacelen)
{
	if(findlen == 0)
		return;
	Ткст r;
	цел i = 0;
	const tchar *p = B::старт();
	for(;;) {
		цел j = найди(findlen, найди, i);
		if(j < 0)
			break;
		r.кат(p + i, j - i);
		r.кат(replace, replacelen);
		i = j + findlen;
	}
	r.кат(p + i, B::дайСчёт() - i);
	B::освободи();
	B::уст0(r);
}

template <class B>
цел АТкст<B>::найдирек(const tchar *s, цел from) const
{
	return найдирек(длинтекс__(s), s, from);
}

template <class B>
цел АТкст<B>::найдирекПосле(const tchar *s, цел from) const
{
	return найдирекПосле(длинтекс__(s), s, from);
}

template <class B>
цел АТкст<B>::найдирек(цел chr) const
{
	return B::дайСчёт() ? найдирек(chr, B::дайСчёт() - 1) : -1;
}

template <class B>
проц АТкст<B>::замени(const Ткст& найди, const Ткст& replace)
{
	замени(~найди, найди.дайСчёт(), ~replace, replace.дайСчёт());
}

template <class B>
force_inline
проц АТкст<B>::замени(const tchar *найди, const tchar *replace)
{
	замени(найди, (цел)длинтекс__(найди), replace, (цел)длинтекс__(replace));
}

template <class B>
force_inline
проц АТкст<B>::замени(const Ткст& найди, const tchar *replace)
{
	замени(~найди, найди.дайСчёт(), replace, (цел)длинтекс__(replace));
}

template <class B>
force_inline
проц АТкст<B>::замени(const tchar *найди, const Ткст& replace)
{
	замени(найди, (цел)длинтекс__(найди), ~replace, replace.дайСчёт());
}

template <class B>
бул АТкст<B>::начинаетсяС(const tchar *s, цел len) const
{
	if(len > дайДлину()) return false;
	return memcmp(s, B::старт(), len * sizeof(tchar)) == 0;
}

template <class B>
force_inline
бул АТкст<B>::начинаетсяС(const tchar *s) const
{
	return начинаетсяС(s, длинтекс__(s));
}

template <class B>
бул АТкст<B>::заканчиваетсяНа(const tchar *s, цел len) const
{
	цел l = дайДлину();
	if(len > l) return false;
	return memcmp(s, B::старт() + l - len, len * sizeof(tchar)) == 0;
}

template <class B>
force_inline
бул АТкст<B>::заканчиваетсяНа(const tchar *s) const
{
	return заканчиваетсяНа(s, длинтекс__(s));
}

template <class B>
цел АТкст<B>::найдиПервыйИз(цел len, const tchar *s, цел from) const
{
	ПРОВЕРЬ(from >= 0 && from <= дайДлину());
	const tchar *укз = B::старт();
	const tchar *e = B::стоп();
	const tchar *se = s + len;
	if(len == 1) {
		tchar c1 = s[0];
		for(const tchar *bs = укз + from; bs < e; bs++) {
			if(*bs == c1)
				return (цел)(bs - укз);
		}
		return -1;
	}
	if(len == 2) {
		tchar c1 = s[0];
		tchar c2 = s[1];
		for(const tchar *bs = укз + from; bs < e; bs++) {
			tchar ch = *bs;
			if(ch == c1 || ch == c2)
				return (цел)(bs - укз);
		}
		return -1;
	}
	if(len == 3) {
		tchar c1 = s[0];
		tchar c2 = s[1];
		tchar c3 = s[2];
		for(const tchar *bs = укз + from; bs < e; bs++) {
			tchar ch = *bs;
			if(ch == c1 || ch == c2 || ch == c3)
				return (цел)(bs - укз);
		}
		return -1;
	}
	if(len == 4) {
		tchar c1 = s[0];
		tchar c2 = s[1];
		tchar c3 = s[2];
		tchar c4 = s[3];
		for(const tchar *bs = укз + from; bs < e; bs++) {
			tchar ch = *bs;
			if(ch == c1 || ch == c2 || ch == c3 || ch == c4)
				return (цел)(bs - укз);
		}
		return -1;
	}
	for(const tchar *bs = укз + from; bs < e; bs++)
		for(const tchar *ss = s; ss < se; ss++)
			if(*bs == *ss)
				return (цел)(bs - укз);
	return -1;
}

force_inline
проц Ткст0::уст(кткст0 s, цел len)
{
	очисть();
	if(len <= 14) {
		копирпам8(chr, s, len);
		SLen() = len;
		Dsyn();
		return;
	}
	SetL(s, len);
	Dsyn();
}

force_inline
проц Ткст0::уст0(кткст0 s, цел len)
{
	обнули();
	if(len <= 14) {
		SLen() = len;
		копирпам8(chr, s, len);
		Dsyn();
		return;
	}
	SetL(s, len);
	Dsyn();
}

inline
бул Ткст0::равен(кткст0 s) const
{ // This is optimized for comparison with string literals...
	т_мера len = strlen(s);
	const ук p;
	if(смолл_ли()) {
		if(len > 14 || len != (т_мера)chr[SLEN]) return false; // len > 14 fixes issue with GCC warning, might improves performance too
		p = chr;
	}
	else {
		if(len != (т_мера)w[LLEN]) return false;
		p = укз;
	}
	return memcmp(p, s, len) == 0; // compiler is happy to optimize memcmp out with up to 64bit comparisons for literals...
}

inline
цел Ткст0::сравни(const Ткст0& s) const
{
#ifdef CPU_LE
	if((chr[KIND] | s.chr[KIND]) == 0) { // both strings are small, len is the MSB
	#ifdef CPU_64
		бдол a = эндианРазворот64(q[0]);
		бдол b = эндианРазворот64(s.q[0]);
		if(a != b)
			return a < b ? -1 : 1;
		a = эндианРазворот64(q[1]);
		b = эндианРазворот64(s.q[1]);
		return a == b ? 0 : a < b ? -1 : 1;
	#else
		бдол a = эндианРазворот32(w[0]);
		бдол b = эндианРазворот32(s.w[0]);
		if(a != b)
			return a < b ? -1 : 1;
		a = эндианРазворот32(w[1]);
		b = эндианРазворот32(s.w[1]);
		if(a != b)
			return a < b ? -1 : 1;
		a = эндианРазворот32(w[2]);
		b = эндианРазворот32(s.w[2]);
		if(a != b)
			return a < b ? -1 : 1;
		a = эндианРазворот32(w[3]);
		b = эндианРазворот32(s.w[3]);
		return a == b ? 0 : a < b ? -1 : 1;
	#endif
	}
#endif
	return сравниД(s);
}

force_inline
Ткст& Ткст::operator=(кткст0 s)
{
	присвойДлин(s, длинтекс__(s));
	return *this;
}

force_inline
Ткст::Ткст(кткст0 s)
{
	Ткст0::уст0(s, длинтекс__(s));
}

force_inline
проц Ткст0::разверни(Ткст0& b)
{
	qword a0 = q[0]; // Explicit код_ (instead of разверни) seems to emit some SSE2 код_ with CLANG
	qword a1 = q[1];
	qword b0 = b.q[0];
	qword b1 = b.q[1];
	q[0] = b0;
	q[1] = b1;
	b.q[0] = a0;
	b.q[1] = a1;
	Dsyn(); b.Dsyn();
}

template <class Делец>
inline
Ткст Ткст::сделай(цел alloc, Делец m)
{
	Ткст s;
	if(alloc <= 14) {
		цел len = m(s.chr);
		ПРОВЕРЬ(len <= alloc);
		s.SLen() = len;
		s.Dsyn();
	}
	else {
		if(alloc < 32) {
			s.chr[KIND] = MEDIUM;
			s.укз = (char *)разместиПам32();
		}
		else
			s.укз = s.размести(alloc, s.chr[KIND]);
		цел len = m(s.укз);
		ПРОВЕРЬ(len <= alloc);
		s.укз[len] = 0;
		s.LLen() = len;
		s.SLen() = 15;
		if(alloc >= 32 && alloc > 2 * len)
			s.сожми();
	}
	return s;
}

force_inline
проц ТкстБуф::длинтекс()
{
	устДлину((цел)длинтекс__(pbegin));
}

inline
проц ТкстБуф::кат(кткст0 s, цел l)
{
	if(pend + l > limit)
		переразместиД(s, l);
	else {
		копирпам8(pend, s, l);
		pend += l;
	}
}

inline
проц ТкстБуф::кат(цел c, цел l)
{
	if(pend + l > limit)
		переразместиД(NULL, l);
	устпам8(pend, c, l);
	pend += l;
}

force_inline
проц ТкстБуф::кат(кткст0 s)
{
	кат(s, (цел)длинтекс__(s));
}



////////////////
КОНЕЦ_ПИ_РНЦП//
//////////////

#endif