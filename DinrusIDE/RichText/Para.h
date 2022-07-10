struct RichCaret;
struct RichPos;

struct RichPara {
	enum Code {
		NONE,

		INDEXENTRY  = 2,
		FIELD       = 3,
		FACE        = 4,
		HEIGHT      = 5,
		LANGUAGE    = 6,
		INK         = 7,
		PAPER       = 8,

		TAB         = 9, // placeholder

		LINK        = 10,
		SSCRIPT     = 11,

		BOLD0       = 12,
		BOLD1       = 13,
		BOLDS       = 14,
		ITALIC0     = 15,
		ITALIC1     = 16,
		ITALICS     = 17,
		UNDERLINE0  = 18,
		UNDERLINE1  = 19,
		UNDERLINES  = 20,
		CAPITALS0   = 21,
		CAPITALS1   = 22,
		CAPITALSS   = 23,
		STRIKEOUT0  = 24,
		STRIKEOUT1  = 25,
		STRIKEOUTS  = 26,
		DASHED0     = 27,
		DASHED1     = 28,
		DASHEDS     = 29,

		EXT         = 30,

		OBJECT      = 31
	};
	
	enum ExtCode {
		NONAA0      = 1,
		NONAA1      = 2,
		NONAAS      = 3,
	};

	enum BULLET_STYLE {
		BULLET_NONE,
		BULLET_ROUND,
		BULLET_ROUNDWHITE,
		BULLET_BOX,
		BULLET_BOXWHITE,
		BULLET_TEXT
	};

	enum NUMBER_STYLE {
		NUMBER_NONE,
		NUMBER_1,
		NUMBER_0,
		NUMBER_a,
		NUMBER_A,
		NUMBER_i,
		NUMBER_I,
	};

	enum LINESPACING {
		LSP10 = 0,
		LSP15 = -1,
		LSP20 = -2,
		LSP115 = -3,
	};
	
	enum TabSpecial {
		TAB_RIGHTPOS = 0x8000000
	};
	
	enum RulerStyle {
		RULER_SOLID,
		RULER_DOT,
		RULER_DASH,
	};

	struct CharFormat : public Шрифт {
		int     language;
		Цвет   ink, paper;
		Ткст  link;
		ШТкст indexentry;
		dword   sscript:2;
		bool    capitals;
		bool    dashed;

#ifdef _ОТЛАДКА
		Ткст  вТкст() const;
#endif

		CharFormat();
	};

	struct Вкладка : Движимое<Вкладка> {
		int   pos;
		byte  align;
		byte  fillchar;

		Вкладка() { align = ALIGN_LEFT; pos = 0; fillchar = 0; }
	};

	struct NumberFormat {
		Ткст      before_number, after_number;
		bool        reset_number;
		byte        number[8];

		bool        IsNumbered() const;
		int         GetNumberLevel() const;
	};

	struct фмт : NumberFormat, CharFormat {
		Uuid        styleid;
		Ткст      label;
		WithDeepCopy< Вектор<Вкладка> > tab;
		int         align;
		int         ruler, before, lm, indent, rm, after;
		Цвет       rulerink;
		int         rulerstyle;
		int         tabsize;
		int         bullet;
		int         linespacing;
		bool        newpage, firstonpage, keep, keepnext, orphan, newhdrftr;
		Ткст      header_qtf, footer_qtf;

		void        сортируйТабы();

#ifdef _ОТЛАДКА
		Ткст      вТкст() const;
#endif

		фмт();
	};

	struct Number {
		int     n[8];

		Ткст  AsText(const NumberFormat& формат) const;
		void    TestReset(const NumberFormat& формат);
		void    следщ(const NumberFormat& формат);

		Number();
	};

	struct Part {
		RichObject                  object;
		ШТкст                     text;
		CharFormat                  формат;
		Ид                          field;
		Ткст                      fieldparam;
		WithDeepCopy< Массив<Part> > fieldpart;

		bool        NonText() const    { return object || field; }
		bool        текст_ли() const     { return !NonText(); }
		int         дайДлину() const  { return object || field ? 1 : text.дайДлину(); }
	};

	struct FieldType {
		virtual Массив<Part> Evaluate(const Ткст& param, ВекторМап<Ткст, Значение>& vars,
		                             const CharFormat& fmt) = 0;
		virtual void        Menu(Бар& bar, Ткст *param) const {}
		virtual void        DefaultAction(Ткст *param) const {}
		virtual ~FieldType() {}
	};

	struct HeightInfo {
		int                  ascent;
		int                  descent;
		int                  external;
		int                  ydelta;
		const RichObject    *object;

		int                  сумма() const { return ascent + descent + external; }
	};

	struct Строка : public HeightInfo {
		int  pos;
		int  len;
		int  ppos;
		int  plen;
		int  xpos;
		int  cx;
		int  objecti;
		bool withtabs;
	};

	struct Lines {
		int                        len;
		int                        clen;
		int                        cx;
		Буфер<wchar>              text;
		Буфер<byte>               spell;
		Буфер<int>                pos;
		Буфер<int>                width;
		Буфер<HeightInfo>         height;
		Буфер<const CharFormat *> формат;
		Массив<Строка>                line;
		Массив<CharFormat>          hformat;
		Массив<RichObject>          object;
		int                        first_indent;
		int                        next_indent;


		int                  дайСчёт() const        { return line.дайСчёт(); }
		Строка&                operator[](int i)       { return line[i]; }
		const Строка&          operator[](int i) const { return line[i]; }
		void                 Justify(const фмт& формат);
		int                  BodyHeight() const;

		rval_default(Lines);
	
		Lines();
		~Lines();

	private:
		static Массив<Lines>&       Кэш();
		bool                       incache;
		bool                       justified;
		int64                      cacheid;
		
		friend struct RichPara;
	};

	static const ВекторМап<Ид, FieldType *>& fieldtype() { return fieldtype0(); }
	static void  регистрируй(Ид id, FieldType& ft);
	template <class T> static void регистрируй(Ид id)       { static T x; регистрируй(id, x); }

	int64       cacheid;
	bool        incache;
	Массив<Part> part;
	фмт      формат;

	static void Charformat(Поток& out, const CharFormat& o, const CharFormat& n,
	                       const CharFormat& s);

	static void DrawRuler(Draw& w, int x, int y, int cx, int cy, Цвет ink, int style);

	void        конкат(const ШТкст& s, const CharFormat& f);
	void        конкат(const char *s, const CharFormat& f);
	void        конкат(const RichObject& o, const CharFormat& f);
	void        конкат(Ид fieldtype, const Ткст& param, const CharFormat& f);

	int         дайДлину() const;
	int         дайСчёт() const                          { return part.дайСчёт(); }
	bool        пустой() const;
	Ткст      Pack(const фмт& style, Массив<RichObject>& obj) const;
	void        Unpack(const Ткст& s, const Массив<RichObject>& obj, const фмт& style);
	
	void        ApplyStyle(const фмт& newstyle);

	Part&       operator[](int i)                         { return part[i]; }
	const Part& operator[](int i) const                   { return part[i]; }
	int         FindPart(int& pos) const;
	void        обрежь(int pos);
	void        середина(int pos);
	void        приставь(const RichPara& p)                 { part.приставь(p.part); }

	void        GetRichPos(RichPos& rp, int pos) const;

	Lines       FormatLines(int cx) const;
	void        рисуй(PageDraw& pw, RichContext rc, const PaintInfo& pi, const Number& n, const Биты& spellerror, bool baselevel) const;
	RichCaret   дайКаретку(int pos, RichContext rc) const;
	int         дайПоз(int x, PageY y, RichContext rc) const;
	void        GatherLabels(Вектор<RichValPos>& info, RichContext rc, int pos) const;
	void        GatherIndexes(Вектор<RichValPos>& info, RichContext rc, int pos) const;
	int         GetVertMove(int pos, int gx, const Прям& page, int dir) const;

	ШТкст     дайТекст() const;

	bool        HasPos() const;

	bool        EvaluateFields(ВекторМап<Ткст, Значение>& vars);

	void        operator<<=(const RichPara& p)            { формат = p.формат; part <<= p.part; }

#ifdef _ОТЛАДКА
	void        Dump();
#endif

	void        ApplyZoom(Zoom z);
	
	void        CacheId(int64 id);

	rval_default(RichPara);

	RichPara();
	~RichPara();

private:
	Вкладка         GetNextTab(int pos, int cx) const;
	void        Smh(Lines& lines, HeightInfo *th, int cx) const;
	Lines       старт(RichContext& rc) const;
	bool        BreaksPage(const RichContext& rc, const Lines& pl, int i) const;
	void        PackParts(Поток& out, const CharFormat& chrstyle,
	                      const Массив<Part>& part, CharFormat& cf,
	                      Массив<RichObject>& obj) const;
	void        UnpackParts(Поток& in, const CharFormat& chrstyle,
	                        Массив<Part>& part, const Массив<RichObject>& obj, int& oi);
	static void слей(Draw& draw, const PaintInfo& pi, wchar *text, const CharFormat **i0,
                      int *wd, int pos, int len, int x0, int x, int y0, int y, int linecy,
                      int lineascent, Zoom z, bool highlight);
	int         PosInLine(int x, const Прям& page, const Lines& pl, int lni) const;

	static СтатическийСтопор      cache_lock;
	static Массив<RichPara>& Кэш();

	static ВекторМап<Ид, FieldType *>& fieldtype0();

	struct StorePart;
};

inline bool operator==(const RichPara::Вкладка& a, const RichPara::Вкладка& b) {
	return a.pos == b.pos && a.align == b.align && a.fillchar == b.fillchar;
}

bool NumberingDiffers(const RichPara::фмт& fmt1, const RichPara::фмт& fmt2);

void сортируй(Вектор<RichPara::Вкладка>& tab);

void operator*=(RichPara::фмт& fmt, Zoom z);
