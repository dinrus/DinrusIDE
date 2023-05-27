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

struct ОшПотока {};
struct LoadingError : ОшПотока {};

enum EOLenum { EOL };

class Huge;

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
	virtual   проц  _помести(const ук данные, бцел size);
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
	Ткст      дайТекстОш() const;
	проц      сотриОш()           { style = style & ~STRM_ERROR; errorcode = 0; }

	дол       дайПоз() const         { return бцел(укз - буфер) + pos; }
	дол       GetLeft() const        { return дайРазм() - дайПоз(); }
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

	проц      помести(const ук данные, цел size)  { ПРОВЕРЬ(size >= 0); if(size) { if(укз + size <= wrlim) { копирпам8(укз, данные, size); укз += size; } else _помести(данные, size); } }
	цел       дай(ук данные, цел size)        { ПРОВЕРЬ(size >= 0); if(укз + size <= rdlim) { копирпам8(данные, укз, size); укз += size; return size; } return _получи(данные, size); }

	проц      помести(const Ткст& s)   { помести((const char *) s, s.дайДлину()); }
	Ткст      дай(цел size);
	Ткст      дайВсе(цел size);
	
	цел       пропусти(цел size);
	
	проц      LoadThrowing()         { style |= STRM_THROW; }
	проц      загрузиОш();

	бул      дайВсе(ук данные, цел size);

	проц      помести64(const ук данные, дол size);
	дол      дай64(ук данные, дол size);
	бул      GetAll64(ук данные, дол size);

	т_мера    дай(Huge& h, т_мера size);
	бул       дайВсе(Huge& h, т_мера size);

	цел       дай8()                 { return укз < rdlim ? *укз++ : _получи8(); }
	цел       дай16()                { if(укз + 1 >= rdlim) return _получи16(); цел q = подбери16(укз); укз += 2; return q; }
	цел       дай32()                { if(укз + 3 >= rdlim) return _получи32(); цел q = подбери32(укз); укз += 4; return q; }
	дол       дай64()                { if(укз + 7 >= rdlim) return _получи64(); дол q = подбери64(укз); укз += 8; return q; }

	цел       дайУтф8();

	Ткст    дайСтроку();

	проц      помести16(бкрат q)          { if(укз + 1 < wrlim) { помести16(укз, q); укз += 2; } else помести(&q, 2); }
	проц      помести32(бцел q)         { if(укз + 3 < wrlim) { помести32(укз, q); укз += 4; } else помести(&q, 4); }
	проц      помести64(дол q)         { if(укз + 7 < wrlim) { помести64(укз, q); укз += 8; } else помести(&q, 8); }

#ifdef CPU_LE
	цел       Get16le()              { return дай16(); }
	цел       Get32le()              { return дай32(); }
	дол     Get64le()              { return дай64(); }
	цел       Get16be()              { return эндианРазворот16(дай16()); }
	цел       Get32be()              { return эндианРазворот32(дай32()); }
	дол     Get64be()              { return эндианРазворот64(дай64()); }

	проц      Put16le(бкрат q)        { помести16(q); }
	проц      Put32le(бцел q)       { помести32(q); }
	проц      Put64le(дол q)       { помести64(q); }
	проц      Put16be(бкрат q)        { помести16(эндианРазворот16(q)); }
	проц      Put32be(бцел q)       { помести32(эндианРазворот32(q)); }
	проц      Put64be(дол q)       { помести64(эндианРазворот64(q)); }
#else
	цел       Get16le()              { return эндианРазворот16(дай16()); }
	цел       Get32le()              { return эндианРазворот32(дай32()); }
	дол     Get64le()              { return эндианРазворот64(дай64()); }
	цел       Get16be()              { return дай16(); }
	цел       Get32be()              { return дай32(); }
	дол     Get64be()              { return дай64(); }

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

	проц      помести(Поток& s, дол size = INT64_MAX, бцел click = 4096);

//  Поток as serialization streamer
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
	Поток&   operator%(char& d)           { return SerializeRaw((ббайт *)&d); }
	Поток&   operator%(signed char& d)    { return SerializeRaw((ббайт *)&d); }
	Поток&   operator%(unsigned char& d)  { return SerializeRaw((ббайт *)&d); }
	Поток&   operator%(short& d)          { return SerializeRaw((бкрат *)&d); }
	Поток&   operator%(unsigned short& d) { return SerializeRaw((бкрат *)&d); }
	Поток&   operator%(цел& d)            { return SerializeRaw((бцел *)&d); }
	Поток&   operator%(бцел& d)   { return SerializeRaw((бцел *)&d); }
	Поток&   operator%(long& d)           { return SerializeRaw((бцел *)&d); }
	Поток&   operator%(unsigned long& d)  { return SerializeRaw((бцел *)&d); }
	Поток&   operator%(float& d)          { return SerializeRaw((бцел *)&d); }
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
	проц      PutW(const char16 *s, цел count)       { помести(s, count * 2); }
	бцел     дайШ(char16 *s, цел count)             { return дай(s, count * 2) / 2; }
	бул      GetAllW(char16 *s, цел count)          { return дайВсе(s, count * 2); }
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
	virtual  проц  _помести(const ук данные, бцел size);
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
	virtual   проц  _помести(const ук данные, бцел size);
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
	проц создай(const ук данные, дол size);

	ПамЧтенПоток(const ук данные, дол size);
	ПамЧтенПоток();
};

class БлокПоток : public Поток {
protected:
	virtual  проц  _помести(цел w);
	virtual  цел   _прекрати();
	virtual  цел   _получи();
	virtual  проц  _помести(const ук данные, бцел size);
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
	virtual  проц  пиши(дол at, const ук данные, бцел size);
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
	virtual  проц  пиши(дол at, const ук данные, бцел size);

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

class РамерПоток : public Поток {
protected:
	virtual проц  _помести(цел w);
	virtual проц  _помести(const ук данные, бцел size);

public:
	virtual дол дайРазм() const;
	virtual бул  открыт() const;

protected:
	ббайт    h[256];

public:
	operator дол() const            { return дайРазм(); }

	проц     открой()                   { укз = буфер; сотриОш(); }

	РамерПоток();
};

class СравнПоток : public Поток {
protected:
	virtual  проц  _помести(цел w);
	virtual  проц  _помести(const ук данные, бцел size);

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

	проц     сравни(дол pos, const ук данные, цел size);

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
	virtual  проц  _помести(const ук данные, бцел size);
	virtual  бул  открыт() const;

	virtual  проц  выведи(const ук данные, бцел size) = 0;

public:
	virtual  проц  открой();
	
	проц     слей();
	
	ПотокВывода();
	~ПотокВывода();
};

class TeeStream : public ПотокВывода {
protected:
	virtual  проц  выведи(const ук данные, бцел size);

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


Ткст загрузиПоток(Поток& in);
бул   сохраниПоток(Поток& out, const Ткст& данные);

дол копируйПоток(Поток& dest, Поток& ист, дол count = INT64_MAX);

#ifndef PLATFORM_WINCE
проц    CoutUTF8();
Поток& Cout();
Поток& Cerr();
Ткст  читайСтдВхо();
Ткст  ReadSecret();
проц    EnableEcho(бул b = true);
проц    DisableEcho();
#endif

Поток& NilStream();

Ткст загрузиФайл(кткст0 имяф);
бул   сохраниФайл(кткст0 имяф, const Ткст& данные);

template <class T>
inline Поток& operator%(Поток& s, T& x)
{
	x.сериализуй(s);
	return s;
}

inline Поток& operator<<(Поток& s, кткст0 x)
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
	s.помести((цел) x);
	return s;
}

inline Поток& operator<<(Поток& s, const ук x)
{
	s << фмтУк(x);
	return s;
}

inline Поток& operator<<(Поток& s, ук x)
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

Поток& пакуй16(Поток& s, цел& i);
Поток& пакуй16(Поток& s, цел& i1, цел& i2);
Поток& пакуй16(Поток& s, цел& i1, цел& i2, цел& i3);
Поток& пакуй16(Поток& s, цел& i1, цел& i2, цел& i3, цел& i4);
Поток& пакуй16(Поток& s, цел& i1, цел& i2, цел& i3, цел& i4, цел& i5);

цел     StreamHeading(Поток& stream, цел ver, цел minver, цел maxver, const char* tag);
