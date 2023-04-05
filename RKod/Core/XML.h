Ткст DeXml(const char *s, byte charset = CHARSET_DEFAULT, bool escapelf = false);
Ткст DeXml(const char *s, const char *end, byte charset = CHARSET_DEFAULT, bool escapelf = false);
Ткст DeXml(const Ткст& s, byte charset = CHARSET_DEFAULT, bool escapelf = false);
Ткст XmlPI(const char *text);
Ткст XmlHeader(const char *encoding = "UTF-8", const char *версия = "1.0", const char *standalone = NULL);
Ткст XmlDecl(const char *text);
Ткст XmlDocType(const char *text);
Ткст XmlDoc(const char *имя, const char *xmlbody);
Ткст XmlComment(const char *text);

class ТэгРяр : Движимое<ТэгРяр> {
	Ткст tag;
	Ткст end;

public:
	ТэгРяр& Тэг(const char *s);

	Ткст  operator()();
	Ткст  operator()(const char *text);
	Ткст  operator()(const Ткст& text)                        { return operator()(~text); }
	Ткст  устТекст(const char *s, byte charset = CHARSET_DEFAULT);
	Ткст  устТекст(const Ткст& s, byte charset = CHARSET_DEFAULT) { return устТекст(~s, charset); }
	Ткст  PreservedText(const char *s, byte charset = CHARSET_DEFAULT);
	Ткст  PreservedText(const Ткст& s, byte charset = CHARSET_DEFAULT) { return PreservedText(~s, charset); }
	
	Ткст  дайНачало() const                                      { return tag + '>'; }
	Ткст  дайКонец() const                                        { return end; }

	ТэгРяр& operator()(const char *attr, const char *val);
	ТэгРяр& operator()(const char *attr, int q);
	ТэгРяр& operator()(const char *attr, double q);

	ТэгРяр() {}
	ТэгРяр(const char *tag)                                       { Тэг(tag); }
};

enum { XML_DOC, XML_TAG, XML_END, XML_TEXT, XML_DECL, XML_PI, XML_COMMENT, XML_EOF };

struct ОшибкаРяр : public Искл
{
	ОшибкаРяр(const char *s) : Искл(s) {}
};

class ПарсерРяр {
	enum {
#ifdef flagTEST_XML // This is for testing purposes only to increase boundary condition frequency
		MCHARS = 128,
		CHUNK = 256
#else
		MCHARS = 256,
		CHUNK = 16384
#endif
	};
	
	struct Nesting {
		Nesting(Ткст tag = Null, bool blanks = false) : tag(tag), preserve_blanks(blanks) {}
		Ткст tag;
		bool   preserve_blanks;
	};

	ВекторМап<Ткст, Ткст> entity;

	Поток                   *in;
	Буфер<char>              буфер;
	int                       len;
	int                       begincolumn;
	const char               *begin;
	const char               *term;

	Ткст                    attr1, attrval1;
	ВекторМап<Ткст, Ткст> attr;
	Массив<Nesting>            stack;

	int                       тип;
	Ткст                    nattr1, nattrval1;
	ВекторМап<Ткст, Ткст> nattr;
	Ткст                    tagtext;
	Ткст                    cdata;
	bool                      empty_tag;
	bool                      npreserve, preserveall;
	bool                      relaxed;
	bool                      raw;

	int                       line;
	
	byte                      acharset;
	byte                      scharset;

	void                      иниц();
	void                      грузиЕщё0();
	void                      грузиЕщё()              { if(len - (term - begin) < MCHARS) грузиЕщё0(); }
	bool                      ещё();
	bool                      естьЕщё()               { return *term || ещё(); }
	void                      Сущ(ТкстБуф& out);
	void                      следщ();
	void                      читайАтр(ТкстБуф& b, int c);
	Ткст                      читайТэг(bool next);
	Ткст                      читайКонец(bool next);
	Ткст                      читайДекл(bool next);
	Ткст                      ReadPI(bool next);
	Ткст                      читайКоммент(bool next);
	int                       дайКолонку0() const;
	Ткст                      Преобр(ТкстБуф& b);

public:
	void   пропустиПробелы();
	
	void   регСущность(const Ткст& ид, const Ткст& text);

	bool   кф_ли();
	const char *дайУк() const                                { return term; }

	bool   тэг_ли();
	Ткст PeekTag()                                          { return читайТэг(false); }
	Ткст читайТэг()                                          { return читайТэг(true); }
	bool   Тэг(const char *tag);
	bool   Тэг(const Ткст& tag);
	void   передайТэг(const char *tag);
	void   передайТэг(const Ткст& tag);
	bool   конец_ли();
	Ткст PeekEnd()                                          { return читайКонец(false); }
	Ткст читайКонец()                                          { return читайКонец(true); }
	bool   стоп();
	bool   стоп(const char *tag);
	bool   стоп(const Ткст& tag);
	void   передайКонец();
	void   передайКонец(const char *tag);
	bool   TagE(const char *tag);
	void   PassTagE(const char *tag);
	bool   TagElseSkip(const char *tag);
	bool   LoopTag(const char *tag);

	int    дайСчётАтров() const                               { return attr.дайСчёт() + !пусто_ли(attr1); }
	Ткст   дайАтр(int i) const                               { return i ? attr.дайКлюч(i - 1) : attr1; }
	bool   атр_ли(const char *ид) const                       { return attr1 == ид || attr.найди(ид) >= 0; }
	Ткст operator[](int i) const                            { return i ? attr[i - 1] : attrval1; }
	Ткст operator[](const char *ид) const                   { return attr1 == ид ? attrval1 : attr.дай(ид, Null); }
	int    Цел(const char *ид, int опр = Null) const;
	double Дво(const char *ид, double опр = Null) const;

	bool   текст_ли();
	Ткст PeekText()                                         { return cdata; }
	Ткст читайТекст();
	Ткст ReadTextE();

	bool   декл_ли();
	Ткст PeekDecl()                                         { return читайДекл(false); }
	Ткст читайДекл()                                         { return читайДекл(true); }

	bool   IsPI();
	Ткст PeekPI()                                           { return ReadPI(false); }
	Ткст ReadPI()                                           { return ReadPI(true); }

	bool   коммент_ли();
	Ткст PeekComment()                                      { return читайКоммент(false); }
	Ткст читайКоммент()                                      { return читайКоммент(true); }

	void   пропусти();
	void   пропустиКонец();

	ВекторМап<Ткст, Ткст> PickAttrs();

	int    дайСтроку() const                                    { return line; }
	int    дайКолонку() const                                  { return дайКолонку0() + 1; }

	void   Relaxed(bool b = true)                             { relaxed = b; }
	void   PreserveAllWhiteSpaces(bool b = true)              { preserveall = b; }
	void   дайРяд(bool b = true)                                 { raw = b; }

	ПарсерРяр(const char *s);
	ПарсерРяр(Поток& in);
};

class УзелРяр : Движимое< УзелРяр, DeepCopyOption<УзелРяр> > {
	int                              тип;
	Ткст                           text;
	Массив<УзелРяр>                   node;
	Один< ВекторМап<Ткст, Ткст> > attr;

public:
	static const УзелРяр& Проц();
	bool           проц_ли() const                             { return this == &Проц(); }

	int            дайТип() const                            { return тип; }
	Ткст         дайТекст() const                            { return text; }
	Ткст         дайТэг() const                             { return text; }
	bool           тэг_ли() const                              { return тип == XML_TAG; }
	bool           тэг_ли(const char *tag) const               { return тэг_ли() && text == tag; }
	bool           текст_ли() const                             { return тип == XML_TEXT; }

	void           очисть()                                    { text.очисть(); attr.очисть(); node.очисть(); тип = -1; }
	void           создайТэг(const char *tag)                 { тип = XML_TAG; text = tag; }
	void           создайТекст(const Ткст& txt)              { тип = XML_TEXT; text = txt; }
	void           CreatePI(const Ткст& pi)                 { тип = XML_PI; text = pi; }
	void           создайДекл(const Ткст& decl)             { тип = XML_DECL; text = decl; }
	void           создайКоммент(const Ткст& comment)       { тип = XML_COMMENT; text = comment; }
	void           создайДок()                           { очисть(); }
	bool           пустой() const                            { return тип == XML_DOC && node.дайСчёт() == 0; }
	operator bool() const                                     { return !пустой(); }

	int            дайСчёт() const                           { return node.дайСчёт(); }
	УзелРяр&       по(int i)                                  { return node.по(i); }
	const УзелРяр& Узел(int i) const                          { return node[i]; }
	const УзелРяр& operator[](int i) const                    { return i >= 0 && i < node.дайСчёт() ? node[i] : Проц(); }
	const УзелРяр& operator[](const char *tag) const;
	УзелРяр&       добавь()                                      { return node.добавь(); }
	void           удали(int i);
	void           добавьТекст(const Ткст& txt)                 { добавь().создайТекст(txt); }
	int            найдиТэг(const char *tag) const;
	УзелРяр&       добавь(const char *tag);
	УзелРяр&       дайДобавь(const char *tag);
	УзелРяр&       operator()(const char *tag)                { return дайДобавь(tag); }
	void           удали(const char *tag);

	Ткст         собериТекст() const;
	Ткст         operator~() const                          { return собериТекст(); }
	bool           естьТэги() const;

	int            дайСчётАтров() const                       { return attr ? attr->дайСчёт() : 0; }
	Ткст         идАтра(int i) const                        { return attr->дайКлюч(i); }
	Ткст         Атр(int i) const                          { return (*attr)[i]; }
	Ткст         Атр(const char *ид) const                 { return attr ? attr->дай(ид, Null) : Ткст(); }
	УзелРяр&       устАтр(const char *ид, const Ткст& val);
	int            целАтр(const char *ид, int опр = Null) const;
	УзелРяр&       устАтр(const char *ид, int val);

	void           устАтры(ВекторМап<Ткст, Ткст>&& a);
	
	void           сожми();
	
	rval_default(УзелРяр);

	УзелРяр(const УзелРяр& n, int);
	
	УзелРяр()                                                 { тип = XML_DOC; }

	typedef Массив<УзелРяр>::КонстОбходчик КонстОбходчик;
	КонстОбходчик          старт() const                      { return node.старт(); }
	КонстОбходчик          стоп() const                        { return node.стоп(); }

	typedef УзелРяр        value_type;
	typedef КонстОбходчик  const_iterator;
	typedef const УзелРяр& const_reference;
	typedef int            size_type;
	typedef int            difference_type;
	const_iterator         begin() const                      { return старт(); }
	const_iterator         end() const                        { return стоп(); }
};

enum {
	XML_IGNORE_DECLS    = 0x01,
	XML_IGNORE_PIS      = 0x02,
	XML_IGNORE_COMMENTS = 0x04,
};

struct ФильтрРазбораРяр {
	virtual bool сделайТэг(const Ткст& tag) = 0;
	virtual void завершиТэг();
};

УзелРяр разбериРЯР(ПарсерРяр& p, dword style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);
УзелРяр разбериРЯР(const char *s, dword style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);
УзелРяр разбериРЯР(Поток& in, dword style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);
УзелРяр разбериФайлРЯР(const char *path, dword style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);

УзелРяр разбериРЯР(ПарсерРяр& p, ФильтрРазбораРяр& filter, dword style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);
УзелРяр разбериРЯР(const char *s, ФильтрРазбораРяр& filter, dword style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);
УзелРяр разбериРЯР(Поток& in, ФильтрРазбораРяр& filter, dword style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);
УзелРяр разбериФайлРЯР(const char *path, ФильтрРазбораРяр& filter, dword style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);

class IgnoreXmlPaths : public ФильтрРазбораРяр {
public:
	virtual bool сделайТэг(const Ткст& ид);
	virtual void завершиТэг();

private:
	Индекс<Ткст>  list;
	Вектор<Ткст> path;

public:
	IgnoreXmlPaths(const char *s);
};

enum {
	XML_HEADER  = 0x01,
	XML_DOCTYPE = 0x02,
	XML_PRETTY =  0x04,
};

void    какРЯР(Поток& out, const УзелРяр& n, dword style = XML_HEADER|XML_DOCTYPE|XML_PRETTY);
Ткст  какРЯР(const УзелРяр& n, dword style = XML_HEADER|XML_DOCTYPE|XML_PRETTY);
bool    какФайлРЯР(const char *path, const УзелРяр& n, dword style = XML_HEADER|XML_DOCTYPE|XML_PRETTY);
