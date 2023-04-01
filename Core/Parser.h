#define case_id_rus \
	case L'а':case L'б':case L'в':case L'г':case L'д':case L'е':case L'ё':case L'ж':case L'з': \
	case L'и':case L'й':case L'к':case L'л':case L'м':case L'н':case L'о':case L'п':case L'р': \
	case L'с':case L'т':case L'у':case L'ф':case L'х':case L'ц':case L'ч':case L'ш':case L'щ': \
	case L'ъ':case L'ы':case L'ь':case L'э':case L'ю':case L'я': \
	case L'А':case L'Б':case L'В':case L'Г':case L'Д':case L'Е':case L'Ё':case L'Ж':case L'З': \
	case L'И':case L'Й':case L'К':case L'Л':case L'М':case L'Н':case L'О':case L'П':case L'Р': \
	case L'С':case L'Т':case L'У':case L'Ф':case L'Х':case L'Ц':case L'Ч':case L'Ш':case L'Щ': \
	case L'Ъ':case L'Ы':case L'Ь':case L'Э':case L'Ю':case L'Я'

inline bool iscib(char c) {
	return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c == '$';
}

inline bool iscid(char c) {
	switch(c) {
		case_id_rus: return true;
			}
		return iscib(c) || c >= '0' && c <= '9';
}

class CParser {
protected:
	const char *term;
	const char *wspc;
	const char *lineptr;
	int         line;
	String      fn;
	bool        skipspaces;
	bool        skipcomments;
	bool        nestcomments;
	bool        uescape;

	bool        Spaces0();
	const char *IsId0(const char *s) const;
	bool        Id0(const char *id);
	void        DoSpaces()                    { if(skipspaces) Spaces(); }
	dword       ReadHex();
	bool        ReadHex(dword& hex, int n);

public:
	struct Error : public Exc                 { Error(const char *s) : Exc(s) {} };

	void   ThrowError(const char *s);
	void   ThrowError()                       { ThrowError(""); }

	bool   Spaces()                           { wspc = term; return ((byte)*term <= ' ' || *term == '/') && Spaces0(); }
	char   PeekChar() const                   { return *term; }
	char   GetChar();

	bool   IsChar(char c) const               { return *term == c; }
	bool   IsChar2(char c1, char c2) const    { return term[0] == c1 && term[1] == c2; }
	bool   IsChar3(char c1, char c2, char c3) const { return term[0] == c1 && term[1] == c2 && term[2] == c3; }
	bool   Char(char c);
	bool   Char2(char c1, char c2);
	bool   Char3(char c1, char c2, char c3);
	void   PassChar(char c);
	void   PassChar2(char c1, char c2);
	void   PassChar3(char c1, char c2, char c3);
	bool   Id(const char *s)                  { return term[0] == s[0] && (s[1] == 0 || term[1] == s[1]) && Id0(s); }
	void   PassId(const char *s);
	bool   IsId() const                       { return iscib(*term); }
	bool   IsId(const char *s) const          { return term[0] == s[0] && (s[1] == 0 || term[1] == s[1]) && IsId0(s); }
	String ReadId();
	String ReadIdt();
	bool   IsInt() const;
	int    Sgn();
	int    ReadInt();
	int    ReadInt(int min, int max);
	int64  ReadInt64();
	int64  ReadInt64(int64 min, int64 max);
	bool   IsNumber() const                   { return IsDigit(*term); }
	bool   IsNumber(int base) const;
	uint32 ReadNumber(int base = 10);
	uint64 ReadNumber64(int base = 10);
	bool   IsDouble() const                   { return IsInt(); }
	bool   IsDouble2() const;
	double ReadDouble();
	bool   IsString() const                   { return IsChar('\"'); };
	String ReadOneString(bool chkend = true);
	String ReadString(bool chkend = true);
	String ReadOneString(int delim, bool chkend = true);
	String ReadString(int delim, bool chkend = true);

	void   Skip();
	void   SkipTerm()                         { Skip(); }

	struct Pos {
		const char *ptr;
		const char *wspc;
		const char *lineptr;
		int         line;
		String      fn;

		int GetColumn(int tabsize = 4) const;

		Pos(const char *ptr = NULL, int line = 1, String fn = Null) : ptr(ptr), line(line), fn(fn) {}
	};

	const char *GetPtr() const                { return (const char *)term; }
	const char *GetSpacePtr() const           { return (const char *)wspc; }

	Pos         GetPos() const;
	void        SetPos(const Pos& pos);

	bool   IsEof() const                      { return *term == '\0'; }
	operator bool() const                     { return *term; }

	int    GetLine() const                    { return line; }
	int    GetColumn(int tabsize = 4) const;
	String GetFileName() const                { return fn; }

	static String LineInfoComment(const String& filename, int line = 1, int column = 1);
	String GetLineInfoComment(int tabsize = 4) const;
	enum { LINEINFO_ESC = '\2' };

	void   Set(const char *ptr, const char *fn, int line = 1);
	void   Set(const char *ptr);

	CParser& SkipSpaces(bool b = true)        { skipspaces = b; return *this; }
	CParser& NoSkipSpaces()                   { skipspaces = false; return *this; }
	CParser& UnicodeEscape(bool b = true)     { uescape = b; return *this; }
	CParser& SkipComments(bool b = true);
	CParser& NoSkipComments()                 { return SkipComments(false); }
	CParser& NestComments(bool b = true);
	CParser& NoNestComments()                 { return NestComments(false); }

	CParser(const char *ptr);
	CParser(const char *ptr, const char *fn, int line = 1);
	CParser();
};

inline bool CParser::Char(char c)
{
	if(IsChar(c)) {
		term++;
		DoSpaces();
		return true;
	}
	return false;
}

inline bool CParser::Char2(char c1, char c2)
{
	if(IsChar2(c1, c2)) {
		term += 2;
		DoSpaces();
		return true;
	}
	return false;
}

inline bool CParser::Char3(char c1, char c2, char c3)
{
	if(IsChar3(c1, c2, c3)) {
		term += 3;
		DoSpaces();
		return true;
	}
	return false;
}

enum {
	ASCSTRING_SMART     = 0x01,
	ASCSTRING_OCTALHI   = 0x02,
	ASCSTRING_JSON      = 0x04,
};

String AsCString(const char *s, const char *end, int linemax = INT_MAX, const char *linepfx = NULL,
                 dword flags = 0);
String AsCString(const char *s, int linemax = INT_MAX, const char *linepfx = NULL,
                 dword flags = 0);
String AsCString(const String& s, int linemax = INT_MAX, const char *linepfx = NULL,
                 dword flags = 0);