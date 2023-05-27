inline бул iscib(char c) {
	return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c == '$';
}

inline бул iscid(char c) {
	return iscib(c) || c >= '0' && c <= '9';
}

class СиПарсер {
protected:
	кткст0 term;
	кткст0 wspc;
	кткст0 lineptr;
	цел         line;
	Ткст      фн;
	бул        skipspaces;
	бул        skipcomments;
	бул        nestcomments;
	бул        uescape;

	бул        пробелы0();
	кткст0 ид0_ли(кткст0 s) const;
	бул        ид0(кткст0 ид);
	проц        сделайПробелы()                    { if(skipspaces) пробелы(); }
	бцел       читайГекс();
	бул        читайГекс(бцел& hex, цел n);

public:
	struct Ошибка : public Искл                 { Ошибка(кткст0 s) : Искл(s) {} };

	проц   выведиОш(кткст0 s);
	проц   выведиОш()                       { выведиОш(""); }

	бул   пробелы()                           { wspc = term; return ((ббайт)*term <= ' ' || *term == '/') && пробелы0(); }
	char   подбериСим() const                   { return *term; }
	char   дайСим();

	бул   сим_ли(char c) const               { return *term == c; }
	бул   сим2_ли(char c1, char c2) const    { return term[0] == c1 && term[1] == c2; }
	бул   сим3_ли(char c1, char c2, char c3) const { return term[0] == c1 && term[1] == c2 && term[2] == c3; }
	бул   сим(char c);
	бул   сим2(char c1, char c2);
	бул   сим3(char c1, char c2, char c3);
	проц   передайСим(char c);
	проц   передайСим2(char c1, char c2);
	проц   передайСим3(char c1, char c2, char c3);
	бул   ид(кткст0 s)                  { return term[0] == s[0] && (s[1] == 0 || term[1] == s[1]) && ид0(s); }
	проц   передайИд(кткст0 s);
	бул   ид_ли() const                       { return iscib(*term); }
	бул   ид_ли(кткст0 s) const          { return term[0] == s[0] && (s[1] == 0 || term[1] == s[1]) && ид0_ли(s); }
	Ткст читайИд();
	Ткст ReadIdt();
	бул   цел_ли() const;
	цел    знак();
	цел    читайЦел();
	цел    читайЦел(цел мин, цел макс);
	дол  читайЦел64();
	дол  читайЦел64(дол мин, дол макс);
	бул   число_ли() const                   { return цифра_ли(*term); }
	бул   число_ли(цел base) const;
	бцел читайЧисло(цел base = 10);
	бдол читайЧисло64(цел base = 10);
	бул   дво_ли() const                   { return цел_ли(); }
	бул   дво2_ли() const;
	дво читайДво();
	бул   ткст_ли() const                   { return сим_ли('\"'); };
	Ткст читай1Ткст(бул chkend = true);
	Ткст читайТкст(бул chkend = true);
	Ткст читай1Ткст(цел delim, бул chkend = true);
	Ткст читайТкст(цел delim, бул chkend = true);

	проц   пропусти();
	проц   пропустиТерм()                         { пропусти(); }

	struct Поз {
		кткст0 укз;
		кткст0 wspc;
		кткст0 lineptr;
		цел         line;
		Ткст      фн;
		
		цел дайКолонку(цел tabsize = 4) const;

		Поз(кткст0 укз = NULL, цел line = 1, Ткст фн = Null) : укз(укз), line(line), фн(фн) {}
	};

	кткст0 дайУк() const                { return (const char *)term; }
	кткст0 дайПбелУк() const           { return (const char *)wspc; }

	Поз         дайПоз() const;
	проц        устПоз(const Поз& pos);

	бул   кф_ли() const                      { return *term == '\0'; }
	operator бул() const                     { return *term; }

	цел    дайСтроку() const                    { return line; }
	цел    дайКолонку(цел tabsize = 4) const;
	Ткст дайИмяф() const                { return фн; }

	static Ткст LineInfoComment(const Ткст& имяф, цел line = 1, цел column = 1);
	Ткст GetLineInfoComment(цел tabsize = 4) const;
	enum { LINEINFO_ESC = '\2' };
	
	проц   уст(кткст0 укз, кткст0 фн, цел line = 1);
	проц   уст(кткст0 укз);

	СиПарсер& пропустиПробелы(бул b = true)        { skipspaces = b; return *this; }
	СиПарсер& безПропускаПробелов()                   { skipspaces = false; return *this; }
	СиПарсер& искейпЮникод(бул b = true)     { uescape = b; return *this; }
	СиПарсер& пропустиКомменты(бул b = true);
	СиПарсер& безПропускаКомментов()                 { return пропустиКомменты(false); }
	СиПарсер& гнездиКомменты(бул b = true);
	СиПарсер& безГнездКомментов()                 { return гнездиКомменты(false); }

	СиПарсер(кткст0 укз);
	СиПарсер(кткст0 укз, кткст0 фн, цел line = 1);
	СиПарсер();
};

inline бул СиПарсер::сим(char c)
{
	if(сим_ли(c)) {
		term++;
		сделайПробелы();
		return true;
	}
	return false;
}

inline бул СиПарсер::сим2(char c1, char c2)
{
	if(сим2_ли(c1, c2)) {
		term += 2;
		сделайПробелы();
		return true;
	}
	return false;
}

inline бул СиПарсер::сим3(char c1, char c2, char c3)
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

Ткст какТкстСи(кткст0 s, кткст0 end, цел linemax = INT_MAX, кткст0 linepfx = NULL,
                 бцел flags = 0);
Ткст какТкстСи(кткст0 s, цел linemax = INT_MAX, кткст0 linepfx = NULL,
                 бцел flags = 0);
Ткст какТкстСи(const Ткст& s, цел linemax = INT_MAX, кткст0 linepfx = NULL,
                 бцел flags = 0);
