#ifndef _CodeEditor_Highlight_h_
#define _CodeEditor_Highlight_h_

#define CTIMING(x) // RTIMING(x)

struct HlStyle {
	Цвет color;
	bool  bold;
	bool  italic;
	bool  underline;
};

struct Isx : Движимое<Isx> { // '(', '[' position
	int    line;
	int    pos;
	
	void сериализуй(Поток& s)    { s % line % pos; }

	friend bool operator==(Isx a, Isx b) { return a.line == b.line && a.pos == b.pos; }
	friend bool operator!=(Isx a, Isx b) { return !(a == b); }
};

struct IfState : Движимое<IfState> {
	enum        { IF = '0', ELIF, ELSE, ELSE_ERROR, ENDIF_ERROR };

	ШТкст iftext;
	short   ifline;
	char    state;

	void сериализуй(Поток& s)         { s % iftext % ifline % state; }

	bool operator==(const IfState& b) const {
		return iftext == b.iftext && state == b.state && ifline == b.ifline;
	}

	IfState()                         { ifline = state = 0; }
};

struct HighlightSetup { // Global highlighting настройки
public:
#define HL_COLOR(x, a, b)      x,
	enum {
#include "hl_color.i"
		HL_COUNT
	};
#undef HL_COLOR

	static HlStyle hl_style[HL_COUNT];
	static byte    hilite_scope;
	static byte    hilite_ifdef;
	static byte    hilite_bracket;
	static bool    thousands_separator;
	static bool    indent_spaces;
	static int     indent_amount;
	static bool    no_parenthesis_indent;

	static const HlStyle& GetHlStyle(int i);
	static void           SetHlStyle(int i, Цвет c, bool bold = false, bool italic = false, bool underline = false);
	static void           LoadHlStyles(const char *s);
	static Ткст         StoreHlStyles();
	static void           тёмнаяТема();
	static void           WhiteTheme();
	static void           DefaultHlStyles();

	static const char    *GetHlName(int i);
	static bool           HasHlFont(int i);
};

struct HighlightOutput : HighlightSetup {
	Вектор<СтрокРедакт::Highlight>& v;
	СтрокРедакт::Highlight          def;
	int                          pos;

public:
	void SetChar(int pos, int chr)                    { v[pos].chr = chr; }
	void уст(int pos, int count, const HlStyle& ink);
	void SetFlags(int pos, int count, word flags);
	void устШрифт(int pos, int count, const HlStyle& f);
	void SetPaper(int pos, int count, Цвет paper);
	void устЧернила(int pos, int count, Цвет ink);
	void SetFlags(int count, word flags)              { SetFlags(pos, count, flags); }
	void помести(int count, const HlStyle& ink)           { уст(pos, count, ink); pos += count; }
	void помести(int count, const HlStyle& ink, Цвет paper);
	void помести(int count, const HlStyle& ink, const HlStyle& paper);
	void помести(const HlStyle& ink)                      { помести(1, ink); }
	void помести(const HlStyle& ink, word flags)          { помести(1, ink); v[pos - 1].flags = flags; }
	void Flags(word flags)                            { v[pos - 1].flags = flags; }
	int  дайСчёт() const                             { return v.дайСчёт(); }

	const wchar *CString(const wchar *p);
	
	HighlightOutput(Вектор<СтрокРедакт::Highlight>& v);
	~HighlightOutput();
};

class EditorSyntax : public HighlightSetup { // Inheriting to make static members available
	struct SyntaxDef {
		Событие<Один<EditorSyntax>&> factory;
		Ткст                    patterns;
		Ткст                    description;
	};
	
	static МассивМап<Ткст, SyntaxDef>& defs();

protected:
	bool                    ignore_errors;
	int                     comments_lang;
	
public:
	virtual void            очисть();
	virtual void            ScanSyntax(const wchar *ln, const wchar *e, int line, int tab_size);
	virtual void            сериализуй(Поток& s);
	virtual void            IndentInsert(РедакторКода& editor, int chr, int count);
	virtual bool            проверьФигСкобы(РедакторКода& e, int64& bpos0, int64& bpos); // TODO: замени with generic mechanism
	virtual void            проверьОсвежиСинтакс(РедакторКода& e, int64 pos, const ШТкст& text);
	virtual bool            CanAssist() const;
	virtual void            Highlight(const wchar *s, const wchar *end, HighlightOutput& hls,
	                                  РедакторКода *editor, int line, int64 pos);
	virtual Вектор<IfState> PickIfStack();
	virtual void            перефмтКоммент(РедакторКода& e);
	virtual ~EditorSyntax();

	static Цвет IfColor(char ifstate);

	void    уст(const Ткст& s)           { CTIMING("уст"); if(s.дайСчёт() == 0) очисть(); else грузиИзТкст(*this, s); }
	Ткст  дай()                          { CTIMING("дай"); return сохраниКакТкст(*this); }
	
	void    IgnoreErrors()                 { ignore_errors = true; }
	void    SpellCheckComments(int lang)   { comments_lang = lang; }

	EditorSyntax()                         { очисть(); ignore_errors = false; }

	static void регистрируй(const char *id, Событие<Один<EditorSyntax>&> factory,
	                     const char *exts, const char *description);
	static Один<EditorSyntax> создай(const char *id);
	static Ткст            GetSyntaxForFilename(const char *фн);
	static int               GetSyntaxCount()             { return defs().дайСчёт(); }
	static Ткст            дайСинтакс(int i)             { return defs().дайКлюч(i); }
	static Ткст            GetSyntaxDescription(int i);
	static Ткст            GetSyntaxPatterns(int i)     { return defs()[i].patterns; }
};

#endif
