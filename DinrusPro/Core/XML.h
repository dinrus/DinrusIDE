Ткст DeXml(кткст0 s, ббайт charset = CHARSET_DEFAULT, бул escapelf = false);
Ткст DeXml(кткст0 s, кткст0 end, ббайт charset = CHARSET_DEFAULT, бул escapelf = false);
Ткст DeXml(const Ткст& s, ббайт charset = CHARSET_DEFAULT, бул escapelf = false);
Ткст XmlPI(кткст0 text);
Ткст XmlHeader(кткст0 encoding = "UTF-8", кткст0 версия = "1.0", кткст0 standalone = NULL);
Ткст XmlDecl(кткст0 text);
Ткст XmlDocType(кткст0 text);
Ткст XmlDoc(кткст0 имя, кткст0 xmlbody);
Ткст XmlComment(кткст0 text);

class ТэгРяр : Движ<ТэгРяр> {
	Ткст tag;
	Ткст end;

public:
	ТэгРяр& Тэг(кткст0 s);

	Ткст  operator()();
	Ткст  operator()(кткст0 text);
	Ткст  operator()(const Ткст& text)                        { return operator()(~text); }
	Ткст  устТекст(кткст0 s, ббайт charset = CHARSET_DEFAULT);
	Ткст  устТекст(const Ткст& s, ббайт charset = CHARSET_DEFAULT) { return устТекст(~s, charset); }
	Ткст  PreservedText(кткст0 s, ббайт charset = CHARSET_DEFAULT);
	Ткст  PreservedText(const Ткст& s, ббайт charset = CHARSET_DEFAULT) { return PreservedText(~s, charset); }
	
	Ткст  дайНачало() const                                      { return tag + '>'; }
	Ткст  дайКонец() const                                        { return end; }

	ТэгРяр& operator()(кткст0 attr, кткст0 знач);
	ТэгРяр& operator()(кткст0 attr, цел q);
	ТэгРяр& operator()(кткст0 attr, дво q);

	ТэгРяр() {}
	ТэгРяр(кткст0 tag)                                       { Тэг(tag); }
};

enum { XML_DOC, XML_TAG, XML_END, XML_TEXT, XML_DECL, XML_PI, XML_COMMENT, XML_EOF };

struct ОшибкаРяр : public Искл
{
	ОшибкаРяр(кткст0 s) : Искл(s) {}
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
		Nesting(Ткст tag = Null, бул blanks = false) : tag(tag), preserve_blanks(blanks) {}
		Ткст tag;
		бул   preserve_blanks;
	};

	ВекторМап<Ткст, Ткст> entity;

	Поток                   *in;
	Буфер<char>              буфер;
	цел                       len;
	цел                       begincolumn;
	const char               *begin;
	const char               *term;

	Ткст                    attr1, attrval1;
	ВекторМап<Ткст, Ткст> attr;
	Массив<Nesting>            stack;

	цел                       тип;
	Ткст                    nattr1, nattrval1;
	ВекторМап<Ткст, Ткст> nattr;
	Ткст                    tagtext;
	Ткст                    cdata;
	бул                      empty_tag;
	бул                      npreserve, preserveall;
	бул                      relaxed;
	бул                      raw;

	цел                       line;
	
	ббайт                      acharset;
	ббайт                      scharset;

	проц                      иниц();
	проц                      грузиЕщё0();
	проц                      грузиЕщё()              { if(len - (term - begin) < MCHARS) грузиЕщё0(); }
	бул                      ещё();
	бул                      естьЕщё()               { return *term || ещё(); }
	проц                      Сущ(ТкстБуф& out);
	проц                      следщ();
	проц                      читайАтр(ТкстБуф& b, цел c);
	Ткст                      читайТэг(бул next);
	Ткст                      читайКонец(бул next);
	Ткст                      читайДекл(бул next);
	Ткст                      ReadPI(бул next);
	Ткст                      читайКоммент(бул next);
	цел                       дайКолонку0() const;
	Ткст                      Преобр(ТкстБуф& b);

public:
	проц   пропустиПробелы();
	
	проц   регСущность(const Ткст& ид, const Ткст& text);

	бул   кф_ли();
	кткст0 дайУк() const                                { return term; }

	бул   тэг_ли();
	Ткст PeekTag()                                          { return читайТэг(false); }
	Ткст читайТэг()                                          { return читайТэг(true); }
	бул   Тэг(кткст0 tag);
	бул   Тэг(const Ткст& tag);
	проц   передайТэг(кткст0 tag);
	проц   передайТэг(const Ткст& tag);
	бул   конец_ли();
	Ткст PeekEnd()                                          { return читайКонец(false); }
	Ткст читайКонец()                                          { return читайКонец(true); }
	бул   стоп();
	бул   стоп(кткст0 tag);
	бул   стоп(const Ткст& tag);
	проц   передайКонец();
	проц   передайКонец(кткст0 tag);
	бул   TagE(кткст0 tag);
	проц   PassTagE(кткст0 tag);
	бул   TagElseSkip(кткст0 tag);
	бул   LoopTag(кткст0 tag);

	цел    дайСчётАтров() const                               { return attr.дайСчёт() + !пусто_ли(attr1); }
	Ткст   дайАтр(цел i) const                               { return i ? attr.дайКлюч(i - 1) : attr1; }
	бул   атр_ли(кткст0 ид) const                       { return attr1 == ид || attr.найди(ид) >= 0; }
	Ткст operator[](цел i) const                            { return i ? attr[i - 1] : attrval1; }
	Ткст operator[](кткст0 ид) const                   { return attr1 == ид ? attrval1 : attr.дай(ид, Null); }
	цел    Цел(кткст0 ид, цел опр = Null) const;
	дво Дво(кткст0 ид, дво опр = Null) const;

	бул   текст_ли();
	Ткст PeekText()                                         { return cdata; }
	Ткст читайТекст();
	Ткст ReadTextE();

	бул   декл_ли();
	Ткст PeekDecl()                                         { return читайДекл(false); }
	Ткст читайДекл()                                         { return читайДекл(true); }

	бул   IsPI();
	Ткст PeekPI()                                           { return ReadPI(false); }
	Ткст ReadPI()                                           { return ReadPI(true); }

	бул   коммент_ли();
	Ткст PeekComment()                                      { return читайКоммент(false); }
	Ткст читайКоммент()                                      { return читайКоммент(true); }

	проц   пропусти();
	проц   пропустиКонец();

	ВекторМап<Ткст, Ткст> PickAttrs();

	цел    дайСтроку() const                                    { return line; }
	цел    дайКолонку() const                                  { return дайКолонку0() + 1; }

	проц   Relaxed(бул b = true)                             { relaxed = b; }
	проц   PreserveAllWhiteSpaces(бул b = true)              { preserveall = b; }
	проц   дайРяд(бул b = true)                                 { raw = b; }

	ПарсерРяр(кткст0 s);
	ПарсерРяр(Поток& in);
};

class УзелРяр : Движ< УзелРяр, ОпцияГлубокойКопии<УзелРяр> > {
	цел                              тип;
	Ткст                           text;
	Массив<УзелРяр>                   node;
	Один< ВекторМап<Ткст, Ткст> > attr;

public:
	static const УзелРяр& Проц();
	бул           проц_ли() const                             { return this == &Проц(); }

	цел            дайТип() const                            { return тип; }
	Ткст         дайТекст() const                            { return text; }
	Ткст         дайТэг() const                             { return text; }
	бул           тэг_ли() const                              { return тип == XML_TAG; }
	бул           тэг_ли(кткст0 tag) const               { return тэг_ли() && text == tag; }
	бул           текст_ли() const                             { return тип == XML_TEXT; }

	проц           очисть()                                    { text.очисть(); attr.очисть(); node.очисть(); тип = -1; }
	проц           создайТэг(кткст0 tag)                 { тип = XML_TAG; text = tag; }
	проц           создайТекст(const Ткст& txt)              { тип = XML_TEXT; text = txt; }
	проц           CreatePI(const Ткст& pi)                 { тип = XML_PI; text = pi; }
	проц           создайДекл(const Ткст& decl)             { тип = XML_DECL; text = decl; }
	проц           создайКоммент(const Ткст& comment)       { тип = XML_COMMENT; text = comment; }
	проц           создайДок()                           { очисть(); }
	бул           пустой() const                            { return тип == XML_DOC && node.дайСчёт() == 0; }
	operator бул() const                                     { return !пустой(); }

	цел            дайСчёт() const                           { return node.дайСчёт(); }
	УзелРяр&       по(цел i)                                  { return node.по(i); }
	const УзелРяр& Узел(цел i) const                          { return node[i]; }
	const УзелРяр& operator[](цел i) const                    { return i >= 0 && i < node.дайСчёт() ? node[i] : Проц(); }
	const УзелРяр& operator[](кткст0 tag) const;
	УзелРяр&       добавь()                                      { return node.добавь(); }
	проц           удали(цел i);
	проц           добавьТекст(const Ткст& txt)                 { добавь().создайТекст(txt); }
	цел            найдиТэг(кткст0 tag) const;
	УзелРяр&       добавь(кткст0 tag);
	УзелРяр&       дайДобавь(кткст0 tag);
	УзелРяр&       operator()(кткст0 tag)                { return дайДобавь(tag); }
	проц           удали(кткст0 tag);

	Ткст         собериТекст() const;
	Ткст         operator~() const                          { return собериТекст(); }
	бул           естьТэги() const;

	цел            дайСчётАтров() const                       { return attr ? attr->дайСчёт() : 0; }
	Ткст         идАтра(цел i) const                        { return attr->дайКлюч(i); }
	Ткст         Атр(цел i) const                          { return (*attr)[i]; }
	Ткст         Атр(кткст0 ид) const                 { return attr ? attr->дай(ид, Null) : Ткст(); }
	УзелРяр&       устАтр(кткст0 ид, const Ткст& знач);
	цел            целАтр(кткст0 ид, цел опр = Null) const;
	УзелРяр&       устАтр(кткст0 ид, цел знач);

	проц           устАтры(ВекторМап<Ткст, Ткст>&& a);
	
	проц           сожми();
	
	rval_default(УзелРяр);

	УзелРяр(const УзелРяр& n, цел);
	
	УзелРяр()                                                 { тип = XML_DOC; }

	typedef Массив<УзелРяр>::КонстОбходчик КонстОбходчик;
	КонстОбходчик          старт() const                      { return node.старт(); }
	КонстОбходчик          стоп() const                        { return node.стоп(); }

	typedef УзелРяр        value_type;
	typedef КонстОбходчик  const_iterator;
	typedef const УзелРяр& const_reference;
	typedef цел            size_type;
	typedef цел            difference_type;
	const_iterator         begin() const                      { return старт(); }
	const_iterator         end() const                        { return стоп(); }
};

enum {
	XML_IGNORE_DECLS    = 0x01,
	XML_IGNORE_PIS      = 0x02,
	XML_IGNORE_COMMENTS = 0x04,
};

struct ФильтрРазбораРяр {
	virtual бул сделайТэг(const Ткст& tag) = 0;
	virtual проц завершиТэг();
};

УзелРяр разбериРЯР(ПарсерРяр& p, бцел style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);
УзелРяр разбериРЯР(кткст0 s, бцел style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);
УзелРяр разбериРЯР(Поток& in, бцел style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);
УзелРяр разбериФайлРЯР(кткст0 path, бцел style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);

УзелРяр разбериРЯР(ПарсерРяр& p, ФильтрРазбораРяр& фильтр, бцел style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);
УзелРяр разбериРЯР(кткст0 s, ФильтрРазбораРяр& фильтр, бцел style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);
УзелРяр разбериРЯР(Поток& in, ФильтрРазбораРяр& фильтр, бцел style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);
УзелРяр разбериФайлРЯР(кткст0 path, ФильтрРазбораРяр& фильтр, бцел style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);

class IgnoreXmlPaths : public ФильтрРазбораРяр {
public:
	virtual бул сделайТэг(const Ткст& ид);
	virtual проц завершиТэг();

private:
	Индекс<Ткст>  list;
	Вектор<Ткст> path;

public:
	IgnoreXmlPaths(кткст0 s);
};

enum {
	XML_HEADER  = 0x01,
	XML_DOCTYPE = 0x02,
	XML_PRETTY =  0x04,
};

проц    какРЯР(Поток& out, const УзелРяр& n, бцел style = XML_HEADER|XML_DOCTYPE|XML_PRETTY);
Ткст  какРЯР(const УзелРяр& n, бцел style = XML_HEADER|XML_DOCTYPE|XML_PRETTY);
бул    какФайлРЯР(кткст0 path, const УзелРяр& n, бцел style = XML_HEADER|XML_DOCTYPE|XML_PRETTY);
