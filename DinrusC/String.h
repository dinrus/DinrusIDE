class Обнул;

inline int strlen8(const char *s) { return s ? (int)strlen(s) : 0; }
int strlen16(const char16 *s);
int strlen32(const wchar *s);

#ifdef PLATFORM_POSIX
inline int stricmp(const char *a, const char *b)         { return strcasecmp(a, b); }
inline int strnicmp(const char *a, const char *b, int n) { return strncasecmp(a, b, n); }
#endif

#ifdef PLATFORM_WINCE
inline int stricmp(const char *a, const char *b)         { return _stricmp(a, b); }
inline int strnicmp(const char *a, const char *b, int n) { return _strnicmp(a, b, n); }
#endif

force_inline int strlen__(const char *s)  { return strlen8(s); }
inline int strlen__(const wchar *s)       { return strlen32(s); }

inline int cmpval__(char x)               { return (byte)x; }
inline int cmpval__(wchar x)              { return x; }

int find(const char *text, int len, const char *needle, int nlen, int from);
int find(const wchar *text, int len, const wchar *needle, int nlen, int from);

class Ткст;
class ШТкст;
class ТкстБуф;
class ШТкстБуф;

class Поток;

template <class B>
class АТкст : public B {
	typedef typename B::tchar  tchar;
	typedef typename B::bchar  bchar;
	typedef typename B::Буфер буфер;
	typedef typename B::Ткст Ткст;

public:
	void очисть()                                              { B::освободи(); B::обнули(); }
	int  дайДлину() const                                    { return B::дайСчёт(); }
	bool пустой() const                                      { return B::дайСчёт() == 0; }

	const tchar *стоп() const                                  { return B::старт() + дайДлину(); }
	const tchar *end() const                                  { return стоп(); }
	const tchar *последний() const                                 { return стоп() - !!B::дайСчёт(); }
	const tchar *дайОбх(int i) const                         { ПРОВЕРЬ(i >= 0 && i <= B::дайСчёт()); return B::старт() + i; }

	int operator*() const                                     { return *B::старт(); }
	int operator[](int i) const                               { ПРОВЕРЬ(i >= 0 && i <= B::дайСчёт()); return B::старт()[i]; }

	operator const tchar *() const                            { return B::старт(); }
	const tchar *operator~() const                            { return B::старт(); }
	operator const bchar *() const                            { return (bchar *)B::старт(); }
	operator const void *() const                             { return B::старт(); }

	void вставь(int pos, int c)                               { *B::вставь(pos, 1, NULL) = c; }
	void вставь(int pos, const tchar *s, int count)           { B::вставь(pos, count, s); }
	void вставь(int pos, const Ткст& s)                     { вставь(pos, s, s.дайСчёт()); }
	void вставь(int pos, const char *s);
	
	void обрежьПоследн(int count = 1)                              { B::обрежь(B::дайСчёт() - count); }

	void  конкат(int c)                                          { B::конкат(c); }
	void  конкат(const tchar *s, int len)                        { B::конкат(s, len); }
	void  конкат(const tchar *s);
	void  конкат(const Ткст& s)                                { конкат(~s, s.дайДлину()); }
	void  конкат(int c, int count);
	void  конкат(const tchar *s, const tchar *lim)               { ПРОВЕРЬ(s <= lim); конкат(s, int(lim - s)); }
	void  конкат(const Ткст& s, int len)                       { B::конкат(~s, len); }
	void  конкат(const bchar *s, int len)                        { конкат((const tchar *) s, len); }

	Ткст& конкат()                                             { return *(Ткст *)this; }

	int    сравни(const Ткст& s) const                     { return B::сравни(s); }
	int    сравни(const tchar *s) const;

	bool   равен(const Ткст& s) const                     { return B::равен(s); }
	bool   равен(const tchar *s) const                      { return B::равен(s); }

	Ткст середина(int pos, int length) const;
	Ткст середина(int pos) const                                 { return середина(pos, дайДлину() - pos); }
	Ткст право(int count) const                             { return середина(дайДлину() - count); }
	Ткст лево(int count) const                              { return середина(0, count); }

	int    найди(int chr, int from = 0) const;
	int    найдирек(int chr, int from) const;
	int    найдирек(int chr) const;

	int    найди(int len, const tchar *s, int from) const      { return find(B::старт(), B::дайСчёт(), s, len, from); }
	int    найди(const tchar *s, int from = 0) const           { return найди(strlen__(s), s, from); }
	int    найди(const Ткст& s, int from = 0) const          { return найди(s.дайСчёт(), ~s, from); }

	int    найдиПосле(const tchar *s, int from = 0) const      { int n = strlen__(s); int q = найди(n, s, from); return q < 0 ? -1 : q + n; }
	int    найдиПосле(const Ткст& s, int from = 0) const     { int n = s.дайСчёт(); int q = найди(n, ~s, from); return q < 0 ? -1 : q + n; }
	
	int    найдирек(int len, const tchar *s, int from) const;
	int    найдирек(const tchar *s, int from) const;
	int    найдирек(const Ткст& s, int from) const       { return найдирек(s.дайСчёт(), ~s, from); }
	int    найдирек(const tchar *s) const                  { return дайДлину() ? найдирек(s, дайДлину()-1) : -1;}
	int    найдирек(const Ткст& s) const                 { return дайДлину() ? найдирек(s, дайДлину()-1) : -1;}

	int    найдирекПосле(int len, const tchar *s, int from) const;
	int    найдирекПосле(const tchar *s, int from) const;
	int    найдирекПосле(const Ткст& s, int from) const  { return найдирекПосле(s.дайСчёт(), ~s, from); }
	int    найдирекПосле(const tchar *s) const             { return дайДлину() ? найдирекПосле(s, дайДлину()-1) : -1;}
	int    найдирекПосле(const Ткст& s) const            { return дайДлину() ? найдирекПосле(s, дайДлину()-1) : -1;}
	
	void   замени(const tchar *find, int findlen, const tchar *replace, int replacelen);
	void   замени(const Ткст& find, const Ткст& replace);
	void   замени(const tchar *find, const tchar *replace);
	void   замени(const Ткст& find, const tchar *replace);
	void   замени(const tchar *find, const Ткст& replace);
	
	bool   начинаетсяС(const tchar *s, int len) const;
	bool   начинаетсяС(const tchar *s) const;
	bool   начинаетсяС(const Ткст& s) const                  { return начинаетсяС(~s, s.дайДлину()); }

	bool   обрежьСтарт(const tchar *s, int len)                 { if(!начинаетсяС(s, len)) return false; B::удали(0, len); return true; }
	bool   обрежьСтарт(const tchar *s)                          { return обрежьСтарт(s, strlen__(s)); }
	bool   обрежьСтарт(const Ткст& s)                         { return обрежьСтарт(~s, s.дайДлину()); }

	bool   заканчиваетсяНа(const tchar *s, int len) const;
	bool   заканчиваетсяНа(const tchar *s) const;
	bool   заканчиваетсяНа(const Ткст& s) const                    { return заканчиваетсяНа(~s, s.дайДлину()); }
	
	bool   обрежьКонец(const tchar *s, int len)                   { if(!заканчиваетсяНа(s, len)) return false; обрежьПоследн(len); return true; }
	bool   обрежьКонец(const tchar *s)                            { return обрежьКонец(s, strlen__(s)); }
	bool   обрежьКонец(const Ткст& s)                           { return обрежьКонец(~s, s.дайДлину()); }

	int    найдиПервыйИз(int len, const tchar *set, int from = 0) const;
	int    найдиПервыйИз(const tchar *set, int from = 0) const  { return найдиПервыйИз(strlen__(set), set, from); }
	int    найдиПервыйИз(const Ткст& set, int from = 0) const { return найдиПервыйИз(set.дайСчёт(), ~set, from); }
	
	friend bool operator<(const Ткст& a, const Ткст& b)   { return a.сравни(b) < 0; }
	friend bool operator<(const Ткст& a, const tchar *b)    { return a.сравни(b) < 0; }
	friend bool operator<(const tchar *a, const Ткст& b)    { return b.сравни(a) > 0; }

	friend bool operator<=(const Ткст& a, const Ткст& b)  { return a.сравни(b) <= 0; }
	friend bool operator<=(const Ткст& a, const tchar *b)   { return a.сравни(b) <= 0; }
	friend bool operator<=(const tchar *a, const Ткст& b)   { return b.сравни(a) >= 0; }

	friend bool operator>(const Ткст& a, const Ткст& b)   { return a.сравни(b) > 0; }
	friend bool operator>(const Ткст& a, const tchar *b)    { return a.сравни(b) > 0; }
	friend bool operator>(const tchar *a, const Ткст& b)    { return b.сравни(a) < 0; }

	friend bool operator>=(const Ткст& a, const Ткст& b)   { return a.сравни(b) >= 0; }
	friend bool operator>=(const Ткст& a, const tchar *b)    { return a.сравни(b) >= 0; }
	friend bool operator>=(const tchar *a, const Ткст& b)    { return b.сравни(a) <= 0; }

	friend bool operator==(const Ткст& a, const Ткст& b)   { return a.равен(b); }
	friend bool operator!=(const Ткст& a, const Ткст& b)   { return !a.равен(b); }
	friend bool operator==(const Ткст& a, const tchar *b)    { return a.равен(b); }
	friend bool operator==(const tchar *a, const Ткст& b)    { return b.равен(a); }
	friend bool operator!=(const Ткст& a, const tchar *b)    { return !a.равен(b); }
	friend bool operator!=(const tchar *a, const Ткст& b)    { return !b.равен(a); }

	friend Ткст operator+(const Ткст& a, const Ткст& b)  { Ткст c(a); c += b; return c; }
	friend Ткст operator+(const Ткст& a, const tchar *b)   { Ткст c(a); c += b; return c; }
	friend Ткст operator+(const Ткст& a, tchar b)          { Ткст c(a); c += b; return c; }
	friend Ткст operator+(Ткст&& a, const Ткст& b)       { Ткст c(pick(a)); c += b; return c; }
	friend Ткст operator+(Ткст&& a, const tchar *b)        { Ткст c(pick(a)); c += b; return c; }
	friend Ткст operator+(Ткст&& a, tchar b)               { Ткст c(pick(a)); c += b; return c; }
	friend Ткст operator+(const tchar *a, const Ткст& b)   { Ткст c(a); c += b; return c; }
	friend Ткст operator+(tchar a, const Ткст& b)          { Ткст c(a, 1); c += b; return c; }
};

class Ткст0 : Движимое<Ткст0> {
	enum { // 
		KIND = 14,    // chr[KIND] is Ткст tier flag, 0 - small, 31 - medium, 32..254 ref alloc, 255 - read alloc from Реф
		SLEN = 15,    // chr[SLEN] stores the length of small tier strings (up to 14 bytes)
		LLEN = 2,     // chr[LLEN] stores the length of medium (< 32) and large tier strings
		SPECIAL = 13, // chr[SPECIAL]: stores Значение тип
	};
	enum { // chr[KIND] predefined values, if > MEDIUM, it is Rc тип; if <255, also stores alloc
		SMALL = 0, // SVO ending zero, also has to be 0 because of дайОсобый, also flag of small
	    MEDIUM = 31 // medium, ptr points to 32 bytes block on heap
	};

	struct Rc {
		Атомар refcount;
		int    alloc;

		char *дайУк() const  { return (char*)(this + 1); }
	};

	union {
		char   chr[16];
		char  *ptr; // medium and large tier, in large tier (Rc *)ptr - 1 points to Rc
		dword *wptr;
		qword *qptr;
		word   v[8];
		dword  w[4];
		qword  q[2];
	};


#if defined(_ОТЛАДКА) && defined(COMPILER_GCC)
	int          len;
	const char  *s;
#endif

#ifdef _ОТЛАДКА
	void Dsyn();
#else
	void Dsyn() {}
#endif

	char&  SLen()                { return chr[SLEN]; }
	char   SLen() const          { return chr[SLEN]; }
	dword& LLen()                { return w[LLEN]; }
	dword  LLen() const          { return w[LLEN]; }
	bool   смолл_ли() const       { return chr[KIND] == SMALL; }
	bool   IsLarge() const       { return chr[KIND] != SMALL; }
	bool   IsMedium() const      { return chr[KIND] == MEDIUM; }
	bool   реф_ли() const         { return (byte)chr[KIND] > MEDIUM; }
	Rc    *Реф() const           { return (Rc *)ptr - 1; }
	bool   IsShared() const      { return Реф()->refcount != 1; }
	bool   IsSharedRef() const   { return реф_ли() && IsShared(); }
	int    LAlloc() const        { int b = (byte)chr[KIND]; return b == 255 ? Реф()->alloc : b; }
	dword  LEqual(const Ткст0& s) const;

	void LSet(const Ткст0& s);
	void LFree();
	void LCat(int c);
	hash_t LHashValue() const;

	void UnShare();
	void SetSLen(int l);

	char *Ук()                   { return смолл_ли() ? chr : ptr; }
	char *Alloc_(int count, char& kind);
	char *размести(int count, char& kind);

	static Ткст0::Rc voidptr[2];

	void разверни(Ткст0& b);
	
	// interface for Значение
	static dword StW(byte st)     { return MAKE4B(0, st, 0, 0); }
	void SetSpecial0(byte st)     { w[3] = StW(st); }
	void устОсобо(byte st)      { ПРОВЕРЬ(смолл_ли() && дайСчёт() == 0); SetSpecial0(st); }
	byte дайОсобый() const       { return (chr[SLEN] | chr[KIND]) == 0 ? chr[SPECIAL] : 0; }
	byte GetSt() const            { return chr[SPECIAL]; }
	dword GetStW() const          { return w[3]; }
	bool особый_ли() const        { return !v[7] && v[6]; }
	bool особый_ли(byte st) const { return w[3] == StW(st); }
	
	friend class Ткст;
	friend class ТкстБуф;
	friend class Значение;
	friend class ТекстКтрл;

protected:
	void обнули()                     { q[0] = q[1] = 0; Dsyn(); }
	void устСмолл(const Ткст0& s) { q[0] = s.q[0]; q[1] = s.q[1]; }
	void освободи()                     { if(IsLarge()) LFree(); }
	void подбери0(Ткст0&& s) {
		устСмолл(s);
		s.обнули();
	}
	void уст0(const Ткст0& s) {
		if(s.смолл_ли()) устСмолл(s); else LSet(s);
		Dsyn();
	}
	void присвой(const Ткст0& s) {
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
	void  уст0(const char *s, int len);
	void  SetL(const char *s, int len);
	char *вставь(int pos, int count, const char *str);

	typedef char         tchar;
	typedef byte         bchar;
	typedef ТкстБуф Буфер;
	typedef РНЦПДинрус::Ткст  Ткст;

	int    CompareL(const Ткст0& s) const;

public:
	bool LEq(const Ткст0& s) const;
	bool равен(const Ткст0& s) const {
		uint64 q1 = q[1];
		uint64 sq1 = s.q[1];
		return q1 == sq1 && q[0] == s.q[0] || ((q1 | sq1) & MAKE8B(0,0,0,0,0,0,255,0)) && LEq(s);
	}
	bool равен(const char *s) const;

	int    сравни(const Ткст0& s) const;

	hash_t дайХэшЗнач() const {
#ifdef HASH64
		return chr[KIND] ? LHashValue() : (hash_t)комбинируйХэш(q[0], q[1]);
#else
		return chr[KIND] ? LHashValue() : (hash_t)комбинируйХэш(w[0], w[1], w[2], w[3]);
#endif
	}

	void конкат(int c) {
		if(SLen() < 14)
			chr[int(SLen()++)] = c;
		else
			LCat(c);
		Dsyn();
	}

	void конкат(const char *s, int len);
	void уст(const char *s, int len);

	void уст(int i, int chr);
	void обрежь(int pos);

	const char *старт() const   { return смолл_ли() ? chr : ptr; }
	const char *begin() const   { return старт(); }
	const char *стоп() const     { return старт() + дайДлину(); }
	const char *end() const     { return стоп(); }

	int operator[](int i) const { ПРОВЕРЬ(i >= 0 && i <= дайСчёт()); return старт()[i]; }

	operator const char *() const   { return старт(); }
	const char *operator~() const   { return старт(); }

	void удали(int pos, int count = 1);
	void очисть()                { освободи(); обнули(); }

	int дайСчёт() const        { return смолл_ли() ? chr[SLEN] : w[LLEN]; }
	int дайДлину() const       { return дайСчёт(); }
	int дайРазмест() const        { return смолл_ли() ? 14 : LAlloc(); }

	void резервируй(int r);
	
	Ткст0()                   {}
	~Ткст0()                  { освободи(); }
};

class Ткст : public Движимое<Ткст, АТкст<Ткст0> > {
	void разверни(Ткст& b)                                   { Ткст0::разверни(b); }

#ifdef _ОТЛАДКА
#ifndef COMPILER_GCC
	int          len;
	const char  *s;
#endif
	friend class Ткст0;
#endif

	void присвойДлин(const char *s, int slen);
	
	enum SSPECIAL { SPECIAL };
	
	template <class T>
	Ткст(const T& x, byte st, SSPECIAL) {
		*(T*)chr = x;
		SetSpecial0(st);
	}
	Ткст(SSPECIAL) {}

	friend class Значение;
	
public:
	const Ткст& operator+=(char c)                       { конкат(c); return *this; }
	const Ткст& operator+=(const char *s)                { конкат(s); return *this; }
	const Ткст& operator+=(const Ткст& s)              { конкат(s); return *this; }

	Ткст& operator=(const char *s);
	Ткст& operator=(const Ткст& s)                     { Ткст0::присвой(s); return *this; }
	Ткст& operator=(Ткст&& s)                          { if(this != &s) { освободи(); подбери0(pick(s)); } return *this; }
	Ткст& operator=(ТкстБуф& b)                     { *this = Ткст(b); return *this; }

	void   сожми()                                        { *this = Ткст(старт(), дайДлину()); }
	int    дайСчётСим() const;

	Ткст()                                               { обнули(); }
	Ткст(const Обнул&)                                  { обнули(); }
	Ткст(const Ткст& s)                                { Ткст0::уст0(s); }
	Ткст(Ткст&& s)                                     { Ткст0::подбери0(pick(s)); }
	Ткст(const char *s);
	Ткст(const Ткст& s, int n)                         { ПРОВЕРЬ(n >= 0 && n <= s.дайДлину()); Ткст0::уст0(~s, n); }
	Ткст(const char *s, int n)                           { Ткст0::уст0(s, n); }
	Ткст(const byte *s, int n)                           { Ткст0::уст0((const char *)s, n); }
	Ткст(const char *s, const char *lim)                 { Ткст0::уст0(s, (int)(lim - s)); }
	Ткст(int chr, int count)                             { Ткст0::обнули(); конкат(chr, count); }
	Ткст(ТкстБуф& b);


	Ткст(char16 *s);
	Ткст(wchar *s);

	ШТкст вШТкст() const;
	const Ткст& вТкст() const                         { return *this; }

	static Ткст дайПроц();
	bool   проц_ли() const;
	
	friend void разверни(Ткст& a, Ткст& b)                 { a.разверни(b); }
	
	Ткст(const std::string& s)                           { Ткст0::уст0(s.c_str(), (int)s.length()); }
	std::string вСтд() const                              { return std::string(старт(), стоп()); }
	
	template <class Делец>
	static Ткст сделай(int alloc, Делец m);
};

inline std::string to_string(const Ткст& s)              { return std::string(s.старт(), s.стоп()); }

class ТкстБуф : БезКопий {
	char   *pbegin;
	char   *pend;
	char   *limit;
	char    буфер[256];

	friend class Ткст;

	typedef Ткст0::Rc Rc;

	char *размести(int len, int& alloc);
	void  переразмести(dword n, const char *cat = NULL, int l = 0);
	void  ReallocL(const char *s, int l);
	void  расширь();
	void  обнули()                    { pbegin = pend = буфер; limit = pbegin + 255; }
	void  освободи();
	void  уст(Ткст& s);

public:
	char *старт()                   { *pend = '\0'; return pbegin; }
	char *begin()                   { return старт(); }
	char *стоп()                     { *pend = '\0'; return pend; }
	char *end()                     { return стоп(); }

	char& operator*()               { return *старт(); }
	char& operator[](int i)         { return старт()[i]; }
	operator char*()                { return старт(); }
	operator byte*()                { return (byte *)старт(); }
	operator void*()                { return старт(); }
	char *operator~()               { return старт(); }

	void устДлину(int l);
	void устСчёт(int l)            { устДлину(l); }
	int  дайДлину() const          { return (int)(pend - pbegin); }
	int  дайСчёт() const           { return дайДлину(); }
	void длинткт();
	void очисть()                    { освободи(); обнули(); }
	void резервируй(int r)             { int l = дайДлину(); устДлину(l + r); устДлину(l); }
	void сожми();

	void конкат(int c)                        { if(pend >= limit) расширь(); *pend++ = c; }
	void конкат(int c, int count);
	void конкат(const char *s, int l);
	void конкат(const char *s, const char *e) { конкат(s, int(e - s)); }
	void конкат(const char *s);
	void конкат(const Ткст& s)              { конкат(s, s.дайДлину()); }

	int  дайРазмест() const           { return (int)(limit - pbegin); }

	void operator=(Ткст& s)       { освободи(); уст(s); }

	ТкстБуф()                  { обнули(); }
	ТкстБуф(Ткст& s)         { уст(s); }
	ТкстБуф(int len)           { обнули(); устДлину(len); }
	~ТкстБуф()                 { if(pbegin != буфер) освободи(); }
};

inline bool  пустой(const Ткст& s)      { return s.пустой(); }

Ткст фмтУк(const void *p);

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

force_inline Ткст& operator<<(Ткст& s, const char *x)
{
	s.конкат(x, strlen__(x));
	return s;
}

force_inline Ткст& operator<<(Ткст& s, char *x)
{
	s.конкат(x);
	return s;
}

inline Ткст& operator<<(Ткст& s, const Ткст &x)
{
	s.конкат(x);
	return s;
}

inline Ткст& operator<<(Ткст& s, char x)
{
	s.конкат((int) x);
	return s;
}

inline Ткст& operator<<(Ткст& s, const void *x)
{
	s << фмтУк(x);
	return s;
}

inline Ткст& operator<<(Ткст& s, void *x)
{
	s << фмтУк(x);
	return s;
}

template <class T>
inline Ткст& operator<<(Ткст& s, const T& x)
{
	s.конкат(какТкст(x));
	return s;
}

template<>
inline Ткст& operator<<(Ткст& s, const char * const &x)
{
	s.конкат(x);
	return s;
}

template<>
inline Ткст& operator<<(Ткст& s, const Ткст &x)
{
	s.конкат(x);
	return s;
}

template<>
inline Ткст& operator<<(Ткст& s, const char& x)
{
	s.конкат(x);
	return s;
}

force_inline Ткст& operator<<(Ткст&& s, const char *x)
{
	s.конкат(x, strlen__(x));
	return s;
}

force_inline Ткст& operator<<(Ткст&& s, char *x)
{
	s.конкат(x);
	return s;
}

inline Ткст& operator<<(Ткст&& s, const Ткст &x)
{
	s.конкат(x);
	return s;
}

inline Ткст& operator<<(Ткст&& s, char x)
{
	s.конкат((int) x);
	return s;
}

inline Ткст& operator<<(Ткст&& s, const void *x)
{
	s << фмтУк(x);
	return s;
}

inline Ткст& operator<<(Ткст&& s, void *x)
{
	s << фмтУк(x);
	return s;
}

template <class T>
inline Ткст& operator<<(Ткст&& s, const T& x)
{
	s.конкат(какТкст(x));
	return s;
}

template<>
inline Ткст& operator<<(Ткст&& s, const char * const &x)
{
	s.конкат(x);
	return s;
}

template<>
inline Ткст& operator<<(Ткст&& s, const Ткст &x)
{
	s.конкат(x);
	return s;
}

template<>
inline Ткст& operator<<(Ткст&& s, const char& x)
{
	s.конкат(x);
	return s;
}

template<>
inline bool  пусто_ли(const Ткст& s)       { return s.пустой(); }

template<>
inline Ткст какТкст(const Ткст& s)     { return s; }

template<>
inline hash_t дайХэшЗнач(const Ткст& s) { return s.дайХэшЗнач(); }

int CompareNoCase(const Ткст& a, const Ткст& b);
int CompareNoCase(const Ткст& a, const char *b);

inline
int CompareNoCase(const char *a, const Ткст& b) {
	return -CompareNoCase(b, a);
}

Ткст обрежьЛево(const Ткст& s);
Ткст обрежьПраво(const Ткст& s);
Ткст обрежьОба(const Ткст& s);

Ткст обрежьЛево(const char *prefix, int len, const Ткст& s);
force_inline Ткст обрежьЛево(const char *prefix, const Ткст& s)    { return обрежьЛево(prefix, (int)strlen(prefix), s); }
force_inline Ткст обрежьЛево(const Ткст& prefix, const Ткст& s)  { return обрежьЛево(~prefix, prefix.дайСчёт(), s); }

Ткст обрежьПраво(const char *suffix, int len, const Ткст& s);
force_inline Ткст обрежьПраво(const char *suffix, const Ткст& s)   { return обрежьПраво(suffix, (int)strlen(suffix), s); }
force_inline Ткст обрежьПраво(const Ткст& suffix, const Ткст& s) { return обрежьПраво(~suffix, suffix.дайСчёт(), s); }

inline ТкстБуф& operator<<(ТкстБуф& s, const char *x)
{
	s.конкат(x);
	return s;
}

inline ТкстБуф& operator<<(ТкстБуф& s, char *x)
{
	s.конкат(x);
	return s;
}

inline ТкстБуф& operator<<(ТкстБуф& s, const Ткст &x)
{
	s.конкат(x);
	return s;
}

inline ТкстБуф& operator<<(ТкстБуф& s, char x)
{
	s.конкат((int) x);
	return s;
}

inline ТкстБуф& operator<<(ТкстБуф& s, const void *x)
{
	s << фмтУк(x);
	return s;
}

inline ТкстБуф& operator<<(ТкстБуф& s, void *x)
{
	s << фмтУк(x);
	return s;
}

template <class T>
inline ТкстБуф& operator<<(ТкстБуф& s, const T& x)
{
	s.конкат(какТкст(x));
	return s;
}

template<>
inline ТкстБуф& operator<<(ТкстБуф& s, const char * const &x)
{
	s.конкат(x);
	return s;
}

template<>
inline ТкстБуф& operator<<(ТкстБуф& s, const Ткст &x)
{
	s.конкат(x);
	return s;
}

template<>
inline ТкстБуф& operator<<(ТкстБуф& s, const char& x)
{
	s.конкат(x);
	return s;
}

template <class T>
void RawCat(Ткст& s, const T& x)
{
	s.конкат((const char *)&x, sizeof(x));
}

template <class T>
void RawCat(ТкстБуф& s, const T& x)
{
	s.конкат((const char *)&x, sizeof(x));
}

class WString0 {
	enum { SMALL = 23 };

	wchar *ptr;
	int    length;
	int    alloc;

#ifdef _ОТЛАДКА
	void Dsyn();
#else
	void Dsyn() {}
#endif

	static Атомар voidptr[2];

	bool    IsRc() const  { return alloc > SMALL; }
	Атомар& Rc()          { return *((Атомар *)ptr - 1); }
	bool    IsShared()    { return IsRc() && Rc() > 1; }

	wchar  *размести(int& count);
	void    LCat(int c);
	void    UnShare();

	friend class ШТкстБуф;
	friend class ШТкст;

protected:
	typedef wchar         tchar;
	typedef int16         bchar;
	typedef ШТкстБуф Буфер;
	typedef ШТкст       Ткст;

	void    обнули()                       { static wchar e[2]; length = alloc = 0; ptr = e; Dsyn(); ПРОВЕРЬ(*ptr == 0); }
	void    уст0(const wchar *s, int length);
	void    уст0(const WString0& s);
	void    подбери0(WString0&& s)          { ptr = s.ptr; length = s.length; alloc = s.alloc; s.обнули(); Dsyn(); }
	void    освободи();
	void    FFree()                      { if(alloc > 0) освободи(); }
	void    разверни(WString0& b)            { РНЦПДинрус::разверни(ptr, b.ptr); РНЦПДинрус::разверни(length, b.length); РНЦПДинрус::разверни(alloc, b.alloc); Dsyn(); b.Dsyn(); }
	wchar  *вставь(int pos, int count, const wchar *данные);

public:
	const wchar *старт() const           { return ptr; }
	const wchar *стоп() const             { return старт() + дайДлину(); }
	const wchar *begin() const           { return старт(); }
	const wchar *end() const             { return стоп(); }
	int   operator[](int i) const        { return ptr[i]; }

	operator const wchar *() const       { return старт(); }
	const wchar *operator~() const       { return старт(); }

	void конкат(int c)                      { if(!IsRc() && length < alloc) { ptr[length++] = c; ptr[length] = 0; } else LCat(c); Dsyn(); }
	void конкат(const wchar *s, int length);
	void уст(const wchar *s, int length);

	int  дайСчёт() const                { return length; }
	int  дайДлину() const               { return length; }
	int  дайРазмест() const                { return alloc; }

	hash_t   дайХэшЗнач() const             { return memhash(ptr, length * sizeof(wchar)); }
	bool     равен(const WString0& s) const { return s.length == length && memeq_t(ptr, s.ptr, length); }
	bool     равен(const wchar *s) const    { int l = strlen__(s); return l == дайСчёт() && memeq_t(begin(), s, l); }
	int      сравни(const WString0& s) const;

	void удали(int pos, int count = 1);
	void вставь(int pos, const wchar *s, int count);
	void очисть()                         { освободи(); обнули(); }

	void уст(int pos, int ch);
	void обрежь(int pos);

	WString0()                           { обнули(); }
	~WString0()                          { освободи(); }

//	WString0& operator=(const WString0& s) { освободи(); уст0(s); return *this; }
};

class ШТкст : public Движимое<ШТкст, АТкст<WString0> >
{
	void разверни(ШТкст& b)                                   { WString0::разверни(b); }

#ifdef _ОТЛАДКА
	int          len;
	const wchar *s;
	friend class WString0;
#endif

public:
	РНЦП::Ткст вТкст() const;

	const ШТкст& operator+=(wchar c)                      { конкат(c); return *this; }
	const ШТкст& operator+=(const wchar *s)               { конкат(s); return *this; }
	const ШТкст& operator+=(const ШТкст& s)             { конкат(s); return *this; }

	ШТкст& operator<<(wchar c)                            { конкат(c); return *this; }
	ШТкст& operator<<(const ШТкст& s)                   { конкат(s); return *this; }
	ШТкст& operator<<(const wchar *s)                     { конкат(s); return *this; }

	ШТкст& operator=(const wchar *s);
	ШТкст& operator=(const ШТкст& s)                    { if(this != &s) { WString0::FFree(); WString0::уст0(s); } return *this; }
	ШТкст& operator=(ШТкст&& s)                         { if(this != &s) { WString0::FFree(); WString0::подбери0(pick(s)); } return *this; }
	ШТкст& operator=(ШТкстБуф& b)                    { *this = ШТкст(b); return *this; }
//	ШТкст& operator<<=(const ШТкст& s)                  { if(this != &s) { WString0::освободи(); WString0::уст0(s, s.дайСчёт()); } return *this; }

	void   сожми()                                         { *this = ШТкст(старт(), дайДлину()); }

	ШТкст()                                               {}
	ШТкст(const Обнул&)                                  {}
	ШТкст(const ШТкст& s)                               { WString0::уст0(s); }
	ШТкст(ШТкст&& s)                                    { WString0::подбери0(pick(s)); }
	ШТкст(const wchar *s)                                 { WString0::уст0(s, strlen__(s)); }
	ШТкст(const ШТкст& s, int n)                        { ПРОВЕРЬ(n >= 0 && n <= s.дайДлину()); WString0::уст0(~s, n); }
	ШТкст(const wchar *s, int n)                          { WString0::уст0(s, n); }
	ШТкст(const wchar *s, const wchar *lim)               { WString0::уст0(s, (int)(lim - s)); }
	ШТкст(int chr, int count)                             { WString0::обнули(); конкат(chr, count); }
	ШТкст(ШТкстБуф& b);

	ШТкст(const char *s);
	ШТкст(const char *s, int n);
	ШТкст(const char *s, const char *lim);

	ШТкст(const char16 *s);

	static ШТкст дайПроц();
	bool   проц_ли() const                                   { return alloc < 0; }

	friend void разверни(ШТкст& a, ШТкст& b)                { a.разверни(b); }
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
	wchar   *pbegin;
	wchar   *pend;
	wchar   *limit;

	friend class ШТкст;

	wchar *размести(int len, int& alloc);
	void   расширь(dword n, const wchar *cat = NULL, int l = 0);
	void   расширь();
	void   обнули();
	void   освободи();
	void   уст(ШТкст& s);

public:
	wchar *старт()                   { *pend = '\0'; return pbegin; }
	wchar *begin()                   { return старт(); }
	wchar *стоп()                     { *pend = '\0'; return pend; }
	wchar *end()                     { return стоп(); }

	wchar& operator*()               { return *старт(); }
	wchar& operator[](int i)         { return старт()[i]; }
	operator wchar*()                { return старт(); }
	operator int16*()                { return (int16 *)старт(); }
	operator void*()                 { return старт(); }
	wchar *operator~()               { return старт(); }

	void  устДлину(int l);
	void  устСчёт(int l)            { устДлину(l); }
	int   дайДлину() const          { return (int)(pend - pbegin); }
	int   дайСчёт() const           { return дайДлину(); }
	void  длинткт()                   { устДлину(strlen__(pbegin)); }
	void  очисть()                    { освободи(); обнули(); }
	void  резервируй(int r)             { int l = дайДлину(); устДлину(l + r); устДлину(l); }

	void  конкат(int c)                          { if(pend >= limit) расширь(); *pend++ = c; }
	void  конкат(int c, int count);
	void  конкат(const wchar *s, int l);
	void  конкат(const wchar *s, const wchar *e) { конкат(s, int(e - s)); }
	void  конкат(const wchar *s)                 { конкат(s, strlen__(s)); }
	void  конкат(const ШТкст& s)               { конкат(s, s.дайДлину()); }
	void  конкат(const char *s)                  { конкат(ШТкст(s)); }

	int   дайРазмест() const           { return (int)(limit - pbegin); }

	void operator=(ШТкст& s)       { освободи(); уст(s); }

	ШТкстБуф()                  { обнули(); }
	ШТкстБуф(ШТкст& s)        { уст(s); }
	ШТкстБуф(int len)           { обнули(); устДлину(len); }
	~ШТкстБуф()                 { освободи(); }
};

inline bool  пустой(const ШТкст& s)      { return s.пустой(); }

template<>
inline bool  пусто_ли(const ШТкст& s)       { return s.пустой(); }

//template<>
//inline Ткст какТкст(const ШТкст& s)     { return s; }

template<>
inline hash_t дайХэшЗнач(const ШТкст& s) { return memhash(~s, 2 * s.дайДлину()); }

ШТкст обрежьЛево(const ШТкст& str);
ШТкст обрежьПраво(const ШТкст& s);

int CompareNoCase(const ШТкст& a, const ШТкст& b);
int CompareNoCase(const ШТкст& a, const wchar *b);

inline
int CompareNoCase(const wchar *a, const ШТкст& b) {
	return -CompareNoCase(b, a);
}

template<> inline Ткст какТкст(const char * const &s)    { return s; }
template<> inline Ткст какТкст(char * const &s)          { return s; }
template<> inline Ткст какТкст(const char *s)            { return s; }
template<> inline Ткст какТкст(char *s)                  { return s; }
template<> inline Ткст какТкст(const char& a)            { return Ткст(a, 1); }
template<> inline Ткст какТкст(const signed char& a)     { return Ткст(a, 1); }
template<> inline Ткст какТкст(const unsigned char& a)   { return Ткст(a, 1); }
template<> inline Ткст какТкст(const bool& a)            { return a ? "true" : "false"; }
template<> inline Ткст какТкст(const std::string& s)     { return Ткст(s); }
template<> inline Ткст какТкст(const std::wstring& s)    { return ШТкст(s).вТкст(); }

unsigned ctoi(int c);

typedef int (*CharFilter)(int);

int CharFilterAscii(int c);
int CharFilterAscii128(int c);
int CharFilterUnicode(int c);
int CharFilterDigit(int c);
int CharFilterWhitespace(int c);
int CharFilterNotWhitespace(int c);
int CharFilterAlpha(int c);
int CharFilterToUpper(int c);
int CharFilterToLower(int c);
int CharFilterToUpperAscii(int c);
int CharFilterAlphaToUpper(int c);
int CharFilterAlphaToLower(int c);
int CharFilterInt(int c);
int CharFilterDouble(int c);
int CharFilterDefaultToUpperAscii(int c);
int CharFilterCrLf(int c);
int CharFilterNoCrLf(int c);

Ткст фильтруй(const char *s, int (*filter)(int));
Ткст FilterWhile(const char *s, int (*filter)(int));

ШТкст фильтруй(const wchar *s, int (*filter)(int));
ШТкст FilterWhile(const wchar *s, int (*filter)(int));

#include "AString.hpp"
