class TagSyntax : public EditorSyntax { // Тэг based languages (XML, HTML)
public:
	virtual void            очисть();
	virtual void            ScanSyntax(const wchar *ln, const wchar *e, int line, int tab_size);
	virtual void            сериализуй(Поток& s);
	virtual void            Highlight(const wchar *s, const wchar *end, HighlightOutput& hls,
	                                  РедакторКода *editor, int line, int64 pos);
	virtual void            проверьОсвежиСинтакс(РедакторКода& e, int64 pos, const ШТкст& text);
	virtual void            IndentInsert(РедакторКода& editor, int chr, int count);
	virtual bool            проверьФигСкобы(РедакторКода& e, int64& bpos0, int64& bpos);

private:

	enum { TEXT, TAG0, TAG, ENDTAG, ATTR, COMMENT, DECL, PI, SCRIPT };
	
	bool    html;
	bool    witz;
	int     status;
	int     hl_ink;
	int     hl_paper;
	Ткст  tagname;

	CSyntax script;     // for <style> or <script>
	enum { CSS, JS };
	int     script_type; 

	HighlightOutput *hout;

	const wchar *пробелы(const wchar *s, const wchar *e);
	void DoScript(const wchar *s, const wchar *e, РедакторКода *editor, int line, int tabsize, int64 pos);
	void Do(const wchar *s, const wchar *e, РедакторКода *editor, int line, int tabsize, int64 pos);
	void Put0(int ink, int n = 1, int paper = PAPER_NORMAL);

	void уст(int ink, int paper = PAPER_NORMAL)                 { hl_ink = ink; hl_paper = paper; }
	void помести(int n = 1)                                         { Put0(hl_ink, n, hl_paper); }
	void SetPut(int ink, int n = 1, int paper = PAPER_NORMAL)   { уст(ink, paper); помести(n); }

public:
	TagSyntax&  Html(bool b)                                    { html = b; return *this; }
	TagSyntax&  Witz(bool b)                                    { html = b; return *this; }
	CSyntax *GetScript()                                        { return status == SCRIPT ? &script : NULL; }

	TagSyntax();
};
