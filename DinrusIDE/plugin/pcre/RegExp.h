#ifndef _PcreTest_RegExp_h_
#define _PcreTest_RegExp_h_

//#include <Core/Core.h>

class RegExp : public Движимое<RegExp> {
public:
	enum
	{
		PARTIAL   = PCRE_PARTIAL,
		/* compile options */
		UNICODE   = PCRE_UTF8,
		UTF8      = PCRE_UTF8,
		CASELESS  = PCRE_CASELESS,
		MULTILINE = PCRE_MULTILINE,
		UNGREEDY  = PCRE_UNGREEDY,
		DOTALL    = PCRE_DOTALL,
		
		EXECUTE_OPTIONS = PCRE_ANCHORED
		                | PCRE_NEWLINE_CR
		                | PCRE_NEWLINE_LF
		                | PCRE_NEWLINE_CRLF
		                | PCRE_NEWLINE_ANYCRLF
		                | PCRE_NEWLINE_ANY
		                | PCRE_NOTBOL
		                | PCRE_NOTEOL
		                | PCRE_NOTEMPTY
		                | PCRE_NOTEMPTY_ATSTART
		                | PCRE_NO_START_OPTIMIZE
		                | PCRE_NO_UTF8_CHECK
		                | PCRE_PARTIAL_SOFT
		                | PCRE_PARTIAL_HARD,
		COMPILE_OPTIONS = PCRE_ANCHORED
		                | PCRE_AUTO_CALLOUT
		                | PCRE_BSR_ANYCRLF
		                | PCRE_BSR_UNICODE
		                | PCRE_CASELESS
		                | PCRE_DOLLAR_ENDONLY
		                | PCRE_DOTALL
		                | PCRE_DUPNAMES
		                | PCRE_EXTENDED
		                | PCRE_EXTRA
		                | PCRE_FIRSTLINE
		                | PCRE_JAVASCRIPT_COMPAT
		                | PCRE_MULTILINE
		                | PCRE_NEWLINE_CR
		                | PCRE_NEWLINE_LF
		                | PCRE_NEWLINE_CRLF
		                | PCRE_NEWLINE_ANYCRLF
		                | PCRE_NEWLINE_ANY
		                | PCRE_NO_AUTO_CAPTURE
		                | PCRE_NO_START_OPTIMIZE
		                | PCRE_UCP
		                | PCRE_UNGREEDY
		                | PCRE_UTF8
		                | PCRE_NO_UTF8_CHECK
	};

private:
	Ткст pattern;
	Ткст text;
	pcre * cpattern;
	pcre_extra * study;
	const char * error_string;
	int error_offset;
	int error_code;
	int pos[120]; // if modified-chk config.h as well, keep at multiple of 3
	int rc;
	bool first;
	int compile_options;
	int execute_options;

	bool ReplacePos(Ткст& t, int p, int q, const Ткст& r);
	int  Replace0(Ткст& t, const Вектор<Ткст>& rv, const int& rv_count, int& offset);
	Вектор<Ткст> ResolveBackRef(const Вектор<Ткст>& rv);
	Вектор<Ткст> Make_rv(const Ткст& r);
	
public:
	void           очисть(bool freemem = false);
	void           SetOptions(int options);
	void           SetPattern(const char * p);
	void           SetPattern(const Ткст &p);
	bool           Compile(bool recompile = false);
	bool           Study(bool restudy = false);
	int            выполни(const Ткст& t, int offset = 0);
	bool           сверь(const Ткст& t, bool copy = true);
	bool           FastMatch(const Ткст& t);
	bool           GlobalMatch(const Ткст& t);
	Ткст         operator[](const int i);
	int            дайСчёт();
	Ткст         дайТкст(int i);
	void           GetMatchPos(int i, int& iPosStart, int& iPosAfterEnd);
	
	int            дайСмещ() const;
	int            дайДлину() const;
	int            GetSubOffset(int i) const;
	int            GetSubLength(int i) const;

	int            замени(Ткст& t, const Вектор<Ткст>& rv, bool backref=false);
	int            замени(Ткст& t, const Ткст& r, bool backref=false);
	int            замени(Ткст& t, Событие<Вектор<Ткст>&> cbr);
	
	int            ReplaceGlobal(Ткст& t, const Вектор<Ткст>& rv, bool backref=false);
	int            ReplaceGlobal(Ткст& t, const Ткст& r, bool backref=false);
	int            ReplaceGlobal(Ткст& t, Событие<Вектор<Ткст>&> cbr);
	
	Вектор<Ткст> GetStrings();
	Ткст         GetPattern() const { return pattern; }

	bool           ошибка_ли() { return error_code != 0; }
	const char    *дайОш() { return error_string; }
	int            GetErrorCode() { return error_code; }

	RegExp(int options = UTF8);
	RegExp(const char * p, int options = UTF8);
	RegExp(const Ткст &p, int options = UTF8);
	~RegExp();
};

#endif


