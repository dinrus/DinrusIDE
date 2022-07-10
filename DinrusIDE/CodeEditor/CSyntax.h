class CSyntax : public EditorSyntax { // Curly braces languages (C++, Java, C#, Javascript...) common support
public:
	virtual void            очисть();
	virtual void            ScanSyntax(const wchar *ln, const wchar *e, int line, int tab_size);
	virtual void            сериализуй(Поток& s);
	virtual void            IndentInsert(РедакторКода& editor, int chr, int count);
	virtual bool            проверьФигСкобы(РедакторКода& e, int64& bpos0, int64& bpos);
	virtual bool            CanAssist() const;
	virtual void            Highlight(const wchar *s, const wchar *end, HighlightOutput& hls,
	                                  РедакторКода *editor, int line, int64 pos);
	virtual void            проверьОсвежиСинтакс(РедакторКода& e, int64 pos, const ШТкст& text);
	virtual Вектор<IfState> PickIfStack(); // TODO: Refactor?
	virtual void            перефмтКоммент(РедакторКода& e);

protected:
	bool        comment;       // we are in /* */ block comment
	bool        linecomment;   // we are in // line comment (because it can be continued by '\')
	bool        string;        // we are in string (becase it can be continued by '\')
	bool        linecont;      // line ended with '\'
	bool        was_namespace; // true if there was 'namespace', until '{' or ';' (not in ( [ brackets)
	ШТкст     raw_string;    // we are in C++11 raw string literal, this is end delimiter, e.g. )"
	char        macro;         // can be one of:
	enum        {
		MACRO_OFF = 0,  // last line was not #define
	    MACRO_CONT, // last line was #define and ended with '\'
	    MACRO_END   // last line was a macro, but ended
	};

	int         cl, bl, pl; // levels of { [ (

	Вектор<int>     brk; // { ( [ stack (contains '{', ')', ']')
	Вектор<int>     blk; // { line stack //TODO:SYNTAX: Join blk and bid
	Вектор<int>     bid; // { indentation stack
	Вектор<Isx>     par; // ( [ position stack
	Вектор<IfState> ifstack;

	int         stmtline;     // line of latest "if", "else", "while", "do", "for" or -1
	int         endstmtline;  // line of latest ';' (not in ( [ brackets)
	int         seline;       // stmtline stored here on ';' (not in ( [ brackets)
	int         spar;         // ( [ level, reset on "if", "else", "while", "do", "for"
	
	int         highlight;    // subtype (temporary) TODO

	static int  InitUpp(const char **q);
	static void InitKeywords();
	const wchar *DoComment(HighlightOutput& hls, const wchar *p, const wchar *e);

	static Вектор< Индекс<Ткст> > keyword;
	static Вектор< Индекс<Ткст> > имя;
	static Индекс<Ткст> kw_upp;
	static int kw_macros, kw_logs, kw_sql_base, kw_sql_func;

	
	static Цвет BlockColor(int level);

	int     GetCommentPos(РедакторКода& e, int l, ШТкст& ch) const;
	ШТкст GetCommentHdr(РедакторКода& e, int l) const { ШТкст h; GetCommentPos(e, l, h); return h; }
	void    IndentInsert0(РедакторКода& e, int chr, int count, bool reformat);

	void Bracket(int64 pos, HighlightOutput& hls, РедакторКода *editor);

	void  ClearBraces();

	void  Grounding(const wchar *ln, const wchar *e);

	bool CheckBracket(РедакторКода& e, int li, int64 pos, int64 ppos, int64 pos0, ШТкст ln, int d, int limit, int64& bpos0, int64& bpos);
	bool CheckLeftBracket(РедакторКода& e, int64 pos, int64& bpos0, int64& bpos);
	bool CheckRightBracket(РедакторКода& e, int64 pos, int64& bpos0, int64& bpos);

	bool RawString(const wchar *p, int& n);
	
public:
	static int  LoadSyntax(const char *keywords[], const char *names[]);

	enum HighlightType {
		HIGHLIGHT_NONE = -1, HIGHLIGHT_CPP = 0, HIGHLIGHT_USC, HIGHLIGHT_JAVA, HIGHLIGHT_T,
		HIGHLIGHT_CALC, HIGHLIGHT_LAY, HIGHLIGHT_SCH, HIGHLIGHT_SQL, HIGHLIGHT_CS,
		HIGHLIGHT_JAVASCRIPT, HIGHLIGHT_CSS, HIGHLIGHT_JSON, HIGHLIGHT_PHP,
		HIGHLIGHT_COUNT
	};
	
	void    SetHighlight(int h)           { highlight = h; }

	CSyntax()                             { очисть(); }
};

const wchar *HighlightNumber(HighlightOutput& hls, const wchar *p, bool ts, bool octal, bool css);
const wchar *HighlightHexBin(HighlightOutput& hls, const wchar *p, int plen, bool thousands_separator);
