#ifdef  _ОТЛАДКА
#define NEWBLOCKSTREAM
#endif

enum {
	STRM_ERROR   =  0x20,
	STRM_READ    =  0x10,
	STRM_WRITE   =  0x08,
	STRM_SEEK    =  0x04,
	STRM_LOADING =  0x02,
	STRM_THROW   =  0x01,

	ERROR_NOT_ENOUGH_SPACE = -1,
	ERROR_LOADING_FAILED   = -2
};

struct StreamError {};
struct LoadingError : StreamError {};

enum EOLenum { EOL };

class Huge;

class Поток {
protected:
	int64  pos;
	byte  *буфер;
	byte  *ptr;
	byte  *rdlim;
	byte  *wrlim;

	unsigned style:6;
	unsigned errorcode:16;
	
	int      версия = 0;

	enum {
		BEGINOFLINE = 0x02,
	};

	virtual   void  _помести(int w);
	virtual   int   _прекрати();
	virtual   int   _получи();
	virtual   void  _помести(const void *данные, dword size);
	virtual   dword _получи(void *данные, dword size);

private:
	int       _получи8();
	int       _получи16();
	int       _получи32();
	int64     _получи64();

public:
	virtual   void  перейди(int64 pos);
	virtual   int64 дайРазм() const;
	virtual   void  устРазм(int64 size);
	virtual   void  слей();
	virtual   void  закрой();
	virtual   bool  открыт() const = 0;

	Поток();
	virtual  ~Поток();

	word      дайСтиль() const       { return style; }
	
	void      устВерсию(int ver)    { версия = ver; }
	int       дайВерсию() const     { return версия; }

	bool      ошибка_ли() const        { return style & STRM_ERROR; }
	bool      ок_ли() const           { return !ошибка_ли(); }
	void      устОш(int c = 0)    { style |= STRM_ERROR; errorcode = c; }
#ifdef PLATFORM_WIN32
	void      устПоследнОш()         { устОш(GetLastError()); }
#endif
#ifdef PLATFORM_POSIX
	void      устПоследнОш()         { устОш(errno); }
#endif
	int       дайОш() const       { return errorcode; }
	Ткст    дайТекстОш() const;
	void      сотриОш()           { style = style & ~STRM_ERROR; errorcode = 0; }

	int64     дайПоз() const         { return dword(ptr - буфер) + pos; }
	int64     GetLeft() const        { return дайРазм() - дайПоз(); }
	void      SeekEnd(int64 rel = 0) { перейди(дайРазм() + rel); }
	void      SeekCur(int64 rel)     { перейди(дайПоз() + rel); }

	bool      кф_ли()                { return ptr >= rdlim && _прекрати() < 0; }

	void      помести(int c)             { if(ptr < wrlim) *ptr++ = c; else _помести(c); }
	int       прекрати()                 { return ptr < rdlim ? *ptr : _прекрати(); }
	int       подбери()                 { return прекрати(); }
	int       дай()                  { return ptr < rdlim ? *ptr++ : _получи(); }

	const byte *подбериУк(int size = 1){ ПРОВЕРЬ(size > 0); return ptr + size <= rdlim ? ptr : NULL; }
	const byte *дайУк(int size = 1) { ПРОВЕРЬ(size > 0); if(ptr + size <= rdlim) { byte *p = ptr; ptr += size; return p; }; return NULL; }
	byte       *поместиУк(int size = 1) { ПРОВЕРЬ(size > 0); if(ptr + size <= wrlim) { byte *p = ptr; ptr += size; return p; }; return NULL; }
	const byte *GetSzPtr(int& size)  { прекрати(); size = int(rdlim - ptr); byte *p = ptr; ptr += size; return p; }

	void      помести(const void *данные, int size)  { ПРОВЕРЬ(size >= 0); if(size) { if(ptr + size <= wrlim) { memcpy8(ptr, данные, size); ptr += size; } else _помести(данные, size); } }
	int       дай(void *данные, int size)        { ПРОВЕРЬ(size >= 0); if(ptr + size <= rdlim) { memcpy8(данные, ptr, size); ptr += size; return size; } return _получи(данные, size); }

	void      помести(const Ткст& s)   { помести((const char *) s, s.дайДлину()); }
	Ткст    дай(int size);
	Ткст    дайВсе(int size);
	
	int       пропусти(int size);
	
	void      LoadThrowing()         { style |= STRM_THROW; }
	void      загрузиОш();

	bool      дайВсе(void *данные, int size);

	void      помести64(const void *данные, int64 size);
	int64     дай64(void *данные, int64 size);
	bool      GetAll64(void *данные, int64 size);

	size_t    дай(Huge& h, size_t size);
	bool      дайВсе(Huge& h, size_t size);

	int       дай8()                 { return ptr < rdlim ? *ptr++ : _получи8(); }
	int       дай16()                { if(ptr + 1 >= rdlim) return _получи16(); int q = подбери16(ptr); ptr += 2; return q; }
	int       дай32()                { if(ptr + 3 >= rdlim) return _получи32(); int q = подбери32(ptr); ptr += 4; return q; }
	int64     дай64()                { if(ptr + 7 >= rdlim) return _получи64(); int64 q = подбери64(ptr); ptr += 8; return q; }

	int       дайУтф8();

	Ткст    дайСтроку();

	void      помести16(word q)          { if(ptr + 1 < wrlim) { Poke16(ptr, q); ptr += 2; } else помести(&q, 2); }
	void      помести32(dword q)         { if(ptr + 3 < wrlim) { Poke32(ptr, q); ptr += 4; } else помести(&q, 4); }
	void      помести64(int64 q)         { if(ptr + 7 < wrlim) { Poke64(ptr, q); ptr += 8; } else помести(&q, 8); }

#ifdef CPU_LE
	int       Get16le()              { return дай16(); }
	int       Get32le()              { return дай32(); }
	int64     Get64le()              { return дай64(); }
	int       Get16be()              { return эндианРазворот16(дай16()); }
	int       Get32be()              { return эндианРазворот32(дай32()); }
	int64     Get64be()              { return SwapEndian64(дай64()); }

	void      Put16le(word q)        { помести16(q); }
	void      Put32le(dword q)       { помести32(q); }
	void      Put64le(int64 q)       { помести64(q); }
	void      Put16be(word q)        { помести16(эндианРазворот16(q)); }
	void      Put32be(dword q)       { помести32(эндианРазворот32(q)); }
	void      Put64be(int64 q)       { помести64(SwapEndian64(q)); }
#else
	int       Get16le()              { return эндианРазворот16(дай16()); }
	int       Get32le()              { return эндианРазворот32(дай32()); }
	int64     Get64le()              { return SwapEndian64(дай64()); }
	int       Get16be()              { return дай16(); }
	int       Get32be()              { return дай32(); }
	int64     Get64be()              { return дай64(); }

	void      Put16le(word q)        { помести16(эндианРазворот16(q)); }
	void      Put32le(dword q)       { помести32(эндианРазворот32(q)); }
	void      Put64le(int64 q)       { помести64(SwapEndian64(q)); }
	void      Put16be(word q)        { помести16(q); }
	void      Put32be(dword q)       { помести32(q); }
	void      Put64be(int64 q)       { помести64(q); }
#endif

	void      PutUtf8(int c);

	void      помести(const char *s);
	void      помести(int c, int count);
	void      Put0(int count)        { помести(0, count); }

	void      PutCrLf()              { помести16(MAKEWORD('\r', '\n')); }
#ifdef PLATFORM_WIN32
	void      PutEol()               { PutCrLf(); }
#else
	void      PutEol()               { помести('\n'); }
#endif

	Поток&   operator<<(EOLenum)    { PutEol(); return *this; }

	void      PutLine(const char *s);
	void      PutLine(const Ткст& s);

	void      помести(Поток& s, int64 size = INT64_MAX, dword click = 4096);

//  Поток as serialization streamer
	void      SetLoading()                 { ПРОВЕРЬ(style & STRM_READ); style |= STRM_LOADING; }
	void      SetStoring()                 { ПРОВЕРЬ(style & STRM_WRITE); style &= ~STRM_LOADING; }
	bool      грузится() const            { return style & STRM_LOADING; }
	bool      сохраняется() const            { return !грузится(); }

	void      SerializeRaw(byte *данные, int64 count);
	void      SerializeRaw(word *данные, int64 count);
	void      SerializeRaw(dword *данные, int64 count);
	void      SerializeRaw(uint64 *данные, int64 count);

	Ткст    GetAllRLE(int size);
	void      SerializeRLE(byte *данные, int count);
	
	Поток&   SerializeRaw(byte *данные)     { if(грузится()) *данные = дай(); else помести(*данные); return *this; }
	Поток&   SerializeRaw(word *данные)     { if(грузится()) *данные = Get16le(); else Put16le(*данные); return *this; }
	Поток&   SerializeRaw(dword *данные)    { if(грузится()) *данные = Get32le(); else Put32le(*данные); return *this; }
	Поток&   SerializeRaw(uint64 *данные)   { if(грузится()) *данные = Get64le(); else Put64le(*данные); return *this; }

	Поток&   operator%(bool& d)           { byte b; if(сохраняется()) b = d; SerializeRaw(&b); d = b; return *this; }
	Поток&   operator%(char& d)           { return SerializeRaw((byte *)&d); }
	Поток&   operator%(signed char& d)    { return SerializeRaw((byte *)&d); }
	Поток&   operator%(unsigned char& d)  { return SerializeRaw((byte *)&d); }
	Поток&   operator%(short& d)          { return SerializeRaw((word *)&d); }
	Поток&   operator%(unsigned short& d) { return SerializeRaw((word *)&d); }
	Поток&   operator%(int& d)            { return SerializeRaw((dword *)&d); }
	Поток&   operator%(unsigned int& d)   { return SerializeRaw((dword *)&d); }
	Поток&   operator%(long& d)           { return SerializeRaw((dword *)&d); }
	Поток&   operator%(unsigned long& d)  { return SerializeRaw((dword *)&d); }
	Поток&   operator%(float& d)          { return SerializeRaw((dword *)&d); }
	Поток&   operator%(double& d)         { return SerializeRaw((uint64 *)&d); }
	Поток&   operator%(int64& d)          { return SerializeRaw((uint64 *)&d); }
	Поток&   operator%(uint64& d)         { return SerializeRaw((uint64 *)&d); }

	Поток&   operator%(Ткст& s);
	Поток&   operator/(Ткст& s);

	Поток&   operator%(ШТкст& s);
	Поток&   operator/(ШТкст& s);

	void      Pack(dword& i);
	Поток&   operator/(int& i);
	Поток&   operator/(unsigned int& i);
	Поток&   operator/(long& i);
	Поток&   operator/(unsigned long& i);

	void      Magic(dword magic = 0x7d674d7b);

	void      Pack(bool& a, bool& b, bool& c, bool& d, bool& e, bool& f, bool& g, bool& h);
	void      Pack(bool& a, bool& b, bool& c, bool& d, bool& e, bool& f, bool& g);
	void      Pack(bool& a, bool& b, bool& c, bool& d, bool& e, bool& f);
	void      Pack(bool& a, bool& b, bool& c, bool& d, bool& e);
	void      Pack(bool& a, bool& b, bool& c, bool& d);
	void      Pack(bool& a, bool& b, bool& c);
	void      Pack(bool& a, bool& b);

#ifdef DEPRECATED
	int       дайШ()                 { return дай16(); }
	int       GetL()                 { return дай32(); }
	int       GetIW()                { return Get16le(); }
	int       GetIL()                { return Get32le(); }
	int       GetMW()                { return Get16be(); }
	int       GetML()                { return Get32be(); }
	void      PutW(int c)            { помести16(c); }
	void      PutL(int c)            { помести32(c); }
	void      PutIW(int c)           { Put16le(c); }
	void      PutIL(int c)           { Put32le(c); }
	void      PutMW(int c)           { Put16be(c); }
	void      PutML(int c)           { Put32be(c); }
	void      PutW(const char16 *s, int count)       { помести(s, count * 2); }
	dword     дайШ(char16 *s, int count)             { return дай(s, count * 2) / 2; }
	bool      GetAllW(char16 *s, int count)          { return дайВсе(s, count * 2); }
#endif
private: // No copy
	Поток(const Поток& s);
	void operator=(const Поток& s);
};

class ТкстПоток : public Поток {
protected:
	virtual  void  _помести(int w);
	virtual  int   _прекрати();
	virtual  int   _получи();
	virtual  void  _помести(const void *данные, dword size);
	virtual  dword _получи(void *данные, dword size);

public:
	virtual  void  перейди(int64 pos);
	virtual  int64 дайРазм() const;
	virtual  void  устРазм(int64 size);
	virtual  bool  открыт() const;

protected:
	bool           writemode;
	Ткст         данные;
	ТкстБуф   wdata;
	dword          size;
	int            limit = INT_MAX;

	void           иницРежимЧтен();
	void           SetWriteBuffer();
	void           SetReadMode();
	void           SetWriteMode();

public:
	void        открой(const Ткст& данные);
	void        создай();
	void        резервируй(int n);

	Ткст      дайРез();
	operator    Ткст()                              { return дайРез(); }
	
	void        Limit(int sz)                         { limit = sz; }
	
	struct LimitExc : public StreamError {};

	ТкстПоток()                           { создай(); }
	ТкстПоток(const Ткст& данные)         { открой(данные); }
};

class ПамПоток : public Поток {
protected:
	virtual   void  _помести(const void *данные, dword size);
	virtual   dword _получи(void *данные, dword size);

public:
	virtual   void  перейди(int64 pos);
	virtual   int64 дайРазм() const;
	virtual   bool  открыт() const;

public:
	void создай(void *данные, int64 size);

	ПамПоток();
	ПамПоток(void *данные, int64 size);
};

class ПамЧтенПоток : public ПамПоток {
public:
	void создай(const void *данные, int64 size);

	ПамЧтенПоток(const void *данные, int64 size);
	ПамЧтенПоток();
};

class БлокПоток : public Поток {
protected:
	virtual  void  _помести(int w);
	virtual  int   _прекрати();
	virtual  int   _получи();
	virtual  void  _помести(const void *данные, dword size);
	virtual  dword _получи(void *данные, dword size);

public:
	virtual  void  перейди(int64 pos);
	virtual  int64 дайРазм() const;
	virtual  void  устРазм(int64 size);
	virtual  void  слей();

private:
	int           pagesize;
	int64         pagemask;
	int64         pagepos;
	bool          pagedirty;

	int64         streamsize;

	void          устПоз(int64 p);
	void          синхРазм();
	bool          синхСтраницу();
	bool          синхПоз();
	void          ReadData(void *данные, int64 at, int size);

protected:
	virtual  dword читай(int64 at, void *ptr, dword size);
	virtual  void  пиши(int64 at, const void *данные, dword size);
	virtual  void  устРазмПотока(int64 size);

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

	dword     дайРазмБуф() const           { return pagesize; }
	void      устРазмБуф(dword newsize);
	int64     дайРазмПотока() const           { return streamsize; }

	БлокПоток();
	virtual ~БлокПоток();

protected:
	void     иницОткр(dword mode, int64 file_size);
};

class ФайлПоток : public БлокПоток {
protected:
	virtual  void  устРазмПотока(int64 size);
	virtual  dword читай(int64 at, void *ptr, dword size);
	virtual  void  пиши(int64 at, const void *данные, dword size);

public:
	virtual  void  закрой();
	virtual  bool  открыт() const;

protected:
#ifdef PLATFORM_WIN32
	HANDLE    handle;
#endif
#ifdef PLATFORM_POSIX
	int       handle;
#endif

	void      устПоз(int64 pos);
	void      иниц(int64 size);

public:
	operator  bool() const                 { return открыт(); }
	ФВремя  дайВремя() const;

#ifdef PLATFORM_WIN32
	void      устВремя(const ФВремя& tm);
	bool      открой(const char *filename, dword mode);

	ФайлПоток(const char *filename, dword mode);
#endif

#ifdef PLATFORM_POSIX
	bool      открой(const char *filename, dword mode, mode_t acm = 0644);
	ФайлПоток(const char *filename, dword mode, mode_t acm = 0644);
	ФайлПоток(int std_handle);
#endif

	ФайлПоток();
	~ФайлПоток();

#ifdef PLATFORM_WIN32
	HANDLE    дайУк() const            { return handle; }
#endif
#ifdef PLATFORM_POSIX
	int       дайУк() const            { return handle; }
#endif
};

class ФайлВывод : public ФайлПоток {
public:
#ifdef PLATFORM_POSIX
	bool открой(const char *фн, mode_t acm = 0644);
#endif
#ifdef PLATFORM_WIN32
	bool открой(const char *фн);
#endif

	ФайлВывод(const char *фн)                { открой(фн); }
	ФайлВывод()                              {}
};

class ФайлДоп : public ФайлПоток {
public:
	bool открой(const char *фн)         { return ФайлПоток::открой(фн, ФайлПоток::APPEND|ФайлПоток::NOWRITESHARE); }

	ФайлДоп(const char *фн)        { открой(фн); }
	ФайлДоп()                      {}
};

class ФайлВвод : public ФайлПоток {
public:
	bool открой(const char *фн)         { return ФайлПоток::открой(фн, ФайлПоток::READ); }

	ФайлВвод(const char *фн)            { открой(фн); }
	ФайлВвод()                          {}
};

class РамерПоток : public Поток {
protected:
	virtual void  _помести(int w);
	virtual void  _помести(const void *данные, dword size);

public:
	virtual int64 дайРазм() const;
	virtual bool  открыт() const;

protected:
	byte    h[256];

public:
	operator int64() const            { return дайРазм(); }

	void     открой()                   { ptr = буфер; сотриОш(); }

	РамерПоток();
};

class СравнПоток : public Поток {
protected:
	virtual  void  _помести(int w);
	virtual  void  _помести(const void *данные, dword size);

public:
	virtual  void  перейди(int64 pos);
	virtual  int64 дайРазм() const;
	virtual  void  устРазм(int64 size);
	virtual  void  открой();
	virtual  bool  открыт() const;
	virtual  void  слей();

private:
	Поток  *stream;
	bool     equal;
	int64    size;
	byte     h[128];

	void     сравни(int64 pos, const void *данные, int size);

public:
	void     открой(Поток& aStream);

	bool     равен()                         { слей(); return equal; }
	operator bool()                            { слей(); return equal; }

	СравнПоток();
	СравнПоток(Поток& aStream);
};

class ПотокВывода : public Поток {
	byte *h;

protected:
	virtual  void  _помести(int w);
	virtual  void  _помести(const void *данные, dword size);
	virtual  bool  открыт() const;

	virtual  void  выведи(const void *данные, dword size) = 0;

public:
	virtual  void  открой();
	
	void     слей();
	
	ПотокВывода();
	~ПотокВывода();
};

class TeeStream : public ПотокВывода {
protected:
	virtual  void  выведи(const void *данные, dword size);

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
	КартФайл(const char *file = NULL, bool delete_share = false);
	~КартФайл() { открой(); }

	bool        открой(const char *file, bool delete_share = false);
	bool        создай(const char *file, int64 filesize, bool delete_share = false);
	bool        расширь(int64 filesize);
	bool        вКарту(int64 offset, size_t len);
	bool        изКарты();
	bool        открой();

	bool        открыт() const            { return hfile != INVALID_HANDLE_VALUE; }

	int64       дайРазмФайла() const       { return filesize; }
	Время        дайВремя() const;
	Ткст      дайДанные(int64 offset, int len);

	int64       дайСмещ() const         { return offset; }
	size_t      дайСчёт() const          { return size; }

	int64       GetRawOffset() const      { return rawoffset; }
	size_t      GetRawCount() const       { return rawsize; }

	const byte *operator ~ () const       { ПРОВЕРЬ(открыт()); return base; }
	const byte *старт() const             { ПРОВЕРЬ(открыт()); return base; }
	const byte *стоп() const               { ПРОВЕРЬ(открыт()); return base + size; }
	const byte *дайОбх(int i) const      { ПРОВЕРЬ(открыт() && i >= 0 && (size_t)i <= size); return base + i; }
	const byte& operator [] (int i) const { ПРОВЕРЬ(открыт() && i >= 0 && (size_t)i < size); return base[i]; }

	byte       *operator ~ ()             { ПРОВЕРЬ(открыт()); return base; }
	byte       *старт()                   { ПРОВЕРЬ(открыт()); return base; }
	byte       *стоп()                     { ПРОВЕРЬ(открыт()); return base + size; }
	byte       *дайОбх(int i)            { ПРОВЕРЬ(открыт() && i >= 0 && (size_t)i <= size); return base + i; }
	byte&       operator [] (int i)       { ПРОВЕРЬ(открыт() && i >= 0 && (size_t)i < size); return base[i]; }

private:
#ifdef PLATFORM_WIN32
	HANDLE      hfile;
	HANDLE      hmap;
#endif
#ifdef PLATFORM_POSIX
	enum { INVALID_HANDLE_VALUE = -1 };
	int	        hfile;
	struct stat hfstat;
#endif
	byte       *base;
	byte       *rawbase;
	int64       filesize;
	int64       offset;
	int64       rawoffset;
	size_t      size;
	size_t      rawsize;
	bool        write;
};


Ткст загрузиПоток(Поток& in);
bool   сохраниПоток(Поток& out, const Ткст& данные);

int64 копируйПоток(Поток& dest, Поток& ист, int64 count = INT64_MAX);

#ifndef PLATFORM_WINCE
void    CoutUTF8();
Поток& Cout();
Поток& Cerr();
Ткст  читайСтдВхо();
Ткст  ReadSecret();
void    EnableEcho(bool b = true);
void    DisableEcho();
#endif

Поток& NilStream();

Ткст загрузиФайл(const char *filename);
bool   сохраниФайл(const char *filename, const Ткст& данные);

template <class T>
inline Поток& operator%(Поток& s, T& x)
{
	x.сериализуй(s);
	return s;
}

inline Поток& operator<<(Поток& s, const char *x)
{
	s.помести(x);
	return s;
}

inline Поток& operator<<(Поток& s, char *x)
{
	s.помести(x);
	return s;
}

inline Поток& operator<<(Поток& s, const Ткст &x)
{
	s.помести(x);
	return s;
}

inline Поток& operator<<(Поток& s, char x)
{
	s.помести((int) x);
	return s;
}

inline Поток& operator<<(Поток& s, const void *x)
{
	s << фмтУк(x);
	return s;
}

inline Поток& operator<<(Поток& s, void *x)
{
	s << фмтУк(x);
	return s;
}

template <class T>
inline Поток& operator<<(Поток& s, const T& x) {
	s << какТкст(x);
	return s;
}

// ------

Поток& пакуй16(Поток& s, int& i);
Поток& пакуй16(Поток& s, int& i1, int& i2);
Поток& пакуй16(Поток& s, int& i1, int& i2, int& i3);
Поток& пакуй16(Поток& s, int& i1, int& i2, int& i3, int& i4);
Поток& пакуй16(Поток& s, int& i1, int& i2, int& i3, int& i4, int& i5);

int     StreamHeading(Поток& stream, int ver, int minver, int maxver, const char* tag);
