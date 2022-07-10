inline bool iscib(int c) {
	return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c == '$';
}

inline bool iscid(int c) {
	return iscib(c) || c >= '0' && c <= '9';
}

class СиПарсер {
protected:
	const char *term;
	const char *wspc;
	const char *lineptr;
	int         line;
	Ткст      фн;
	bool        skipspaces;
	bool        skipcomments;
	bool        nestcomments;
	bool        uescape;

	bool        пробелы0();
	const char *ид0_ли(const char *s) const;
	bool        ид0(const char *ид);
	void        сделайПробелы()                    { if(skipspaces) пробелы(); }
	dword       читайГекс();
	bool        читайГекс(dword& hex, int n);

public:
	struct Ошибка : public Искл                 { Ошибка(const char *s) : Искл(s) {} };

	void   выведиОш(const char *s);
	void   выведиОш()                       { выведиОш(""); }

	bool   пробелы()                           { wspc = term; return ((byte)*term <= ' ' || *term == '/') && пробелы0(); }
	char   подбериСим() const                   { return *term; }
	char   дайСим();

	bool   сим_ли(char c) const               { return *term == c; }
	bool   сим2_ли(char c1, char c2) const    { return term[0] == c1 && term[1] == c2; }
	bool   сим3_ли(char c1, char c2, char c3) const { return term[0] == c1 && term[1] == c2 && term[2] == c3; }
	bool   сим(char c);
	bool   сим2(char c1, char c2);
	bool   сим3(char c1, char c2, char c3);
	void   передайСим(char c);
	void   передайСим2(char c1, char c2);
	void   передайСим3(char c1, char c2, char c3);
	bool   ид(const char *s)                  { return term[0] == s[0] && (s[1] == 0 || term[1] == s[1]) && ид0(s); }
	void   передайИд(const char *s);
	bool   ид_ли() const                       { return iscib(*term); }
	bool   ид_ли(const char *s) const          { return term[0] == s[0] && (s[1] == 0 || term[1] == s[1]) && ид0_ли(s); }
	Ткст читайИд();
	Ткст ReadIdt();
	bool   цел_ли() const;
	int    знак();
	int    читайЦел();
	int    читайЦел(int min, int max);
	int64  читайЦел64();
	int64  читайЦел64(int64 min, int64 max);
	bool   число_ли() const                   { return цифра_ли(*term); }
	bool   число_ли(int base) const;
	uint32 читайЧисло(int base = 10);
	uint64 читайЧисло64(int base = 10);
	bool   дво_ли() const                   { return цел_ли(); }
	bool   дво2_ли() const;
	double читайДво();
	bool   ткст_ли() const                   { return сим_ли('\"'); };
	Ткст читай1Ткст(bool chkend = true);
	Ткст читайТкст(bool chkend = true);
	Ткст читай1Ткст(int delim, bool chkend = true);
	Ткст читайТкст(int delim, bool chkend = true);

	void   пропусти();
	void   пропустиТерм()                         { пропусти(); }

	struct Поз {
		const char *ptr;
		const char *wspc;
		const char *lineptr;
		int         line;
		Ткст      фн;
		
		int дайКолонку(int tabsize = 4) const;

		Поз(const char *ptr = NULL, int line = 1, Ткст фн = Null) : ptr(ptr), line(line), фн(фн) {}
	};

	const char *дайУк() const                { return (const char *)term; }
	const char *дайПбелУк() const           { return (const char *)wspc; }

	Поз         дайПоз() const;
	void        устПоз(const Поз& pos);

	bool   кф_ли() const                      { return *term == '\0'; }
	operator bool() const                     { return *term; }

	int    дайСтроку() const                    { return line; }
	int    дайКолонку(int tabsize = 4) const;
	Ткст дайИмяф() const                { return фн; }

	static Ткст LineInfoComment(const Ткст& filename, int line = 1, int column = 1);
	Ткст GetLineInfoComment(int tabsize = 4) const;
	enum { LINEINFO_ESC = '\2' };
	
	void   уст(const char *ptr, const char *фн, int line = 1);
	void   уст(const char *ptr);

	СиПарсер& пропустиПробелы(bool b = true)        { skipspaces = b; return *this; }
	СиПарсер& безПропускаПробелов()                   { skipspaces = false; return *this; }
	СиПарсер& искейпЮникод(bool b = true)     { uescape = b; return *this; }
	СиПарсер& пропустиКомменты(bool b = true);
	СиПарсер& безПропускаКомментов()                 { return пропустиКомменты(false); }
	СиПарсер& гнездиКомменты(bool b = true);
	СиПарсер& безГнездКомментов()                 { return гнездиКомменты(false); }

	СиПарсер(const char *ptr);
	СиПарсер(const char *ptr, const char *фн, int line = 1);
	СиПарсер();
};

inline bool СиПарсер::сим(char c)
{
	if(сим_ли(c)) {
		term++;
		сделайПробелы();
		return true;
	}
	return false;
}

inline bool СиПарсер::сим2(char c1, char c2)
{
	if(сим2_ли(c1, c2)) {
		term += 2;
		сделайПробелы();
		return true;
	}
	return false;
}

inline bool СиПарсер::сим3(char c1, char c2, char c3)
{
	if(сим3_ли(c1, c2, c3)) {
		term += 3;
		сделайПробелы();
		return true;
	}
	return false;
}

enum {
	ASCSTRING_SMART     = 0x01,
	ASCSTRING_OCTALHI   = 0x02,
	ASCSTRING_JSON      = 0x04,
};

Ткст какТкстСи(const char *s, const char *end, int linemax = INT_MAX, const char *linepfx = NULL,
                 dword flags = 0);
Ткст какТкстСи(const char *s, int linemax = INT_MAX, const char *linepfx = NULL,
                 dword flags = 0);
Ткст какТкстСи(const Ткст& s, int linemax = INT_MAX, const char *linepfx = NULL,
                 dword flags = 0);
